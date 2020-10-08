/*!
 *  Copyright (c) 2015 by Contributors
 * \file uri_spec.h
 * \brief common specification of sugars in URI
 *    string passed to dmlc Create functions
 *    such as local file cache
 * \author Tianqi Chen
 */
#ifndef DMLC_IO_URI_SPEC_H_
#define DMLC_IO_URI_SPEC_H_

#include <dmlc/common.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

namespace dmlc {
namespace io {
/*!
 * \brief some super set of URI
 *  that allows sugars to be passed around
 *  Example:
 *
 *  hdfs:///mylibsvm/?format=libsvm&clabel=0#mycache-file.
 */
class URISpec {
 public:
  /*! \brief the real URI */
  std::string uri;
  /*! \brief arguments in the URL */
  std::map<std::string, std::string> args;
  /*! \brief the path to cache file */
  std::string cache_file;
  /*!
   * \brief constructor.
   * \param uri The raw uri string.
   * \param part_index The parition index of the part.
   * \param num_parts total number of parts.
   */
  explicit URISpec(const std::string& uri,
                   unsigned part_index,
                   unsigned num_parts) {
    std::vector<std::string> name_cache = Split(uri, '#');

    if (name_cache.size() == 2) {
#ifndef _LIBCPP_SGX_NO_IOSTREAMS
      std::ostringstream os;
      os << name_cache[1];
      if (num_parts != 1) {
        os << ".split" << num_parts << ".part" << part_index;
      }
      this->cache_file = os.str();
#else
      std::string os = name_cache[1];
      if (num_parts != 1) {
        os.append(".split") + std::to_string(num_parts) + ".part" + std::to_string(part_index);
      }
      this->cache_file = os;
#endif
    } else {
      CHECK_EQ(name_cache.size(), 1U)
          << "only one `#` is allowed in file path for cachefile specification";
    }
    std::vector<std::string> name_args = Split(name_cache[0], '?');
    if (name_args.size() == 2) {
      std::vector<std::string> arg_list = Split(name_args[1], '&');
      for (size_t i = 0; i < arg_list.size(); ++i) {
#ifndef _LIBCPP_SGX_NO_IOSTREAMS
        std::istringstream is(arg_list[i]);
        std::pair<std::string, std::string> kv;
        CHECK(std::getline(is, kv.first, '=')) << "Invalid uri argument format"
          << " for key in arg " << i + 1;
        CHECK(std::getline(is, kv.second)) << "Invalid uri argument format"
          << " for value in arg " << i + 1;
        this->args.insert(kv);
#else
        std::string is(arg_list[i]);
        std::pair<std::string, std::string> kv;
        std::vector<std::string> ret = Split(is, '=');
        kv.first  = ret[0];
        kv.second = ret[1];
        this->args.insert(kv);
#endif
      }
    } else {
      CHECK_EQ(name_args.size(), 1U)
          << "only one `#` is allowed in file path for cachefile specification";
    }
    this->uri = name_args[0];
  }
};
}  // namespace io
}  // namespace dmlc
#endif  // DMLC_IO_URI_SPEC_H_
