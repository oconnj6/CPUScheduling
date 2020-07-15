#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"

void FCFS(std::vector<Process> &queue,std::vector<Process> &processes, int numP) {
  std::vector<int> readyQueue(numP);
  int readyQSize = numP;
  int waitQSize = 0;
  bool add = false;
  double time = 0.0;
  int count = 0;
  std::vector<int> waitQueue(numP);

  //Initializes ready Queue
  for (int i = 0;i <numP;i++) {
    std::cout << "Process " << processes[i].getName() << " [NEW] (arrival time ";
    std::cout << processes[i].getIAT() << " ms) " << processes[i].getBurstNum();
    std::cout << " CPU bursts" << std::endl;
    readyQueue[i] = i;
  }

  std::cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << std::endl;
  //Do not have running state variable since readyQueue[0] is the same thing

  //Does not work
  while(1) {
    //Checks if process has completed all bursts
    if (!processes[readyQueue[0]].isDone() ) {
        //std::cout << processes[1].getCPUTime() << " - " << readyQueue[1].getCPUTime() << std::endl;
        //  std::cout << readyQueue[0].getCPUTime() << " - ";

      //Adds the amount of time the next process takes on the CPU
      double CPUTime = 0.0;
      double IOTime = 0.0;
      //If readyQueue has a process, find the CPU time
      if (readyQSize > 0) {
        CPUTime = processes[readyQueue[0]].getCPUTime();
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]].getName();
        std::cout << " started using the CPU for " << CPUTime << "ms burst" << std::endl;
      }
      //If waitQ has a process, find the I/O time
      if (waitQSize > 0) {
        IOTime = processes[waitQueue[0]].getIOTime();
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]].getName();
        std::cout << " switching out of CPU; will block on I/O until time ";
        std::cout << time + IOTime << "ms" << std::endl;
      }

      //Needs to be fixed!!
      if (CPUTime < IOTime) {
        time += IOTime;
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]].getName();
        std::cout << " completed I/O; added to ready queue" << std::endl;
      }
      else {
        time += CPUTime;
        count++;
        std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]].getName();
        std::cout << " completed a CPU burst; " << processes[readyQueue[0]].getBurstNum() - count;
        std::cout << " bursts to go" << std::endl;
      }

    //  std::cout << time << " - " << processes[readyQueue[0]].getName() << " - " << count << std::endl;
    //  std::cout << waitQSize << " - " << readyQSize << std::endl;

      //Moves process from waitQ to readyQ
      if (waitQSize > 0) {
        readyQueue[readyQSize] = waitQueue[0];
        waitQSize--;
        add = true;
        //readyQSize++;
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

      //Breaks infinite loop
      /*
      if (count == 50) {
        break;
      }
      */

    }
    //Handles when a process has completed its bursts
    //Not correct/complete
    else {
      std::cout << "time " << time << "ms: Process " << processes[readyQueue[0]].getName();
      std::cout << " terminated" << std::endl;
      for (int i = 1; i<readyQSize; i++) {
        readyQueue[i-1] = readyQueue[i];
      }
      readyQSize--;
      readyQueue[readyQSize] = waitQueue[0];
      //processes[readyQueue[0]].printName();
      waitQSize--;
      readyQSize++;
      break;
    }
  }
  std::cout << "time " << time << "ms: Simulator ended for FCFS" << std::endl;

}

void orderQueue(std::vector<Process> &queue, int alg,
                    int numP, std::vector<Process> &processes) {
  if (alg == 1) {
    FCFS(queue, processes, numP);
  }
  if (alg == 2) {
    //SJF();
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
    std::vector<Process> queue;

    //Empty vector to be arranged by scheduling algorithm
    std::vector<Process> orderedQueue(numProcesses);

    //Creation of processes
    for ( int k = 0; k <numProcesses;k++) {
      //Figures out name
      std::string letter;
      letter = char (65+k);

      //Creates Process object
      Process* temp = new Process(letter);

      //temp -> printName();
      //Adds to the vector of processes
      queue.push_back(*temp);

      //Calculates initial process arrival time
      //DO NOT KNOW IF I DID THIS CORRECTLY
      double r = drand48();
      double x = -log( r ) / lambda;
      if (x > maxNum) {
        continue;
      }
      queue[k].setIAT(x);

      //Calculates number of CPU bursts for a process
      r = drand48();
      x = int (r * 100);
      queue[k].setBurstNum(x);

      //Allocates data that is not yet freed
      std::vector<std::pair<double,double> >* times = new std::vector<std::pair<double,double> >(x);
      //Calculates CPU and IO burst times for each CPU bursts
      for (int i = 0; i < x ;i++) {

        //Allocates data that is not yet freed
        std::pair<double, double>* p = new std::pair<double, double>;
        p -> first = drand48();

        //Prevents the last pair to have an io time since processes end in CPU
        if (i != x-1) {
          p -> second = drand48();
        }

        (*times)[i] = *p;
        //std::cout << (*times)[i].first << std::endl;
      }
      //Sets burst times
      queue[k].setBurstTimes(times);

    }
    //std::cout << queue[1].getBurstNum() << std::endl;
    //Orders the queue according to the proper algorithm
    orderQueue(orderedQueue, 1, numProcesses, queue);

  std::cout << "----------------------------------------------------------" << std::endl;

  //queue[2].printName();
  //Process a = Process();



  //printf("%f\n", lambda);
  }
  return 0;

}
