#include <iostream>
#include "format.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
  // std::cout << Format::ElapsedTime(system.UpTime()) << '\n';
  // std::cout << "and";
  // std::cout << ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str();

  // // std::__cxx11::string
  // std::cout << Format::ElapsedTime(long(123)) << "\n";
  // auto someString = Format::ElapsedTime(long(123));
  // printf("%s\n", someString.c_str());
}