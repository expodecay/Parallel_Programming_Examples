#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <iomanip>


using namespace std;

#define PI          3.141592 
#define MIN_POINTS  10
#define MAX_POINTS  1000000 
#define MIN_THREADS 1
#define MAX_THREADS 10

mutex myMutex;

static void printUsage(char** argv) {
    cout << "usage: monte [1...10] [10...1000000]" << endl;
}

// Command line arguments
int points, thread_count;
static int parseCmdArgs(int argc, char** argv) {
    if ((argc == 1) | (argc > 3)) {
        printUsage(argv);
        return -1;
    }
    if ((atof(argv[1]) < MIN_THREADS) | (atof(argv[1]) > MAX_THREADS)) {
        printUsage(argv);
        return -1;
    }
    if ((atof(argv[2]) < MIN_POINTS) | (atof(argv[2]) > MAX_POINTS)) {
        printUsage(argv);
        return -1;
    }
    
    thread_count = static_cast<int>(atof(argv[1]));
    points = static_cast<int>(atof(argv[2]));

    return 0;
}

// Monte Carl Pi calculation
void calc_pi(int& test_interval, double& PI_approx, int thread_number) {
    double x, y, dist, local_PI, inside = 0;
    
    srand(time(NULL) + thread_number);
    for (int i = 0; i < test_interval; i++) {
        x = ((double)rand()) / ((double)RAND_MAX);
        y = ((double)rand()) / ((double)RAND_MAX);

        dist = pow(x, 2) + pow(y, 2);

        if (dist <= 1) {
            inside += 1;
        }
    }
    local_PI = (double)(4 * inside) / test_interval;

    printf("Thread[%d] -- Local value of Pi... %f\n", thread_number, local_PI);

    // Critical Section
    // Add local Pi calculation to PI_approx
    lock_guard<mutex> myLock(myMutex);
    PI_approx += local_PI;
}

int main(int argc, char* argv[]) { 
    double PI_approx, delta;

    // Parse arguments and calculate interval for each thread 
    int retval = parseCmdArgs(argc, argv);
    int interval = (int)floor(points / thread_count);
    
    // Start timer
    auto start = chrono::system_clock::now();

    // Allocate space for an array of threads
    thread* myThreads = new thread[thread_count];

    // For each thread, approximate pi using 'inverval' number of test points
    for (int i = 0; i < thread_count; ++i) {
        myThreads[i] = thread(calc_pi, ref(interval), std::ref(PI_approx), i);
    }

    // Join threads when they finish their work
    for (int i = 0; i < thread_count; ++i) {
        myThreads[i].join();
    }
    
    // Calculate average Pi and delta from true PI
    PI_approx = PI_approx / thread_count;
    delta = abs(PI - PI_approx);

    // End timer
    chrono::duration<double> dur = chrono::system_clock::now() - start;
    
    cout << endl;
    std::cout << std::setprecision(4) << std::fixed;
    cout << "Global Pi approximation..... " << PI_approx << endl;
    std::cout << std::setprecision(4) << std::fixed;
    cout << "Delta....................... " << delta << endl;
    std::cout << std::setprecision(6) << std::fixed;
    cout << "Compute time................ " << dur.count() << " seconds" << endl;

    // Destroy array to prevent memory leak
    delete[] myThreads;
    return 0;
}