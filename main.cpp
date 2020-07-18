#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"

void FCFS(std::vector<Process*> &processes, int numP) {
  std::vector<int> readyQueue;
  int pAdded = 0;
  double time = 0.0;
  int count = 0;
  double dif = 0;
  int doneP = 0;
  double CPUTime = 0;
  double IOTime = 0;
  double waitBlock = 9999;
  double readyBlock = 0;
  bool justAdded = false;
  std::vector<int> waitQueue;

  std::cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << std::endl;
  //Do not have running state variable since readyQueue[0] is the same thing

  //Does not work
  while(1) {
    //Checks if process has completed all bursts
        //std::cout << processes[1].getCPUTime() << " - " << readyQueue[1].getCPUTime() << std::endl;
        //  std::cout << readyQueue[0].getCPUTime() << " - ";

      //Adds the amount of time the next process takes on the CPU
      CPUTime = 9999;
      IOTime = 9999;
      justAdded = false;

      if (readyQueue.size() > 0) {
        CPUTime = processes[readyQueue[0]]->getCPUTime();
        if (time > readyBlock) {
          std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]]->getName();
          std::cout << " started using the CPU for " << CPUTime << "ms burst" << std::endl;
          readyBlock = time + CPUTime;
        }
      }

      if (waitQueue.size() > 0) {
        IOTime = processes[waitQueue[0]]->getIOTime();
      }

      //If readyQueue has a process, find the CPU time
      //If waitQ has a process, find the I/O time
      if (waitQueue.size() > 0) {
        IOTime = processes[waitQueue[0]]->getIOTime();
        std::cout << "time " << time << "ms: Process " << processes[waitQueue[0]]->getName();
        std::cout << " switching out of CPU; will block on I/O until time ";
        std::cout << time + IOTime << "ms" << std::endl;
        waitBlock = time + IOTime;
      }

      //If processes Added is less than the total number of processes
      if ( pAdded < numP ) {
          if ( time + CPUTime > processes[pAdded]->getIAT() && time + IOTime > processes[pAdded]->getIAT()) {
            readyQueue.push_back(pAdded);
            if (pAdded != 0)
              dif = processes[pAdded]->getIAT() - time;
            time = processes[pAdded]->getIAT();
            justAdded = true;
            std::cout << "time " << time << "ms: Process " << processes[readyQueue[readyQueue.size()-1]]->getName();
            std::cout << " arrived; added to the ready queue" << std::endl;
            pAdded++;
          }
      }

      //Needs to be fixed!!
      if (CPUTime < IOTime) {
        if ( processes[readyQueue[0]]->getCount() != 0) {
          time += CPUTime;
          count++;
          std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]]->getName();
          std::cout << " completed a CPU burst; " << processes[readyQueue[0]]->getRemBursts();
          std::cout << " bursts to go" << std::endl;

          if (processes[readyQueue[0]]->getRemBursts() != 0) {
            waitQueue.push_back(readyQueue[0]);
            readyQueue.erase(readyQueue.begin());
          }
        }
      }
      else if (CPUTime == 9999 && IOTime == 9999) {
        continue;
      }
      else {
        if (justAdded) {
          CPUTime = processes[readyQueue[0]]->getCPUTime();
          std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]]->getName();
          std::cout << " started using the CPU for " << CPUTime << "ms burst" << std::endl;
        }
        time += (IOTime - dif);
        dif = 0;

        std::cout << "time " << time << "ms: Process " << processes[waitQueue[0]]->getName();
        std::cout << " completed I/O; added to ready queue" << std::endl;

        readyQueue.push_back(waitQueue[0]);
        waitQueue.erase(waitQueue.begin());
      }

    //  std::cout << time << " - " << processes[readyQueue[0]].getName() << " - " << count << std::endl;
    //  std::cout << waitQSize << " - " << readyQSize << std::endl;

      //std::cout << readyQSize << std::endl;
      //Moves process from waitQ to readyQ
      /*
      if (waitQSize > 0) {
        readyQueue[readyQSize] = waitQueue[0];
        waitQSize--;
        add = true;
      }

      //Adds running process to the waitQueue
      if (readyQSize > 0) {
        waitQueue[waitQSize] = readyQueue[0];
        waitQSize++;

        //Moves all elements in the ready queue up
        for (int i = 1; i<readyQSize+1; i++) {
          readyQueue[i-1] = readyQueue[i];
        }
        readyQSize--;
      }

      //Adds one to readyQ size after the previous if statement runs
      if (add) {
        readyQSize++;
        add = false;
      }

      */
      //Breaks infinite loop
      /*
      if (count == 50) {
        std::cout << "BROKEN" << std::endl;
        break;
      }*/
      if ( processes[readyQueue[0]]->isDone()) {
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]]->getName();
        std::cout << " terminated" << std::endl;
        doneP++;
        readyQueue.erase(readyQueue.begin());
        if ( doneP == numP ) {
          break;
        }
      }
    }
  std::cout << "time " << time << "ms: Simulator ended for FCFS" << std::endl;

}


bool comparator(Process * & lhs, Process * & rhs) {
  return lhs->getIAT() > rhs->getIAT();
}

bool SJFcomparator(Process * & lhs, Process * & rhs) {
  return lhs->getCPUTimeNoSped() > rhs->getCPUTimeNoSped();
}

bool isIOBlockTime(std::vector<std::pair<int, Process*> > & ioBlockUntil, int time) {
  for (int i = 0; i < ioBlockUntil.size(); i++) {
    if (ioBlockUntil[i].first == time) return true;
  }
  return false;
}

bool isComplete(std::vector<Process*> & processes, int time, int offCPUTime) {
  bool t = true;
  for (int i = 0; i < processes.size(); i++) {
    if (!processes[i]->isDone()) t = false;
    else if (processes[i]->isDone() && !processes[i]->printedIsDone){
      processes[i]->printedIsDone = true;
      if (processes.size() == 1 || processes.size() == 2)
        std::cout << "time " << offCPUTime << "ms: Process " << processes[i]->getName() << " terminated [ADD Q]" << std::endl;
    }
  }
  return t;
}

void SJF(std::vector<Process*> & processes, float lambda, int contextSwitch) {
  std::cout << "time 0ms: Simulator started for SJF [Q <empty>]" << std::endl;
  std::vector<Process*> readyQueue;
  std::vector<Process*> waitQueue;
  std::vector<Process*> arrivedProcesses; //processes that are in the ready queue
  Process * CPUProcess = NULL;
  int tau = 1 / lambda;
  int offCPUTime = -1;
  std::vector<std::pair<int, Process *> > ioBlockUntil;
  std::vector<std::pair<int, Process *> > arrivalTimes;
  int switchToCPUAt = -1;

  std::cout << "time " << processes[0]->getIAT() << "ms: Process " << processes[0]->getName() << " (tau " << tau << "ms) arrived; added to ready queue [ADD Q]" << std::endl;
  for (int i = 0; i < processes.size(); i++) {
    std::pair<int, Process *> t = std::make_pair(processes[i]->getIAT(), processes[i]);
    arrivalTimes.push_back(t);
    processes[i]->addToBurstAvg(100);
    readyQueue.push_back(processes[i]);
  }
  int count = 0;
  for (int time = 0; time > - 1; time++) {

    //if (time == readyQueue[0]->getIAT()) { //this handles when processes first arrive
    if (isIOBlockTime(arrivalTimes, time)) {
      std::pair<int, Process*> arrivalPair;
      int i;
      for (i = 0; i < arrivalTimes.size(); i++) {
        if (arrivalTimes[i].first == time) arrivalPair = arrivalTimes[i];
      }
      arrivedProcesses.push_back(arrivalPair.second);
      if (CPUProcess == NULL) { //add curr process to CPU and shift readyQueue down
        CPUProcess = arrivalPair.second; //add curr process to CPU
        readyQueue.erase(readyQueue.begin()); //remove curr process from readyQ
        time += contextSwitch / 2; //THIS MAY CAUSE ISSUES IN THE FUTURE W TIMES BEING OUT OF ORDER
        if (readyQueue.size() > 1) std::rotate(readyQueue.begin(), readyQueue.begin() + 1, readyQueue.end()); //shift left once if there are multiple processes
        int t = CPUProcess->getCPUTime();
        count++;
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) started using the CPU for " << t << "ms burst [ADD Q]" << std::endl;
        offCPUTime = time + t;
        CPUProcess->addToBurstAvg(t); //to calculate tau in the future
      }
      else
        std::cout << "time " << time << "ms: Process " << arrivalPair.second->getName() << " (tau " << arrivalPair.second->getRunningBurstAvg() << "ms) arrived; added to ready queue [ADD Q]" << std::endl;
    }

    if (time == offCPUTime) { //CPUProcess has finished a burst
      bool finishedProcess = false;
      if (CPUProcess->printedIsDone) {
        if (processes.size() != 1 && processes.size() != 2)
        std::cout << "time " << offCPUTime << "ms: Process " << CPUProcess->getName() << " terminated [ADD Q]" << std::endl;
        CPUProcess = NULL;
        switchToCPUAt = time;
        finishedProcess = true;
      }
      if (!finishedProcess && CPUProcess->printedIsDone == false && CPUProcess->isDone()) {
        CPUProcess->printedIsDone = true;
        CPUProcess = NULL;
        continue;
      }
      if (!finishedProcess) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) completed a CPU burst; " << CPUProcess->getRemBursts() << " bursts to go [ADD Q]" << std::endl;
        std::cout << "time " << time << "ms: Recalculated tau = " << CPUProcess->getRunningBurstAvg() << "ms for process " << CPUProcess->getName() << " [ADD Q]" << std::endl;
        double ioBlockTime = time + CPUProcess->getIOTime(); //THIS MAY NOT BE GETTING THE RIGHT IO TIME
        std::pair<int, Process*> t = std::make_pair(ioBlockTime, CPUProcess);
        ioBlockUntil.push_back(t);
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " switching out of CPU; will block on I/O until time " << ioBlockTime << "ms [ADD Q]" << std::endl;
        waitQueue.push_back(CPUProcess);
        CPUProcess->inWaitQueue = true;
        CPUProcess = NULL;
        switchToCPUAt = time + contextSwitch;
      }
    }

    if (time == switchToCPUAt && CPUProcess == NULL) { //add shortest cpu burst process first (may need to make arrivedProcesses vector)
      std::sort(arrivedProcesses.begin(), arrivedProcesses.end(), SJFcomparator);
      std::reverse(arrivedProcesses.begin(), arrivedProcesses.end());
      int i;
      for (i = 0; i < arrivedProcesses.size(); i++) {
        //std::cout << "Process: " << arrivedProcesses[i]->getName() << std::endl;
        if (!arrivedProcesses[i]->inWaitQueue && !arrivedProcesses[i]->printedIsDone) {
          CPUProcess = arrivedProcesses[i];
        }
      }
      if (CPUProcess == NULL) {
        continue;
      }
      double t = CPUProcess->getCPUTime();
      count++;
      std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) started using the CPU for " << t << "ms burst [ADD Q]" << std::endl;
      offCPUTime = time + t;
      CPUProcess->addToBurstAvg(t); //to calculate tau in the future
    }

    if (isIOBlockTime(ioBlockUntil, time)) {
      std::pair<int, Process*> processPair;
      for (int i = 0; i < ioBlockUntil.size(); i++) {
        if (ioBlockUntil[i].first == time) processPair = ioBlockUntil[i];
      }
      Process * t = processPair.second; //first process in i/o that will finish
      std::cout << "time " << time << "ms: Process " << t->getName() <<  " (tau " << t->getRunningBurstAvg() << "ms) completed I/O; added to ready queue [ADD Q]" << std::endl;
      t->inWaitQueue = false;
      readyQueue.push_back(t);
      if (CPUProcess == NULL) {
        CPUProcess = t;
        double tempTime = t->getCPUTime();
        count++;
        std::cout << "time " << time + contextSwitch / 2 << "ms: Process " << t->getName() << " (tau " << t->getRunningBurstAvg() << "ms) started using the CPU for " << tempTime << "ms burst [ADD Q]" << std::endl;
        t->addToBurstAvg(tempTime); //to calculate tau in the future
        offCPUTime = time + tempTime;
        if (t->isDone()) {
          switchToCPUAt = time;
        }
      }
    }

    if (isComplete(processes, time, offCPUTime)) {
      if (processes.size() != 1 && processes.size() != 2) {
        std::cout << "time " << offCPUTime << "ms: Process " << CPUProcess->getName() << " terminated [ADD Q]" << std::endl;
        std::cout << "time " << offCPUTime << "ms: Simulator ended for SJF [ADD Q]" << std::endl;
      }
      else {
        std::cout << "time " << offCPUTime << "ms: Simulator ended for SJF [ADD Q]" << std::endl;
      }
      break;
    }
  }
}

void SRT(std::vector<Process*> & processes, float lambda, int contextSwitch) {
  std::cout << "time 0ms: Simulator started for SRT [Q <empty>]" << std::endl;
  std::vector<Process*> readyQueue;
  std::vector<Process*> waitQueue;
  std::vector<Process*> arrivedProcesses; //processes that are in the ready queue
  std::vector<Process*> readyQueueCopy;
  Process * CPUProcess = NULL;
  int tau = 1 / lambda;
  int offCPUTime = -1;
  std::vector<std::pair<int, Process *> > ioBlockUntil;
  std::vector<std::pair<int, Process *> > arrivalTimes;
  int switchToCPUAt = -1;

  std::cout << "time " << processes[0]->getIAT() << "ms: Process " << processes[0]->getName() << " (tau " << tau << "ms) arrived; added to ready queue [ADD Q]" << std::endl;
  for (int i = 0; i < processes.size(); i++) {
    std::pair<int, Process *> t = std::make_pair(processes[i]->getIAT(), processes[i]);
    arrivalTimes.push_back(t);
    processes[i]->addToBurstAvg(100);
    readyQueue.push_back(processes[i]);
  }
  int count = 0;
  for (int time = 0; time > - 1; time++) {

    //if (time == readyQueue[0]->getIAT()) { //this handles when processes first arrive
    if (isIOBlockTime(arrivalTimes, time)) {
      std::pair<int, Process*> arrivalPair;
      int i;
      for (i = 0; i < arrivalTimes.size(); i++) {
        if (arrivalTimes[i].first == time) arrivalPair = arrivalTimes[i];
      }
      arrivalPair.second->hasArrived = true;
      arrivedProcesses.push_back(arrivalPair.second);
      if (CPUProcess == NULL) { //add curr process to CPU and shift readyQueue down
        CPUProcess = arrivalPair.second; //add curr process to CPU
        readyQueue.erase(readyQueue.begin()); //remove curr process from readyQ
        time += contextSwitch / 2; //THIS MAY CAUSE ISSUES IN THE FUTURE W TIMES BEING OUT OF ORDER
        if (readyQueue.size() > 1) std::rotate(readyQueue.begin(), readyQueue.begin() + 1, readyQueue.end()); //shift left once if there are multiple processes
        int t = CPUProcess->getCPUTimeNoSped(); //will need to get sped version if its fastest
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) started using the CPU with " << t << "ms burst remaining [ADD Q]" << std::endl;

        readyQueueCopy = std::vector<Process*>(readyQueue);
        std::sort(readyQueueCopy.begin(), readyQueueCopy.end(), SJFcomparator);
        std::reverse(readyQueueCopy.begin(), readyQueueCopy.end());
        if (readyQueueCopy[0]->getCPUTimeNoSped() < t) {
          std::cout << "time " << time << "ms: Process " << readyQueueCopy[0]->getName() << " (tau " << readyQueueCopy[0]->getRunningBurstAvg() << "ms) will preempt " << CPUProcess->getName() << "[ADD Q]" << std::endl;

        }

        offCPUTime = time + t;
        CPUProcess->addToBurstAvg(t); //to calculate tau in the future
      }
      else
        std::cout << "time " << time << "ms: Process " << arrivalPair.second->getName() << " (tau " << arrivalPair.second->getRunningBurstAvg() << "ms) arrived; added to ready queue [ADD Q]" << std::endl;
    }

    if (time == offCPUTime) { //CPUProcess has finished a burst
      bool finishedProcess = false;
      if (CPUProcess->printedIsDone) {
        if (processes.size() != 1 && processes.size() != 2)
        std::cout << "time " << offCPUTime << "ms: Process " << CPUProcess->getName() << " terminated [ADD Q]" << std::endl;
        CPUProcess = NULL;
        switchToCPUAt = time;
        finishedProcess = true;
      }
      if (!finishedProcess && CPUProcess->printedIsDone == false && CPUProcess->isDone()) {
        CPUProcess->printedIsDone = true;
        CPUProcess = NULL;
        continue;
      }
      if (!finishedProcess) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) completed a CPU burst; " << CPUProcess->getRemBursts() << " bursts to go [ADD Q]" << std::endl;
        std::cout << "time " << time << "ms: Recalculated tau = " << CPUProcess->getRunningBurstAvg() << "ms for process " << CPUProcess->getName() << " [ADD Q]" << std::endl;
        double ioBlockTime = time + CPUProcess->getIOTime(); //THIS MAY NOT BE GETTING THE RIGHT IO TIME
        std::pair<int, Process*> t = std::make_pair(ioBlockTime, CPUProcess);
        ioBlockUntil.push_back(t);
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " switching out of CPU; will block on I/O until time " << ioBlockTime << "ms [ADD Q]" << std::endl;
        waitQueue.push_back(CPUProcess);
        CPUProcess->inWaitQueue = true;
        CPUProcess = NULL;
        switchToCPUAt = time + contextSwitch;
      }
    }

    if (time == switchToCPUAt && CPUProcess == NULL) { //add shortest cpu burst process first (may need to make arrivedProcesses vector)
      std::sort(arrivedProcesses.begin(), arrivedProcesses.end(), SJFcomparator);
      std::reverse(arrivedProcesses.begin(), arrivedProcesses.end());
      int i;
      for (i = 0; i < arrivedProcesses.size(); i++) {
        if (!arrivedProcesses[i]->inWaitQueue && !arrivedProcesses[i]->printedIsDone) {
          CPUProcess = arrivedProcesses[i];
        }
      }
      if (CPUProcess == NULL) {
        continue;
      }
      double t = CPUProcess->getCPUTime();
      count++;
      std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " (tau " << CPUProcess->getRunningBurstAvg() << "ms) started using the CPU with " << t << "ms burst remaining [ADD Q]" << std::endl;
      offCPUTime = time + t;
      CPUProcess->addToBurstAvg(t); //to calculate tau in the future
    }

    if (isIOBlockTime(ioBlockUntil, time)) {
      std::pair<int, Process*> processPair;
      for (int i = 0; i < ioBlockUntil.size(); i++) {
        if (ioBlockUntil[i].first == time) processPair = ioBlockUntil[i];
      }
      Process * t = processPair.second; //first process in i/o that will finish
      if (t->getCPUTimeNoSped() + time < offCPUTime) {
        std::cout << "time " << time << "ms: Process " << t->getName() <<  " (tau " << t->getRunningBurstAvg() << "ms) completed I/O; preempting " << CPUProcess->getName() << " [ADD Q]" << std::endl;
        readyQueue.push_back(CPUProcess);
        CPUProcess = t;
        int tempTime = t->getCPUTime();
        std::cout << "time " << time + contextSwitch << "ms: Process " << t->getName() << " (tau " << t->getRunningBurstAvg() << "ms) started using the CPU with " << tempTime << "ms burst remaining [ADD Q]" << std::endl;
        offCPUTime = time + tempTime + contextSwitch;
      }
      else
        std::cout << "time " << time << "ms: Process " << t->getName() <<  " (tau " << t->getRunningBurstAvg() << "ms) completed I/O; added to ready queue [ADD Q]" << std::endl;
      t->inWaitQueue = false;
      readyQueue.push_back(t);
      if (CPUProcess == NULL) {
        CPUProcess = t;
        double tempTime = t->getCPUTime();
        count++;
        std::cout << "time " << time + contextSwitch / 2 << "ms: Process " << t->getName() << " (tau " << t->getRunningBurstAvg() << "ms) started using the CPU with " << tempTime << "ms burst remaining [ADD Q]" << std::endl;
        t->addToBurstAvg(tempTime); //to calculate tau in the future
        offCPUTime = time + tempTime;
        if (t->isDone()) {
          switchToCPUAt = time;
        }
      }
    }

    if (isComplete(processes, time, offCPUTime)) {
      if (processes.size() != 1 && processes.size() != 2) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName() << " terminated [ADD Q]" << std::endl;
        std::cout << "time " << time << "ms: Simulator ended for SRT [ADD Q]" << std::endl;
      }
      else {
        std::cout << "time " << offCPUTime << "ms: Simulator ended for SRT [ADD Q]" << std::endl;
      }
      break;
    }
  }
}


bool operator<(const Process & lhs, const Process & rhs) {
    return lhs.getIAT() < rhs.getIAT();
}

void orderQueue(int alg,
                    int numP, std::vector<Process*> &processes, float lambda, int contextSwitch, int tSlice) {
  if (alg == 1) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << "ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts" << std::endl;
    }
    //Need to sort array by IAT()

    FCFS(processes, numP);
  }
  if (alg == 2) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    sort(processes.begin(), processes.end(), comparator);
    std::reverse(processes.begin(), processes.end());
    SJF(processes, lambda, contextSwitch);
  }
  if (alg == 3) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    sort(processes.begin(), processes.end(), comparator);
    std::reverse(processes.begin(), processes.end());
    SRT(processes, lambda, contextSwitch);
  }
  if (alg == 4) {
    //RR();
  }
}

int main(int argc, char* argv[]) {
  //Loop to run all scheduling algorithms
  for (int algo = 1; algo < 4; algo++) {
    int numProcesses = 0;
    double lambda;
    int maxNum;

    //Read and store command line information
    numProcesses = atoi(argv[1]);
    srand48(atoi(argv[2]));
    lambda = atof(argv[3]);
    maxNum = atoi(argv[4]);
    int contextSwitch = atoi(argv[5]);
    int tSlice = atoi(argv[6]);
    //Vector of processes
    std::vector<Process*> queue(numProcesses);

    //Empty vector to be arranged by scheduling algorithm
    std::vector<Process> orderedQueue(numProcesses);

    //Creation of processes
    for ( int k = 0; k <numProcesses;k++) {
      //Figures out name
      std::string letter;
      letter = char (65+k);

      //Creates Process object
      Process *temp = new Process(letter);

      //Adds to the vector of processes
      queue[k] = temp;

      //Calculates initial process arrival time
      double r = drand48();
      double x = -log( r ) / lambda;
      if (x > maxNum) {
        continue;
      }
      queue[k]->setIAT(floor(x));

      //Calculates number of CPU bursts for a process
      r = drand48();
      x = int (r * 100);
      x++;
      queue[k]->setBurstNum(trunc(x));

      //Allocates data that is not yet freed
      std::vector<std::pair<double,double> >* times = new std::vector<std::pair<double,double> >(x);
      //Calculates CPU and IO burst times for each CPU bursts
      for (int i = 0; i < x ;i++) {

        //Allocates data that is not yet freed
        std::pair<double, double>* p = new std::pair<double, double>;
        int t = -log(drand48()) / lambda;
        p -> first = ceil(t);

        //Prevents the last pair to have an io time since processes end in CPU
        if (i != x-1) {
          int t1 = -log(drand48()) / lambda;
          p -> second = ceil(t1);
        }

        (*times)[i] = *p;
        //std::cout << (*times)[i].first << std::endl;
      }
      //Sets burst times
      queue[k]->setBurstTimes(times);

    }
    //Orders the queue according to the proper algorithm
    orderQueue(algo, numProcesses, queue, lambda, contextSwitch, tSlice);
    std::cout << "----------------------------------------------------------" << std::endl;
  }
  return 0;

}
