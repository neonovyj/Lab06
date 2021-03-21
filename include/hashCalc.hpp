// Copyright 2020 ivan <ikhonyak@gmail.com>
#include <vector>
#include <atomic>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <csignal>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <thread>
#include <utility>

#include "../third-party/PicoSHA2/picosha2.h"

#ifndef INCLUDE_HASHCALC_HPP_
#define INCLUDE_HASHCALC_HPP_

using nlohmann::json;

class hashCalc {
 public:
  // hashCalc();
  hashCalc(const size_t& M,
           const std::string& str = "");
  explicit hashCalc(const std::string& str);
  ~hashCalc();
  void countHash();
  void openFilePath();
  void initThreads();
  void initLogs();
  bool directionIsOpen();
  void jsonOut(const int& data, const std::string& hash, const time_t& time);

 private:
  size_t sizeOfThread = boost::thread::hardware_concurrency();
  std::string nameOfReport;
  std::ofstream file_log;
  std::vector<boost::thread> listOfthread;
  json j;
};

#endif  // INCLUDE_HASHCALC_HPP_
