#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include  <iostream> 

void count_words(std::promise<int> && p, const std::vector<word_t>& words, histogram_t & histogram, int init, int end){
    std::for_each(begin(words), end(words), [&histogram](const word_t& word)
            {
            int res = getNameIndex(word.data());
            if (res != -1){ histogram[res]++;}
            });
}

void get_histogram(const std::vector<word_t> & words, histogram_t & histogram,const int num_threads){
    std::thread * threads= new std::thread[num_threads];
    std::promise<int> * promise_histograms=new std::promise<int>[num_threads];
    std::future<int> * future_histograms=new std::future<int>[num_threads];

    int num_words=words.size(); 
    int nWordsPThreads=num_words/num_threads;
    int resWords=num_words%nWordsPThreads; 

    if(resWords==0){
        for(int i=0;i<num_threads;i++){
            future_histograms[i]=promise_histograms[i].get_future();
            threads[i]=std::thread(count_words,std::move(promise_histograms[i]), words, histogram, i*nWordsPThreads, (i+1)*nWordsPThreads);
        }
    }else{
        for(int i=1;i<num_threads;i++){
            future_histograms[i]=promise_histograms[i].get_future();
            threads[i]=std::thread(count_words,std::move(promise_histograms[i]), words, histogram, (i-1)*nWordsPThreads, i*nWordsPThreads);
        }
        future_histograms[num_threads-1]=promise_histograms[num_threads-1].get_future();
        threads[num_threads-1]=std::thread(count_words,std::move(promise_histograms[num_threads-1]), words, histogram,
                (num_threads-1)*nWordsPThreads, (num_threads)*nWordsPThreads+resWords);
    }
    for(int i=0;i<num_threads;i++){
        threads[i].join();
    }
}


