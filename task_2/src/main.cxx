#include <iostream>
#include <random>
#include <thread>
#include <vector>

template<class InputIterator, class Function>
void parallel_for_each(InputIterator begin, InputIterator end, Function f) {
    auto range = std::distance(begin, end);
    size_t numThreads = std::thread::hardware_concurrency();
 
    // number of elements for one thread
    size_t jobsForThread = range / numThreads;
    // number of elements for one thread + remainder
    size_t jobsForMainThread = range % numThreads + jobsForThread;
 
    // minus main thread
    std::vector<std::thread> threads(numThreads - 1);
 
    InputIterator block_start = begin;
    InputIterator block_end = begin + jobsForThread;
 
    for(size_t i = 0; i < numThreads - 1; ++i) {
        threads[i] = std::thread(std::for_each<InputIterator, Function>, block_start, block_end, f);
        block_start = block_end;
        block_end += jobsForThread;
    }

    std::for_each(block_start, block_start + jobsForMainThread, f);
 
    for(auto& t : threads) {
            t.join();
    }
}

void add_data(std::vector<int>& otherVector) {
    for (int i=0; i < 20; ++i) {
        std::random_device rd;
	    std::mt19937 gen{ rd() };
	    std::uniform_int_distribution<> dis(0, 100);
        int randValues = dis(gen);
        otherVector.push_back(randValues);
    }
}

void multiply(int& n) { n *= 2; }

int main(void) {
    std::vector<int> data;
    add_data(data);

    // Before
    for (auto const &v : data)
        std::cout << " " << v;
    std::cout << std::endl;

    // Just multiply numbers
    parallel_for_each(data.begin(), data.end(), multiply);

    // After
    for (auto const &v : data)
        std::cout << " " << v;
    std::cout << std::endl;


    return EXIT_SUCCESS;
}
