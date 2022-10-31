#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
using namespace std;
constexpr long long value = 1000000;
mutex myMutex;

void Min_Max( long& min, long& max,  const vector<int>& val, long beg, long end) {

    // Grab first element of sub-array
    int local_min = val[beg];
    int local_max = val[beg];
   
    // Iterate through sub-array
    for (auto it = beg; it < end; ++it) {
        // Compare current value to local minimum
        if (val[it] < local_min) {
            
            local_min = val[it];
        }
        // Compare current value to local maximum
        else if ( val[it] > local_max) {
            local_max = val[it];
        }
    }
    // Lock critical section
    // Update current min and max
    // Only allow one thread to update global min and max at a time
    lock_guard<mutex> myLock(myMutex);
    if (local_min < min) {
        min = local_min;
    }
    if (local_max > max) {
        max = local_max;
    }
}

int main() {
    cout << endl;
    // Generate array of random values seeded with the current time
    vector<int> randValues;
    randValues.reserve(value);
    mt19937 engine(time(NULL));
    uniform_int_distribution<> uniformDist(INT_MIN, INT_MAX);
    for (long long i = 0; i < value; ++i) {
        randValues.push_back(uniformDist(engine));
    }

    // Initialize global min and max
    long min = randValues[0];
    long max = randValues[0];

    // Start timer
    auto start = chrono::system_clock::now();

    // Create thread pool and partition array evenly among them
    const int threads = 2; //////////////////////////// CHANGED TO CONST ////////////////////////////
    thread t[threads];
    long long slice = value / threads;
    int startIdx = 0;
    for (int i = 0; i < threads; ++i) {
        cout << "Thread[" << i << "] - slice [" << startIdx << ":" << startIdx + slice - 1 << "]" << endl;

        t[i] = thread(Min_Max, &min, ref(max), ref(randValues), startIdx, startIdx + slice - 1);

        startIdx += slice;
    }
    // Join threads one-by-one 
    // They will return when they're finished with their work
    for (int i = 0; i < threads; ++i) {
        t[i].join();
    }
    
   /* cout << "Random List:" << endl;
    for (int i = 0; i < randValues.size(); i++) {
        cout << " " <<  randValues[i];
    }
    cout << endl;*/

    chrono::duration<double> dur = chrono::system_clock::now() - start;
    cout << "Time for addition " << dur.count() << " seconds" << endl;
    cout << "Result min: " << min << endl;
    cout << "Result max: " << max << endl;
    cout << endl;
}