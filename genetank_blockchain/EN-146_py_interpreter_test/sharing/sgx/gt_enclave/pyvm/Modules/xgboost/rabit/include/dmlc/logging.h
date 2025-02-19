/*!
 *  Copyright (c) 2015 by Contributors
 * \file logging.h
 * \brief defines logging macros of dmlc
 *  allows use of GLOG, fall back to internal
 *  implementation when disabled
 */
#ifndef DMLC_LOGGING_H_
#define DMLC_LOGGING_H_
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>
#include "./base.h"

//#if DMLC_LOG_STACK_TRACE
//#include <execinfo.h>
//#endif

namespace dmlc {
/*!
 * \brief exception class that will be thrown by
 *  default logger if DMLC_LOG_FATAL_THROW == 1
 */
struct Error : public std::runtime_error {
  /*!
   * \brief constructor
   * \param s the error message
   */
  explicit Error(const std::string &s) : std::runtime_error(s) {}
};
}  // namespace dmlc

#if DMLC_USE_GLOG
#include <glog/logging.h>

namespace dmlc {
/*!
 * \brief optionally redirect to google's init log
 * \param argv0 The arguments.
 */
inline void InitLogging(const char* argv0) {
  google::InitGoogleLogging(argv0);
}
}  // namespace dmlc

#else
// use a light version of glog
#include <assert.h>
#include <iostream>
#include <sstream>
#include <ctime>

#if defined(_MSC_VER)
#pragma warning(disable : 4722)
#endif

namespace dmlc {
inline void InitLogging(const char*) {
  // DO NOTHING
}

class LogCheckError {
 public:
  LogCheckError() : str(nullptr) {}
  explicit LogCheckError(const std::string& str_) : str(new std::string(str_)) {}
  ~LogCheckError() { if (str != nullptr) delete str; }
  operator bool() {return str != nullptr; }
  std::string* str;
};

#define DEFINE_CHECK_FUNC(name, op)                               \
  template <typename X, typename Y>                               \
  inline LogCheckError LogCheck##name(const X& x, const Y& y) {   \
    if (x op y) return LogCheckError();                           \
    std::string os;                                        \
    os+=" (";                                              \
    os+=x;                                                 \
    os+=" vs.";                                           \
	os+=y;                                          \
	os+=") ";/* CHECK_XX(x, y) requires x and y can be serialized to string. Use CHECK(x OP y) otherwise. NOLINT(*) */ \
    return LogCheckError(os);                               \
  }                                                               \
  inline LogCheckError LogCheck##name(int x, int y) {             \
    return LogCheck##name<int, int>(x, y);                        \
  }

#define CHECK_BINARY_OP(name, op, x, y)                               \
  if (dmlc::LogCheckError _check_err = dmlc::LogCheck##name(x, y))    \
    dmlc::LogMessageFatal(__FILE__, __LINE__).stream()                \
      << "Check failed: " << #x " " #op " " #y << *(_check_err.str)

DEFINE_CHECK_FUNC(_LT, <)
DEFINE_CHECK_FUNC(_GT, >)
DEFINE_CHECK_FUNC(_LE, <=)
DEFINE_CHECK_FUNC(_GE, >=)
DEFINE_CHECK_FUNC(_EQ, ==)
DEFINE_CHECK_FUNC(_NE, !=)

// Always-on checking
#define CHECK(x)                                           \
  if (!(x))                                                \
    dmlc::LogMessageFatal(__FILE__, __LINE__).stream()     \
      + "Check failed: " #x + ' '
#define CHECK_LT(x, y) CHECK_BINARY_OP(_LT, <, x, y)
#define CHECK_GT(x, y) CHECK_BINARY_OP(_GT, >, x, y)
#define CHECK_LE(x, y) CHECK_BINARY_OP(_LE, <=, x, y)
#define CHECK_GE(x, y) CHECK_BINARY_OP(_GE, >=, x, y)
#define CHECK_EQ(x, y) CHECK_BINARY_OP(_EQ, ==, x, y)
#define CHECK_NE(x, y) CHECK_BINARY_OP(_NE, !=, x, y)
#define CHECK_NOTNULL(x) \
  ((x) == NULL ? dmlc::LogMessageFatal(__FILE__, __LINE__).stream() << "Check  notnull: "  #x << ' ', (x) : (x)) // NOLINT(*)
// Debug-only checking.
#ifdef NDEBUG
#define DCHECK(x) \
  while (false) CHECK(x)
#define DCHECK_LT(x, y) \
  while (false) CHECK((x) < (y))
#define DCHECK_GT(x, y) \
  while (false) CHECK((x) > (y))
#define DCHECK_LE(x, y) \
  while (false) CHECK((x) <= (y))
#define DCHECK_GE(x, y) \
  while (false) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) \
  while (false) CHECK((x) == (y))
#define DCHECK_NE(x, y) \
  while (false) CHECK((x) != (y))
#else
#define DCHECK(x) CHECK(x)
#define DCHECK_LT(x, y) CHECK((x) < (y))
#define DCHECK_GT(x, y) CHECK((x) > (y))
#define DCHECK_LE(x, y) CHECK((x) <= (y))
#define DCHECK_GE(x, y) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) CHECK((x) == (y))
#define DCHECK_NE(x, y) CHECK((x) != (y))
#endif  // NDEBUG

#if DMLC_LOG_CUSTOMIZE
#define LOG_INFO dmlc::CustomLogMessage(__FILE__, __LINE__)
#else
#define LOG_INFO dmlc::LogMessage(__FILE__, __LINE__)
#endif
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL dmlc::LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL

// Poor man version of VLOG
#define VLOG(x) LOG_INFO.stream()

#define LOG(severity) LOG_##severity.stream()
#define LG LOG_INFO.stream()
#define LOG_IF(severity, condition) \
  !(condition) ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)

#ifdef NDEBUG
#define LOG_DFATAL LOG_ERROR
#define DFATAL ERROR
#define DLOG(severity) true ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)
#define DLOG_IF(severity, condition) \
  (true || !(condition)) ? (void)0 : dmlc::LogMessageVoidify() & LOG(severity)
#else
#define LOG_DFATAL LOG_FATAL
#define DFATAL FATAL
#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#endif

// Poor man version of LOG_EVERY_N
#define LOG_EVERY_N(severity, n) LOG(severity)

class DateLogger {
 public:
  DateLogger() {
#if defined(_MSC_VER)
    _tzset();
#endif
  }
  const char* HumanDate() {
#if defined(_MSC_VER)
    _strtime_s(buffer_, sizeof(buffer_));
#else
    //time_t time_value = time(NULL);
    time_t time_value =0;
    struct tm *pnow;
#if !defined(_WIN32)
    //struct tm now;
    //pnow = localtime_r(&time_value, &now);
#else
    pnow = localtime(&time_value);  // NOLINT(*)
#endif
    snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d",
             pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
#endif
    return buffer_;
  }

 private:
  char buffer_[9];
};

class LogMessage {
 public:
  LogMessage(const char* file, int line)
      :
#ifdef __ANDROID__
        log_stream_(std::cout)
#else
        //log_stream_(std::cerr)
        log_stream_(""){}
#endif
//  {
    //log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":"
    //            << line << ": ";
//  }
  ~LogMessage() { /*log_stream_ << '\n';*/ }
  std::string stream() { return log_stream_; }

 protected:
  std::string log_stream_;

 private:
  DateLogger pretty_date_;
  LogMessage(const LogMessage&);
  void operator=(const LogMessage&);
};

// customized logger that can allow user to define where to log the message.
class CustomLogMessage {
 public:
  CustomLogMessage(const char* file, int line) {
    //log_stream_ << "[" << DateLogger().HumanDate() << "] " << file << ":"
    //            << line << ": ";
  }
  ~CustomLogMessage() {
    //Log(log_stream_.str());
  }
  std::string& stream() { return log_stream_; }
  /*!
   * \brief customized logging of the message.
   * This function won't be implemented by libdmlc
   * \param msg The message to be logged.
   */
  static void Log(const std::string& msg);

 private:
  //std::ostringstream log_stream_;
  std::string log_stream_;
};

#if DMLC_LOG_FATAL_THROW == 0
class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line) : LogMessage(file, line) {}
  ~LogMessageFatal() {
#if DMLC_LOG_STACK_TRACE
    const int MAX_STACK_SIZE = 10;
    void *stack[MAX_STACK_SIZE];

    int nframes = backtrace(stack, MAX_STACK_SIZE);
    log_stream_ << "\n\n" << "Stack trace returned " << nframes << " entries:\n";
    char **msgs = backtrace_symbols(stack, nframes);
    if (msgs != nullptr) {
      for (int i = 0; i < nframes; ++i) {
        log_stream_ << "[bt] (" << i << ") " << msgs[i] << "\n";
      }
    }
#endif

    log_stream_ << "\n";
    abort();
  }

 private:
  LogMessageFatal(const LogMessageFatal&);
  void operator=(const LogMessageFatal&);
};
#else
class LogMessageFatal {
 public:
  LogMessageFatal(const char* file, int line) {
	  log_stream_ += "[";
	  log_stream_ += pretty_date_.HumanDate();
	  log_stream_ += "] ";
	  log_stream_ += file;
	  log_stream_ +=":";
	  log_stream_ += std::to_string(line);
	  log_stream_ +=": ";
    //log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":"
    //            << line << ": ";
  }
  std::string& stream() { return log_stream_; }
  ~LogMessageFatal() DMLC_THROW_EXCEPTION {
//#if DMLC_LOG_STACK_TRACE
//    const int MAX_STACK_SIZE = 10;
//    void *stack[MAX_STACK_SIZE];
//
//    int nframes = backtrace(stack, MAX_STACK_SIZE);
//    log_stream_ << "\n\n" << "Stack trace returned " << nframes << " entries:\n";
//    char **msgs = backtrace_symbols(stack, nframes);
//    if (msgs != nullptr) {
//      for (int i = 0; i < nframes; ++i) {
//        log_stream_ << "[bt] (" << i << ") " << msgs[i] << "\n";
//      }
//    }
//#endif

    // throwing out of destructor is evil
    // hopefully we can do it here
    // also log the message before throw
#if DMLC_LOG_BEFORE_THROW
    //LOG(ERROR) << log_stream_.str();
#endif
    throw Error(log_stream_);
  }

 private:
  std::string log_stream_;
  DateLogger pretty_date_;
  LogMessageFatal(const LogMessageFatal&);
  void operator=(const LogMessageFatal&);
};
#endif

// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
 public:
  LogMessageVoidify() {}
  // This has to be an operator with a precedence lower than << but
  // higher than "?:". See its usage.
  void operator&(std::string&) {}
};

}  // namespace dmlc

#endif
#endif  // DMLC_LOGGING_H_
