// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "covid_sonif_app.h"

// According to the project guide, backtracking through the directory is
// supposedly incorrect. Need to figure what to do about my directory structure
#include "../blocks/Cinder-Stk/src/cistk/CinderStk.h"
#include "../blocks/Cinder-Stk/src/cistk/Util.h"

#include <cinder/app/App.h>
#include <cinder/audio/audio.h>


namespace covidsonifapp {

using cinder::app::KeyEvent;


MyApp::MyApp() { }

void MyApp::setup() {
  stk::Stk::setSampleRate(
      ci::audio::master()->getSampleRate());  // 1) set the sample rate
  cistk::initRawwavePath();                   // 2) configure the path to the rawwaves folder
  std::cout << "Success!" << std::endl;
}

void MyApp::update() { }

void MyApp::draw() { }

void MyApp::keyDown(KeyEvent event) { }

}  // namespace covidsonifapp
