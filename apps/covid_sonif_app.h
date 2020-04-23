// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>


namespace covidsonifapp {

class CovidSonificationApp : public cinder::app::App {
 public:
  CovidSonificationApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
};

}  // namespace covidsonifapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
