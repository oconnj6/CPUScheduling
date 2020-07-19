#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"
#include <ostream>
#include <fstream>


void printQueue(std::vector<Process*> &processes, std::vector<int> readyQueue) {
  if (readyQueue.size() == 0) {
    std::cout << " [Q <empty>]" << std::endl;
  }
  else {
    std::cout << " [Q ";
    for (unsigned int i = 0;i < readyQueue.size();i++) {
      std::cout << processes[readyQueue[i]]->getName();
      if (i != readyQueue.size()-1) {
        std::cout << " ";
      }
    }
    std::cout << "]" << std::endl;
  }
}

void printQueue(std::vector<Process*> readyQueue) {
  if (readyQueue.size() == 0) {
    std::cout << " [Q <empty>]" << std::endl;
  }
  else {
    std::cout << " [Q ";
    for (unsigned int i = 0;i < readyQueue.size();i++) {
      std::cout << readyQueue[i]->getName();
      if (i != readyQueue.size()-1) {
        std::cout << " ";
      }
    }
    std::cout << "]" << std::endl;
  }
}

void FCFS(std::vector<Process*> &processes, int contextSwitch, std::ofstream & outputFile) {
  std::vector<int> readyQueue;
  std::vector<int> waitQueue;
  unsigned int doneP = 0;
  int CPUTime = 0;
  int CPUProcess = 99;
  int CPUFinished = -1;
  int pAdded = 0;
  int CPUtimeRem = 0;
  int CPUStart = -1;
  int waitBlock = 0;
  double totalBurstTime = 0;
  int totalBursts = 0;
  double totalWaitTime = 0;
  int totalWaits = 0;
  int totalTurnATime = 0;
  int totalTurns = 0;
  int totalCSwitch = 0;
  int time;
  for (time = 0; time > -1;time++) {
    for (unsigned int i = 0;i < processes.size(); i++) {
      if (processes[i]->getIAT() == time) {
        readyQueue.push_back(i);
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[readyQueue.size()-1]]->getName();
        std::cout << " arrived; added to the ready queue";
        printQueue(processes, readyQueue);
        pAdded++;
      }
    }

    if ( CPUStart == time ) {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst";
        printQueue(processes, readyQueue);
        totalBurstTime += CPUTime;
        totalBursts++;
        totalCSwitch++;

        totalTurnATime += (CPUTime + 2);
        totalTurns++;
    }

    if (readyQueue.size() > 0) {
      if ( CPUProcess == 99 ) {
        CPUProcess = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        CPUTime = processes[CPUProcess]->getCPUTime();
        int cs = 1;
        if (processes[CPUProcess]->getIAT() == time)
          cs = 2;

        CPUStart = time + cs;
        processes[CPUProcess]->setCPUDone( CPUTime );
        CPUFinished = time + CPUTime + cs;
        //std::cout << CPUFinished << std::endl;
      }
    }

    if ( time >= CPUFinished && CPUFinished != -1) {
      if (processes[CPUProcess]->getCPUDone() != 0 && waitBlock < 2 && processes[CPUProcess]->getRemBursts() != 0 ) {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " completed a CPU burst; " << processes[CPUProcess]->getRemBursts();
        std::cout << " bursts to go";
        printQueue(processes, readyQueue);
        if (waitBlock > 0) {
          waitBlock++;
        }
      }

      if ( CPUProcess != 99) {
        if (processes[CPUProcess]->getRemBursts() != 0 && waitBlock == 0) {
          int IOTime = processes[CPUProcess]->getIOTime();
          std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
          std::cout << " switching out of CPU; will block on I/O until time ";
          std::cout << time + IOTime << "ms";
          printQueue(processes, readyQueue);
          waitBlock++;
          processes[CPUProcess]->setWaitDone(time + IOTime);
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          totalCSwitch++;
          totalWaitTime += IOTime;
          totalWaits ++;

          waitQueue.push_back(CPUProcess);
          CPUProcess = 99;
        }
        else if (processes[CPUProcess]->getRemBursts() == 0 ){
          std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
          std::cout << " terminated";
          printQueue(processes, readyQueue);
          doneP++;
          CPUProcess = 99;
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          if (doneP == processes.size())
            break;
        }
      }
    }
    if ( waitQueue.size() > 0 ) {
      if (time == processes[waitQueue[0]]->getWaitDone()) {
        waitBlock = 0;

        readyQueue.push_back(waitQueue[0]);
        waitQueue.erase(waitQueue.begin());
        std::cout << "time " << time << "ms: Process " << processes[waitQueue[0]]->getName();
        std::cout << " completed I/O; added to ready queue";
        printQueue(processes, readyQueue);
      }
    }
  }
  std::cout << "Simulator ended for FCFS";
  printQueue(processes, readyQueue);

  if (outputFile.is_open()) {
    outputFile << "Algorithm FCFS" << std::endl;
    outputFile.setf(std::ios::fixed,std::ios::floatfield);
    outputFile.precision(3);
    outputFile << "-- average CPU burst time: " << totalBurstTime / (double)totalBursts << " ms" << std::endl;
    outputFile << "-- average wait time: " << totalWaitTime / (double)totalWaits << " ms" << std::endl;
    outputFile << "-- average turnaround time: " << totalTurnATime / (double)totalTurns << " ms" << std::endl;
    outputFile << "-- total number of context switches: " << totalCSwitch << std::endl;
    outputFile << "-- total number of preemptions: 0" << std::endl;
  }
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

void SJF(std::vector<Process*> &processes, double lambda, int contextSwitch, std::ofstream & outputFile) {
  std::vector<Process*> readyQueue;
  std::vector<Process*> waitQueue;
  unsigned int doneP = 0;
  int CPUTime = 0;
  Process * CPUProcess = NULL;
  int CPUFinished = -1;
  int pAdded = 0;
  int CPUtimeRem = 0;
  int CPUStart = -1;
  int waitBlock = 0;
  double totalBurstTime = 0;
  int totalBursts = 0;
  double totalWaitTime = 0;
  int totalWaits = 0;
  int totalTurnATime = 0;
  int totalTurns = 0;
  int totalCSwitch = 0;
  int time;
  for (time = 0; time > -1;time++) {
    for (unsigned int i = 0;i < processes.size(); i++) {
      if (processes[i]->getIAT() == time) {
        readyQueue.push_back(processes[i]);
        std::sort(readyQueue.begin(), readyQueue.end(), SJFcomparator);
        std::reverse(readyQueue.begin(), readyQueue.end());
        std::cout << "time " << time << "ms: Process " << readyQueue[readyQueue.size()-1]->getName();
        std::cout << " arrived; added to the ready queue";
        printQueue(readyQueue);
        processes[i]->addToBurstAvg(1 / lambda);
        pAdded++;
      }
    }

    if ( CPUStart == time ) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst";
        printQueue(readyQueue);
        totalBurstTime += CPUTime;
        totalBursts++;

        totalCSwitch++;
        totalTurnATime += (CPUTime + 2);
        totalTurns++;
    }

    if (readyQueue.size() > 0) {
      if ( CPUProcess == NULL ) {
        CPUProcess = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        CPUTime = CPUProcess->getCPUTime();
        int cs = 1;
        if (CPUProcess->getIAT() == time)
          cs = 2;

        CPUStart = time + cs;
        CPUProcess->setCPUDone( CPUTime );
        CPUFinished = time + CPUTime + cs;
        //std::cout << CPUFinished << std::endl;
      }
    }

    if ( time >= CPUFinished && CPUFinished != -1) {
      if (CPUProcess->getCPUDone() != 0 && waitBlock < 2 && CPUProcess->getRemBursts() != 0 ) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
        std::cout << " completed a CPU burst; " << CPUProcess->getRemBursts();
        std::cout << " bursts to go";
        printQueue(readyQueue);
        CPUProcess->addToBurstAvg(CPUTime);
        std::cout << "time " << time << "ms: Recalculated tau = " << CPUProcess->getRunningBurstAvg();
        std::cout << "ms for process " << CPUProcess->getName();
        printQueue(readyQueue);

        if (waitBlock > 0) {
          waitBlock++;
        }
      }

      if ( CPUProcess != NULL) {
        if (CPUProcess->getRemBursts() != 0 && waitBlock == 0) {
          int IOTime = CPUProcess->getIOTime();
          std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
          std::cout << " switching out of CPU; will block on I/O until time ";
          std::cout << time + IOTime << "ms";
          printQueue(readyQueue);
          waitBlock++;
          CPUProcess->setWaitDone(time + IOTime);
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          totalCSwitch++;
          totalWaitTime += IOTime;
          totalWaits ++;

          waitQueue.push_back(CPUProcess);
          CPUProcess = NULL;
        }
        else if (CPUProcess->getRemBursts() == 0 ){
          std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
          std::cout << " terminated";
          printQueue(readyQueue);
          doneP++;
          CPUProcess = NULL;
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          if (doneP == processes.size())
            break;
        }
      }
    }
    if ( waitQueue.size() > 0 ) {
      if (time == waitQueue[0]->getWaitDone()) {
        waitBlock = 0;

        readyQueue.push_back(waitQueue[0]);
        std::sort(readyQueue.begin(), readyQueue.end(), SJFcomparator);
        std::reverse(readyQueue.begin(), readyQueue.end());
        waitQueue.erase(waitQueue.begin());
        std::cout << "time " << time << "ms: Process " << waitQueue[0]->getName();
        std::cout << " completed I/O; added to ready queue";
        printQueue(readyQueue);
      }
    }
  }
  std::cout << "Simulator ended for SJF";
  printQueue(readyQueue);

  if (outputFile.is_open()) {
    outputFile << "Algorithm SJF" << std::endl;
    outputFile.setf(std::ios::fixed,std::ios::floatfield);
    outputFile.precision(3);
    outputFile << "-- average CPU burst time: " << totalBurstTime / (double)totalBursts << " ms" << std::endl;
    outputFile << "-- average wait time: " << totalWaitTime / (double)totalWaits << " ms" << std::endl;
    outputFile << "-- average turnaround time: " << totalTurnATime / (double)totalTurns << " ms" << std::endl;
    outputFile << "-- total number of context switches: " << totalCSwitch << std::endl;
    outputFile << "-- total number of preemptions: 0" << std::endl;
  }
}

void SRT(std::vector<Process*> &processes, double lambda, int contextSwitch, std::ofstream & outputFile) {
  std::vector<Process*> readyQueue;
  std::vector<Process*> waitQueue;
  unsigned int doneP = 0;
  int CPUTime = 0;
  Process * CPUProcess = NULL;
  int CPUFinished = -1;
  int pAdded = 0;
  int CPUtimeRem = 0;
  int CPUStart = -1;
  int waitBlock = 0;
  double totalBurstTime = 0;
  int totalBursts = 0;
  double totalWaitTime = 0;
  int totalWaits = 0;
  double totalTurnATime = 0;
  int totalTurns = 0;
  int totalPreemptions = 0;
  int totalCSwitch = 0;

  int time;
  for (time = 0; time > -1;time++) {
    for (unsigned int i = 0;i < processes.size(); i++) {
      if (processes[i]->getIAT() == time) {
        readyQueue.push_back(processes[i]);
        std::sort(readyQueue.begin(), readyQueue.end(), SJFcomparator);
        std::reverse(readyQueue.begin(), readyQueue.end());
        std::cout << "time " << time << "ms: Process " << readyQueue[readyQueue.size()-1]->getName();
        std::cout << " arrived; added to the ready queue";
        printQueue(readyQueue);
        processes[i]->addToBurstAvg(1 / lambda);
        pAdded++;
      }
    }
    if (readyQueue.size() > 0 && CPUProcess != NULL) {
      for (unsigned int i = 0; i < readyQueue.size(); i++) {
        if (!CPUProcess->isDone() && !readyQueue[i]->isDone() &&
          CPUProcess->getCPUTimeNoSped() > readyQueue[i]->getCPUTimeNoSped()) {
          std::cout << "time " << time << "ms: Process " << readyQueue[i]->getName();
          std::cout << " (tau " << readyQueue[i]->getRunningBurstAvg() << "ms) will preempt ";
          std::cout << CPUProcess->getName();
          readyQueue.push_back(CPUProcess);
          CPUProcess = readyQueue[i];
          CPUTime = CPUProcess->getCPUTime();
          readyQueue.erase(readyQueue.begin() + i);
          std::sort(readyQueue.begin(), readyQueue.end(), SJFcomparator);
          std::reverse(readyQueue.begin(), readyQueue.end());
          printQueue(readyQueue);
          totalPreemptions++;
        }
      }
    }

    if ( CPUStart == time ) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst";
        printQueue(readyQueue);
        totalBurstTime += CPUTime;
        totalBursts++;

        totalCSwitch++;
        totalTurnATime += (CPUTime + 2);
        totalTurns++;
    }

    if (readyQueue.size() > 0) {
      if ( CPUProcess == NULL ) {
        CPUProcess = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        CPUTime = CPUProcess->getCPUTime();
        std::cout << "Count for process: " << CPUProcess->getName() << " is: " << CPUProcess->getCount() << std::endl;
        int cs = 1;
        if (CPUProcess->getIAT() == time)
          cs = 2;

        CPUStart = time + cs;
        CPUProcess->setCPUDone( CPUTime );
        CPUFinished = time + CPUTime + cs;
        //std::cout << CPUFinished << std::endl;
      }
    }

    if ( time >= CPUFinished && CPUFinished != -1) {
      if (CPUProcess->getCPUDone() != 0 && waitBlock < 2 && CPUProcess->getRemBursts() != 0 ) {
        std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
        std::cout << " completed a CPU burst; " << CPUProcess->getRemBursts();
        std::cout << " bursts to go";
        printQueue(readyQueue);
        CPUProcess->addToBurstAvg(CPUTime);
        std::cout << "time " << time << "ms: Recalculated tau = " << CPUProcess->getRunningBurstAvg();
        std::cout << "ms for process " << CPUProcess->getName();
        printQueue(readyQueue);

        if (waitBlock > 0) {
          waitBlock++;
        }
      }

      if ( CPUProcess != NULL) {
        if (CPUProcess->getRemBursts() != 0 && waitBlock == 0) {
          int IOTime = CPUProcess->getIOTime();
          std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
          std::cout << " switching out of CPU; will block on I/O until time ";
          std::cout << time + IOTime << "ms";
          printQueue(readyQueue);
          waitBlock++;
          CPUProcess->setWaitDone(time + IOTime);
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          totalCSwitch++;
          totalWaitTime += IOTime;
          totalWaits++;

          waitQueue.push_back(CPUProcess);
          CPUProcess = NULL;
        }
        else if (CPUProcess->getRemBursts() == 0 ){
          std::cout << "time " << time << "ms: Process " << CPUProcess->getName();
          std::cout << " terminated";
          printQueue(readyQueue);
          doneP++;
          CPUProcess = NULL;
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }
          if (doneP == processes.size())
            break;
        }
      }
    }
    if ( waitQueue.size() > 0 ) {
      bool isPreemption = true;
      Process * t = waitQueue[0];
      if (time == waitQueue[0]->getWaitDone()) {
        for (unsigned int i = 0; i < readyQueue.size(); i++) {
          std::cout << readyQueue[i]->getCPUTimeNoSped() << std::endl;
          std::cout << waitQueue[0]->getCPUTimeNoSped() << std::endl;
          if (waitQueue[0]->getCPUTimeNoSped() > readyQueue[i]->getCPUTimeNoSped()) {
            isPreemption = false;
          }
        }

        waitBlock = 0;

        std::cout << "time " << time << "ms: Process " << waitQueue[0]->getName();
        if (isPreemption && CPUProcess != NULL) {
          std::cout << " completed I/O; preempting " << CPUProcess->getName();
          totalPreemptions++;
          readyQueue.push_back(CPUProcess);
          CPUProcess = t;
        }
        else {
          readyQueue.push_back(waitQueue[0]);
          std::cout << " completed I/O; added to ready queue";
        }
        std::sort(readyQueue.begin(), readyQueue.end(), SJFcomparator);
        std::reverse(readyQueue.begin(), readyQueue.end());
        waitQueue.erase(waitQueue.begin());
        printQueue(readyQueue);
      }
    }
  }
  std::cout << "Simulator ended for SRT";
  printQueue(readyQueue);

  if (outputFile.is_open()) {
    outputFile << "Algorithm SRT" << std::endl;
    outputFile.setf(std::ios::fixed,std::ios::floatfield);
    outputFile.precision(3);
    outputFile << "-- average CPU burst time: " << totalBurstTime / (double)totalBursts << " ms" << std::endl;
    outputFile << "-- average wait time: " << totalWaitTime / (double)totalWaits << " ms" << std::endl;
    outputFile << "-- average turnaround time: " << totalTurnATime / (double)totalTurns << " ms" << std::endl;
    outputFile << "-- total number of context switches: " << totalCSwitch << std::endl;
    outputFile << "-- total number of preemptions: " << totalPreemptions << std::endl;
  }
}

void RR(std::vector<Process*> &processes, int tSlice, int contextSwitch, std::ofstream & outputFile) {
  std::vector<int> readyQueue;
  std::vector<int> waitQueue;
  unsigned int doneP = 0;
  int CPUTime = 0;
  int CPUProcess = 99;
  int CPUFinished = -1;
  int pAdded = 0;
  int CPUtimeRem = 0;
  int CPUStart = -1;
  int waitBlock = 0;
  double totalBurstTime = 0;
  int totalBursts = 0;
  double totalWaitTime = 0;
  int totalWaits = 0;
  double totalTurnATime = 0;
  int totalTurns = 0;
  int totalPreemptions = 0;
  int totalCSwitch = 0;

  int time;
  for (time = 0; time > -1;time++) {
    for (unsigned int i = 0;i < processes.size(); i++) {
      if (processes[i]->getIAT() == time) {
        readyQueue.push_back(i);
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[readyQueue.size()-1]]->getName();
        std::cout << " arrived; added to the ready queue";
        printQueue(processes, readyQueue);
        pAdded++;
      }
    }

    if ( CPUStart == time ) {
      if ( processes[CPUProcess]->getTimeRem() != 0 ) {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst remaining";
        printQueue(processes, readyQueue);
        processes[CPUProcess]->setTimeRem(0);
      }
      else {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst";
        printQueue(processes, readyQueue);
      }
      totalBurstTime += CPUTime;
      totalBursts++;
      totalTurnATime += CPUTime + contextSwitch;
      totalTurns++;
      totalCSwitch++;
    }

    if (readyQueue.size() > 0) {
      if ( CPUProcess == 99 ) {
        CPUProcess = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        if (processes[CPUProcess]->getTimeRem() != 0) {
          CPUTime = processes[CPUProcess]->getTimeRem();
        }
        else {
          CPUTime = processes[CPUProcess]->getCPUTime();
        }
        int cs = 1;
        if (processes[CPUProcess]->getIAT() == time)
          cs = 2;

        CPUStart = time + cs;
        if ( CPUTime < tSlice ) {
          processes[CPUProcess]->setCPUDone( tSlice - CPUTime );
          CPUFinished = time + CPUTime + cs;
        }
        else if (CPUTime == tSlice ) {
          processes[CPUProcess]->setCPUDone( 1 );
          CPUFinished = time + CPUTime + cs;
        }
        else {
          //NEED VARIABLE HERE
          CPUFinished = time + tSlice + cs;
          CPUtimeRem = CPUTime - tSlice;
          processes[CPUProcess]->setCPUDone( 0 );
        }
      }
    }

    if ( time >= CPUFinished && CPUFinished != -1 && CPUProcess != 99) {
      bool done = true;
    //  std::cout << "CPUProcess is : " << CPUProcess << std::endl;
      if (processes[CPUProcess]->getCPUDone() != 0 && waitBlock == 0) {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " completed a CPU burst; " << processes[CPUProcess]->getRemBursts();
        std::cout << " bursts to go";
        printQueue(processes, readyQueue);
      }
      else if (processes[CPUProcess]->getCPUDone() == 0){
        if (readyQueue.size() == 0 ) {
          std::cout << "time " <<  time << "ms: Time slice expired; no preemption";
          std::cout << " because ready queue is empty";
          printQueue(processes, readyQueue);
          done = false;
          CPUFinished = time + CPUtimeRem;
          processes[CPUProcess]->setCPUDone(1);
        }
        else {
          processes[CPUProcess]->setTimeRem(CPUtimeRem);
          readyQueue.push_back(CPUProcess);
          std::cout << "time " <<  time << "ms: Time slice expired; process ";
          std::cout << processes[CPUProcess]->getName() << " preempted with ";
          std::cout << CPUtimeRem << "ms to go";
          printQueue(processes, readyQueue);
          totalPreemptions++;
          CPUProcess = 99;
        }
      }

      if ( CPUProcess != 99) {
        /*std::cout << "processes[CPUProcess]->getRemBursts(): " << processes[CPUProcess]->getRemBursts() << std::endl;
        std::cout << "done: " << done << std::endl;
        std::cout << "waitblock: " << waitBlock << std::endl;*/
        if (processes[CPUProcess]->getRemBursts() != 0 && done && waitBlock == 0) {
          int IOTime = processes[CPUProcess]->getIOTime();
          std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
          std::cout << " switching out of CPU; will block on I/O until time ";
          std::cout << time + IOTime << "ms";
          printQueue(processes, readyQueue);
          waitBlock++;
          processes[CPUProcess]->setWaitDone(time + IOTime);
          if (readyQueue.size() == 0) {
            CPUFinished = -1;
          }

          waitQueue.push_back(CPUProcess);
          CPUProcess = 99;
          totalWaitTime += IOTime;
          totalWaits++;
          totalCSwitch++;
        }
        else if (processes[CPUProcess]->getRemBursts() == 0 && done){
          std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
          std::cout << " terminated";
          printQueue(processes, readyQueue);
          doneP++;
          CPUProcess = 99;
          if (doneP == processes.size())
            break;
        }
      }
    }
    if ( waitQueue.size() > 0 ) {
      if (time == processes[waitQueue[0]]->getWaitDone()) {
        waitBlock = 0;

        readyQueue.push_back(waitQueue[0]);
        waitQueue.erase(waitQueue.begin());

        std::cout << "time " << time << "ms: Process " << processes[waitQueue[0]]->getName();
        std::cout << " completed I/O; added to ready queue";
        printQueue(processes, readyQueue);
      }
    }
  }
  std::cout << "Simulator ended for RR";
  printQueue(processes, readyQueue);
  if (outputFile.is_open()) {
    outputFile << "Algorithm RR" << std::endl;
    outputFile.setf(std::ios::fixed,std::ios::floatfield);
    outputFile.precision(3);
    outputFile << "-- average CPU burst time: " << totalBurstTime / (double)totalBursts << " ms" << std::endl;
    outputFile << "-- average wait time: " << totalWaitTime / (double)totalWaits << " ms" << std::endl;
    outputFile << "-- average turnaround time: " << totalTurnATime / (double)totalTurns << " ms" << std::endl;
    outputFile << "-- total number of context switches: " << totalCSwitch << std::endl;
    outputFile << "-- total number of preemptions: " << totalPreemptions << std::endl;
  }
}


bool operator<(const Process & lhs, const Process & rhs) {
    return lhs.getIAT() < rhs.getIAT();
}

void orderQueue(int alg,
                    int numP, std::vector<Process*> &processes, float lambda, int contextSwitch, int tSlice, std::ofstream & outputFile) {
  if (alg == 1) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << "ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts" << std::endl;
    }
    std::cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << std::endl;

    //Need to sort array by IAT()

    FCFS(processes, contextSwitch, outputFile);
  }
  if (alg == 2) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    std::cout << "time 0ms: Simulator started for SJF [Q <empty>]" << std::endl;
    sort(processes.begin(), processes.end(), comparator);
    std::reverse(processes.begin(), processes.end());
    SJF(processes, lambda, contextSwitch, outputFile);
  }
  if (alg == 3) {
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    std::cout << "time 0ms: Simulator started for SRT [Q <empty>]" << std::endl;
    sort(processes.begin(), processes.end(), comparator);
    std::reverse(processes.begin(), processes.end());
    SRT(processes, lambda, contextSwitch, outputFile);
  }
  if (alg == 4) {
    std::cout << processes.size() << std::endl;
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    std::cout << "time 0ms: Simulator started for RR [Q <empty>]" << std::endl;
    RR(processes, tSlice, contextSwitch, outputFile);
  }
}

int main(int argc, char* argv[]) {
  //Loop to run all scheduling algorithms
  std::ofstream outputFile("simout.txt");
  for (int algo = 1; algo < 5; algo++) {
    int numProcesses = 0;
    double lambda;
    int maxNum;

    //Read and store command line information
    numProcesses = atoi(argv[1]);
    srand48(atoi(argv[2]));
    lambda = atof(argv[3]);
    maxNum = atoi(argv[4]);
    int contextSwitch = atoi(argv[5]);
    int tSlice = atoi(argv[7]);
    //Vector of processes
    std::vector<Process*> queue(numProcesses);

    //Empty vector to be arranged by scheduling algorithm
    std::vector<Process> orderedQueue(numProcesses);

    std::vector<std::pair<double,double>* >* times;
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
      times = new std::vector<std::pair<double,double>* >;
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

        times->push_back(p);
        //std::cout << (*times)[i].first << std::endl;
      }
      //Sets burst times
      queue[k]->setBurstTimes(times);
    }
    //Orders the queue according to the proper algorithm
    sort(queue.begin(), queue.end(), comparator);
    std::reverse(queue.begin(), queue.end());
    orderQueue(algo, numProcesses, queue, lambda, contextSwitch, tSlice, outputFile);
    for (int i = 0;i<numProcesses;i++) {
      queue[i]->removeProcess();
      delete queue[i];
    }
    std::cout << "----------------------------------------------------------" << std::endl;
  }
  return 0;

}
