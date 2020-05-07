//
// Created by Renzo Ledesma on 4/25/2020.
//

#include "coviddata/csvparser.h"

#include <iostream>
#include <fstream>

namespace coviddata {

/**
 * Parses through a single CSV file and stores the values
 * @param filename name of file to parse
 */
CsvParser::CsvParser(const std::string& filename) : fail_(false) {
  std::ifstream csv_file(filename);

  if (csv_file.fail()) {
    fail_ = true;
    return;
  }

  std::istream& csv_filestream = csv_file;
  std::string csv_line;

  while (std::getline(csv_filestream, csv_line)) {
    Line l;
    l.values = (SplitStr(csv_line, ","));
    lines_.push_back(l);
  }
}

/**
 * Retrieves all lines read from a file
 * @return lines as a vector reference
 */
std::vector<CsvParser::Line>& CsvParser::GetLines() { return lines_; }

// Implementation taken from an answer on StackOverflow:
// "Parse (split) a string in C++ using string delimiter (standard C++)"
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c#comment44856986_14266139
/**
 * Splits a string based off a delimiter value; used to separate values from commas
 * @param s string to split
 * @param delimiter string that determines split point
 * @return vector of strings split by delimiter
 */
std::vector<std::string> CsvParser::SplitStr(const std::string& s, const std::string& delimiter) {
  std::vector<std::string> split_string;
  size_t last = 0;
  size_t next = 0;

  while ((next = s.find(delimiter, last)) != std::string::npos) {
    split_string.push_back(s.substr(last, next - last));
    last = next + 1;
  }
  split_string.push_back(s.substr(last));

  return split_string;
}

/**
 * Returns true if file failed to read
 * @return true if file failed to read
 */
bool CsvParser::Fail() const { return fail_; }

} // namespace coviddata
