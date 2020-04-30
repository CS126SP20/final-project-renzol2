//
// Created by Renzo Ledesma on 4/27/2020.
//

#include "coviddata/regiondata.h"

namespace coviddata {

RegionData::RegionData()
    : date_to_amount_(), region_name_(), region_index_(0) {}

RegionData::RegionData(const std::string& region_name, size_t region_index)
    : date_to_amount_() {
  region_name_ = region_name;
  region_index_ = region_index;
}

void RegionData::SetAmountToDate(const std::string& date, int amount) {
  date_to_amount_.insert({date, amount});
}

int RegionData::GetAmountAtDate(const std::string& date) const {
  return date_to_amount_.at(date);
}

std::string RegionData::GetRegionName() const { return region_name_; }

size_t RegionData::GetRegionIndex() const { return region_index_; }

size_t RegionData::Size() const { return date_to_amount_.size(); }

std::vector<std::string> RegionData::GetDates() const {
  std::vector<std::string> dates;

  for (auto & it : date_to_amount_) {
    dates.push_back(it.first);
  }

  return dates;
}

}  // namespace coviddata
