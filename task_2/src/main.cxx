#include <iostream>
#include <future>
#include <algorithm>
#include <random>
#include <vector>

template<class InputIterator, class Function>
void parallel_for_each(InputIterator begin, InputIterator end, Function f) {
    size_t range = std::distance(begin, end);
    size_t max_chunk = 50;
    if (range <= max_chunk)
        std::for_each(begin, end, f);
    else {
        InputIterator block_start = begin;
        InputIterator block_end = begin + max_chunk;
        auto res = std::async(std::launch::async, parallel_for_each<InputIterator, Function>, block_start, block_end, f);
        block_start = block_end;
        parallel_for_each(block_start, end, f);
    }
}

void add_data(std::vector<int>& otherVector, int n_ = 50) {
    for (int i=0; i < n_; ++i) {
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
    int nNumbers = 200;
    add_data(data, nNumbers);

    // Before
    std::cout << "Before:\n";
    for (auto const &v : data)
        std::cout << " " << v;
    std::cout << std::endl;

    // Just multiply numbers
    parallel_for_each(data.begin(), data.end(), multiply);

    // After
    std::cout << "After:\n";
    for (auto const &v : data)
        std::cout << " " << v;
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
