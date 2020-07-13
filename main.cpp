#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Process.h"

void FCFS(std::vector<Process> &queue,std::vector<Process> &processes, int numP) {
  for (int i = 0; i<numP;i++) {
    queue[i] = processes[i];
    std::cout << i << " - " << queue[i].getBurstNum() << std::endl;
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

      //Calculates CPU and IO burst times for each CPU bursts
      //NOT FINISHED
      for (int i = 0; i < x ;i++) {
        std::pair<double, double> p;
        p.first = drand48();
        p.second = drand48();
      }

    }

    //Orders the queue according to the proper algorithm
    orderQueue(orderedQueue, 1, numProcesses, queue);

  //  std::cout << "----------------------------------------------------------" << std::endl;

  //queue[2].printName();
  //Process a = Process();



  //printf("%f\n", lambda);
  }
  return 0;

}
