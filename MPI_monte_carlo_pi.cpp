#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

#include <math.h>
#include <iostream>

# define PI        3.14159265358979323846  /* pi */
#define MAX_POINTS 1000000 

// Command line argument
int points;

static int parseCmdArgs(int argc, char** argv) {
    points = static_cast<float>(atof(argv[1]));
    return 0;
}

int main(int argc, char* argv[]) {
    int myid, numprocs, namelen, inside = 0, outside = 0, interval;

    char processor_name[MPI_MAX_PROCESSOR_NAME];

    double x, y, distance, loc_pi_approx, glob_pi_approx, avg_pi, delta, startwtime, runTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    
    int retval = parseCmdArgs(argc, argv);
    
    interval = (int)floor(points / numprocs);

    srand((unsigned)(myid));
    if (myid == 0) {

        startwtime = MPI_Wtime();
        std::cout << "points............ " << points << std::endl;
        std::cout << "interval............ " << interval << std::endl;
        printf("Total points: %d Interval: %d points per node\n", points, interval);

        for (int i = 0; i < interval; i++) {
            x = ((double)rand()) / ((double)RAND_MAX);
            y = (double)rand() / (double)RAND_MAX;
            //calculate distance
            distance = pow(x, 2) + pow(y, 2);
            
            if (distance <= 1) {
                inside += 1;
            }           
        }
        loc_pi_approx = (double)(4 * inside) / interval;
        printf("Process %d on %s, approximate value of PI: %f\n", myid, processor_name, loc_pi_approx);
    }
    else {

        for (int i = 0; i < interval; i++) {
            x = ((double)rand()) / ((double)RAND_MAX);
            y = (double)rand() / (double)RAND_MAX;
            //calculate distance
            distance = pow(x, 2) + pow(y, 2);

            if (distance <= 1) {
                inside += 1;
            }
        }
        loc_pi_approx = (double)(4 * inside) / interval;
        printf("Process %d on %s, approximate value of PI: %f\n", myid, processor_name, loc_pi_approx);
    }

    MPI_Reduce(&loc_pi_approx, &glob_pi_approx, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    avg_pi = glob_pi_approx / numprocs;
    delta = abs(avg_pi - PI);

    if (myid == 0) {
        runTime = MPI_Wtime() - startwtime;
        printf("Execution time (sec) = %f\n", runTime);
        std::cout << "REDUCE VALUE............ " << glob_pi_approx << std::endl;
        std::cout << "AVERAGE PI VALUE............ " << avg_pi << std::endl;
        std::cout << "DELTA VALUE............ " << delta << std::endl;
    }

    MPI_Finalize();
}

