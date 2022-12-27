#include "processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  auto s_list = LinuxParser::CpuUtilization();
  std::vector<float> list;
  for (size_t i = 0; i < s_list.size(); i++) {
    list.push_back(std::stof(s_list[i]));
  }
  float Idle = list[3] + list[4];
  float NonIdle = list[0] + list[1] + list[2] + list[5] + list[6] + list[7];
  return NonIdle / (Idle + NonIdle);
}