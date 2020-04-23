// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "covid_sonif_app.h"

// According to the project guide, backtracking through the directory is
// supposedly incorrect. Need to figure what to do about my directory structure
#include "../blocks/Cinder-Stk/src/cistk/CinderStk.h"
#include "../blocks/Cinder-Stk/src/cistk/Util.h"

#include <cinder/app/App.h>
#include <cinder/audio/audio.h>

#include <vector>

namespace covidsonifapp {

using cinder::app::KeyEvent;

CovidSonificationApp::CovidSonificationApp() { }

void CovidSonificationApp::setup() {
  stk::Stk::setSampleRate(
      ci::audio::master()->getSampleRate());  // 1) set the sample rate
  cistk::initRawwavePath();                   // 2) configure the path to the rawwaves folder
  std::cout << "Success!" << std::endl;
}

void CovidSonificationApp::update() { }

void CovidSonificationApp::draw() { }

void CovidSonificationApp::keyDown(KeyEvent event) { }

}  // namespace covidsonifapp
