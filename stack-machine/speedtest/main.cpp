#include <sys/time.h>
#include <iostream>

using namespace std;

__int64_t getUsecTimestamp(void) {
    timeval tv;
    gettimeofday(&tv, NULL);
    __int64_t s = tv.tv_sec * 1000000L + tv.tv_usec;
    return s;
}

int main(int argc, char* argv[]) {
    auto start_time = getUsecTimestamp();
    volatile long int i = 5000000000L;

    while(i > 0)
        i--;

    auto end_time = getUsecTimestamp();

    cout << "Delta time: " << (end_time-start_time)/1000L << endl;

    return 0;
}
