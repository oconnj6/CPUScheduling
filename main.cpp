#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"

void FCFS(std::vector<Process> &queue,std::vector<Process> &processes, int numP) {
  std::vector<Process> readyQueue = processes;
  int readyQSize = numP;
  int waitQSize = 0;
  double time = 0.0;
  std::vector<Process> waitQueue;

  //Do not have running state variable since readyQueue[0] is the same thing

  //Does not work
  while(1) {
    std::cout << readyQueue[0].getCPUTime() << " - ";
    //Adds the amount of time the next process takes on the CPU
    time += readyQueue[0].getCPUTime();
    std::cout << time << std::endl;

    //Make sure operator = works correctly -- Is this the best way to do it. I Dont think so.
    //Adds running process to the waitQueue
    waitQueue[waitQSize] = readyQueue[0];
    waitQSize++;

    //Moves all elements in the ready queue up
    for (int i = 1; i<readyQSize; i++) {
      readyQueue[i-1] = readyQueue[i];
    }
    readyQSize--;

    //Breaks infinite loop
    if (waitQSize == 50) {
      break;
    }
  }

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
    std::vector<Process> queue(numProcesses);

    //Empty vector to be arranged by scheduling algorithm
    std::vector<Process> orderedQueue(numProcesses);

    //Creation of processes
    for ( int k = 0; k <numProcesses;k++) {
      //Figures out name
      std::string letter;
      letter = char (65+k);

      //Creates Process object
      Process temp = Process(letter);

      //Adds to the vector of processes
      queue[k] = temp;

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
      //NOT FINISHED
      for (int i = 0; i < x ;i++) {

        //Allocates data that is not yet freed
        std::pair<double, double>* p = new std::pair<double, double>;
        p -> first = drand48();

        //Prevents the last pair to have an io time since processes end in CPU
        if (i != x-1) {
          p -> second = drand48();
        }

        (*times)[i] = *p;
      }
      //Sets burst times
      queue[k].setBurstTimes(times);

    }
    //std::cout << queue[1].getCPUTime() << std::endl;
    //Orders the queue according to the proper algorithm
    orderQueue(orderedQueue, 1, numProcesses, queue);

  //  std::cout << "----------------------------------------------------------" << std::endl;

  //queue[2].printName();
  //Process a = Process();



  //printf("%f\n", lambda);
  }
  return 0;

}
