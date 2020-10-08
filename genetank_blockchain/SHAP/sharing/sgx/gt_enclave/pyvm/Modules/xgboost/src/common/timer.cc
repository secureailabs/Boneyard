/*!
 * Copyright by Contributors 2019
 */
//#include <rabit/rabit.h>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <vector>
#include <sstream>
#include "timer.h"
#include "xgboost/json.h"

#if defined(XGBOOST_USE_NVTX)
#include <nvToolsExt.h>
#endif  // defined(XGBOOST_USE_NVTX)

namespace xgboost {
namespace common {

void Monitor::Start(std::string const &name) {
  if (ConsoleLogger::ShouldLog(ConsoleLogger::LV::kDebug)) {
    auto &stats = statistics_map_[name];
    stats.timer.Start();
#if defined(XGBOOST_USE_NVTX)
    std::string nvtx_name = label_ + "::" + name;
    stats.nvtx_id = nvtxRangeStartA(nvtx_name.c_str());
#endif  // defined(XGBOOST_USE_NVTX)
  }
}

void Monitor::Stop(const std::string &name) {
  if (ConsoleLogger::ShouldLog(ConsoleLogger::LV::kDebug)) {
    auto &stats = statistics_map_[name];
    stats.timer.Stop();
    stats.count++;
#if defined(XGBOOST_USE_NVTX)
    nvtxRangeEnd(stats.nvtx_id);
#endif  // defined(XGBOOST_USE_NVTX)
  }
}

std::vector<Monitor::StatMap> Monitor::CollectFromOtherRanks() const {
  // Since other nodes might have started timers that this one haven't, so
  // we can't simply call all reduce.
  size_t const world_size = 1;//rabit::GetWorldSize();
  size_t const rank = 0;//rabit::GetRank();

  // It's much easier to work with rabit if we have a string serialization.  So we go with
  // json.
  Json j_statistic { Object() };
  j_statistic["rank"] = Integer(rank);
  j_statistic["statistic"] = Object();

  auto& statistic = j_statistic["statistic"];
  for (auto const& kv : statistics_map_) {
    statistic[kv.first] = Object();
    auto& j_pair = statistic[kv.first];
    j_pair["count"] = Integer(kv.second.count);
#ifdef _LIBCPP_SGX_AUXILIARY
    j_pair["elapsed"] = Integer(static_cast<int64_t>(0));
#else
    j_pair["elapsed"] = Integer(static_cast<int64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
        kv.second.timer.elapsed).count()));
#endif
  }

#ifndef _LIBCPP_SGX_AUXILIARY
  std::stringstream ss;
  Json::Dump(j_statistic, &ss);
  std::string const str { ss.str() };
#else
  std::string str;
  Json::Dump(j_statistic, &str);
#endif

  size_t str_size = str.size();
  //rabit::Allreduce<rabit::op::Max>(&str_size, 1);
  std::string buffer;
  buffer.resize(str_size);

  // vector storing stat from all workers
  std::vector<StatMap> world(world_size);

  // Actually only rank 0 is printing.
  for (size_t i = 0; i < world_size; ++i) {
    std::copy(str.cbegin(), str.cend(), buffer.begin());
    //rabit::Broadcast(&buffer, i);
    auto j_other = Json::Load(StringView{buffer.c_str(), buffer.size()});
    auto& other = world[i];

    auto const& j_statistic = get<Object>(j_other["statistic"]);

    for (auto const& kv : j_statistic) {
      std::string const& timer_name = kv.first;
      auto const& pair = kv.second;
      other[timer_name] = {get<Integer>(pair["count"]), get<Integer>(pair["elapsed"])};
    }

    // FIXME(trivialfis): How to ask rabit to block here?
  }

  return world;
}

void Monitor::PrintStatistics(StatMap const& statistics) const {
  for (auto &kv : statistics) {
    if (kv.second.first == 0) {
#ifndef _LIBCPP_SGX_AUXILIARY
      	    LOG(WARNING) <<
          "Timer for " << kv.first << " did not get stopped properly.";
#endif	   
      continue;
    }
    #ifndef _LIBCPP_SGX_AUXILIARY

    LOG(CONSOLE) << kv.first << ": " << static_cast<double>(kv.second.second) / 1e+6
                 << "s, " << kv.second.first << " calls @ "
                 << kv.second.second
                 << "us" << std::endl;
    #endif
  }
}

void Monitor::Print() const {
  if (!ConsoleLogger::ShouldLog(ConsoleLogger::LV::kDebug)) { return; }

  bool is_distributed = false;//rabit::IsDistributed();

  if (is_distributed) {
    auto world = this->CollectFromOtherRanks();
    // rank zero is in charge of printing
    //if (rabit::GetRank() == 0) {
      #ifndef _LIBCPP_SGX_AUXILIARY
	LOG(CONSOLE) << "======== Monitor: " << label_ << " ========";
	#endif
      for (size_t i = 0; i < world.size(); ++i) {
#ifndef _LIBCPP_SGX_AUXILIARY
      	      LOG(CONSOLE) << "From rank: " << i << ": " << std::endl;
	#endif
        auto const& statistic = world[i];
        this->PrintStatistics(statistic);
      }
    //}
  } else {
    StatMap stat_map;
    for (auto const& kv : statistics_map_) {
#ifndef _LIBCPP_SGX_AUXILIARY
  	    stat_map[kv.first] = std::make_pair(
          kv.second.count, std::chrono::duration_cast<std::chrono::microseconds>(
              kv.second.timer.elapsed).count());
#else
  	     stat_map[kv.first] = std::make_pair(
          kv.second.count, 0);
#endif
    }
    #ifndef _LIBCPP_SGX_AUXILIARY
  	    LOG(CONSOLE) << "======== Monitor: " << label_ << " ========";
    #endif
    this->PrintStatistics(stat_map);
  }
}

}  // namespace common
}  // namespace xgboost
