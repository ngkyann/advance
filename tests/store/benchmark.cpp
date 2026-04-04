#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include "../../include/adv/store/store.hpp"
// #include "store.hpp" neu luu cung thu muc

using namespace std;
using namespace std::chrono;

const int ELEMENTS = 10000000;

void print_time(string label, nanoseconds ns) {
    cout << label << ": " << duration_cast<milliseconds>(ns).count() << "ms (" 
         << ns.count() << " ns)" << endl;
}

int main() {
    cout << "--- He thong Benchmark adv::store vs std::vector ---" << endl;
    cout << "So luong phan tu: " << ELEMENTS << "\n" << endl;

    // --- TEST 1: PUSH BACK ---
    auto start = high_resolution_clock::now();
    vector<int> v_raw;
    for(int i = 0; i < ELEMENTS; ++i) v_raw.push_back(i);
    auto end = high_resolution_clock::now();
    auto time_v_push = end - start;

    start = high_resolution_clock::now();
    adv::store<int> s_store;
    for(int i = 0; i < ELEMENTS; ++i) s_store.push_back(i);
    end = high_resolution_clock::now();
    auto time_s_push = end - start;
    cout << "------------------------------------------" << endl;

    // --- TEST 2: DUYET & TINH TONG (Read performance) ---
    long long sum_v = 0;
    start = high_resolution_clock::now();
    for(int i = 0; i < ELEMENTS; ++i) sum_v += v_raw[i];
    end = high_resolution_clock::now();
    auto time_v_read = end - start;

    long long sum_s = 0;
    start = high_resolution_clock::now();
    for(int i = 0; i < ELEMENTS; ++i) sum_s += s_store[i];
    end = high_resolution_clock::now();
    auto time_s_read = end - start;
    cout << "------------------------------------------" << endl;

    // --- TEST 3: SLICING (Dac san) ---
    // Lay 1/2 mảng ở giữa
    int start_slice = ELEMENTS / 4;
    int end_slice = ELEMENTS / 2;

    start = high_resolution_clock::now();
    vector<int> v_sub(v_raw.begin() + start_slice, v_raw.begin() + end_slice);
    auto vv_sub = v_sub;
    end = high_resolution_clock::now();
    auto time_v_slice = end - start;

    start = high_resolution_clock::now();
    auto s_sub = s_store(start_slice, end_slice);
    end = high_resolution_clock::now();
    auto time_s_slice = end - start;
    
    print_time("std::vector PushBack", time_v_push);
    print_time("adv::store PushBack", time_s_push);
    
    
    print_time("std::vector Read/Access", time_v_read);
    print_time("adv::store   Read/Access", time_s_read);
    
    print_time("std::vector Sub-range copy", time_v_slice);
    print_time("adv::store   operator() slice", time_s_slice);
    
    return 0;
}