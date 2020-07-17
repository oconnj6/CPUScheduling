#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"

void FCFS(std::vector<Process> &queue,std::vector<Process*> &processes, int numP) {
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

void SJF(std::vector<Process*> & processes, float lambda) {
  std::cout << "time 0ms: Simulator started for SJF [Q <empty>]" << std::endl;
  std::vector<Process> readyQueue;
  std::vector<Process> waitQueue;
  int burstCount = 0;

  while(1) {
    break;
  }
}


bool operator<(const Process & lhs, const Process & rhs) {
    return lhs.getIAT() < rhs.getIAT();
}

void orderQueue(std::vector<Process> &queue, int alg,
                    int numP, std::vector<Process*> &processes, float lambda) {
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
    //RR();
  }
}

int main(int argc, char* argv[]) {
  //Loop to run all scheduling algorithms
  for (int algo = 1; algo < 3; algo++) {
    int numProcesses = 0;
    double lambda;
    int maxNum;

    //Read and store command line information
    numProcesses = atoi(argv[1]);
    srand48(atoi(argv[2]));
    lambda = atof(argv[3]);
    maxNum = atoi(argv[4]);

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
    sort(queue.begin(), queue.end(), comparator);
    std::reverse(queue.begin(), queue.end());
    orderQueue(orderedQueue, algo, numProcesses, queue, lambda);
    std::cout << "----------------------------------------------------------" << std::endl;
  }
  return 0;

}
