#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <random>

#define ARRAY_SIZE 1000000

int main(int argc, char* argv[]) {

    int myid, numprocs;
    int namelen;
    int* numbers = new int[ARRAY_SIZE];
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    printf("Process %d on %s\n", myid, processor_name);

    srand((unsigned)(myid));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = (int)rand();  //could be randomly generated
    }

    int s = (int)floor(ARRAY_SIZE / numprocs);
    int s0 = s + ARRAY_SIZE % numprocs;

    int startIndex = s0 + (myid - 1) * s;
    int endIndex = startIndex + s;

    double startwtime;
    if (myid == 0) {
        startwtime = MPI_Wtime();
    }

    int i;

    int local_max = numbers[startIndex];
    int local_min = numbers[endIndex];

    if (myid == 0) {
        
        for (i = 0; i < s0; i++) {
            if (numbers[i] > local_max)
                local_max = numbers[i];
            if (numbers[i] < local_min)
                local_min = numbers[i];
        }
        printf("Process %d - startIndex 0 endIndex %d; Local Min: %ld Local Max: %ld\n", myid, s0 - 1, local_min, local_max);
    }
    else {
        
        for (i = startIndex; i < endIndex; i++) {
            if (numbers[i] > local_max)
                local_max = numbers[i];
            if (numbers[i] < local_min)
                local_min = numbers[i];
        }
        printf("Process %d - startIndex %d endIndex %d; Local Min: %ld Local Max: %ld\n", myid, startIndex, endIndex - 1, local_min, local_max);
    }

    int global_max = 0;
    int global_min = 0;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        double runTime = MPI_Wtime() - startwtime;
        printf("Execution time (sec) = %f Global Min: %ld Global Man: %ld\n", runTime, global_min, global_max);
    }
    
    delete[] numbers;

    MPI_Finalize();
}

