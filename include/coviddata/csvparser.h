//
// Created by Renzo Ledesma on 4/25/2020.
//

#ifndef FINALPROJECT_CSVPARSER_H
#define FINALPROJECT_CSVPARSER_H

#include "datapoint.h"

#include <string>
#include <vector>

namespace coviddata {

class CsvParser {
 public:
  struct Line {
    std::vector<std::string> values;
  };

  CsvParser(const std::string& filename);
  std::vector<Line> ExportData();

 private:
  std::vector<Line> lines_;
  static std::vector<std::string> SplitStr(const std::string& s,
                                           const std::string& delimiter);
};

} // namespace coviddata

#endif  // FINALPROJECT_CSVPARSER_H
