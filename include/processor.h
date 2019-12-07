#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    vector<int> jiffies;
};

#endif