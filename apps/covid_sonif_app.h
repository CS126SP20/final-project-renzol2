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
 public:
  struct Scale {  // put this somewhere else...
    std::string scale_name;
    std::vector<float> scale_degrees;
    size_t scale_length;
  };

  CovidSonificationApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void mouseDrag(cinder::app::MouseEvent event) override;
  void mouseUp(cinder::app::MouseEvent event) override;

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
  void RemoveDataSonificationParams();

  static int GetHighestRegionalAmount(const coviddata::RegionData& rd);
  static int GetHighestAmountInData(const coviddata::DataSet &ds,
                                    bool include_world);

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
  // 21 and 108 are the lowest/highest keys on the piano keyboard, respectively
  size_t max_midi_pitch_ = 108;
  size_t min_midi_pitch_ = 21;

  size_t instrument_enum_selection_ = 13;
  size_t generator_enum_selection_ = 0;
  size_t effect_enum_selection = 7;
  size_t dataset_selection_ = 0;
  size_t region_selection_ = 0;
  size_t scale_selection_ = 0;

  float last_freq_ = 0;
};

}  // namespace covidsonifapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
