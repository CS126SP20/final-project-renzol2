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

  generator_enum_selection_ = 0;  // set generator to "none"
  generator_ = nullptr;
  generator_gain_->setValue(0);

  // Get the name of selected instrument and notify user
  std::string name = instrument_enum_names_.at(instrument_enum_selection_);
  CI_LOG_I("Selecting instrument '" << name << "'" );

  // Set instrument accordingly
  auto ctx = cinder::audio::master();
  if (name == "BandedWG") {
    auto instr = ctx->makeNode<cistk::BandedWGNode>();
    instr->setPreset(3);  // preset: 'Tibetan Bowl'
    instrument_ = instr;
  } else if (name == "BlowBotl") {
    instrument_ = ctx->makeNode<cistk::BlowBotlNode>();
  } else if (name == "BlowHole") {
    instrument_ = ctx->makeNode<cistk::BlowHoleNode>();
  } else if (name == "Bowed") {
    instrument_ = ctx->makeNode<cistk::BowedNode>();
  } else if( name == "Brass" ) {
    instrument_ = ctx->makeNode<cistk::BrassNode>();
  } else if( name == "Clarinet" ) {
    instrument_ = ctx->makeNode<cistk::ClarinetNode>();
  } else if( name == "Drummer" ) {
    instrument_ = ctx->makeNode<cistk::DrummerNode>();
  } else if( name == "Flute" ) {
    instrument_ = ctx->makeNode<cistk::FluteNode>();
  } else if( name == "Mandolin" ) {
    instrument_ = ctx->makeNode<cistk::MandolinNode>();
  } else if (name == "Mesh2D") {
    auto instr = ctx->makeNode<cistk::Mesh2DNode>();
    instr->setDecay(0.9f);
    instrument_ = instr;
  } else if( name == "ModalBar" ) {
    auto instr = ctx->makeNode<cistk::ModalBarNode>();
    instr->setPreset( 1 ); // preset: 'Vibraphone'
    instrument_ = instr;
  } else if( name == "Moog" ) {
    instrument_ = ctx->makeNode<cistk::MoogNode>();
  } else if( name == "Plucked" ) {
    instrument_ = ctx->makeNode<cistk::PluckedNode>();
  } else if( name == "Resonate" ) {
    instrument_ = ctx->makeNode<cistk::ResonateNode>();
  } else if( name == "Saxofony" ) {
    instrument_ = ctx->makeNode<cistk::SaxofonyNode>();
  } else if( name == "Shakers" ) {
    instrument_ = ctx->makeNode<cistk::ShakersNode>();
  } else if( name == "Simple" ) {
    instrument_ = ctx->makeNode<cistk::SimpleNode>();
  } else if( name == "Sitar" ) {
    instrument_ = ctx->makeNode<cistk::SitarNode>();
  } else if( name == "StifKarp" ) {
    instrument_ = ctx->makeNode<cistk::StifKarpNode>();
  } else if( name == "VoicForm" ) {
    auto instr = ctx->makeNode<cistk::VoicFormNode>();
    if( ! instr->setPhoneme( "ooo" ) ) {
      CI_LOG_W( "phoneme not found" );
    }
    instr->quiet();
    instrument_ = instr;
  } else if( name == "BeeThree" ) {
    instrument_ = ctx->makeNode<cistk::BeeThreeNode>();
  } else if( name == "FMVoices" ) {
    instrument_ = ctx->makeNode<cistk::FMVoicesNode>();
  } else if( name == "HevyMetl" ) {
    instrument_ = ctx->makeNode<cistk::HevyMetlNode>();
  } else if( name == "PercFlut" ) {
    instrument_ = ctx->makeNode<cistk::PercFlutNode>();
  } else if( name == "Rhodey" ) {
    instrument_ = ctx->makeNode<cistk::RhodeyNode>();
  } else if( name == "TubeBell" ) {
    instrument_ = ctx->makeNode<cistk::TubeBellNode>();
  } else if( name == "Wurley" ) {
    instrument_ = ctx->makeNode<cistk::WurleyNode>();
  } else if( name == "Whistle" ) {
    instrument_ = ctx->makeNode<cistk::WhistleNode>();
  } else {
    CI_LOG_E("Unknown instrument name");
    CI_ASSERT_NOT_REACHABLE();
  }

  // Set effect, if applicable
  if (effect_) instrument_ >> effect_ >> master_gain_;
  else         instrument_ >> master_gain_;

  master_gain_ >> ctx->getOutput();
}

void CovidSonificationApp::HandleGeneratorSelected() {}

void CovidSonificationApp::HandleEffectSelected() {}

void CovidSonificationApp::HandleInstrumentSpecificNote(
    const cinder::vec2& pos) {}

void CovidSonificationApp::PrintAudioGraph() {
  CI_LOG_I("\n" << cinder::audio::master()->printGraphToString());
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
