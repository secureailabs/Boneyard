/*!
 * Copyright (c) by Contributors 2019
 */
#ifndef XGBOOST_JSON_IO_H_
#define XGBOOST_JSON_IO_H_
#include <xgboost/json.h>

#include <memory>
#include <string>
#include <cinttypes>
#include <utility>
#include <map>
#include <limits>
#include <sstream>
#include <locale>

namespace xgboost {

#ifndef _LIBCPP_SGX_AUXILIARY
template <typename Allocator>
class FixedPrecisionStreamContainer : public std::basic_stringstream<
  char, std::char_traits<char>, Allocator> {
 public:
  FixedPrecisionStreamContainer() {
    this->precision(std::numeric_limits<double>::max_digits10);
    this->imbue(std::locale("C"));
    this->setf(std::ios::scientific);
  }
};

using FixedPrecisionStream = FixedPrecisionStreamContainer<std::allocator<char>>;
#endif

/*
 * \brief A json reader, currently error checking and utf-8 is not fully supported.
 */
class JsonReader {
 protected:
  size_t constexpr static kMaxNumLength =
      std::numeric_limits<double>::max_digits10 + 1;

  struct SourceLocation {
   private:
    size_t pos_ { 0 };  // current position in raw_str_

   public:
    SourceLocation() = default;
    size_t  Pos()  const { return pos_; }

    SourceLocation& Forward() {
      pos_++;
      return *this;
    }
    SourceLocation& Forward(uint32_t n) {
      pos_ += n;
      return *this;
    }
  } cursor_;

  StringView raw_str_;

 protected:
  void SkipSpaces();

  char GetNextChar() {
    if (cursor_.Pos() == raw_str_.size()) {
      return -1;
    }
    char ch = raw_str_[cursor_.Pos()];
    cursor_.Forward();
    return ch;
  }

  char PeekNextChar() {
    if (cursor_.Pos() == raw_str_.size()) {
      return -1;
    }
    char ch = raw_str_[cursor_.Pos()];
    return ch;
  }

  char GetNextNonSpaceChar() {
    SkipSpaces();
    return GetNextChar();
  }

  char GetChar(char c) {
    char result = GetNextNonSpaceChar();
    if (result != c) { Expect(c, result); }
    return result;
  }

  void Error(std::string msg) const;

  // Report expected character
  void Expect(char c, char got) {
    std::string msg = "Expecting: \"";
    msg += c;
    msg += "\", got: \"";
    msg += std::string {got} + " \"";
    Error(msg);
  }

  virtual Json ParseString();
  virtual Json ParseObject();
  virtual Json ParseArray();
  virtual Json ParseNumber();
  virtual Json ParseBoolean();
  virtual Json ParseNull();

  Json Parse();

 public:
  explicit JsonReader(StringView str) :
      raw_str_{str} {}

  virtual ~JsonReader() = default;

  Json Load();
};

class JsonWriter {
  static constexpr size_t kIndentSize = 2;
#ifndef _LIBCPP_SGX_AUXILIARY  
  FixedPrecisionStream convertor_;
#endif
  
  size_t n_spaces_;
#ifndef _LIBCPP_SGX_AUXILIARY
  std::ostream* stream_;
#else
  std::string* stream_;
#endif
  bool pretty_;

 public:
#ifndef _LIBCPP_SGX_AUXILIARY
  JsonWriter(std::ostream* stream, bool pretty) :
#else
  JsonWriter(std::string* stream, bool pretty) :
#endif
      n_spaces_{0}, stream_{stream}, pretty_{pretty} {}

  virtual ~JsonWriter() = default;

  void NewLine() {
    if (pretty_) {
#ifndef _LIBCPP_SGX_AUXILIARY
      *stream_ << u8"\n" << std::string(n_spaces_, ' ');
#else
      *stream_  = u8"\n"  + std::string(n_spaces_, ' ');
#endif
    }
  }

  void BeginIndent() {
    n_spaces_ += kIndentSize;
  }
  void EndIndent() {
    n_spaces_ -= kIndentSize;
  }

  void Write(std::string str) {
#ifndef _LIBCPP_SGX_AUXILIARY
    *stream_ << str;
#else
     *stream_ += str;
#endif
  }
  void Write(StringView str) {
#ifndef _LIBCPP_SGX_AUXILIARY
    stream_->write(str.c_str(), str.size());
#else
    *stream_ += str.c_str();
#endif
  }

  void Save(Json json);

  virtual void Visit(JsonArray  const* arr);
  virtual void Visit(JsonObject const* obj);
  virtual void Visit(JsonNumber const* num);
  virtual void Visit(JsonInteger const* num);
  virtual void Visit(JsonNull   const* null);
  virtual void Visit(JsonString const* str);
  virtual void Visit(JsonBoolean const* boolean);
};
}      // namespace xgboost

#endif  // XGBOOST_JSON_IO_H_
