// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef ROMEA_CORE_COMMON__LOG__SIMPLEFILELOGGER_HPP_
#define ROMEA_CORE_COMMON__LOG__SIMPLEFILELOGGER_HPP_


// std
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>

namespace romea
{
namespace core
{

class SimpleFileLogger
{
  struct Entry
  {
    Entry(
      const std::string & name,
      const std::string & value)
    : name(name),
      value(value)
    {
    }

    std::string name;
    std::string value;
  };

public:
  SimpleFileLogger()
  : rowEntries_(),
    columnNames_(),
    separator_(),
    file_()
  {
  }

  SimpleFileLogger(const std::string & filename, std::string separator = ",")
  {
    init(filename, separator);
  }

  void init(
    const std::string & filename,
    std::string separator = ",")
  {
    separator_ = separator;
    file_.open(filename);
    if (!file_.is_open()) {
      throw std::runtime_error("Cannot open log file : " + filename);
    }
  }

  template<typename T>
  void addEntry(const std::string & name, const T & value)
  {
    if (file_.is_open()) {
      rowEntries_.emplace_back(name, std::to_string(value));
    }
  }

  void writeRow()
  {
    if (file_.is_open()) {
      if (columnNames_.empty()) {
        file_ << "%";
        for (size_t n = 0; n < rowEntries_.size(); ++n) {
          columnNames_.push_back(rowEntries_[n].name);
          file_ << "(" << n + 1 << ")" << rowEntries_[n].name << separator_;
        }
      } else {
        assert(columnNames_.size() == rowEntries_.size());
        file_ << std::setprecision(10);
        for (size_t n = 0; n < rowEntries_.size(); ++n) {
          assert(columnNames_[n].compare(rowEntries_[n].name) == 0);
          file_ << rowEntries_[n].value << separator_;
        }
      }

      file_ << "\n";
      file_.flush();
      rowEntries_.clear();
    }
  }

private:
  std::vector<Entry> rowEntries_;
  std::vector<std::string> columnNames_;
  std::string separator_;
  std::ofstream file_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__LOG__SIMPLEFILELOGGER_HPP_
