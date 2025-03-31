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

#ifndef ROMEA_CORE_COMMON__LEXICAL__LEXICALCAST_HPP_
#define ROMEA_CORE_COMMON__LEXICAL__LEXICALCAST_HPP_

// std
#include <algorithm>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace romea
{
namespace core
{

template<typename T>
T lexical_cast(const std::string & str, size_t & pos);

template<>
inline double lexical_cast<double>(const std::string & str, size_t & pos)
{
  return std::stod(str, &pos);
}

template<>
inline float lexical_cast<float>(const std::string & str, size_t & pos)
{
  return std::stof(str, &pos);
}

template<>
inline int lexical_cast<int>(const std::string & str, size_t & pos)
{
  return std::stoi(str, &pos);
}

template<>
inline long lexical_cast<long>(const std::string & str, size_t & pos)
{
  return std::stol(str, &pos);
}

template<>
inline long double lexical_cast<long double>(const std::string & str, size_t & pos)
{
  return std::stold(str, &pos);
}

template<>
inline long long lexical_cast<long long>(const std::string & str, size_t & pos)
{
  return std::stoll(str, &pos);
}

template<>
inline unsigned long lexical_cast<unsigned long>(const std::string & str, size_t & pos)
{
  return std::stoul(str, &pos);
}

template<>
inline unsigned long long lexical_cast<unsigned long long>(const std::string & str, size_t & pos)
{
  return std::stoull(str, &pos);
}

template<>
inline bool lexical_cast<bool>(const std::string & str, size_t & pos)
{
  std::string s(str);

  bool result{};
  std::istringstream iss{s};
  iss >> std::noboolalpha >> result;
  if (iss.fail()) {
    std::transform(begin(s), end(s), begin(s), [](unsigned char c) { return std::tolower(c); });
    iss = std::istringstream{s};
    iss >> std::boolalpha >> result;
  }
  // iss.tellg() returns -1 for integer. This one returns the real number of characters
  pos = iss.rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
  return result;
}

template<typename T>
T lexical_cast(const std::string & str)
{
  size_t pos;
  T value = lexical_cast<T>(str, pos);
  if (pos != str.size()) {
    throw(std::invalid_argument("lexical_cast"));
  }
  return value;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__LEXICAL__LEXICALCAST_HPP_
