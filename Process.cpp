#include "Process.h"

//CONSTRUCTORS
Process::Process() {
  name = "A";
}

Process::Process(std::string inName) {
  name = inName;
}

Process Process::operator=(Process p) {
  std::string name = p.getName();
  Process newP = Process(name);
  newP.burstNum = p.getBurstNum();
  newP.initialArrivalT = p.getIAT();
  newP.setBurstTimes(&p.burstTimes);

  return newP;
}

//PRINT
void Process::printName() {
  std::cout << name << std::endl;
}

//SETTERS
void Process::setIAT(double inIAT) {
  initialArrivalT = inIAT;
}

void Process::setBurstNum(int num) {
  burstNum = num;
}

void Process::setBurstTimes(std::vector<std::pair<double,double> >* times) {
  burstTimes = *times;
}

//GETTERS
double Process::getIAT() {
  return initialArrivalT;
}

int Process::getBurstNum() {
  return burstNum;
}

std::string Process::getName() {
  return name;
}

double Process::getCPUTime() {
  return burstTimes[0].first;
}
