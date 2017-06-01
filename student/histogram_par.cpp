#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include  <iostream> 
#include <iterator>

void count_words(const std::vector<word_t>& words, histogram_t & histogram, int vec_init, int vec_end){
    std::for_each(std::next(words.begin(), vec_init), std::next(words.begin(), vec_end), [&histogram](const word_t& word)
            {
            int res = getNameIndex(word.data());
            if (res != -1){ histogram[res]++;}
            });
}

void get_histogram(const std::vector<word_t> & words, histogram_t & histogram,const int num_threads){
    std::vector<std::future<void>> futures;
    histogram_t * histograms_threads=new histogram_t[num_threads];

    int num_words=words.size(); 
    int nWordsPThreads=num_words/num_threads;
    int resWords=num_words%nWordsPThreads; 

    if(resWords==0){
        for(int i=0;i<num_threads;i++){
            futures.push_back(std::async(std::launch::async, count_words, std::ref(words), histograms_threads[i], i*nWordsPThreads, (i+1)*nWordsPThreads-1));
        }
    }else{
        for(int i=1;i<num_threads;i++){
            futures.push_back(std::async(std::launch::async, count_words, std::ref(words), histograms_threads[i], (i-1)*nWordsPThreads, i*nWordsPThreads-1));
        }
        futures.push_back (std::async(std::launch::async, count_words, std::ref(words), histograms_threads[num_threads-1], (num_threads-1)*nWordsPThreads, (num_threads)*nWordsPThreads+resWords-1));
    }

    for(int i=0;i<num_threads;i++){
        futures[i].get();
    }

    for(int j=0; j<NNAMES; j++){
        histogram[j]=0;
        for(int i=0;i<num_threads;i++){
            histogram[j]+=histograms_threads[i][j];
        }
    }

    delete [] histograms_threads;
}
