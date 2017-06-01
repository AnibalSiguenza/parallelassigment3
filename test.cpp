#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

int global=0;

void product(std::promise<int> && p, int * a, int * b){
    if(a[0]==10){
        p.set_value(a[0]++);
    }
    //p.set_value(global=10);
}

int main(){
    int a[3]={10,20,30};
    int b[3]={2,3,4};
    std::thread * myThread=new std::thread[3];
    std::promise <int> *  myPromise=new std::promise<int>[3];
    std::future <int> *  myFuture=new std::future<int>[3];
    for(int i=0;i<3;i++){
        myFuture[i] = myPromise[i].get_future();
        myThread[i]=std::thread(product,std::move(myPromise[i]),a,b);
    }
    for(int i=0;i<3;i++){
        myThread[i].join();
    }
    for(int i=0;i<3;i++){
        std::cout << "a[" << i << "]=" << a[i] << "\n";
    }
    std::cout << a << std::endl;
}
