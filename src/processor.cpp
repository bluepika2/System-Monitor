#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() {
    vector<std::string> res = LinuxParser::CpuUtilization();
    int n = res.size();
    for(int i = 0; i < n; i++) {
        this->jiffies.push_back(stoi(res[i]));
    }
    int userTime = jiffies[0] = jiffies[1];
    int niceTime = jiffies[1] - jiffies[9];
    int idleTime = jiffies[3] + jiffies[4];
    int systemTime = jiffies[2] + jiffies[5] + jiffies[6];
    int virtalTime = jiffies[8] + jiffies[9];
    int stealTime = jiffies[7];
    int totalTime = userTime + niceTime + idleTime + systemTime + virtalTime + stealTime;
    return (totalTime-idleTime)/(float)totalTime;
}