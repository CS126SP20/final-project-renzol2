//
// Created by Renzo Ledesma on 4/25/2020.
//

#ifndef FINALPROJECT_DATASET_H
#define FINALPROJECT_DATASET_H

#include "datapoint.h"

#include <string>
#include <vector>

namespace coviddata {

class DataSet {
 public:
  DataSet();
  void ImportData(const std::string& filename);
 private:
  std::vector<coviddata::DataPoint> data_;
};

} // namespace coviddata

#endif  // FINALPROJECT_DATASET_H
