/*!
 * Copyright 2019-2020 XGBoost contributors
 * \file observer.h
 */
#ifndef XGBOOST_COMMON_OBSERVER_H_
#define XGBOOST_COMMON_OBSERVER_H_

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "xgboost/host_device_vector.h"
#include "xgboost/parameter.h"
#include "xgboost/json.h"
#include "xgboost/base.h"
#include "xgboost/tree_model.h"

#if (defined(XGBOOST_STRICT_R_MODE) && XGBOOST_STRICT_R_MODE == 1 ) || defined(_LIBCPP_SGX_AUXILIARY)
#define OBSERVER_PRINT LOG(INFO)
#define OBSERVER_ENDL ""
#define OBSERVER_NEWLINE ""
#else  // defined(XGBOOST_STRICT_R_MODE) && XGBOOST_STRICT_R_MODE == 1
#define OBSERVER_PRINT std::cout << std::setprecision(17)
#define OBSERVER_ENDL std::endl
#define OBSERVER_NEWLINE "\n"
#endif  // defined(XGBOOST_STRICT_R_MODE) && XGBOOST_STRICT_R_MODE == 1

namespace xgboost {
/*\brief  An observer for logging internal data structures.
 *
 *  This class is designed to be `diff` tool friendly, which means it uses plain
 *  `std::cout` for printing to avoid the time information emitted by `LOG(DEBUG)` or
 *  similiar facilities. Exception: use `LOG(INFO)` for the R package, to comply
 *  with CRAN policy.
 */
class TrainingObserver {
#if defined(XGBOOST_USE_DEBUG_OUTPUT)
  bool constexpr static kObserve {true};
#else
  bool constexpr static kObserve {false};
#endif  // defined(XGBOOST_USE_DEBUG_OUTPUT)

 public:
  void Update(int32_t iter) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << "Iter: " << iter << OBSERVER_ENDL;
#endif
  }
  /*\brief Observe tree. */
  void Observe(RegTree const& tree) {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << "Tree:" << OBSERVER_ENDL;
#endif
    Json j_tree {Object()};
    tree.SaveModel(&j_tree);
    std::string str;
    Json::Dump(j_tree, &str, true);
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << str << OBSERVER_ENDL;
#endif
  }
  /*\brief Observe tree. */
  void Observe(RegTree const* p_tree) {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
    auto const& tree = *p_tree;
    this->Observe(tree);
  }
  /*\brief Observe data hosted by `std::vector'. */
  template <typename T>
  void Observe(std::vector<T> const& h_vec, std::string name,
               size_t n = std::numeric_limits<std::size_t>::max()) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << "Procedure: " << name << OBSERVER_ENDL;
#endif
    for (size_t i = 0; i < h_vec.size(); ++i) {
#ifndef _LIBCPP_SGX_AUXILIARY
      OBSERVER_PRINT << h_vec[i] << ", ";
#endif
      if (i % 8 == 0) {
#ifndef _LIBCPP_SGX_AUXILIARY
        OBSERVER_PRINT << OBSERVER_NEWLINE;
#endif
      }
      if ((i + 1) == n) {
        break;
      }
    }
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << OBSERVER_ENDL;
#endif
  }
  /*\brief Observe data hosted by `HostDeviceVector'. */
  template <typename T>
  void Observe(HostDeviceVector<T> const& vec, std::string name,
               size_t n = std::numeric_limits<std::size_t>::max()) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
    auto const& h_vec = vec.HostVector();
    this->Observe(h_vec, name, n);
  }
  template <typename T>
  void Observe(HostDeviceVector<T>* vec, std::string name,
               size_t n = std::numeric_limits<std::size_t>::max()) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }
    this->Observe(*vec, name, n);
  }

  /*\brief Observe objects with `XGBoostParamer' type. */
  template <typename Parameter,
            typename std::enable_if<
              std::is_base_of<XGBoostParameter<Parameter>, Parameter>::value>::type* = nullptr>
  void Observe(const Parameter &p, std::string name) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }

    Json obj {toJson(p)};
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << "Parameter: " << name << ":\n" << obj << OBSERVER_ENDL;
#endif
  }
  /*\brief Observe parameters provided by users. */
  void Observe(Args const& args) const {
    if (XGBOOST_EXPECT(!kObserve, true)) { return; }

    for (auto kv : args) {
#ifndef _LIBCPP_SGX_AUXILIARY
      OBSERVER_PRINT << kv.first << ": " << kv.second << OBSERVER_NEWLINE;
#endif
    }
#ifndef _LIBCPP_SGX_AUXILIARY
    OBSERVER_PRINT << OBSERVER_ENDL;
#endif
  }

  /*\brief Get a global instance. */
  static TrainingObserver& Instance() {
    static TrainingObserver observer;
    return observer;
  }
};
}  // namespace xgboost
#endif  // XGBOOST_COMMON_OBSERVER_H_
