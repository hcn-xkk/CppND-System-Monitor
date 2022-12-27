#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
std::vector<Process>& System::Processes() {
  // Create processes_
  processes_.clear();
  vector<int> pid_nums = LinuxParser::Pids();
  for (auto num : pid_nums) {
    processes_.emplace_back(num, UpTime());
  }
  // sort processes_
  std::sort(processes_.begin(), processes_.end());
  return processes_;
};