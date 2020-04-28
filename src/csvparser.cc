//
// Created by Renzo Ledesma on 4/25/2020.
//

#include "coviddata/csvparser.h"

#include <iostream>
#include <fstream>

namespace coviddata {

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

std::vector<CsvParser::Line>& CsvParser::GetLines() { return lines_; }

// Implementation taken from an answer on StackOverflow:
// "Parse (split) a string in C++ using string delimiter (standard C++)"
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c#comment44856986_14266139
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

bool CsvParser::Fail() const { return fail_; }

} // namespace coviddata
