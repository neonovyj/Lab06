// Copyright 2020 ivan <ikhonyak@gmail.com>
#include "hashCalc.hpp"

int main(int, char*[]) {
  hashCalc A("/home/ivan/lab-06-multithreads/map.json");
  A.initLogs();
  A.initThreads();
  return 0;
}