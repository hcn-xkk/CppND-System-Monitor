#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float mem_total;
  float mem_free;
  string line;
  string key;
  string value;
  string kB;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kB) {
        if (key == "MemTotal:") {
          mem_total = stof(value);
          continue;
        }
        if (key == "MemFree:") {
          mem_free = stof(value);
          break;
        }
      }
    }
  }
  filestream.close();
  return (mem_total - mem_free) / mem_total;
}

long int LinuxParser::UpTime() {
  // This is the system uptime not the process uptime.
  string line;
  long int value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value) {
        filestream.close();
        return value;
      }
    }
  }
  filestream.close();
  return 0;
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  std::vector<string> values;
  values.reserve(10);
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          for (int i = 0; i < 10; i++) {
            string value;
            linestream >> value;
            values.emplace_back(value);
          }
          filestream.close();
          return values;
        }
      }
    }
  }
  filestream.close();
  return {};
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  throw std::runtime_error("ERROR! TotalProcesses not found.");
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  throw std::runtime_error("ERROR! RunningProcesses not found.");
}

string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kCmdlineFilename);
  string line;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      filestream.close();
      return line;
    }
  }
  filestream.close();
  return string();
}

string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kStatusFilename);
  string line;
  string key;
  string value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return string("0");
}

string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kStatusFilename);
  string line;
  string key;
  string value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return string();
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string keyward = ":" + uid + ":";
      if (line.find(keyward) != line.npos) {
        string out = line.substr(0, line.find(":"));
        filestream.close();
        return out;
      }
    }
  }
  filestream.close();
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid, int system_uptime) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kStatFilename);
  string line;
  string key;
  string value;
  int i = 0;
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
      i++;
      if (!line.empty() && i == 22) {
        break;
      }
    }
  }
  filestream.close();
  return system_uptime - std::stol(line) / sysconf(_SC_CLK_TCK);
}
