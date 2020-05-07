//
// Created by Renzo Ledesma on 4/27/2020.
//

#include "coviddata/regiondata.h"

namespace coviddata {

/**
 * Default constructor
 */
RegionData::RegionData()
    : date_to_amount_(), region_name_(), region_index_(0) {}

/**
 * Assigns specific region name and index.
 * @param region_name name of region
 * @param region_index index of region
 */
RegionData::RegionData(const std::string& region_name, size_t region_index)
    : date_to_amount_() {
  region_name_ = region_name;
  region_index_ = region_index;
}

/**
 * Stores a desired date/amount key/value pair.
 * @param date date key
 * @param amount amount value
 */
void RegionData::SetAmountToDate(const std::string& date, float amount) {
  date_to_amount_.insert({date, amount});
}

/**
 * Retrieves an amount at a specific date.
 * @param date date key to retrieve from
 * @return retrieved amount value
 */
float RegionData::GetAmountAtDate(const std::string& date) const {
  return date_to_amount_.at(date);
}

/**
 * Retrieves region name.
 * @return region name
 */
std::string RegionData::GetRegionName() const { return region_name_; }

/**
 * Retrieves region index.
 * @return region index
 */
size_t RegionData::GetRegionIndex() const { return region_index_; }

/**
 * Returns the amount of key/value data/amount pairs stored
 * @return number of date/amount pairs
 */
size_t RegionData::Size() const { return date_to_amount_.size(); }

/**
 * Returns list of all dates contained in set
 * @return vector of dates as strings
 */
std::vector<std::string> RegionData::GetDates() const {
  std::vector<std::string> dates;

  for (auto & it : date_to_amount_) {
    dates.push_back(it.first);
  }

  return dates;
}

}  // namespace coviddata
