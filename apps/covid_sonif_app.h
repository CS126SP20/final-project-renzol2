// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

// According to the project guide, backtracking through the directory is
// supposedly incorrect. Need to figure what to do about my directory structure
#include "cinder/audio/audio.h"
#include "../blocks/Cinder-Stk/src/cistk/CinderStk.h"
#include "../include/coviddata/dataset.h"

#include <string>
#include <vector>

namespace covidsonifapp {

/*
 * Application that sonifies COVID-19 Data
 *
 * Audio synthesis using source code from the StkTestApp:
 * https://github.com/richardeakin/Cinder-Stk/blob/master/samples/StkTest/src/StkTestApp.cpp
 */
class CovidSonificationApp : public cinder::app::App {
 /*
  * Constructor and Cinder Application Overriden functions
  */
 public:
  CovidSonificationApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void mouseDrag(cinder::app::MouseEvent event) override;
  void mouseUp(cinder::app::MouseEvent event) override;

 /*
  * Public functions
  */
 public:
  void SetupParams();
  void MakeNote(const cinder::vec2 &pos);
  void MakeNoteFromAmount(int amount, int max_amount);
  float QuantizePitch(const cinder::vec2 &pos);
  float QuantizePitchFromAmount(int amount, int max_amount);
  void StopNote();
  void HandleInstrumentsSelected();
  void HandleGeneratorSelected();
  void HandleEffectSelected();
  bool HandleInstrumentSpecificNote(const cinder::vec2 &pos);
  void HandleDataSelected();
  void HandleRegionSelected();
  void HandleScaleSelected();
  void SonifyData();
  static void PrintAudioGraph();
  void DisplayTitle();

 /*
  * Private helper functions
  */
 private:
  void SetupMasterGain();
  void SetupInstruments();
  void SetupGenerators();
  void SetupEffects();
  void SetupData();
  void SetupRegions();
  void SetupMaxMidiPitchParam();
  void SetMaxPitch(size_t new_pitch);
  void SetupMinMidiPitchParam();
  void SetMinPitch(size_t new_pitch);
  void SetupScale();
  void SetupBpm();
  void SetupSonifyButton();
  void AssignBpm(size_t set_bpm);
  void SetupDataSonificationParams();
  void RemoveDataSonificationParams();
  void ShowText(const std::string& text, const cinder::Color& color,
                const cinder::ivec2& size, const cinder::vec2& loc);
  static int GetHighestRegionalAmount(const coviddata::RegionData& rd);
  static int GetHighestAmountInData(const coviddata::DataSet &ds,
                                    bool include_world);
  static int ConvertBpmToMilliseconds(int bpm);

 /*
  * Internal structure class for scales
  */
 private:
  struct Scale {
    std::string scale_name;
    std::vector<float> scale_degrees;
    size_t scale_length;
  };

 /*
  * Instance variables
  */
 private:
  ci::audio::GainNodeRef master_gain_;

  // Note: the input will either be the instrument or generator
  cistk::InstrumentNodeRef instrument_;
  cistk::GeneratorNodeRef generator_;
  cistk::EffectNodeRef effect_;
  Scale current_scale_;
  coviddata::DataSet current_data_;
  coviddata::RegionData current_region_;

  ci::audio::GainNodeRef generator_gain_;

  cinder::params::InterfaceGlRef params_;

  // Instance variables for COVID-19 Data
  std::vector<std::string> region_names_;
  int max_amount_ = 0;

  // Instance variables for sonification parameters
  size_t max_midi_pitch_ = 48;  // C4
  size_t min_midi_pitch_ = 96;  // C8
  int bpm_ = 240;

  // Current selection of parameters for OpenGL params
  size_t instrument_enum_selection_ = 13;
  size_t generator_enum_selection_ = 0;
  size_t effect_enum_selection = 7;
  size_t dataset_selection_ = 0;
  size_t region_selection_ = 0;
  size_t scale_selection_ = 0;

  float last_freq_ = 0;

 /*
  * Names for parameters
  */
 private:
  const std::string kMaxPitchParamName = "Max pitch (MIDI)";
  const std::string kMinPitchParamName = "Min pitch (MIDI)";

  const std::vector<std::string> kInstrumentNames = {
      "none",     "BandedWG", "BlowBotl", "BlowHole", "Bowed",    "Brass",
      "Clarinet", "Drummer",  "Flute",    "Mandolin", "Mesh2D",   "ModalBar",
      "Moog",     "Plucked",  "Resonate", "Saxofony", "Shakers",  "Simple",
      "Sitar",    "StifKarp", "VoicForm", "Whistle",  "BeeThree", "FMVoices",
      "HevyMetl", "PercFlut", "Rhodey",   "TubeBell", "Wurley"
  };

  const std::vector<std::string> kGeneratorNames = {
      "none", "Blit", "Granulate"
  };

  const std::vector<std::string> kEffectNames = {
      "none", "Echo", "Chorus", "PitShift", "LentPitShift",
      "PRCRev", "JCRev", "NRev", "FreeVerb"
  };

  const std::vector<std::string> kDataFileNames = {
      R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\assets\data\new_cases.csv)",
      R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\assets\data\new_deaths.csv)",
      R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\assets\data\total_cases.csv)",
      R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\assets\data\total_deaths.csv)"
  };

  const std::vector<std::string> kDatasetNames = {
      "none", "New cases", "New deaths", "Total cases", "Total deaths"
  };

 /*
  * Scale information
  */
 private:
  const std::vector<std::string> kScaleNames = {
      "Major", "Minor", "Pentatonic", "Whole tone", "Chromatic"
  };

  const std::vector<float> kMajorScale = {0, 2, 4, 5, 7, 9, 11};
  const std::vector<float> kMinorScale = {0, 2, 3, 5, 7, 8, 10};
  const std::vector<float> kPentatonicScale = {0, 2, 4, 7, 9};
  const std::vector<float> kWholeToneScale = {0, 2, 4, 6, 8, 10};
  const std::vector<float> kChromaticScale = {0, 1, 2, 3, 4,  5,
                                              6, 7, 8, 9, 10, 11};


  const std::vector<covidsonifapp::CovidSonificationApp::Scale> kScales = {
      {kScaleNames.at(0), kMajorScale,      kMajorScale.size()},
      {kScaleNames.at(1), kMinorScale,      kMinorScale.size()},
      {kScaleNames.at(2), kPentatonicScale, kPentatonicScale.size()},
      {kScaleNames.at(3), kWholeToneScale,  kWholeToneScale.size()},
      {kScaleNames.at(4), kChromaticScale,  kChromaticScale.size()}
  };
};

}  // namespace covidsonifapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
