#include <dirent.h>
#include <unistd.h>
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
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string memoryName;
  float memory;
  vector<float> memoryVector;
  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> memoryName >> memory;
      memoryVector.push_back(memory);
      if(memoryName == "Cached:") break;
    }
  }
  return (memoryVector[0] - memoryVector[1] + memoryVector[2] + memoryVector[3])/memoryVector[0]; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long res = 0;;
  long number = 0;
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  if(fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> number;
    res += number;
  }
  return res;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpu;
  vector<string> res;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if(fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    for(int i = CPUStates::kUser_;  i <= CPUStates::kGuestNice_; i++) {
      string jiffies;
      lineStream >> jiffies;
      res.push_back(jiffies);
    }
  }
  return res;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string name;
  int number = 0;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> name >> number;      
      if(name == "processes") return number;
    }
  }
  return number;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string name;
  int number = 0;
  std::ifstream fileStream(kProcDirectory + kStatFilename); 
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> name >> number;
      if(name == "procs_running") return number;
    }
  }
  return number;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(fileStream.is_open()) {
    std::getline(fileStream, line);
    return line;
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string name;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> name;
      if(name == "VmSize:") {
        int memory;
        lineStream >> memory;
        memory = memory/1000;
        return to_string(memory);
      }
    }
  }
  return "";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string name, number;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> name >> number;
      if(name == "Uid:") return number;
    }
  }
  return number;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uId = Uid(pid);
  string line;
  string user, temp, uidNumber;
  std::ifstream fileStream(kPasswordPath);
  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> user >> temp >> uidNumber;
      if(uidNumber == uId) return user;
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  long number;
  string str;
  if(fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    for(int i = 0; i < 22; i++) {
      lineStream >> str;
      if(i==21) number = stoi(str);
    }
  }
  return number/sysconf(_SC_CLK_TCK);
}