// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "cinder/audio/audio.h"
#include "../../blocks/Cinder-Stk/src/cistk/CinderStk.h"

namespace covidaudio {

// NOTE: These may have to be non-const, verify later
const size_t kInstrumentEnumSelection = 13;
const size_t kGeneratorEnumSelection = 0;
const size_t kEffectEnumSelection = 7;

// Plays audio using engines provided by Synthesis ToolKit (STK)
class AudioEngine {
 public:
  void SetupParams();
  void MakeNote(const cinder::vec2 &pos);
  float QuantizePitch(const cinder::vec2 &pos);
  void HandleInstrumentsSelected();
  void HandleGeneratorSelected();
  void HandleEffectSelected();
  void HandleInstrumentSpecificNote(const cinder::vec2 &pos);
  void PrintAudioGraph();

  ci::audio::GainNodeRef master_gain_;

  // Note: the input will either be the instrument or generator
  cistk::InstrumentNodeRef instrument_;
  cistk::GeneratorNodeRef generator_;
  cistk::EffectNodeRef effect_;

  ci::audio::GainNodeRef generator_gain_;

  cinder::params::InterfaceGlRef params_;
  std::vector<string> instrument_enum_names_;
  std::vector<string> generator_enum_names_;
  std::vector<string> effect_enum_names_;
};

}  // namespace covidaudio


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
