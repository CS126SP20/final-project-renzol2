// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <string>
#include <vector>

#include "coviddata/csvparser.h"

TEST_CASE("CsvParser reads .csv file correctly") {
  SECTION("Parser throws exception with non-existent file") {
    std::string bad_file = "doesn't exist";
    coviddata::CsvParser parser(bad_file);
    REQUIRE(parser.Fail());
  }

  std::string filename = R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\new_cases.csv)";
  coviddata::CsvParser parser(filename);

  SECTION("Parser reads in correct number of lines") {
    const size_t lines_in_new_cases = 118;
    REQUIRE(parser.GetLines().size() == lines_in_new_cases);
  }

  SECTION("Parser reads correct number of entries per line") {
    const size_t entries_per_line = 208;

    using Line = coviddata::CsvParser::Line;
    for (const Line& line : parser.GetLines())
      REQUIRE(line.values.size() == entries_per_line);
  }
}

/*
 * assets/data/test.csv
 *
 * date,World,United States
 * 2019-12-31,0,0
 * 2020-01-01,20,10
 * 2020-01-02,40,15
 */
TEST_CASE("CsvParser accurately parses individual values") {
  std::string filename = R"(C:\Program Files\Cinder\my-projects\final-project-renzol2\tests\assets\data\test.csv)";
  coviddata::CsvParser parser(filename);

  using Line = coviddata::CsvParser::Line;

  // Actual values: date,World,United States
  SECTION("Parser reads values in header correctly") {
    Line header = parser.GetLines().at(0);
    const std::vector<std::string> actual_header_values = {"date",
                                                           "World",
                                                           "United States"};

    REQUIRE(header.values.size() == actual_header_values.size());

    // Check every value in the header line
    for (size_t col = 0; col < header.values.size(); col++) {
      REQUIRE(header.values.at(col) == actual_header_values.at(col));
    }
  }

  // Actual values: 2019-12-31,0,0
  SECTION("Parser reads values in second line correctly") {
    Line second_line = parser.GetLines().at(1);
    const std::vector<std::string> actual_values = {"2019-12-31",
                                                    "0",
                                                    "0"};

    REQUIRE(second_line.values.size() == actual_values.size());

    // Check every value in the header line
    for (size_t col = 0; col < second_line.values.size(); col++) {
      REQUIRE(second_line.values.at(col) == actual_values.at(col));
    }
  }
}