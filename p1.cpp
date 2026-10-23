#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;


//Bank account class with required variables
class BankAccount {
    public:
    int accountID = 1;
    float balance = 150.25;
    mutex mtx;
    condition_variable cv;


//deposit func
void deposit (float amount, int threadnumber){
    
    mtx.lock ();
    cout << "Thread" << threadnumber << ": submitted the deposit of $ " << amount << endl;
    float temp = balance;
    temp = temp + amount;
    balance = temp;
    cout << "Thread" <<threadnumber << ": Deposited $ " << amount << ". Balance: $" << balance<<endl;
    mtx.unlock ();
    cv.notify_all(); // this cocnept from the inclass example

}

//withdrwal func
void withdrawl (float amount, int threadnumber) {
    unique_lock<mutex> lck(mtx); // from inclass example
    cout << "Thread" << threadnumber<< ": requested withdrawal of $" << amount<< ". Balance: $" << balance << endl;
    while (balance < amount){
    cout << "Thread" << threadnumber<< ": Withdrawal waiting. Needs $" << amount<< ", balance: $" << balance << endl;

    cv.wait(lck);

    }
    float temp = balance;
    temp = temp - amount;
    balance = temp;

cout << "Thread " << threadnumber<< ": Completed withdrawal of $" << amount<< ". Balance: $" << balance << endl;
    
}
//print func
void print (){

    mtx.lock();
    cout << "Account ID: " << accountID << endl;
    cout << "Blance : " << balance << endl;
    mtx.unlock();
}
};

int Part1() {

BankAccount account;
thread threads[100];

//// 16*50=800
for (int i=0; i<50; i++) {
    threads[i]= thread(&BankAccount::deposit, &account, 16, i+1);
};
// withdrwal 18*50=900
for (int i = 50; i < 100; i++) {
    threads[i] = thread(&BankAccount::withdrawl, &account, 18, i+1);

};
for (int i=0; i<100; i++){
    threads[i].join();
};  
// Final blance should be 150.25+800-900=50.25
account.print();
    



cout << "-------------------This is the end of part 1-------------------------" << endl;


return 0;
};
