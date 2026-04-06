#include <iostream>
#include <chrono>
#include <random>
#include "../../include/store/store.hpp"
// #include "store.hpp" neu luu cung thu muc

using namespace adv;

// ===========================
// Performance Test Suite
// ===========================

class PerformanceTests
{
private:
    using Clock = std::chrono::high_resolution_clock;

    template<typename Func>
    static double measure_time(Func&& func)
    {
        auto start = Clock::now();
        func();
        auto end = Clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

public:
    // Test 1: Large Vector Operations
    static void test_large_vector_operations()
    {
        std::cout << "=== Test 1: Large Vector Operations ===\n";

        auto create_time = measure_time([]() {
            store<int> data;
            data.reserve(1000000);
            for (int i = 0; i < 1000000; ++i)
                data.push_back(i);
        });

        std::cout << "Create & populate 1M elements: " << create_time << " ms\n";

        store<int> data;
        data.reserve(1000000);
        for (int i = 0; i < 1000000; ++i)
            data.push_back(i);

        auto access_time = measure_time([&data]() {
            volatile int sum = 0;
            for (size_t i = 0; i < data.size(); ++i)
                sum += data[i];
        });

        std::cout << "Sequential access 1M elements: " << access_time << " ms\n";
    }

    // Test 2: Sorting Performance
    static void test_sorting()
    {
        std::cout << "\n=== Test 2: Sorting Performance ===\n";

        store<int> data;
        data.reserve(100000);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 10000);

        auto populate_time = measure_time([&]() {
            for (int i = 0; i < 100000; ++i)
                data.push_back(dis(gen));
        });

        std::cout << "Populate with 100K random numbers: " << populate_time << " ms\n";

        auto sort_time = measure_time([&data]() {
            data.sort();
        });

        std::cout << "Sort 100K elements: " << sort_time << " ms\n";

        // Verify sorted
        bool is_sorted = true;
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (data[i] < data[i-1])
            {
                is_sorted = false;
                break;
            }
        }
        std::cout << "Verification: " << (is_sorted ? "PASSED" : "FAILED") << "\n";
    }

    // Test 3: String Operations
    static void test_string_operations()
    {
        std::cout << "\n=== Test 3: String Operations ===\n";

        store<std::string> strings;
        strings.reserve(100000);

        auto create_time = measure_time([&]() {
            for (int i = 0; i < 100000; ++i)
                strings.push_back("test_" + std::to_string(i));
        });

        std::cout << "Create 100K strings: " << create_time << " ms\n";

        auto join_time = measure_time([&strings]() {
            std::string result = strings.join(", ");
        });

        std::cout << "Join 100K strings: " << join_time << " ms\n";
    }

    // Test 4: Filtering Performance
    static void test_filtering()
    {
        std::cout << "\n=== Test 4: Filtering Performance ===\n";

        store<int> data;
        data.reserve(100000);
        for (int i = 0; i < 100000; ++i)
            data.push_back(i);

        auto filter_time = measure_time([&data]() {
            auto evens = data.filter([](int x) { return x % 2 == 0; });
        });

        std::cout << "Filter 100K elements: " << filter_time << " ms\n";
    }

    // Test 5: Searching
    static void test_searching()
    {
        std::cout << "\n=== Test 5: Searching Performance ===\n";

        store<int> data;
        data.reserve(100000);
        for (int i = 0; i < 100000; ++i)
            data.push_back(i);

        auto contains_time = measure_time([&data]() {
            bool found = data.contains(50000);
        });

        std::cout << "Contains search: " << contains_time << " ms\n";

        auto find_all_time = measure_time([&data]() {
            auto positions = data.find_all_of(50000);
        });

        std::cout << "Find all (single value): " << find_all_time << " ms\n";

        auto count_time = measure_time([&data]() {
            size_t cnt = data.count_if([](int x) { return x > 50000; });
        });

        std::cout << "Count if (50K values > 50000): " << count_time << " ms\n";
    }

    // Test 6: Clone Operations
    static void test_cloning()
    {
        std::cout << "\n=== Test 6: Clone Operations ===\n";

        store<int> data;
        data.reserve(100000);
        for (int i = 0; i < 100000; ++i)
            data.push_back(i);

        auto set_clone_time = measure_time([&data]() {
            data.set_clone();
        });

        std::cout << "Set clone (100K elements): " << set_clone_time << " ms\n";

        auto restore_time = measure_time([&data]() {
            data.push_back(999999);  // Modify
            data.restore_clone();     // Restore
        });

        std::cout << "Restore clone: " << restore_time << " ms\n";
    }

    // Test 7: Slicing
    static void test_slicing()
    {
        std::cout << "\n=== Test 7: Slicing Performance ===\n";

        store<int> data;
        for (int i = 0; i < 10000; ++i)
            data.push_back(i);

        auto slice_time = measure_time([&data]() {
            auto slice1 = data(0, 5000);      // First 5000
            auto slice2 = data(5000, 10000);  // Last 5000
            auto every_nth = data(0, 10000, 2); // Every 2nd
        });

        std::cout << "Multiple slicing operations: " << slice_time << " ms\n";
    }

    // Test 8: Type Conversion
    static void test_conversion()
    {
        std::cout << "\n=== Test 8: Type Conversion ===\n";

        store<std::string> strings;
        for (int i = 0; i < 10000; ++i)
            strings.push_back(std::to_string(i));

        auto convert_time = measure_time([&strings]() {
            auto numbers = strings.to_ll();
        });

        std::cout << "Convert 10K strings to long long: " << convert_time << " ms\n";
    }

    // Run all tests
    static void run_all()
    {
        std::cout << "╔════════════════════════════════════════════════════════╗\n";
        std::cout << "║   Store Library (C++17 Optimized) - Performance Tests  ║\n";
        std::cout << "╚════════════════════════════════════════════════════════╝\n";

        test_large_vector_operations();
        test_sorting();
        test_string_operations();
        test_filtering();
        test_searching();
        test_cloning();
        test_slicing();
        test_conversion();

        std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    Tests Complete!                      ║\n";
        std::cout << "╚════════════════════════════════════════════════════════╝\n";
    }
};

// ===========================
// Usage Examples
// ===========================

void example_basic_usage()
{
    std::cout << "\n=== Example 1: Basic Usage ===\n";

    store<int> nums;
    nums.push_back(5);
    nums.push_back(3);
    nums.push_back(7);
    nums.push_back(1);

    std::cout << "Original: ";
    for (auto x : nums) std::cout << x << " ";
    std::cout << "\n";

    nums.sort();
    std::cout << "Sorted: ";
    for (auto x : nums) std::cout << x << " ";
    std::cout << "\n";
}

void example_filtering()
{
    std::cout << "\n=== Example 2: Filtering ===\n";

    store<int> nums;
    for (int i = 1; i <= 10; ++i)
        nums.push_back(i);

    auto evens = nums.filter([](int x) { return x % 2 == 0; });

    std::cout << "Evens: ";
    for (auto x : evens) std::cout << x << " ";
    std::cout << "\n";
}

void example_string_join()
{
    std::cout << "\n=== Example 3: String Join ===\n";

    store<std::string> words;
    words.push_back("Hello");
    words.push_back("World");
    words.push_back("from");
    words.push_back("C++17");

    std::string result = words.join(" ");
    std::cout << "Result: " << result << "\n";
}

void example_slicing()
{
    std::cout << "\n=== Example 4: Slicing ===\n";

    store<int> data;
    for (int i = 0; i < 20; ++i)
        data.push_back(i);

    auto slice1 = data(0, 10);      // Elements 0-9
    auto slice2 = data(10, 20);     // Elements 10-19
    auto every_other = data(0, 20, 2);  // Every 2nd element

    std::cout << "Slice [0, 10): ";
    for (auto x : slice1) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Every 2nd (0, 20, 2): ";
    for (auto x : every_other) std::cout << x << " ";
    std::cout << "\n";
}

void example_clone()
{
    std::cout << "\n=== Example 5: Clone Management ===\n";

    store<int> data = {1, 2, 3, 4, 5};
    
    std::cout << "Original: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << "\n";

    data.set_clone();
    data.push_back(6);
    data.push_back(7);

    std::cout << "After modifications: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << "\n";

    data.restore_clone();
    std::cout << "After restore: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << "\n";
}

void example_optional_clone()
{
    std::cout << "\n=== Example 6: Optional Clone (C++17) ===\n";

    store<int> data = {10, 20, 30};
    data.set_clone();

    if (auto cloned = data.clone())
    {
        std::cout << "Clone retrieved successfully\n";
        std::cout << "Clone size: " << cloned->size() << "\n";
    }
    else
    {
        std::cout << "Clone is empty\n";
    }
}

int main()
{
    // Run performance tests
    PerformanceTests::run_all();

    // Run usage examples
    std::cout << "\n\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║              Usage Examples                             ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";

    example_basic_usage();
    example_filtering();
    example_string_join();
    example_slicing();
    example_clone();
    example_optional_clone();

    return 0;
}
