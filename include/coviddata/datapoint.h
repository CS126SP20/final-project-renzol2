// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_DATAPOINT_EXAMPLE_H_
#define FINALPROJECT_DATAPOINT_EXAMPLE_H_

#include <string>

namespace coviddata {

// Represents a single data point (x, y)
struct DataPoint {
  std::string label_x, label_y;
  int x, y;
};

} // namespace coviddata

#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
