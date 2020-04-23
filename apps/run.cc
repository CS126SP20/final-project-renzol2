// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "covid_sonif_app.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace covidsonifapp {

const int kSamples = 8;
const int kWidth = 1920;
const int kHeight = 1000;

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("COVID-19 Data Sonification");
}

}  // namespace covidsonifapp


// This is a macro that runs the application.
CINDER_APP(covidsonifapp::CovidSonificationApp,
           RendererGl(RendererGl::Options().msaa(covidsonifapp::kSamples)),
           covidsonifapp::SetUp)
