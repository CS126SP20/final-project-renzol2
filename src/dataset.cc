//
// Created by Renzo Ledesma on 4/25/2020.
//

#include "coviddata/csvparser.h"
#include "coviddata/dataset.h"

#include <sstream>
#include <stdexcept>


namespace coviddata {

DataSet::DataSet() : region_to_data_(), regions_() { }

void DataSet::ImportData(const std::string& filename) {
  // Reset before assigning data
  Reset();

  // Get data from .csv file and assign its filename
  coviddata::CsvParser parser(filename);
  if (parser.Fail()) throw std::invalid_argument("File does not exist");

  data_type_ = filename;

  using Line = CsvParser::Line;
  Line header = parser.GetLines().at(0);

  // Get all regions from header line
  InitializeRegionalData(header);
  if (region_to_data_.empty())
    throw std::invalid_argument("File does not contain regions in header");

  // Extract data date-by-date region-by-region
  for (size_t line_num = 1; line_num < parser.GetLines().size(); line_num++) {
    Line current_line = parser.GetLines().at(line_num);

    for (size_t region_index = 1;
         region_index < current_line.values.size();
         region_index++) {
      // Find the particular region and its corresponding data
      std::string region_name = header.values.at(region_index);
      RegionData& region_data = region_to_data_.at(region_name);

      // Update the data with information from the line in the .csv file
      std::string date = current_line.values.at(0);
      int amount = GetIntFromString(current_line.values.at(region_index));
      region_data.SetAmountToDate(date, amount);
    }
  }
}

size_t DataSet::Size() { return region_to_data_.size(); }

coviddata::RegionData& DataSet::GetRegionDataByName(
    const std::string& region_name) const {
  const coviddata::RegionData& region_data = region_to_data_.at(region_name);
  return const_cast<RegionData&>(region_data);  // double check this...
}

std::vector<std::string>& DataSet::GetRegions() const {
  return (std::vector<std::string>&)regions_;
}

void DataSet::Reset() {
  region_to_data_.clear();
  regions_.clear();
  data_type_ = std::string();
}

bool DataSet::Empty() {
  return region_to_data_.empty() && regions_.empty() && data_type_.empty();
}

/**
 * Extracts all regions from the header of .csv file
 *
 * Regions are listed in .csv file as follows:
 * ex. date, region_1, region_2, ..., region_n
 *
 * @param header header line of .csv file
 */
void DataSet::InitializeRegionalData(coviddata::CsvParser::Line header) {
  // Start at index 1 because first line is always the date
  for (size_t region_index = 1; region_index < header.values.size();
       region_index++) {
    // Extract region name and create data for each region
    std::string region_name = header.values.at(region_index);
    regions_.push_back(region_name);

    RegionData region_data(region_name, region_index);
    region_to_data_.insert({region_name, region_data});
  }
}


int DataSet::GetIntFromString(const std::string& integer_string) {
  // Empty entries cannot be converted into integers
  if (integer_string.empty())
    return kNullAmount;

  std::stringstream int_string(integer_string);
  int integer;
  int_string >> integer;

  return integer;
}


}  // namespace coviddata
