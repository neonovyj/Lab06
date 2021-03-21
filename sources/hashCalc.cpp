// Copyright 2020 ivan <ikhonyak@gmail.com>
#include "hashCalc.hpp"

namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
boost::mutex mutex;

hashCalc::hashCalc(const size_t& M, const std::string& str) {
  nameOfReport = str;
  if (M < boost::thread::hardware_concurrency()) {  //попробовать заменить
    sizeOfThread = M;
  }
  openFilePath();
}

hashCalc::hashCalc(const std::string& str) : nameOfReport(str) {
  openFilePath();
}

hashCalc::~hashCalc() {
  if (directionIsOpen()) {
    file_log << j.dump(1, '\t') << "\n";  // dump
    file_log.close();
  }
}

void hashCalc::openFilePath() {
  if (!nameOfReport.empty()) {
    file_log.open(nameOfReport);
  }
}

std::atomic<bool> stopFlag = true;
// bool volatile stopFlag = true;

void intHandler([[maybe_unused]] int dummy) { stopFlag = false; }

void hashCalc::countHash() {
  // auto id = boost::this_thread::get_id();
  // boost::unique_lock<boost::mutex> lock(mutex);
  std::srand(time(nullptr));
  for (;;) {  //!
    std::signal(SIGINT, intHandler);
    unsigned int randNum = std::rand();
    time_t timestamp(time(nullptr));
    std::string data =
        picosha2::hash256_hex_string(std::to_string(randNum));  // picosha2
    if (std::regex_match(data, std::regex("(\\w{60}0{4})"))) {
      BOOST_LOG_TRIVIAL(info)
          << "Input data: " << std::left << std::setw(15) << std::hex << randNum
          << " Hash: " << std::setw(74) << data
          << " Thread: " << boost::this_thread::get_id();
      if (directionIsOpen()) jsonOut(randNum, data, timestamp);
    } else {
      BOOST_LOG_TRIVIAL(trace)
          << "Input data: " << std::left << std::setw(15) << std::hex << randNum
          << " Hash: " << std::setw(74) << data
          << " Thread: " << boost::this_thread::get_id();
    }
    if (!stopFlag.load()) break;
  }
}

void hashCalc::initThreads() {
  for (size_t i = 0; i < sizeOfThread; ++i) {
    listOfthread.push_back(boost::thread(&hashCalc::countHash, this));
  }
  for (auto& i : listOfthread) {
    i.join();
  }
  listOfthread.clear();
}

void hashCalc::initLogs() {  //
  boost::log::add_common_attributes();
  boost::log::add_console_log(
      std::cout, keywords::format = "[%TimeStamp%][%Severity%]: %Message%",
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0));
  boost::log::add_file_log(
      keywords::target = "logs/", keywords::file_name = "%y%m%d_%3N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::scan_method = sinks::file::scan_matching,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(12, 0, 0),
      keywords::format = "[%TimeStamp%][%Severity%]: %Message%");
}

void hashCalc::jsonOut(const int& data, const std::string& hash,
                       const time_t& time) {
  mutex.lock();
  json obj =
      json::parse("{\n\t\"data\": " + std::to_string(data) + "," +
                  "\n\t\"hash\": \"" + hash + "\"," + "\n\t\"timestamp\": \"" +
                  std::to_string(time) + "\"" + "\n}");
  j.push_back(obj);
  mutex.unlock();
}
bool hashCalc::directionIsOpen() { return file_log.is_open(); }
