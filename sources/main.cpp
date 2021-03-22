// Copyright 2020 ivan <ikhonyak@gmail.com>
#include "Hash.hpp"

int main(int, char*[]) {
  Hash A("/home/ivan/lab-06-multithreads/map.json");
  A.initLogs();
  A.initThreads();
  return 0;
}
