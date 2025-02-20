#include <iostream>
#include "format.h"
#include "ncurses_display.h"
#include "system.h"

std::string ElapsedTime2(long seconds) {
  long hours, minutes;
  hours = seconds / 3600;
  seconds = seconds % 3600;
  minutes = seconds / 60;
  seconds = seconds % 60;
  return (std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
          std::to_string(seconds));
}

int main() {
  System system;
  NCursesDisplay::Display(system);
}