#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>
#include "../../../include/store/store.hpp"

using namespace std;

struct Item {
    string name;
    int value;
    int price;
	
	bool operator==(const Item& other) const {
        return name == other.name && value == other.value && price == other.price;
    }
    // Helper for display
    bool operator==(const string& other) const { return name == other; }
};

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ===== UTILITY FUNCTIONS =====

// Generate random item choice
Item pick_random_item(const adv::store<Item>& pool) {
    return pool[rand() % pool.size()];
}

// Shuffle store using Fisher-Yates algorithm
template<typename T>
adv::store<T> shuffle_copy(const adv::store<T>& src) {
    auto result = src;
    for (size_t i = result.size() - 1; i > 0; --i) {
        size_t j = rand() % (i + 1);
        swap(result[i], result[j]);
    }
    return result;
}

// ===== LANDLORD GAME CLASS =====

class LandlordGame {
private:
    int money = 0;
    adv::store<int> rent_goals = {25, 60, 100, 150, 225};
    size_t current_rent_idx = 0;
    int spins_left = 5;
    int spin_bonus = 0;

    adv::store<Item> inventory;
    adv::store<Item> pool;

    // Cached board for current spin
    adv::store<Item> current_board;

public:
    LandlordGame() {
        srand(static_cast<unsigned>(time(0)));
        
        // Initialize inventory more efficiently using store features
        inventory.reserve(10);
        inventory.assign(3, {"Coin", 3, 2});
        inventory.assign(7, {"Empty", 1, 0});
        
        // Initialize item pool
        pool = {
            {"Coin", 3, 2},
            {"Cat", 2, 1},
            {"Milk", 2, 1},
            {"Sun", 3, 5},
            {"Flower", 1, 2},
            {"Toddler", 1, 2},
            {"Candy", 1, 1},
            {"Miner", 1, 5},
            {"Ore", 2, 3},
            {"Bee", 2, 3},
            {"Monkey", 3, 4},
            {"Banana", 2, 2},
            {"Rain", 0, 4},
            {"Thief", -2, 0},
            {"Empty", 1, 0}
        };
    }

    // Check if board contains a specific item
    [[nodiscard]] bool has_item(const string& item_name) const {
        return current_board.contains(Item{item_name, 0, 0}) ||
               current_board.any_of([&item_name](const Item& it) {
                   return it.name == item_name;
               });
    }

    // Get count of specific item
    [[nodiscard]] int count_item(const string& item_name) const {
        return static_cast<int>(current_board.count_if(
            [&item_name](const Item& it) { return it.name == item_name; }
        ));
    }

    // Check if neighbors contain specific item
    [[nodiscard]] bool has_neighbor(int idx, const string& target) const {
        int start = max(0, idx - 1);
        int end = min(static_cast<int>(current_board.size()), idx + 2);
        auto neighbors = current_board(start, end);
        return neighbors.any_of([&target](const Item& it) { return it.name == target; });
    }

    // Process board interactions
    int process_effects(string& effect_log) {
        int bonus = 0;

        // Flower + Sun synergy
        if (has_item("Sun")) {
            int flower_count = count_item("Flower");
            if (flower_count > 0) {
                int bonus_amount = flower_count * 2;
                bonus += bonus_amount;
                effect_log += "[Flower x Sun! +" + to_string(bonus_amount) + "] ";
            }
        }

        // Cat + Milk synergy
        if (count_item("Cat") > 0 && has_item("Milk")) {
            bonus += 5;
            effect_log += "[Cat eat Milk! +5] ";
        }

        // Bee + Flower synergy
        int bee_count = count_item("Bee");
        if (bee_count > 0) {
            int flower_count = count_item("Flower");
            if (flower_count > 0) {
                int bonus_amount = bee_count * flower_count * 3;
                bonus += bonus_amount;
                effect_log += "[Bee pollinates x" + to_string(flower_count) + "! +" +
                             to_string(bonus_amount) + "] ";
            }
        }

        // Monkey + Banana synergy
        int monkey_count = count_item("Monkey");
        if (monkey_count > 0) {
            int banana_count = count_item("Banana");
            if (banana_count > 0) {
                int bonus_amount = monkey_count * banana_count * 6;
                bonus += bonus_amount;
                effect_log += "[Monkey eats Banana! +" + to_string(bonus_amount) + "] ";
            }
        }

        // Rain + Flower synergy
        if (has_item("Rain")) {
            int flower_count = count_item("Flower");
            if (flower_count > 0) {
                int bonus_amount = flower_count * 2;
                bonus += bonus_amount;
                effect_log += "[Rain x" + to_string(flower_count) + " Flowers! +" +
                             to_string(bonus_amount) + "] ";
            }
        }

        // Thief penalty
        int thief_count = count_item("Thief");
        if (thief_count > 0) {
            int penalty = thief_count * 2;
            bonus -= penalty;
            effect_log += "[Thief steals! -" + to_string(penalty) + "] ";
        }

        return bonus;
    }

    void spin() {
        if (spins_left <= 0) return;

        // Shuffle inventory using optimized function
        auto shuffled = shuffle_copy(inventory);
        current_board = shuffled(0, 5);

        // Calculate base income using store's sum capability
        int base_income = 0;
        for (const auto& item : current_board) {
            base_income += item.value;
        }

        // Process synergies
        string effect_log;
        int synergy_bonus = process_effects(effect_log);

        int total_income = base_income + synergy_bonus;
        money += total_income;
        spins_left--;

        // Display results
        cout << "\n[BOARD]: | "
             << current_board.join(" | ", [](const Item& it) { return it.name; })
             << " |" << endl;

        if (!effect_log.empty()) {
            cout << "[COMBO]: " << effect_log << endl;
        }

        cout << "[INCOME]: +" << total_income << " | [MONEY]: " << money
             << " | [SPINS]: " << spins_left << endl;

        if (spins_left == 0) {
            check_rent();
        } else {
            add_new_item();
        }
    }

    void add_new_item() {
        cout << "\n[SELECT NEW ITEM FOR DECK]" << endl;

        // Use store to generate and display choices
        adv::store<Item> choices;
        choices.reserve(3);
        for (int i = 0; i < 3; ++i) {
            choices.push_back(pick_random_item(pool));
        }

        // Display using store's indexing
        for (size_t i = 0; i < choices.size(); ++i) {
            cout << (i + 1) << ". " << choices[i].name << " | Price: "
                 << choices[i].price << '\n';
        }

        int c;
        cout << "\nChoose (1-3): ";
        cin >> c;
        
        int idx = max(0, min(2, c - 1));
        Item item = choices[idx];
        
        if (money >= item.price) {
            money -= item.price;
            inventory.push_back(item);
            cout << "Added " << item.name << " to deck." << endl;
        } else {
            cout << "Not enough money! Need " << item.price
                 << ", have " << money << endl;
        }

        system("pause");
    }

    void check_rent() {
        if (current_rent_idx >= rent_goals.size()) {
            cout << "\n[CONGRATULATIONS!] YOU BOUGHT THE BUILDING!" << endl;
            exit(0);
        }

        int goal = rent_goals[current_rent_idx];
        cout << "\n>>> RENT DUE! Target: " << goal << " <<<" << endl;

        if (money >= goal) {
            spin_bonus++;
            money -= goal;
            current_rent_idx++;

            spins_left = min(5 + spin_bonus, 7);

            if (current_rent_idx >= rent_goals.size()) {
                cout << "\n[WIN!] You paid all rent and own the building!" << endl;
                exit(0);
            }

            cout << "[SUCCESS] Next goal: " << rent_goals[current_rent_idx] << endl;
            system("pause");
        } else {
            cout << "\n[GAME OVER] Not enough money to pay rent!" << endl;
            cout << "[Need]: " << goal << " | [Have]: " << money << endl;
            exit(0);
        }
    }

    void display_status() const {
        cout << "\n========== LUCK BE A LANDLORD ==========" << endl;
        cout << "[MONEY]: " << money << " | [RENT DUE]: "
             << rent_goals[current_rent_idx] << endl;
        cout << "[INVENTORY]: " << inventory.size() << " items" << endl;
        cout << "[PROGRESS]: Stage " << (current_rent_idx + 1) << "/"
             << rent_goals.size() << endl;
        cout << "========================================" << endl;
        cout << "\n1. SPIN | 0. EXIT\n";
        cout << "Choose: ";
    }

    void run() {
        while (true) {
            clear_screen();
            display_status();
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                spin();
            } else {
                cout << "Thanks for playing!" << endl;
                break;
            }
        }
    }
};

// ===== MAIN =====

int main() {
    LandlordGame game;
    game.run();
    return 0;
}
