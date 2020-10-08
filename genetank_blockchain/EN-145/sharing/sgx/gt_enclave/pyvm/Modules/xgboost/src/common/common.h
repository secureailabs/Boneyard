/*!
 * Copyright 2015 by Contributors
 * \file common.h
 * \brief Common utilities
 */
#ifndef XGBOOST_COMMON_COMMON_H_
#define XGBOOST_COMMON_COMMON_H_

#include <vector>
#include <string>
#include <sstream>

namespace xgboost {
namespace common {
/*!
 * \brief Split a string by delimiter
 * \param s String to be splitted.
 * \param delim The delimiter.
 */
inline std::vector<std::string> Split(const std::string& s, char delim) {
  std::string item;
  //std::istringstream is(s);
  std::vector<std::string> ret;
  //while (std::getline(is, item, delim)) {
  //  ret.push_back(item);
  //}
  size_t prev=0;
  size_t pos=0;
  while((pos=s.find(delim))!=std::string::npos){
	  ret.push_back(s.substr(prev, pos));
	  prev = pos+1;
  }
  return ret;
}

// simple routine to convert any data to string
template<typename T>
inline std::string ToString(const T& data) {
  //std::ostringstream os;
  //os << data;
  std::string os;
  os+=std::to_string(data);
  return os;
}

}  // namespace common
}  // namespace xgboost
#endif  // XGBOOST_COMMON_COMMON_H_
