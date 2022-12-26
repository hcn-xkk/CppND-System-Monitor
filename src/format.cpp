#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  long mins = (seconds - hours * 3600) / 60;
  long remains = seconds - hours * 3600 - mins * 60;
  return string(std::to_string(hours) + ":" + std::to_string(mins) + ":" +
                std::to_string(remains));
}