#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include  <iostream> 
#include <iterator>

histogram_t count_words( std::vector<word_t>::iterator& vec_init, std::vector<word_t>::iterator & vec_end, histogram_t histogram){
    for (auto w = vec_int; w != vec_end; ++w)
            {
            int res = getNameIndex((*r).data());
            if (res != -1){ histogram[res]++;}

    return histogram;
}

void get_histogram(const std::vector<word_t> & words, histogram_t & histogram,const int num_threads){
    std::vector<std::future<histogram_t>> futures;

    int num_words=words.size(); 
    int nWordsPThreads=num_words/num_threads;
    int resWords=num_words%nWordsPThreads; 

    for(int i=0;i<num_threads;i++){
        futures.push_back(std::async(std::launch::async, count_words, std::ref(words), std::begin(words) + i*nWordsPThreads,std::begin(words) + (i+1)*nWordsPThreads-1));
    }

    for(int i=0;i<num_threads;i++){
        auto temp_histo=futures[i].get();
        for(int j=0; j<NNAMES; j++){
            histogram[j]+=temp_histo[j];
        }
    }
}
