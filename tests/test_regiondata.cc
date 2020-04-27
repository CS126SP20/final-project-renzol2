//
// Created by Renzo Ledesma on 4/27/2020.
//

#include <catch2/catch.hpp>
#include <string>
#include <sstream>

#include "coviddata/regiondata.h"

TEST_CASE("Regional data reads and writes correctly") {
  const std::string region_name = "USA";
  const size_t region_index = 1;

  coviddata::RegionData region_data(region_name, region_index);

  SECTION("Region name is correct") {
    REQUIRE(region_data.GetRegionName() == region_name);
  }

  SECTION("Region index is correct") {
    REQUIRE(region_data.GetRegionIndex() == region_index);
  }

  SECTION("Writing and reading back singular entry to region data succeeds") {
    const std::string date = "1/1/2001";
    const int amount = 1000;

    region_data.SetAmountToDate(date, amount);

    REQUIRE(region_data.GetAmountAtDate(date) == amount);
  }

  SECTION("Writing and reading back n entries succeeds") {
    const size_t n_entries = 28;

    for (size_t amount = 0; amount < n_entries; amount++) {
      std::stringstream date;
      size_t day_num = amount + 1;

      // Set the day appropriately
      std::stringstream day;
      std::string filler = day_num < 10 ? "0" : "";
      day << filler << day_num;

      // Construct date string
      date << "4/" << day.str() << "/20";

      region_data.SetAmountToDate(date.str(), amount);
      REQUIRE(region_data.GetAmountAtDate(date.str()) == amount);
    }
  }
}