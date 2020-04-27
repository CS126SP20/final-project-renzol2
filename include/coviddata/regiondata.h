//
// Created by Renzo Ledesma on 4/27/2020.
//

#ifndef FINALPROJECT_REGIONDATA_H
#define FINALPROJECT_REGIONDATA_H

#include "coviddata/datapoint.h"

#include <map>

namespace coviddata {

/*
 * Holds COVID-19 data for a specific region (global/country-specific)
 */
class RegionData {
 public:
  RegionData() = default;
  RegionData(const std::string& region_name, size_t region_index);
  void SetAmountToDate(const std::string& date, int amount);
  int GetAmountAtDate(const std::string& date) const;
  std::string GetRegionName() const;
  size_t GetRegionIndex() const;

 private:
  std::string region_name_;
  size_t region_index_;
  std::map<std::string, int> date_to_amount_;
};

}  // namespace coviddata

#endif  // FINALPROJECT_REGIONDATA_H
