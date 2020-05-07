//
// Created by Renzo Ledesma on 4/25/2020.
//

#ifndef FINALPROJECT_CSVPARSER_H
#define FINALPROJECT_CSVPARSER_H

#include <string>
#include <vector>

namespace coviddata {

/**
 * Reads and stores values from CSV files
 */
class CsvParser {
 public:
  /**
   * Internal structure that stores values of a single CSV line
   */
  struct Line {
    std::vector<std::string> values{};
  };

  CsvParser(const std::string& filename);
  std::vector<Line>& GetLines();
  bool Fail() const;

 private:
  std::vector<Line> lines_{};
  static std::vector<std::string> SplitStr(const std::string& s,
                                           const std::string& delimiter);
  bool fail_;
};

} // namespace coviddata

#endif  // FINALPROJECT_CSVPARSER_H
