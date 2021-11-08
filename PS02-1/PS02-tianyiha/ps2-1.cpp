using namespace std;
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <ctime>

int main()
{
    printf("24-780 Engineering Computation Problem Set 2-1 -Fast Number Typing Game\n");
    this_thread::sleep_for(chrono::milliseconds(500));
    printf("3\n");
    this_thread::sleep_for(chrono::milliseconds(1000));
    printf("2\n");
    this_thread::sleep_for(chrono::milliseconds(1000));
    printf("1\n");
    this_thread::sleep_for(chrono::milliseconds(500));
    printf("Go!\n");

    srand(time(0));
    int tarNum, inp = 1000;

    auto start = chrono::steady_clock::now();
    
    for (int i = 1; i <= 10; i++) {
        tarNum = rand() % 1000;
        while (tarNum != inp) {
            printf("Type %d:", tarNum);
            scanf("%d", &inp);
            if (tarNum != inp) {
                printf("Incorrect! Try again!\n");
            }
            else {
                break;
            }
        }
    }
    
    auto end = chrono::steady_clock::now();
    double dur = chrono::duration_cast<chrono::seconds>(end - start).count();

    printf("lapsed %d seconds.", int(dur));
    return 0;
}
