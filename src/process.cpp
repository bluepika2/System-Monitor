#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return this->pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return this->cpuUtilization; }
void Process::CpuUtilizationUpdate() {
    string line;
    std::ifstream fileStream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
    int utime, stime, cutime, cstime, starttime;
    if(fileStream.is_open()) {
        std::getline(fileStream, line);
        std::istringstream lineStream(line);        
        string number;
        for(int i = 0; i < 22; i++) {
            lineStream >> number;
            if(i==13) utime = stoi(number);
            if(i==14) stime = stoi(number);
            if(i==15) cutime = stoi(number);
            if(i==16) cstime = stoi(number);
            if(i==21) starttime = stoi(number);
        }
    }
    long cpuUpTime = LinuxParser::UpTime();
    long timeInterval = cpuUpTime - starttime/sysconf(_SC_CLK_TCK);
    this->cpuUtilization = (1.0)*(utime+stime+cutime+cstime)/sysconf(_SC_CLK_TCK)/timeInterval;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const {
    return this->cpuUtilization > a.cpuUtilization;
}