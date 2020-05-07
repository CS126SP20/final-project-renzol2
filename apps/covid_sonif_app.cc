// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "covid_sonif_app.h"

#include <iostream>
#include <chrono>
#include <thread>


/*
 * Source code for audio synthesis taken from StkTestApp:
 * https://github.com/richardeakin/Cinder-Stk/blob/master/samples/StkTest/src/StkTestApp.cpp
 *
 * Original "StkTestApp" allows user to play any of the STK included
 * instruments/generators/effects using the mouse.
 *
 * This modified version builds on top of it, reading .csv files containing
 * COVID-19 data and "sonifying" it.
 *
 * The application provides additional parameters for the user to choose what
 * data to sonify, as well as extra music-related parameters that the original
 * application set as static features.
 */
namespace covidsonifapp {

const int kParamsWindowWidth = 400;
const int kParamsWindowHeight = 700;

const float kAbsoluteMaxPitchMidi = 127;
const float kAbsoluteMinPitchMidi = 0;

const size_t kMinBpm = 0;
const size_t kMaxBpm = 999;

const float kMinVisualizationScaling = 0.25f;
const float kMaxVisualizationScaling = 1.0f;

const float kInitialGain = 0.6f;

const size_t kNumPitchClasses = 12;

const char kNormalFont[] = "Consolas";


using cinder::app::KeyEvent;

/*
 * Overridden Cinder App Methods
 */

CovidSonificationApp::CovidSonificationApp() = default;

/**
 * Initializes Cinder-Stk/Cinder audio nodes and sets all parameters.
 */
void CovidSonificationApp::setup() {
  auto ctx = cinder::audio::master();

  stk::Stk::setSampleRate(ctx->getSampleRate());
  cistk::initRawwavePath();

  master_gain_ = ctx->makeNode<cinder::audio::GainNode>(kInitialGain);
  master_gain_ >> ctx->getOutput();

  SetupParams();
  HandleInstrumentsSelected();
  HandleEffectSelected();
  HandleDataSelected();
  HandleScaleSelected();
  ctx->enable();

  PrintAudioGraph();
}

/**
 * Iterates over dataset and sonifies information as/when needed.
 */
void CovidSonificationApp::update() {
  if (in_sonification_playback) {
    // Break statement; stops when no more dates are in the dataset
    if (current_date_index_ == current_region_.GetDates().size()) {
      in_sonification_playback = false;
      StopNote();
      finished_playback = true;
      return;
    }

    // Play note using data
    current_date_ =
        current_region_.GetDates().at(current_date_index_);
    current_amount_ = current_region_.GetAmountAtDate(current_date_);
    MakeNoteFromAmount(current_amount_, max_amount_);

    // Pause thread using specified BPM
    std::this_thread::sleep_for(interval);

    current_date_index_++;
  }
}

/**
 * Draws all display components depending on state of app.
 */
void CovidSonificationApp::draw() {
  cinder::gl::clear();

  DisplayPitch();
  DisplayCurrentDataset();

  if (!in_sonification_playback && !finished_playback) {
    DisplayDirections();
    DisplayVisualizationToggle();
    params_->draw();
  }

  if (in_sonification_playback || finished_playback) {
    DisplayCurrentNoteData();
    if (is_visualizing) DrawNoteData();
  }
}

/**
 * Plays a note based on mouse position and clears screen after playback.
 * @param event mouse position
 */
void CovidSonificationApp::mouseDown(cinder::app::MouseEvent event) {
  if (finished_playback) {
    current_date_ = {};
    current_amount_ = coviddata::kNullAmount;
    finished_playback = !finished_playback;
  }
  MakeNote(event.getPos());
}

/**
 * Plays a note based on mouse position.
 * @param event mouse position
 */
void CovidSonificationApp::mouseDrag(cinder::app::MouseEvent event) {
  MakeNote(event.getPos());
}

/**
 * Stops note upon release of mouse.
 * @param event mouse position
 */
void CovidSonificationApp::mouseUp(cinder::app::MouseEvent event) {
  StopNote();
}

/*
 * Stk-Cinder specific methods
 */

/**
 * Initializes all parameters.
 */
void CovidSonificationApp::SetupParams() {
  // Define parameters
  params_ = cinder::params::InterfaceGl::create(
      getWindow(), "Settings",
      cinder::app::toPixels(
          cinder::ivec2(kParamsWindowWidth, kParamsWindowHeight)));


  // Setup all parameters for each instance variable
  SetupMasterGain();
  SetupInstruments();
  SetupEffects();
  SetupScale();
  SetupMaxMidiPitchParam();
  SetupMinMidiPitchParam();

  params_->addSeparator();

  SetupData();  // data-specific parameters handled only if data is selected
}

/**
 * Calculates frequency and gain of note upon mouse click.
 * @param pos mouse positino
 */
void CovidSonificationApp::MakeNote(const cinder::vec2& pos) {
  if (instrument_ && HandleInstrumentSpecificNote(pos)) {
    return;
  }

  // Get the quantized freq; check if it's significant enough to change
  float freq = QuantizePitch(pos);
  if (std::fabs(last_freq_ - freq) < 0.01f)
    return;

  // Calculate gain
  // float gain = 1.0f - pos.y / (float)getWindowHeight();  // original
  float gain = 0.0f + pos.x / (float)getWindowWidth();

  // Set frequency and gain to instrument/generator accordingly
  HandleNote(freq, gain);
}

/**
 * Calculates gain and frequency of note based on data.
 * @param amount data amount to sonify
 * @param max_amount maximum amount of dataset
 */
void CovidSonificationApp::MakeNoteFromAmount(float amount, float max_amount) {
  if (amount == coviddata::kNullAmount) return;

  // Get the quantized freq; check if it's significant enough to change
  float freq = QuantizePitchFromAmount(amount, max_amount);
  if (std::fabs(last_freq_ - freq) < 0.01f) return;

  float gain = master_gain_->getValue();

  // Set frequency and gain to instrument/generator accordingly
  HandleNote(freq, gain);
}

/**
 * Plays the note if an instrument is selected.
 * @param freq frequency of note
 * @param gain gain of note
 */
void CovidSonificationApp::HandleNote(float freq, float gain) {
  if (instrument_) {
    instrument_->noteOn(freq, gain);
  }
}

/**
 * Returns a quantized pitch (in hertz) based on mouse position.
 * @param pos position of mouse
 * @return quantized pitch in selected scale
 */
float CovidSonificationApp::QuantizePitch(const cinder::vec2& pos) {
  // Creates a mapping from height of mouse on screen to MIDI pitch,
  //   and finds the converted value of the pos.y of mouse
  // This is VERY helpful (see usage in QuantizePitchFromAmount)
  int pitch_midi = std::lroundf(cinder::lmap(
      pos.y,  // value to map
      (float)getWindowHeight(),
      0.0f,
      (float)min_midi_pitch_,
      (float)max_midi_pitch_));

  bool quantized = false;

  // Decrease the scale degree of the note until it matches
  // with a diatonic note in the selected key
  while (!quantized) {
    int note = pitch_midi % kNumPitchClasses;
    for (size_t i = 0; i < current_scale_.scale_length; i++) {
      if (note == current_scale_.scale_degrees.at(i)) {
        quantized = true;
        break;
      }
    }
    if (!quantized) {
      pitch_midi--;
    }
  }

  // Apply bounds for pitches (Cinder does not automatically set bounds)
  if (pitch_midi > max_midi_pitch_) pitch_midi = max_midi_pitch_;
  else if (pitch_midi < min_midi_pitch_) pitch_midi = min_midi_pitch_;

  // Set current pitch for display
  current_midi_pitch_ = pitch_midi;

  return cinder::audio::midiToFreq((float)pitch_midi);
}

/**
 * Returns a quantized pitch (Hz) based on given data.
 * @param amount data amount
 * @param max_amount maximum amount
 * @return quantized pitch in Hz within selected scale
 */
float CovidSonificationApp::QuantizePitchFromAmount(float amount,
                                                    float max_amount) {
  // Creates a mapping from [0, highest amount in data]
  //                     to [min MIDI pitch, max MIDI pitch]
  // Then finds the mapping of the specific data point to a specific MIDI pitch
  int pitch_midi = std::lroundf(cinder::lmap(
      (float)amount,
      0.0f,
      (float)max_amount,
      (float)min_midi_pitch_,
      (float)max_midi_pitch_));

  bool quantized = false;

  // Decrease the scale degree of the note until it matches
  // with a diatonic note in the selected key
  while (!quantized) {
    int note = pitch_midi % kNumPitchClasses;
    for (size_t i = 0; i < current_scale_.scale_length; i++) {
      if (note == current_scale_.scale_degrees.at(i)) {
        quantized = true;
        break;
      }
    }
    if (!quantized) {
      pitch_midi--;
    }
  }

  // Set current pitch for display
  current_midi_pitch_ = pitch_midi;

  return cinder::audio::midiToFreq((float)pitch_midi);
}

/**
 * Assigns instrument based on user selection.
 */
void CovidSonificationApp::HandleInstrumentsSelected() {
  // Disconnect all currently used instrument_s
  if (instrument_) instrument_->disconnectAll();

  // Get the name of selected instrument_ and notify user
  const std::string& name = kInstrumentNames.at(instrument_selection_);
  CI_LOG_I("Selecting instrument_ '" << name << "'" );

  // Set instrument_ accordingly
  auto ctx = cinder::audio::master();
  if (name == "BandedWG") {
    auto instr = ctx->makeNode<cistk::BandedWGNode>();
    instr->setPreset(3);  // preset: 'Tibetan Bowl'
    instrument_ = instr;
  } else if (name == "BlowHole") {
    instrument_ = ctx->makeNode<cistk::BlowHoleNode>();
  } else if (name == "Bowed") {
    instrument_ = ctx->makeNode<cistk::BowedNode>();
  } else if( name == "Clarinet" ) {
    instrument_ = ctx->makeNode<cistk::ClarinetNode>();
  } else if( name == "Mandolin" ) {
    instrument_ = ctx->makeNode<cistk::MandolinNode>();
  } else if( name == "Plucked" ) {
    instrument_ = ctx->makeNode<cistk::PluckedNode>();
  } else if( name == "Saxofony" ) {
    instrument_ = ctx->makeNode<cistk::SaxofonyNode>();
  } else {
    CI_LOG_E("Unknown instrument_ name");
    // CI_ASSERT_NOT_REACHABLE();
  }

  // Set effect, if applicable
  if (effect_) instrument_ >> effect_ >> master_gain_;
  else         instrument_ >> master_gain_;

  master_gain_ >> ctx->getOutput();
}

/**
 * Assigns effect based on user selection.
 */
void CovidSonificationApp::HandleEffectSelected() {
  // Effects can only be applied if a sound is played.
  CI_ASSERT( instrument_ );

  master_gain_->disconnectAll();
  if( effect_ )
    effect_->disconnectAll();

  // Find the name of the specific effect
  const std::string& name = kEffectNames.at(effect_enum_selection);
  CI_LOG_I("Selecting effect '" << name << "'");

  auto ctx = cinder::audio::master();

  // Assign the effect accordingly
  if (name == "PRCRev") {
    effect_ = ctx->makeNode<cistk::PRCRevNode>();
  } else if(name == "JCRev") {
    effect_ = ctx->makeNode<cistk::JCRevNode>();
  } else if(name == "NRev") {
    effect_ = ctx->makeNode<cistk::NRevNode>();
  } else {
    CI_LOG_E( "Unknown effect name" );
    CI_ASSERT_NOT_REACHABLE();
  }

  // Play either instrument through effect
  if( instrument_ ) instrument_ >> effect_;

  effect_ >> master_gain_ >> ctx->getOutput();
}

/**
 * Handles dataset loading based on user selection.
 */
void CovidSonificationApp::HandleDataSelected() {
  current_data_.Reset();

  // Find the name of the specific dataset
  const std::string& name = kDatasetNames.at(dataset_selection_);
  CI_LOG_I("Selecting data: '" << name << "'");

  // Removing all params ensures that they maintain their order
  RemoveDataSonificationParams();

  if (name != "none") {
    for (size_t i = 1; i < kDatasetNames.size(); i++) {
      if (name == kDatasetNames.at(i)) {
        // Indices are offset by 1 because dataset names must
        // include "none" at the beginning.
        const std::string& filename = kDatasetFilepaths.at(i - 1);

        current_data_.ImportData(filename);
        region_names_ = current_data_.GetRegions();
        SetupDataSonificationParams();
        HandleRegionSelected();
        HandleUpperBoundSelected();

        break;
      }
    }
  }
}

/**
 * Assigns region based on user selection.
 */
void CovidSonificationApp::HandleRegionSelected() {
  current_region_ = current_data_.GetRegionDataByName(
      region_names_.at(region_selection_)
      );
}

/**
 * Assigns scale based on user selection.
 */
void CovidSonificationApp::HandleScaleSelected() {
  for (const Scale& scale : kScales) {
    if (kScaleNames.at(scale_selection_) == scale.scale_name) {
      current_scale_ = scale;
      break;
    }
  }
}

/**
 * Handles the upper bound of data visualization/sonification based on selection.
 */
void CovidSonificationApp::HandleUpperBoundSelected() {
  if (current_region_.GetRegionName() == "World") {
    max_amount_ = GetHighestAmountInData(current_data_, true);
    return;
  }

  if (max_value_selection_ == kRegionalMax) {
    max_amount_ = GetHighestRegionalAmount(current_region_);
  } else if (max_value_selection_ == kInternationalMax) {
    max_amount_ = GetHighestAmountInData(current_data_, false);
  } else if (max_value_selection_ == kCumulativeMax) {
    max_amount_ = GetHighestAmountInData(current_data_, true);
  }
}


/*
 * Private methods
 */

/**
 * Changes note based on instrument edge cases.
 */
bool CovidSonificationApp::HandleInstrumentSpecificNote(const cinder::vec2& pos) {
  // Not sure what this does yet...
  cinder::vec2 pos_normalized =
      glm::clamp(pos / cinder::vec2(getWindowSize()),
          cinder::vec2(0),
          cinder::vec2(1));

  auto mesh_2d = std::dynamic_pointer_cast<cistk::Mesh2DNode>(instrument_);
  if (mesh_2d) {
    mesh_2d->setInputPosition(pos_normalized.x, pos_normalized.y);
    instrument_->noteOn(0, 1.0f);
    return true;
  }

  auto modal_bar = std::dynamic_pointer_cast<cistk::ModalBarNode>(instrument_);
  if (modal_bar) {
    modal_bar->setStickHardness(pos_normalized.y);
    return false;
  }

  return false;
}

/**
 * Prints the audio graph to Cinder's console.
 */
void CovidSonificationApp::PrintAudioGraph() {
  CI_LOG_I("\n" << cinder::audio::master()->printGraphToString());
  std::cout << cinder::audio::master()->printGraphToString() << std::endl;
}

/**
 * Initializes the master gain.
 */
void CovidSonificationApp::SetupMasterGain() {
  // Add master gain as a parameter
  params_
      ->addParam<float>(
          "Master gain", [this](float value) { master_gain_->setValue(value); },
          [this] { return master_gain_->getValue(); })
      .min(0.0f)
      .max(1.0f)
      .step(0.05f);
}

/**
 * Initializes list of instruments as a parameter.
 */
void CovidSonificationApp::SetupInstruments() {
  // Set instruments as a parameter
  params_
      ->addParam("Instrument", kInstrumentNames,
                 (int*)&instrument_selection_)
      .keyDecr("[")
      .keyIncr("]")
      .updateFn([this] {
        HandleInstrumentsSelected();
        PrintAudioGraph();
      });
}

/**
 * Initializes list of instruments as a parameter.
 */
void CovidSonificationApp::SetupEffects() {
  // Add effects as parameter
  params_->addParam("Effect", kEffectNames, (int*)&effect_enum_selection)
      .keyDecr("o")
      .keyIncr("p")
      .updateFn([this] {
        HandleEffectSelected();
        PrintAudioGraph();
      });
}

/**
 * Initializes list of datasets as parameter.
 */
void CovidSonificationApp::SetupData() {
  params_->addParam("Data", kDatasetNames, (int*)&dataset_selection_)
      .keyDecr("t")
      .keyIncr("y")
      .updateFn([this] {
        HandleDataSelected();
        PrintAudioGraph();
      });
}

/**
 * Initializes list of regions as parameter when necessary.
 */
void CovidSonificationApp::SetupRegions() {
  // Data must be populated for regions to be setup
  if (current_data_.Empty()) return;

  // Sort regions by their value before adding as parameter
  std::sort(region_names_.begin(), region_names_.end(),
      [this] (const std::string& x, const std::string& y) {
              float x_max_amount = GetHighestRegionalAmount(
                  current_data_.GetRegionDataByName(x));
              float y_max_amount = GetHighestRegionalAmount(
                  current_data_.GetRegionDataByName(y));
              return x_max_amount < y_max_amount;
            });

  // Get the list of regions in descending order
  std::reverse(region_names_.begin(), region_names_.end());

  params_->addParam("Region", region_names_, (int*)&region_selection_)
      .keyDecr("g")
      .keyIncr("h")
      .updateFn([this] {
        HandleRegionSelected();
        PrintAudioGraph();
      });
}

/**
 * Sets up button to begin sonification playback.
 */
void CovidSonificationApp::SetupSonifyButton() {
  params_->addButton("Sonify!", [this] {
    SonifyData();
  });
}

/**
 * Sets up visualization scaling parameters.
 */
void CovidSonificationApp::SetupVisualizationScaling() {
  params_
      ->addParam<float>(
          "Visualization height scale",
          [this] (float value) { AssignHeightScaling(value); },
          [this] { return visualization_height_scaling_; })
      .min(kMinVisualizationScaling)
      .max(kMaxVisualizationScaling)
      .step(0.05f);

  params_
      ->addParam<float>(
          "Visualization width scale",
          [this] (float value) { AssignWidthScaling(value); },
          [this] { return visualization_width_scaling_; })
      .min(kMinVisualizationScaling)
      .max(kMaxVisualizationScaling)
      .step(0.05f);
}

/**
 * Sets up RGBA value parameters for visualization.
 */
void CovidSonificationApp::SetupRgba() {
  const float min = 0.0f;
  const float max = 1.0f;
  const float step = 0.01f;

  params_->addParam<float>(
          "R",
          [this] (float value) { red_ = value; },
          [this] { return red_; })
      .min(min)
      .max(max)
      .step(step);

  params_->addParam<float>(
      "G",
      [this] (float value) { green_ = value; },
      [this] { return green_; })
      .min(min)
      .max(max)
      .step(step);

  params_->addParam<float>(
      "B",
      [this] (float value) { blue_ = value; },
      [this] { return blue_; })
      .min(min)
      .max(max)
      .step(step);

  params_->addParam<float>(
      "Opacity",
      [this] (float value) { opacity_ = value; },
      [this] { return opacity_; })
      .min(min)
      .max(max)
      .step(step);
}

/**
 * Assigns height scaling based on new value.
 * @param new_scaling new value
 */
void CovidSonificationApp::AssignHeightScaling(float new_scaling) {
  if (new_scaling >= kMinVisualizationScaling &&
      new_scaling <= kMaxVisualizationScaling) {
    visualization_height_scaling_ = new_scaling;
  }
}

/**
 * Assigns width scaling based on new value.
 * @param new_scaling new value
 */
void CovidSonificationApp::AssignWidthScaling(float new_scaling) {
  if (new_scaling >= kMinVisualizationScaling &&
      new_scaling <= kMaxVisualizationScaling) {
    visualization_width_scaling_ = new_scaling;
  }
}

/**
 * Sets up visualization button as parameter.
 */
void CovidSonificationApp::SetupVisualizeButton() {
  params_->addButton("Toggle visualization", [this] {
    is_visualizing = !is_visualizing;
  });
}

/**
 * Sets the maximum MIDI pitch parameter.
 */
void CovidSonificationApp::SetupMaxMidiPitchParam() {
  params_
      ->addParam<size_t>(
          kMaxPitchParamName,  // sets name
          [this] (size_t value) { SetMaxPitch(value); },
          [this] { return max_midi_pitch_; })
      .min(kAbsoluteMinPitchMidi)
      .max(kAbsoluteMaxPitchMidi)
      .step(1);
}

/**
 * Sets the maximum pitch within bounds of MIDI pitch.
 * @param new_pitch new pitch
 */
void CovidSonificationApp::SetMaxPitch(size_t new_pitch) {
  bool valid_midi_num =
      new_pitch >= kAbsoluteMinPitchMidi && new_pitch <= kAbsoluteMaxPitchMidi;

  if (valid_midi_num && new_pitch >= min_midi_pitch_)
    max_midi_pitch_ = new_pitch;
}

/**
 * Sets the minimum MIDI pitch parameter.
 */
void CovidSonificationApp::SetupMinMidiPitchParam() {
  params_
      ->addParam<size_t>(
          kMinPitchParamName,  // sets name
          [this] (size_t value) { SetMinPitch(value); },
          [this] { return min_midi_pitch_; })
      .min(kAbsoluteMinPitchMidi)
      .max(kAbsoluteMaxPitchMidi)
      .step(1);
}

/**
 * Sets the minimum pitch within bounds of MIDI pitch.
 * @param new_pitch new pitch
 */
void CovidSonificationApp::SetMinPitch(size_t new_pitch) {
  bool valid_midi_num =
      new_pitch >= kAbsoluteMinPitchMidi && new_pitch <= kAbsoluteMaxPitchMidi;

  if (valid_midi_num && new_pitch <= max_midi_pitch_)
    min_midi_pitch_ = new_pitch;
}

/**
 * Sets up the list of available scales as a parameter.
 */
void CovidSonificationApp::SetupScale() {
  params_->addParam("Scale", kScaleNames, (int*)&scale_selection_)
      .updateFn([this] {
        HandleScaleSelected();
        PrintAudioGraph();
      });
}

/**
 * Sets the BPM of playback as a parameter.
 */
void CovidSonificationApp::SetupBpm() {
  params_
      ->addParam<int>(
          "BPM",
          [this] (int value) { AssignBpm(value); },
          [this] { return bpm_; })
      .min(kMinBpm)
      .max(kMaxBpm)
      .step(1);
}

/**
 * Sets the upper bound of visualization/sonification playback as a parameter.
 */
void CovidSonificationApp::SetupUpperBound() {
  params_
      ->addParam("Upper bound", kMaxValueSettingNames, (int*)&max_value_selection_)
      .keyDecr("z")
      .keyIncr("x")
      .updateFn([this] {
        HandleUpperBoundSelected();
        PrintAudioGraph();
      });
}

/**
 * Assigns the BPM of playback within set bounds.
 * @param set_bpm new BPM
 */
void CovidSonificationApp::AssignBpm(size_t set_bpm) {
  if (set_bpm >= kMinBpm && set_bpm <= kMaxBpm) bpm_ = set_bpm;
}

/**
 * Sets up all data sonification params in order.
 */
void CovidSonificationApp::SetupDataSonificationParams() {
  SetupRegions();
  SetupBpm();
  SetupUpperBound();
  SetupVisualizeButton();
  SetupVisualizationScaling();
  SetupRgba();
  SetupSonifyButton();
}

/**
 * Removes all data sonification params in order.
 */
void CovidSonificationApp::RemoveDataSonificationParams() {
  params_->removeParam("Region");
  params_->removeParam("BPM");
  params_->removeParam("Upper bound");
  params_->removeParam("Toggle visualization");
  params_->removeParam("Visualization height scale");
  params_->removeParam("Visualization width scale");
  params_->removeParam("R");
  params_->removeParam("G");
  params_->removeParam("B");
  params_->removeParam("Opacity");
  params_->removeParam("Sonify!");
}

/**
 * Changes app state for update() to begin sonification playback.
 */
void CovidSonificationApp::SonifyData() {
  if (dataset_selection_ == 0) return;

  HandleUpperBoundSelected();  // assign max amount

  int ms = ConvertBpmToMilliseconds(bpm_);
  interval = std::chrono::milliseconds(ms);

  current_date_index_ = 0;

  in_sonification_playback = true;
}

/**
 * Displays the centered directions.
 */
void CovidSonificationApp::DisplayDirections() {
  std::string directions =
      "Select a dataset and press 'Sonify' to hear how it sounds.\n"
      "Change the settings as desired.";

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {500, 200};
  const cinder::Color color = cinder::Color::white();

  ShowText(directions, color, size, center);
}

/**
 * Displays pitch information.
 */
void CovidSonificationApp::DisplayPitch() {
  std::stringstream pitch_message;
  if (current_midi_pitch_ > kAbsoluteMaxPitchMidi) {
    pitch_message << "Click anywhere to play a note!";
  } else {
    pitch_message << "Current MIDI pitch: " << current_midi_pitch_;
  }

  const cinder::vec2 location = {getWindowSize().x - 300, 130};
  const cinder::ivec2 size = {500, 50};
  const cinder::Color color = cinder::Color::white();

  ShowText(pitch_message.str(), color, size, location);
}

/**
 * Displays the currently selected dataset.
 */
void CovidSonificationApp::DisplayCurrentDataset() {
  // No display necessary if no dataset is selected
  if (dataset_selection_ == 0) return;

  std::string prefix = in_sonification_playback ? "Sonifying: " : "Selected : ";

  std::stringstream current_dataset_message;
  current_dataset_message << prefix
                          << kDatasetNames.at(dataset_selection_) << " of "
                          << current_region_.GetRegionName();
  const cinder::ivec2 size = {1000, 50};
  const cinder::vec2 location = {getWindowCenter().x, 75};

  ShowText(current_dataset_message.str(),
           cinder::Color::white(),
           size, location);
}

/**
 * Displays the current data during sonification playback.
 */
void CovidSonificationApp::DisplayCurrentNoteData() {
  if (dataset_selection_ == 0) return;

  std::string label = kDatasetNames.at(dataset_selection_);
  std::stringstream amount_display;

  // Display alternate message if no data is available for the current date
  if (current_amount_ == coviddata::kNullAmount) {
    amount_display << "No data";
  } else {
    amount_display << current_amount_ << " " << label;
  }

  // Construct display message
  std::stringstream note_data_message;
  note_data_message << current_date_ << ": \n" << amount_display.str();

  const cinder::ivec2 size = {600, 200};
  const cinder::vec2 location = {getWindowSize().x - 300, 75};

  ShowText(note_data_message.str(), cinder::Color::white(), size, location);
}

/**
 * Displays whether visualization is toggled on or off.
 */
void CovidSonificationApp::DisplayVisualizationToggle() {
  if (dataset_selection_ == 0) return;

  std::stringstream message;
  message << "Visualization: " << (is_visualizing ? "ON" : "OFF");
  const cinder::ivec2 size = {600, 200};
  const cinder::vec2 location = {getWindowCenter().x, 130};

  ShowText(message.str(), cinder::Color::white(), size, location);
}

/**
 * Draws points representing the data being sonified.
 */
void CovidSonificationApp::DrawNoteData() {
  const size_t point_size = 3;
  cinder::gl::color(cinder::ColorA(red_, green_, blue_, opacity_));  // red

  // Draw previous data points
  for (size_t i = 0; i < current_date_index_; i++) {
    float amount =
        current_region_.GetAmountAtDate(current_region_.GetDates().at(i));

    // Skip this data point if the data is unavailable
    if (amount == coviddata::kNullAmount) continue;

    cinder::gl::drawSolidCircle(
        ConvertDataPointToPosition(i, amount),
        point_size);
  }
}

/**
 * Finds the highest data point within a regional dataset.
 * @param rd regional data
 * @return highest amount
 */
float CovidSonificationApp::GetHighestRegionalAmount(
    const coviddata::RegionData& rd) {
  // Find highest note of regional dataset
  float max_amount = 0;

  for (const std::string& date : rd.GetDates()) {
    float amount = rd.GetAmountAtDate(date);
    if (amount > max_amount) max_amount = amount;
  }

  return max_amount;
}

/**
 * Returns highest amount of all regional data in a dataset
 * @param ds dataset
 * @param include_world whether to include the world
 * @return highest amount in dataset
 */
float CovidSonificationApp::GetHighestAmountInData(const coviddata::DataSet& ds,
                                                 bool include_world) {
  float max_amount = 0;

  for (const std::string& region_name : ds.GetRegions()) {
    // Skips world if specified by user
    if (!include_world && region_name == "World") continue;

    coviddata::RegionData rd = ds.GetRegionDataByName(region_name);
    float max_regional_amount = GetHighestRegionalAmount(rd);

    if (max_regional_amount > max_amount) max_amount = max_regional_amount;
  }

  return max_amount;
}

/**
 * Stops the currently played note.
 */
void CovidSonificationApp::StopNote() {
  if (instrument_) {
    instrument_->noteOff(0.5);
  }
}

/**
 * Displays text with given parameters.
 * @param text text to display
 * @param color color of text
 * @param size size of text box
 * @param loc location of text on screen
 */
void CovidSonificationApp::ShowText(const std::string& text,
                                    const cinder::Color& color,
                                    const glm::ivec2& size,
                                    const glm::vec2& loc) {
  cinder::gl::color(color);

  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 30))
      .size(size)
      .color(color)
      .backgroundColor(cinder::ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

/**
 * Converts BPM to millisecond interval between beats as interval
 * @param bpm beats per minute
 * @return interval in milliseconds
 */
int CovidSonificationApp::ConvertBpmToMilliseconds(int bpm) {
  const int seconds_per_minute = 60;
  const int ms_per_second = 1000;
  return (seconds_per_minute * ms_per_second) / bpm;
}

/**
 * Converts a datapoint to a position on screen based on maximum data point
 * @param date_index index of date within dates list
 * @param amount amount to sonify
 * @return position on screen to place visualization point
 */
cinder::vec2 CovidSonificationApp::ConvertDataPointToPosition(size_t date_index,
                                                              float amount) {
  float total_width_empty = (1.0f - visualization_width_scaling_);
  float total_height_empty = (1.0f - visualization_height_scaling_);

  // Map date index to the screen length position and find converted value
  int x = std::lroundf(cinder::lmap(
      (float)date_index,
      (float)0,
      (float)current_region_.GetDates().size(),
      0.0f + (float)getWindowWidth() * (total_width_empty / 2.0f),
      (float)getWindowWidth() *
          (visualization_width_scaling_ + total_width_empty / 2)
      ));

  // Map min/max MIDI pitch to the screen height position and find converted value
  int y = std::lroundf(cinder::lmap(
      amount,  // value to map
      0.0f,
      (float)max_amount_,
      (float)getWindowHeight() *
          (visualization_height_scaling_ + total_height_empty / 2),
      0.0f + (total_height_empty / 2) * (float)getWindowHeight()
      ));

  // Total window space occupied by visualization is centered
  return {x, y};
}

}  // namespace covidsonifapp
