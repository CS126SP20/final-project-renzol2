//
// Created by Renzo Ledesma on 4/27/2020.
//

#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include "coviddata/dataset.h"

TEST_CASE("DataSet successfully reads in .csv files") {
  std::string test_file;
  coviddata::DataSet data_set;

  SECTION("DataSet throws exception on reading a nonexistent file") {
    test_file = "doesn't exist";
    REQUIRE_THROWS(data_set.ImportData(test_file));
  }

  SECTION("DataSet does not throw exception upon reading existing test file") {
    test_file =
        R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\test.csv)";
    REQUIRE_NOTHROW(data_set.ImportData(test_file));
  }

  SECTION("DataSet does not throw exception upon reading actual COVID-19 DATA file") {
    test_file =
        R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\total_cases.csv)";
    REQUIRE_NOTHROW(data_set.ImportData(test_file));
  }
}

/*
 * assets/data/test.csv
 *
 * date,World,United States
 * 2019-12-31,0,0
 * 2020-01-01,20,10
 * 2020-01-02,40,15
 *
 * regional index starts at World=1
 */

TEST_CASE("DataSet accurately parses data in test file") {
  const std::string test_file = R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\test.csv)";
  const std::vector<std::string> actual_regions = {"World", "United States"};
  const std::vector<std::string> dates = {"2019-12-31",
                                          "2020-01-01",
                                          "2020-01-02"};

  coviddata::DataSet data_set;
  REQUIRE_NOTHROW(data_set.ImportData(test_file));

  SECTION("DataSet of test file should have two regions") {
    REQUIRE(data_set.Size() == 2);
  }

  SECTION("DataSet should have same regions as in file") {

    REQUIRE(data_set.GetRegions().at(0) == actual_regions.at(0));
    REQUIRE(data_set.GetRegions().at(1) == actual_regions.at(1));
  }

  SECTION("DataSet should have accurate regional data for first region") {
    coviddata::RegionData world_data =
        data_set.GetRegionDataByName(actual_regions.at(0));

    // Check metadata
    REQUIRE(world_data.GetRegionName() == actual_regions.at(0));
    // Regional index starts at 1
    REQUIRE(world_data.GetRegionIndex() == 1);

    // Check amounts by dates
    std::vector<int> actual_world_amounts = {0, 20, 40};
    REQUIRE(world_data.Size() == actual_world_amounts.size());
    for (size_t i = 0; i < actual_world_amounts.size(); i++) {
      REQUIRE(world_data.GetAmountAtDate(dates.at(i)) == actual_world_amounts.at(i));
    }
  }

  SECTION("DataSet should have accurate regional data for second region") {
    coviddata::RegionData us_data =
        data_set.GetRegionDataByName(actual_regions.at(1));

    // Check metadata
    REQUIRE(us_data.GetRegionName() == actual_regions.at(1));
    // Regional index starts at 1
    REQUIRE(us_data.GetRegionIndex() == 2);

    // Check amounts by dates
    std::vector<int> actual_us_amounts = {0, 10, 15};
    REQUIRE(us_data.Size() == actual_us_amounts.size());
    for (size_t i = 0; i < actual_us_amounts.size(); i++) {
      REQUIRE(us_data.GetAmountAtDate(dates.at(i)) == actual_us_amounts.at(i));
    }
  }
}

TEST_CASE("TEST") {
  coviddata::DataSet data_set;
  data_set.ImportData(
      R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\total_cases.csv)");


}