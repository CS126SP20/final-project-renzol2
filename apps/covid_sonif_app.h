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

#include <string>
#include <vector>

namespace covidsonifapp {

class CovidSonificationApp : public cinder::app::App {
 public:
  CovidSonificationApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

 public:
  void SetupParams();
  void MakeNote(const cinder::vec2 &pos);
  float QuantizePitch(const cinder::vec2 &pos);
  void HandleInstrumentsSelected();
  void HandleGeneratorSelected();
  void HandleEffectSelected();
  void HandleInstrumentSpecificNote(const cinder::vec2 &pos);
  void PrintAudioGraph();

 private:
  void SetupMasterGain();
  void SetupInstruments();
  void SetupGenerators();
  void SetupEffects();

 private:
  ci::audio::GainNodeRef master_gain_;

  // Note: the input will either be the instrument or generator
  cistk::InstrumentNodeRef instrument_;
  cistk::GeneratorNodeRef generator_;
  cistk::EffectNodeRef effect_;

  ci::audio::GainNodeRef generator_gain_;

  cinder::params::InterfaceGlRef params_;
  std::vector<std::string> instrument_enum_names_;
  std::vector<std::string> generator_enum_names_;
  std::vector<std::string> effect_enum_names_;

  size_t instrument_enum_selection_ = 13;
  size_t generator_enum_selection_ = 0;
  size_t effect_enum_selection = 7;
};

}  // namespace covidsonifapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
