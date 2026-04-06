#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "../../../include/store/store.hpp"

using namespace adv;

// ===== STRUCTURES =====

struct User {
    int id;
    std::string name;
    std::string os;

    bool operator==(const User& other) const {
        return id == other.id && name == other.name && os == other.os;
    }

    friend std::ostream& operator<<(std::ostream& os_stream, const User& u) {
        os_stream << "[" << std::setw(2) << u.id << "] "
                  << std::setw(8) << u.name << " | OS: " << u.os;
        return os_stream;
    }
};

struct OSStat {
    std::string os_name;
    int count = 0;

    bool operator==(const OSStat& other) const {
        return os_name == other.os_name;
    }
};

// ===== UTILITY FUNCTIONS =====

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Count occurrences of specific OS using store.count_if()
[[nodiscard]] int count_os(const store<User>& data, const std::string& os_name) {
    return static_cast<int>(data.count_if([&os_name](const User& u) {
        return u.os == os_name;
    }));
}

// Get all unique OS names from dataset
[[nodiscard]] store<std::string> get_unique_os(const store<User>& data) {
    store<std::string> unique_os;
    for (const auto& user : data) {
        if (!unique_os.contains(user.os)) {
            unique_os.push_back(user.os);
        }
    }
    return unique_os;
}

// Build OS statistics efficiently
[[nodiscard]] store<OSStat> build_os_stats(const store<User>& data) {
    store<OSStat> stats;
    auto unique_os = get_unique_os(data);

    for (const auto& os_name : unique_os) {
        int count = count_os(data, os_name);
        stats.push_back({os_name, count});
    }
    return stats;
}

// ===== DISPLAY FUNCTIONS =====

void draw_chart(const store<User>& data) {
    if (data.empty()) return;

    std::cout << "\n[OS DISTRIBUTION CHART]\n";

    // Build statistics using optimized helper
    auto stats = build_os_stats(data);

    // Find max count for proper formatting
    int max_count = 0;
    for (const auto& stat : stats) {
        if (stat.count > max_count) max_count = stat.count;
    }

    // Display chart
    for (const auto& stat : stats) {
        std::cout << std::setw(10) << stat.os_name << ": ";
        // Draw bar
        for (int j = 0; j < stat.count; ++j) {
            std::cout << "[]";
        }
        std::cout << " (" << stat.count << ")\n";
    }
    std::cout << "------------------------------------\n";
}

void display_menu() {
    std::cout << "\n[ANALYSIS MENU]\n";
    std::cout << "1. Count users by OS\n";
    std::cout << "2. Filter Windows & Linux users\n";
    std::cout << "3. Sort by OS density\n";
    std::cout << "4. Restore original data\n";
    std::cout << "5. Show first 5 users\n";
    std::cout << "6. Show statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";
}

void show_statistics(const store<User>& data) {
    if (data.empty()) return;

    std::cout << "\n[DETAILED STATISTICS]\n";
    auto stats = build_os_stats(data);

    std::cout << "Total users: " << data.size() << "\n";
    std::cout << "Unique OS: " << stats.size() << "\n\n";

    for (const auto& stat : stats) {
        double percentage = (static_cast<double>(stat.count) / data.size()) * 100.0;
        std::cout << stat.os_name << ": " << stat.count << " users ("
                  << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }
}

// ===== MAIN PROGRAM =====

int main() {
    store<User> db = {
        {1, "Anh", "Linux"},       {2, "Binh", "Linux"},      {3, "Chi", "MacOS"},
        {4, "Dong", "Windows"},    {5, "En", "Android"},      {6, "Phi", "Linux"},
        {7, "Giang", "Windows"},   {8, "Hoa", "MacOS"},       {9, "Ien", "Windows"},
        {10, "Kiet", "Linux"},     {11, "Lam", "Android"},    {12, "Minh", "Windows"},
        {13, "Nam", "Linux"},      {14, "Oanh", "MacOS"},     {15, "Phuc", "Android"},
        {16, "Quang", "Windows"},  {17, "Son", "Linux"},      {18, "Thanh", "MacOS"},
        {19, "Uyen", "Windows"},   {20, "Viet", "Linux"}
    };

    // Save original data
    db.set_clone();

    int choice;
    while (true) {
        clear_screen();
        draw_chart(db);
        display_menu();
        std::cin >> choice;

        switch (choice) {
            // Case 1: Count users by OS
            case 1: {
                std::cout << "\n[SELECT OS]\n";
                std::cout << "1. Windows\n2. Linux\n3. MacOS\n4. Android\n";
                std::cout << "Choose OS: ";

                int os_choice;
                std::cin >> os_choice;

                std::string os_name;
                switch (os_choice) {
                    case 1:
                        os_name = "Windows";
                        break;
                    case 2:
                        os_name = "Linux";
                        break;
                    case 3:
                        os_name = "MacOS";
                        break;
                    case 4:
                        os_name = "Android";
                        break;
                    default:
                        std::cout << "Invalid selection!\n";
                        break;
                }

                if (!os_name.empty()) {
                    // Use optimized count_os helper
                    int count = count_os(db, os_name);
                    std::cout << "\n[RESULT] " << os_name << " users: "
                              << count << "\n";
                }
                break;
            }

            // Case 2: Filter Windows & Linux users
            case 2: {
                // Use store.filter() with combined predicate
                auto filtered = db.filter([](const User& u) {
                    return u.os == "Windows" || u.os == "Linux";
                });

                std::cout << "\n[WINDOWS & LINUX USERS]\n";
                std::cout << filtered.join("\n") << "\n";
                std::cout << "Total: " << filtered.size() << " users\n";
                break;
            }

            // Case 3: Sort by OS density
            case 3: {
                std::cout << "[SORTING] Analyzing OS density...\n";

                // Save current state before sorting
                db.set_clone();

                // Build stats for sorting reference
                auto stats = build_os_stats(db);

                // Sort using lambda that looks up density
                db.sort([&stats](const User& a, const User& b) {
                    // Find counts for both users' OS
                    int count_a = 0, count_b = 0;
                    for (const auto& stat : stats) {
                        if (stat.os_name == a.os) count_a = stat.count;
                        if (stat.os_name == b.os) count_b = stat.count;
                    }
                    // Sort by count descending, then by OS name ascending
                    if (count_a != count_b) return count_a > count_b;
                    return a.os < b.os;
                });

                std::cout << "[SUCCESS] Data sorted by OS density!\n";
                std::cout << "Use option 4 to restore original order.\n";
                break;
            }

            // Case 4: Restore original data
            case 4: {
                std::cout << "[RESTORE] Restoring original data...\n";
                
                // Check if clone exists
                if (auto restored = db.clone()) {
                    db = *restored;
                    std::cout << "[SUCCESS] Data restored!\n";
                } else {
                    std::cout << "[ERROR] No saved data to restore.\n";
                }
                break;
            }

            // Case 5: Show first 5 users
            case 5: {
                std::cout << "\n[FIRST 5 USERS]\n";
                // Use store slicing operator
                auto top5 = db(0, std::min(5, static_cast<int>(db.size())));
                std::cout << top5.join("\n") << "\n";
                break;
            }

            // Case 6: Show statistics
            case 6: {
                show_statistics(db);
                break;
            }

            // Exit
            case 0: {
                std::cout << "Goodbye!\n";
                return 0;
            }

            default: {
                std::cout << "Invalid choice!\n";
            }
        }

        system("pause");
    }

    return 0;
}
