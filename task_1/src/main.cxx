#include <iostream>
#include <random>
#include <future>
#include <vector>

void add_data(std::vector<int>& otherVector);
void selection_sort(std::vector<int>& otherVector);

int main(void) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::vector<int> data;
    add_data(data);
    std::cout << "До сортировки: ";
    for (const auto& v : data)
        std::cout << v << " ";
    std::cout << std::endl;

    // Вроде работает как надо, но не совсем понятно
    // ...
    selection_sort(data);
    // ...

    std::cout << "После соритровки: ";
    for (const auto& v : data)
        std::cout << v << " ";
    std::cout << std::endl;
    return EXIT_SUCCESS;
}

void selection_sort(std::vector<int>& data_) {
    int v_size = data_.size();
    int tmp, min = 0;
    for (int i = 0; i < v_size; i++) {
        min = i;
        std::promise<int> ff_promise;
        std::future<int> ff_future = ff_promise.get_future();
        auto res = std::async(std::launch::async, [&i, &v_size, &data_](int min, std::promise<int> pr_){
            for (int j = i; j < v_size; j++)
                if (data_[min] > data_[j]) min = j;
            pr_.set_value(min);
        }, min, std::move(ff_promise));
        ff_future.wait(); // Если это не писать, то работает так же
        min = ff_future.get();
        tmp = data_[i];
        data_[i] = data_[min];
        data_[min] = tmp;
    }
}

void add_data(std::vector<int>& otherVector) {
    for (int i=0; i < 10; ++i) {
        std::random_device rd;
	    std::mt19937 gen{ rd() };
	    std::uniform_int_distribution<> dis(0, 100);
        int randValues = dis(gen);
        otherVector.push_back(randValues);
    }
}