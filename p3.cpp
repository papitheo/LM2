#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<condition_variable>
#include <chrono> 
#include <cstdlib>
#include <ctime>
using namespace std;
std::mutex mtx;
std::condition_variable cv;
bool shuffled = false;
/// code here is wwritten and based on inclass example method
vector<int> numbers = {2,5,7,4,79,35,68,90};


void printarray (){
    for (int i=0; i< numbers.size(); i++){
        cout << numbers[i] << " " ;
    }
    cout << endl;
};

// checking if array is sorted or not 
bool isSorted() {
    for (int i = 1; i < numbers.size(); i++) {
        if (numbers[i] < numbers[i - 1]) {
            return false;
        }
    }
    return true;
}
/// Shuffle the array 
// Source: https://documents.uow.edu.au/~lukes/textbook/notes-cpp/misc/random-shuffle.html and Inclas example 
void shufflearray() {
    for (int rotate= 0; rotate<5; rotate++){
        unique_lock<mutex> lck(mtx);
            while (shuffled)
                cv.wait(lck);
            int n = numbers.size();
            for (int i = 0; i < n; i++) {
                int r = rand() % n;
                int temp = numbers[i];
                numbers[i] = numbers[r];
                numbers[r] = temp;
            }

    cout << "array is shuffled: " << endl;
    shuffled = true;
    cv.notify_all();
        }
}

// this concept taken from inclass example
void sortarray(){
    for (int rotate= 0; rotate<5; rotate++){
        unique_lock<mutex> lck(mtx);
        cout << "thread waiting for shuffle to be true" << endl;

        while (!shuffled) 
            cv.wait(lck);
        int n = numbers.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (numbers[j] > numbers[j + 1]) {
                    int temp = numbers[j];
                    numbers[j] = numbers[j + 1];
                    numbers[j + 1] = temp;
                }
            }
        }
        cout << "array is sorted: " << endl;
        cout << "rotation" << rotate << endl;
        printarray();

cout << "Is the array sorted? : " << isSorted() << endl;


        shuffled= false;
        lck.unlock();
        cv.notify_all();
            }

        }
    








int p3 (){
 
    thread t1(sortarray);
    thread t2(shufflearray);

    t1.join();
    t2.join();


    return 0;
}