#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  long mins = (seconds - hours * 3600) / 60;
  long remains = seconds - hours * 3600 - mins * 60;

  std::string s_sec = std::to_string(remains);
  s_sec.insert(0, 2 - s_sec.length(), '0');
  std::string s_min = std::to_string(mins);
  s_min.insert(0, 2 - s_min.length(), '0');

  return string(std::to_string(hours) + ":" + s_min + ":" + s_sec);
}