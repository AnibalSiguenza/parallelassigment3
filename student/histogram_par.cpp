#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include  <iostream> 
#include <iterator>

histogram_t count_words(const std::vector<word_t> & words, int vec_init, int vec_end, histogram_t histogram){
    std::for_each(std::next(words.begin(), vec_init), std::next(words.begin(), vec_end), [&histogram](const word_t& word)
            {
            int res = getNameIndex(word.data());
            if (res != -1){ histogram[res]++;}
            });
    return histogram;
}

void get_histogram(const std::vector<word_t> & words, histogram_t & histogram, const int num_threads){
    std::vector<std::future<histogram_t>> futures;

    int num_words=words.size(); 
    int nWordsPThreads=num_words/num_threads;
    int resWords=num_words%nWordsPThreads; 

    for(int j=0; j<NNAMES; j++){
        histogram[j]=0;
    }

    if(resWords==0){
        for(int i=0;i<num_threads;i++){
            futures.push_back(std::async(std::launch::async, count_words, std::ref(words), i*nWordsPThreads, (i+1)*nWordsPThreads-1, histogram));
        }
    }else{
        for(int i=1;i<num_threads;i++){
           futures.push_back(std::async(std::launch::async, count_words, std::ref(words), (i-1)*nWordsPThreads, i*nWordsPThreads-1, histogram));
        }
        futures.push_back (std::async(std::launch::async, count_words, std::ref(words), (num_threads-1)*nWordsPThreads,\
                    (num_threads)*nWordsPThreads+resWords-1, histogram));
    }

    for(int i=0;i<num_threads;i++){
        auto temp_histo=futures[i].get();
        for(int j=0; j<NNAMES; j++){
            histogram[j]+=temp_histo[j];
        }
    }
}
