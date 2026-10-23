#include <iostream>
#include <thread>
#include<mutex>
#include <condition_variable>
using namespace std;
// Bnak account class 
class Bankaccount {
    public:
    int accountID = 1;
    float balance = 300;
    int pendingPriorityWithdrawals = 0; //setting it to 0 so it cant count how many pending priority withdrwal are waiting
    mutex mtx;
    condition_variable cv;


//deposit stay smae as part 1
    void deposit (float amount, int threadnumber){
        mtx.lock ();
        cout << "Thread " << threadnumber<< ": Requested deposit of $" << amount << endl;
        float temp = balance;
        temp = temp + amount;
        balance = temp;
        cout << "Thread " << threadnumber<< ": Deposited $" << amount<< ". Balance: $" << balance << endl;
        mtx.unlock ();
        cv.notify_all(); 

    }   
    
    



//Normal withdrwal fucntion now checks two reuiremtns, 
// balance must be greater than amount and pending priority withdrawals must be 0
    void withdrawl (float amount, int threadnumber) {
        unique_lock<mutex> lck(mtx); 
        cout << "Thread " << threadnumber << "; Requested withdrawl of $" << amount << ". Balance: $" << balance<<endl;
        
        while (balance < amount || pendingPriorityWithdrawals > 0) {
            cout << "Thread " <<threadnumber <<  "; Normal withdrawl waiting. Needs $" << amount << ", balance: $" << balance << ",pending withdrawl: " << pendingPriorityWithdrawals  << endl;
            cv.wait(lck);
        }
        float temp = balance;
        temp = temp - amount;
        balance = temp;
        cout << "Thread" << threadnumber << ": Completed normal withdrawl of $" << amount << ". Balance: $" << balance <<endl; 
        lck.unlock();
    }







//prioirty withdrwa function 
// works same as withdrwal but doesnot check for pendin prioirty withdrwal
    void Prioritywithdrawl (float amount, int threadnumber) {
        unique_lock<mutex> lck(mtx); 
        pendingPriorityWithdrawals++; //increment this so normal withrawl wait for this to finish
        cout << "Thread" << threadnumber << ": Requested priority withdrawl of $" << amount << ", Blance: $" << balance <<endl;





        while (balance < amount){

        cout << "Thread " << threadnumber<< ": Priority withdrawal waiting. Needs $" << amount<< ", balance: $" << balance << endl;

            cv.wait(lck);

        }
            
        float temp = balance;
        temp = temp - amount;
        balance = temp;
        cout << "Thread  " << threadnumber << "; completed priority withdrawl of $ " << amount << ". Balance: $" << balance << endl;
        pendingPriorityWithdrawals--;
        lck.unlock();
        cv.notify_all(); //notifying simple wthdrwal function   
    }









//smae print func as part 1
    void print() {
        
        mtx.lock();
        cout << "Account ID: " << accountID << endl;
        cout << "Blance : " << balance << endl;
        mtx.unlock();
    }

};


int part2 (){

Bankaccount account;
thread threads[100];

//// 25*16=400 priority withdrwal

for (int i=0; i<25; i++) {
    threads[i]= thread(&Bankaccount::Prioritywithdrawl, &account, 16, i+1);
    
};
// after priority withdrwal 25*18=450 normal withdrwal

for (int i = 25; i < 50; i++) {
    threads[i] = thread(&Bankaccount::withdrawl, &account, 18, i+1);
    
};
//deposit 50*18=900
for (int i = 50; i < 100; i++) {
    threads[i] = thread(&Bankaccount::deposit, &account, 18, i+1);
    
};
for (int i=0; i<100; i++){
    threads[i].join();
};  

account.print();


cout << "___________________PART2 ENDS HERE ________________________" << endl;


    return 0; 
}

