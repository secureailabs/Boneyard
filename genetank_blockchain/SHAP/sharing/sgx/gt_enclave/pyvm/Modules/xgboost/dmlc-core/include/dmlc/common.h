/*!
 *  Copyright (c) 2015 by Contributors
 * \file common.h
 * \brief defines some common utility function.
 */
#ifndef DMLC_COMMON_H_
#define DMLC_COMMON_H_

#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <utility>
#include "./logging.h"

namespace dmlc {
/*!
 * \brief Split a string by delimiter
 * \param s String to be splitted.
 * \param delim The delimiter.
 * \return a splitted vector of strings.
 */
inline std::vector<std::string> Split(const std::string& s, char delim) {
#ifndef _LIBCPP_SGX_NO_IOSTREAMS
  std::string item;
  std::istringstream is(s);
  std::vector<std::string> ret;
  while (std::getline(is, item, delim)) {
    ret.push_back(item);
  }
  return ret;
#else
  size_t start;
  size_t end = 0;
  std::vector<std::string> ret;
  while ((start = s.find_first_not_of(delim, end)) != std::string::npos) {
      end = s.find(delim, start);
      ret.push_back(s.substr(start, end - start));
  }
  return ret;
#endif
}

/*!
 * \brief hash an object and combines the key with previous keys
 */
template<typename T>
inline size_t HashCombine(size_t key, const T& value) {
  std::hash<T> hash_func;
  return key ^ (hash_func(value) + 0x9e3779b9 + (key << 6) + (key >> 2));
}

/*!
 * \brief specialize for size_t
 */
template<>
inline size_t HashCombine<size_t>(size_t key, const size_t& value) {
  return key ^ (value + 0x9e3779b9 + (key << 6) + (key >> 2));
}

/*!
 * \brief OMP Exception class catches, saves and rethrows exception from OMP blocks
 */
class OMPException {
 private:
  // exception_ptr member to store the exception
#if defined(_LIBCPP_SGX_NO_EXCEPTION_PTR)
  std::exception *omp_exception_ {nullptr};
#else
  std::exception_ptr omp_exception_;
#endif
  // mutex to be acquired during catch to set the exception_ptr
  std::mutex mutex_;

 public:
#if defined(_LIBCPP_SGX_NO_EXCEPTION_PTR)
  ~OMPException() {
    if (!omp_exception_) {
        delete omp_exception_;
        omp_exception_ = NULL;
    }
  }
#endif
  /*!
   * \brief Parallel OMP blocks should be placed within Run to save exception
   */
  template <typename Function, typename... Parameters>
  void Run(Function f, Parameters... params) {
    try {
      f(params...);
    } catch (dmlc::Error &ex) {
      std::lock_guard<std::mutex> lock(mutex_);
      if (!omp_exception_) {
#if defined(_LIBCPP_SGX_NO_EXCEPTION_PTR)
        omp_exception_ = new std::runtime_error(ex.what());
#else
        omp_exception_ = std::current_exception();
#endif
      }
    } catch (std::exception &ex) {
      std::lock_guard<std::mutex> lock(mutex_);
      if (!omp_exception_) {
#if defined(_LIBCPP_SGX_NO_EXCEPTION_PTR)
        omp_exception_ = new std::runtime_error(ex.what());
#else
        omp_exception_ = std::current_exception();
#endif

      }
    }
  }

  /*!
   * \brief should be called from the main thread to rethrow the exception
   */
  void Rethrow() {
#if defined(_LIBCPP_SGX_NO_EXCEPTION_PTR)
    if (this->omp_exception_) throw std::runtime_error(this->omp_exception_->what());
#else
    if (this->omp_exception_) std::rethrow_exception(this->omp_exception_);
#endif
  }
};

}  // namespace dmlc

#endif  // DMLC_COMMON_H_
