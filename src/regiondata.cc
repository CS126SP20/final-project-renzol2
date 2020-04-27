//
// Created by Renzo Ledesma on 4/27/2020.
//

#include "coviddata/regiondata.h"

namespace coviddata {

RegionData::RegionData(const std::string& region_name, size_t region_index) {
  region_name_ = region_name;
  region_index_ = region_index;
}

void RegionData::SetAmountToDate(const std::string& date, int amount) {
  date_to_amount_[date] = amount;
}

int RegionData::GetAmountAtDate(const std::string& date) const {
  return date_to_amount_.at(date);
}

std::string RegionData::GetRegionName() const { return region_name_; }

size_t RegionData::GetRegionIndex() const { return region_index_; }

}  // namespace coviddata
