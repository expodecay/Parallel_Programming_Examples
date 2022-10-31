#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>


using namespace std;

#define PI         3.14159265358979323846  
#define MAX_POINTS 1000000 
mutex myMutex;

// Command line argument
int points, thread_count;
static int parseCmdArgs(int argc, char** argv) {
    thread_count = static_cast<float>(atof(argv[1]));
    points = static_cast<float>(atof(argv[2]));

    return 0;
}


//void Calculate_PI(int& interval, double& PI_approx) {
//    std::cout << "points............ " << points << std::endl;
//    std::cout << "interval............ " << interval << std::endl;
//
//    PI_approx = 3;
//    //printf("Total points: %d   Interval: %d points per node   PI Approximation: %d\n", points, interval, PI_approx);
//}
//
//int main(int argc, char* argv[]) {
//
//    int retval = parseCmdArgs(argc, argv);
//    int interval = (int)floor(points / thread_count); 
//    int PI_approx=0;
//
//    thread t1(Calculate_PI, ref(interval), ref(PI_approx));
//    t1.join();
//    return 0;
//}



void calc_pi(int& test_interval, double& PI_approx, int thread_number)  
{
    double x, y, dist, local_PI, inside = 0;
    
    srand(time(NULL) + thread_number);
    for (int i = 0; i < test_interval; i++) {
        x = ((double)rand()) / ((double)RAND_MAX);
        y = ((double)rand()) / ((double)RAND_MAX);

        /*cout << "x:  " << x << endl;
        cout << "y:  " << y << endl;*/

        //calculate distance
        dist = pow(x, 2) + pow(y, 2);

        if (dist <= 1) {
            inside += 1;
        }
    }
    //cout << "x: " << x << " y" << y << endl;
    local_PI = (double)(4 * inside) / test_interval;
    printf("Thread[%d] - Approximatex value of Pi: %f\n", thread_number, local_PI);
    // Critical Section
    // Add current PI calculation to PI_approximate
    lock_guard<mutex> myLock(myMutex);
    PI_approx += local_PI;
}

int main(int argc, char* argv[])
{ 
    cout << endl;
    int retval = parseCmdArgs(argc, argv);
    int interval = (int)floor(points / thread_count);
    double PI_approx = 0;

    //const int threads = 3; ///// fix this /////////
    //thread t[threads];
    //for (int i = 0; i < threads; ++i) {
    //    t[i] = thread(calc_pi, ref(interval), std::ref(PI_approx), i);
    //}
    //
    //
    //for (int i = 0; i < threads; ++i) {
    //    t[i].join();
    //}

    thread* myThreads = new thread[thread_count];

    for (int i = 0; i < thread_count; ++i) {
        myThreads[i] = thread(calc_pi, ref(interval), std::ref(PI_approx), i);
    }


    for (int i = 0; i < thread_count; ++i) {
        myThreads[i].join();
    }

    //std::thread t1(calc_pi,ref(interval), std::ref(PI_approx));  
    // t1.join();
    cout << endl;
    cout << "PI approximation: " << PI_approx << endl;

    delete[] myThreads;
    return 0;
}