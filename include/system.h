#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu() { return cpu_; };
  std::vector<Process>& Processes();
  float MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }
  long int UpTime() const { return LinuxParser::UpTime(); }
  int TotalProcesses() const { return LinuxParser::TotalProcesses(); }
  int RunningProcesses() const { return LinuxParser::RunningProcesses(); };
  std::string Kernel() const { return LinuxParser::Kernel(); };
  std::string OperatingSystem() const {
    return LinuxParser::OperatingSystem();
  };

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif