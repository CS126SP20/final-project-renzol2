// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <string>

#include "coviddata/csvparser.h"

TEST_CASE("CsvParser reads .csv file correctly") {
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