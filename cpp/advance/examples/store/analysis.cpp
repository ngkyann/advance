#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "../../include/adv/store/store.hpp"
// #include "store.hpp" neu luu cung thu muc
using namespace adv;

struct User {
    int id;
    std::string name;
    std::string os;
    
    friend std::ostream& operator<<(std::ostream& os_stream, const User& u) {
        os_stream << "[" << std::setw(2) << u.id << "] " 
                  << std::setw(8) << u.name << " | OS: " << u.os;
        return os_stream;
    }
};


struct OSStat {
    std::string os_name;
    int count = 0;
};

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void draw_chart(const store<User>& data) {
    if (data.empty()) return;

    std::cout << "\n--- BIEU DO HE DIEU HANH ---\n";

    store<OSStat> stats;

    for (size_t i = 0; i < data.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < stats.size(); ++j) {
            if (stats[j].os_name == data[i].os) {
                stats[j].count++;
                found = true;
                break;
            }
        }
        if (!found) {
            stats.push_back({data[i].os, 1});
        }
    }
    
    for (size_t i = 0; i < stats.size(); ++i) {
        std::cout << std::setw(8) << stats[i].os_name << ": ";
        for (int j = 0; j < stats[i].count; ++j) {
            std::cout << "[]";
        }
        std::cout << " (" << stats[i].count << ")\n";
    }
    std::cout << "------------------------------------\n";
}

int main() {
    store<User> db = {
        {1, "Anh", "Linux"}, {2, "Binh", "Linux"}, {3, "Chi", "MacOS"},
        {4, "Dong", "Windows"}, {5, "En", "Android"}, {6, "Phi", "Linux"},
        {7, "Giang", "Windows"}, {8, "Hoa", "MacOS"}, {9, "Ien", "Windows"},
        {10, "Kiet", "Linux"}, {11, "Lam", "Android"}, {12, "Minh", "Windows"},
        {13, "Nam", "Linux"}, {14, "Oanh", "MacOS"}, {15, "Phuc", "Android"},
        {16, "Quang", "Windows"}, {17, "Son", "Linux"}, {18, "Thanh", "MacOS"},
        {19, "Uyen", "Windows"}, {20, "Viet", "Linux"}
    };

    int choice;
    while (true) {
    	clear_screen();
        draw_chart(db);
        std::cout << "\n[MENU PHAN TICH]\n";
        std::cout << "1. Dem so nguoi dung\n";
        std::cout << "2. Loc nguoi dung (Windows & Linux)\n";
        std::cout << "3. Sap xep mat do OS\n";
        std::cout << "4. Khoi phuc du lieu goc\n";
        std::cout << "5. Xem danh sach 5 nguoi dau\n";
        std::cout << "0. Thoat\n";
        std::cout << "Chon: "; 
        std::cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1: {
            	std::cout << "\n1. Windows";
            	std::cout << "\n2. Linux";
            	std::cout << "\n3. MacOS";
            	std::cout << "\n4. Android\n";
            	std::cout << "Chon he dieu hanh: ";
            	int select = 0;
            	std::cin >> select;
            	std::string inp;
            	switch(select)
            	{
            		case 1:
            			inp = "Windows";
            			break;
            		case 2:
            			inp = "Linux";
            			break;
            		case 3:
            			inp = "MacOS";
            			break;
            		case 4:
            			inp = "Android";
            			break;
            		default:
            			break;
				}
            	if(inp != "Windows" && inp != "Linux" && inp != "MacOS" && inp != "Android") break;
                int win_users = db.filter([inp](const User& u) { return u.os == inp; }).size();
                std::cout << "=> So nguoi dung " + inp + ": " << win_users << "\n";
                break;
            }
            case 2: {
                auto filtered = db.filter([](const User& u) {
                    return u.os == "Windows" || u.os == "Linux";
                });
                std::cout << "=> Danh sach nguoi dung Win & Linux:\n" << filtered.join("\n") << "\n";
                break;
            }
            case 3: {
                std::cout << "=> Dang phan tich mat do va sap xep...\n";
                db.set_clone(); 
                store<OSStat> temp_stats;
                for (size_t i = 0; i < db.size(); ++i) {
                    bool found = false;
                    for (size_t j = 0; j < temp_stats.size(); ++j) {
                        if (temp_stats[j].os_name == db[i].os) {
                            temp_stats[j].count++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) temp_stats.push_back({db[i].os, 1});
                }
                db.sort([&](const User& a, const User& b) {
                    int count_a = 0, count_b = 0;
                    for(size_t i = 0; i < temp_stats.size(); ++i) {
                        if (temp_stats[i].os_name == a.os) count_a = temp_stats[i].count;
                        if (temp_stats[i].os_name == b.os) count_b = temp_stats[i].count;
                    }
                    if (count_a != count_b) return count_a > count_b;
                    return a.os < b.os;
                });

                std::cout << "=> Da sap xep theo mat do!\n";
                break;
            }
            case 4: {
                std::cout << "=> Dang khoi phuc tu Clone...\n";
                db.get_clone();
                break;
            }
            case 5: {
                std::cout << "=> 5 nguoi dung dau tien:\n";
                auto top5 = db(0, 5); 
                std::cout << top5.join("\n") << "\n";
                break;
            }
        }
        system("pause");
    }
    return 0;
}