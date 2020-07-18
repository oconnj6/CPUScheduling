#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Process.h"

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

void FCFS(std::vector<Process> &queue,std::vector<Process*> &processes, int numP) {
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
  int totalBurstTime = 0;
  int totalBursts = 0;
  int totalWaitTime = 0;
  int totalWaits = 0;
  int totalTurnATime = 0;
  int totalTurns = 0;

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

  std::cout << "AVG burst time = " << totalBurstTime / totalBursts << std::endl;
  std::cout << "AVG wait time = " << totalWaitTime / totalWaits << std::endl;
  std::cout << "AVG Turn Around time = " << totalTurnATime / totalTurns << std::endl;
  std::cout << "Total Preemptions = 0" << std::endl;

}


bool comparator(Process * & lhs, Process * & rhs) {
  return lhs->getIAT() > rhs->getIAT();
}

void SJF(std::vector<Process*> & processes, float lambda) {
  std::cout << "time 0ms: Simulator started for SJF [Q <empty>]" << std::endl;
  std::vector<Process> readyQueue;
  std::vector<Process> waitQueue;
  int burstCount = 0;

  while(1) {
    break;
  }
}

void RR(std::vector<Process*> &processes, int tSlice) {
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
        //std::cout << CPUFinished << std::endl;
      }
    }

    if ( time >= CPUFinished && CPUFinished != -1) {
      bool done = true;
      if (processes[CPUProcess]->getCPUDone() != 0 && waitBlock < 2) {
        std::cout << "time " << time << "ms: Process " << processes[CPUProcess]->getName();
        std::cout << " completed a CPU burst; " << processes[CPUProcess]->getRemBursts();
        std::cout << " bursts to go";
        printQueue(processes, readyQueue);
        if (waitBlock > 0) {
          waitBlock++;
        }
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
          CPUProcess = 99;
        }
      }

      if ( CPUProcess != 99) {
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
}


bool operator<(const Process & lhs, const Process & rhs) {
    return lhs.getIAT() < rhs.getIAT();
}

void orderQueue(std::vector<Process> &queue, int alg,
                    int numP, std::vector<Process*> &processes, float lambda, int tSlice) {
  if (alg == 1) {
    std::cout << processes.size() << std::endl;
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << "ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts" << std::endl;
    }
    //Need to sort array by IAT()

    FCFS(queue, processes, numP);
  }
  if (alg == 2) {
    std::cout << processes.size() << std::endl;
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    SJF(processes, lambda);
  }
  if (alg == 3) {
    //SRT();
  }
  if (alg == 4) {
    std::cout << processes.size() << std::endl;
    for (unsigned int i = 0;i < processes.size();i++) {
      std::cout << "Process " << processes[i]->getName() << " [NEW] (arrival time ";
      std::cout << processes[i]->getIAT() << " ms) " << processes[i]->getBurstNum();
      std::cout << " CPU bursts (tau " << 1 / lambda << "ms)" << std::endl;
    }
    RR(processes, tSlice);
  }


}

int main(int argc, char* argv[]) {
  //Loop to run all scheduling algorithms
  for (int algo = 1; algo < 2; algo++) {
    int numProcesses = 0;
    double lambda;
    int maxNum;
    int tSlice;

    //Read and store command line information
    numProcesses = atoi(argv[1]);
    srand48(atoi(argv[2]));
    lambda = atof(argv[3]);
    maxNum = atoi(argv[4]);
    tSlice = atoi(argv[7]);


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
    orderQueue(orderedQueue, algo, numProcesses, queue, lambda, tSlice);
    for (int i = 0;i<numProcesses;i++) {
      queue[i]->removeProcess();
      delete queue[i];
    }
    std::cout << "----------------------------------------------------------" << std::endl;
  }
  return 0;

}
