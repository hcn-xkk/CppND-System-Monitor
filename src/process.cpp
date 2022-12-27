#include "process.h"
#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, int system_uptime)
    : pid_(pid), system_uptime_(system_uptime){};

// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Process::CpuUtilization() const {
  std::ifstream filestream(LinuxParser::kProcDirectory + "/" +
                           std::to_string(pid_) + LinuxParser::kStatFilename);
  string line;
  string key;
  string value;
  std::vector<string> values;
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
      if (!line.empty() && values.size() < 22) {
        values.emplace_back(line);
      }
    }
  }
  filestream.close();

  if (values.empty()) {
    failed_ = true;
    return 0.0;
  }
  float total_time = std::stol(values[13]) + std::stol(values[14]) +
                     std::stol(values[15]) + std::stol(values[16]);
  // Another equation. But with this one sometimes ratio > 1?
  // float seconds =
  //     float(system_uptime_) - std::stol(values[21]) / sysconf(_SC_CLK_TCK);
  float seconds = float(system_uptime_);
  float ratio = total_time / sysconf(_SC_CLK_TCK) / seconds;
  if (ratio > 1) {
    throw std::runtime_error("error");
  }
  return ratio;
}

string Process::Command() const { return LinuxParser::Command(pid_); }

string Process::Ram() const {
  int ram_kb = stoi(LinuxParser::Ram(pid_));
  int ram_mb = ram_kb / 1024;
  return std::to_string(ram_mb);
}

string Process::User() const { return LinuxParser::User(pid_); }

long int Process::UpTime() const {
  return LinuxParser::UpTime(pid_, system_uptime_);
}

bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}