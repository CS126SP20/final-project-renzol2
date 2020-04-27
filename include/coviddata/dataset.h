//
// Created by Renzo Ledesma on 4/25/2020.
//

#ifndef FINALPROJECT_DATASET_H
#define FINALPROJECT_DATASET_H

#include <string>
#include <map>

#include "csvparser.h"
#include "datapoint.h"
#include "regiondata.h"

namespace coviddata {

/*
 * Represents global and country-specific data for COVID-19.
 */
class DataSet {
 public:
  DataSet();
  void ImportData(const std::string& filename);
 private:
  std::map<std::string, coviddata::RegionData> region_to_data_;
  std::string data_type_;
 private:
  void InitializeRegionalData(coviddata::CsvParser::Line header);
  static int GetIntFromString(const std::string& integer_string);
};

} // namespace coviddata

#endif  // FINALPROJECT_DATASET_H
