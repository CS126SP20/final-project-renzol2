//
// Created by Renzo Ledesma on 4/25/2020.
//

#ifndef FINALPROJECT_DATASET_H
#define FINALPROJECT_DATASET_H

#include <map>
#include <string>
#include <vector>

#include "csvparser.h"
#include "regiondata.h"

namespace coviddata {

const int kNullAmount = -1;

/**
 * Represents global and country-specific data for COVID-19.
 *
 * DataSet takes in a .csv file with following format:
 *   Date,country1,country2,country3... (header)
 *   2020-05-06,100,0,2
 *   ...
 * and stores those values by region for ease of retrieval.
 */
class DataSet {
 public:
  DataSet();
  void ImportData(const std::string& filename);
  size_t Size();
  coviddata::RegionData& GetRegionDataByName(
      const std::string& region_name) const;
  std::vector<std::string>& GetRegions() const;
  void Reset();
  bool Empty();
 private:
  std::map<std::string, coviddata::RegionData> region_to_data_;
  std::vector<std::string> regions_;
  std::string data_type_;
 private:
  void InitializeRegionalData(coviddata::CsvParser::Line header);
  static float GetNumberFromString(const std::string& num_string);
};

} // namespace coviddata

#endif  // FINALPROJECT_DATASET_H
