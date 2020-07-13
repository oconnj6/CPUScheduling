#include "Process.h"

//CONSTRUCTORS
Process::Process() {
  name = "A";
}

Process::Process(std::string inName) {
  name = inName;
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
