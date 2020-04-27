// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_DATAPOINT_EXAMPLE_H_
#define FINALPROJECT_DATAPOINT_EXAMPLE_H_

#include <string>
#include <vector>

namespace coviddata {

/*
 * Represents a single point of data in a COVID-19 data set.
 *
 * Each point has three attributes:
 * 1.) the DATE on which it was recorded
 * 2.) the PLACE (usually a country) in which it pertains to
 * 3.) the AMOUNT (i.e. data) that it holds
 *
 * The dataset it is located in is not recorded internally, though this
 * may change in future implementations.
 */
struct DataPoint {
  std::string date;
  std::string place;
  size_t amount;
};

} // namespace coviddata

#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
