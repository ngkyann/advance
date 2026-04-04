#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>
#include "../../include/adv/store/store.hpp"
// #include "store.hpp" neu luu cung thu muc

using namespace std;

struct Item {
    string name;
    int value;
    int price;
};
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
class LandlordGame {
private:
    int money = 0;
    adv::store<int> rent_goals = {25, 60, 100, 150, 225};
    int current_rent_idx = 0;
    int spins_left = 5;
    int spin_bonus = 0;

    adv::store<Item> inventory;
    adv::store<Item> pool;

public:
    LandlordGame() {
        srand(time(0));
        for(int i=0; i<5; ++i) inventory.push_back({"Coin", 3, 2});
        for(int i=0; i<15; ++i) inventory.push_back({"Empty", 1, 0});
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
        {"Empty", 1, 0} };
    }

    void spin() {
        if (spins_left <= 0) return;
        auto temp_inv = inventory; 
        for(int i=0; i<temp_inv.size(); i++) swap(temp_inv[i], temp_inv[rand()%temp_inv.size()]);
        auto board = temp_inv(0, 5);
        int turn_income = 0;
        for(auto& item : board) turn_income += item.value;
        string effect_log = "";
        bool has_sun = false;
        for(auto& item : board) if(item.name == "Sun") has_sun = true;

        for(int i=0; i<board.size(); ++i) {
            if(board[i].name == "Flower" && has_sun) {
                turn_income += 2; 
                effect_log += "[Flower x Sun! +2] ";
            }
            if(board[i].name == "Cat") {
                auto neighbors = board(max(0, i-1), min((int)board.size(), i+2));
                for(auto& n : neighbors) if(n.name == "Milk") { 
                    turn_income += 5; 
                    effect_log += "[Cat eat Milk! +5] ";
                    break; 
                }
            }
            if(board[i].name == "Bee") {
		        auto neighbors = board(max(0, i-1), min((int)board.size(), i+2));
		        for(auto& n : neighbors) if(n.name == "Flower") {
		            turn_income += 3;
		            effect_log += "[Bee pollinates Flower! +3] ";
		        }
		    }
		    if(board[i].name == "Monkey") {
		        auto neighbors = board(max(0, i-1), min((int)board.size(), i+2));
		        for(auto& n : neighbors) if(n.name == "Banana") {
		            turn_income += 6;
		            effect_log += "[Monkey eats Banana! +6] ";
		        }
		    }
		    if(board[i].name == "Rain") {
			    int flower_count = board.count_if([](const Item& it) {
			        return it.name == "Flower";
			    });
			
			    if(flower_count > 0) {
			        int bonus = flower_count * 2;
			        turn_income += bonus;
			        effect_log += "[Rain x" + to_string(flower_count) + " Flowers! +" + to_string(bonus) + "] ";
			    }
			}
		    if(board[i].name == "Thief") {
		        turn_income -= 2;
		        effect_log += "[Thief steals! -2] ";
		    }
		        }

        money += turn_income;
        spins_left--;
        cout << "\nBOARD: | " << board.join(" | ", [](const Item& it){ return it.name; }) << " |" << endl;
        if(!effect_log.empty()) cout << "COMBO: " << effect_log << endl;
        cout << "Thu nhap: +" << turn_income << " | Money: " << money << " | Luot con: " << spins_left << endl;
        
        if (spins_left == 0) check_rent();
        else add_new_item();
    }

    void add_new_item() {
        cout << "\nCHON 1 ITEM MOI VAO DECK:" << endl;
        adv::store<Item> choices;
        for(int i=0; i<3; ++i) choices.push_back(pool[rand()%pool.size()]);
        
        for(int i=0; i<3; ++i) cout << i+1 << ". " << choices[i].name << " ; Gia: " << choices[i].price << '\n';
        int c; cout << "\nChon (1-3): "; cin >> c;
        int idx = clamp(c-1, 0, 2);
        Item item = choices[idx];
        money -= item.price;
        inventory.push_back(item);
    }

    void check_rent() {
        int goal = rent_goals[current_rent_idx];
        cout << "\n>>> DEN HAN TRA TIEN NHA! Muc tieu: " << goal << " <<<" << endl;
        if (money >= goal) {
        	spin_bonus++;
            money -= goal;
            current_rent_idx++;
            spins_left = min(5 + spin_bonus, 7);
            if (current_rent_idx == rent_goals.size()) {
                cout << "CHUC MUNG! BAN DA THANG CHU NHA VA MUA LAI TOA NHA!" << endl;
                exit(0);
            }
            cout << "Tra no thanh cong. Dot tiep theo: " << rent_goals[current_rent_idx] << endl;
            system("pause");
        } else {
            cout << "Ban khong du tien... Chu nha da duoi ban ra khoi phong." << endl;
            cout << "GAME OVER." << endl;
            exit(0);
        }
    }

    void run() {
        while(true) {
        	clear_screen();
            cout << "\n--- LUCK BE A LANDLORD---";
            cout << "\nTien hien tai: " << money << " | No sap toi: " << rent_goals[current_rent_idx];
            cout << "\n1. QUAY! | 0. Thoat\n";
            cout << "Ban chon: ";
            int choice; cin >> choice;
            if(choice == 1) spin();
            else break;
        }
    }
};

int main() {
    LandlordGame game;
    game.run();
}