// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "covid_sonif_app.h"

#include <iostream>

namespace covidsonifapp {


using cinder::app::KeyEvent;

CovidSonificationApp::CovidSonificationApp() { }

void CovidSonificationApp::setup() {
  auto ctx = cinder::audio::master();

  stk::Stk::setSampleRate(ctx->getSampleRate());
  cistk::initRawwavePath();

  master_gain_ = ctx->makeNode<cinder::audio::GainNode>(0.85f);
  master_gain_ >> ctx->getOutput();

  // Mapping generator gain to mouse y value
  generator_gain_ = ctx->makeNode<cinder::audio::GainNode>(0);

  SetupParams();
  HandleInstrumentsSelected();
  HandleEffectSelected();
  ctx->enable();

  PrintAudioGraph();
}

void CovidSonificationApp::update() { }

void CovidSonificationApp::draw() {
  cinder::gl::clear();
  params_->draw();
}

void CovidSonificationApp::keyDown(KeyEvent event) { }

void CovidSonificationApp::SetupParams() {
  // Define parameters
  params_ = cinder::params::InterfaceGl::create(
      getWindow(), "STK Params",
      cinder::app::toPixels(cinder::ivec2(300, 200)));

  // Setup all parameters for each instance variable
  SetupMasterGain();
  SetupInstruments();
  SetupGenerators();
  SetupEffects();
}

void CovidSonificationApp::MakeNote(const cinder::vec2& pos) {}

float CovidSonificationApp::QuantizePitch(const cinder::vec2& pos) { return 0; }

void CovidSonificationApp::HandleInstrumentsSelected() {
  // Disconnect all currently used instruments
  if (instrument_) instrument_->disconnectAll();
  if (generator_)  generator_->disconnectAll();


}

void CovidSonificationApp::HandleGeneratorSelected() {}

void CovidSonificationApp::HandleEffectSelected() {}

void CovidSonificationApp::HandleInstrumentSpecificNote(
    const cinder::vec2& pos) {}

void CovidSonificationApp::PrintAudioGraph() {
  std::cout << cinder::audio::master()->printGraphToString() << std::endl;
}

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

void CovidSonificationApp::SetupInstruments() {
  // Define instrument names
  // TODO: make this an actual enum
  instrument_enum_names_ = {
      "none",     "BandedWG", "BlowBotl", "BlowHole", "Bowed",    "Brass",
      "Clarinet", "Drummer",  "Flute",    "Mandolin", "Mesh2D",   "ModalBar",
      "Moog",     "Plucked",  "Resonate", "Saxofony", "Shakers",  "Simple",
      "Sitar",    "StifKarp", "VoicForm", "Whistle",  "BeeThree", "FMVoices",
      "HevyMetl", "PercFlut", "Rhodey",   "TubeBell", "Wurley"};

  // Set instruments as a parameter
  params_
      ->addParam("Instrument", instrument_enum_names_,
                 (int*)&instrument_enum_selection_)  // double check this line
      .keyDecr("[")
      .keyIncr("]")
      .updateFn([this] {
        HandleInstrumentsSelected();
        PrintAudioGraph();
      });
}

void CovidSonificationApp::SetupGenerators() {
  // Define generator names
  generator_enum_names_ = {
      "none", "Blit", "Granulate"
  };

  // Set generator as a parameter
  params_
      ->addParam("Generator", generator_enum_names_,
                 (int*)&generator_enum_selection_)
      .keyDecr("u")
      .keyIncr("i")
      .updateFn([this] {
        HandleGeneratorSelected();
        PrintAudioGraph();
      });
}

void CovidSonificationApp::SetupEffects() {
  // Define effects names
  effect_enum_names_ = {
      "none", "Echo", "Chorus", "PitShift", "LentPitShift",
      "PRCRev", "JCRev", "NRev", "FreeVerb"
  };

  // Add effects as parameter
  params_->addParam("Effect", effect_enum_names_, (int*)&effect_enum_selection)
      .keyDecr("o")
      .keyIncr("p")
      .updateFn([this] {
        HandleEffectSelected();
        PrintAudioGraph();
      });
}


}  // namespace covidsonifapp
