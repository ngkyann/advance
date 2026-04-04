#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "../../include/adv/store/store.hpp"
// #include "store.hpp" neu luu cung thu muc

using namespace std;

int main() {
    cout << "--- Bat dau kiem tra adv::store ---" << endl;

    // 1. Khoi tao da dang (C++17)
    adv::store<int> s1 = {1, 2, 3, 4, 5};
    assert(s1.size() == 5);
    cout << "1. Khoi tao List: OK" << endl;

    // 2. Truy cap phan tu (const &)
    assert(s1.front() == 1);
    assert(s1.back() == 5);
    cout << "2. Truy cap Front/Back: OK" << endl;

    // 3. Tim Max/Min (O(n))
    assert(s1.max() == 5);
    assert(s1.min() == 1);
    cout << "3. Max/Min: OK" << endl;

    // 4. Tinh nang doc quyen: Slicing operator() - Giong Python
    // Lay tu index 1 den 4 (loai tru 4), buoc nhay 2
    auto s_slice = s1(1, 4, 2); // {2, 4}
    assert(s_slice.size() == 2);
    assert(s_slice[0] == 2 && s_slice[1] == 4);
    cout << "4. Cat lat (Slicing): OK" << endl;

    // 5. Slice dao nguoc (Step am)
    auto s_reverse = s1(4, -3, -1);
    assert(s_reverse[0] == 5 && s_reverse[1] == 4);
    cout << "5. Cat lat dao nguoc: OK" << endl;

    // 6. Tinh nang doc quyen: join()
    string joined = s1.join(" - ");
    assert(joined == "1 - 2 - 3 - 4 - 5");
    cout << "6. Noi chuoi (Join): OK" << endl;

    // 7. Chuyen doi kieu du lieu (Cast Types) - to_str()
    auto s_str = s1.to_str();
    assert(s_str[0] == "1");
    cout << "7. Chuyen so sang chuoi: OK" << endl;

    // 8. Chuyen doi phuc tap (String to Int)
	adv::store<string> s_raw = {"10", "20", "abc", ""};
	bool caught = false;
	
	try {
	    auto s_int = s_raw.to_ll(); 
	    // Nếu to_ll() của bạn ném lỗi ở "abc", dòng dưới này sẽ KHÔNG bao giờ chạy
	    assert(s_int[0] == 10); 
	} catch (const std::exception& e) {
	    caught = true;
	    cout << "8. Parse chuoi sang so: Da bat duoc loi dung nhu du kien! (" << e.what() << ")" << endl;
	}
	
	assert(caught == true); // Neu khong bat duoc loi thi moi la "fail" bài test này

    // 9. clone
    s1.set_clone(); // Tao ban sao clone
    s1.push_back(100);
    assert(s1.back() == 100);
    
    s1.get_clone(); // Khoi phuc tu clone
    assert(s1.back() == 5);
    assert(s1.size() == 5);
    cout << "9. Snapshot (Undo/Redo): OK" << endl;

    // 10. Toi uu Memory Snapshot
    s1.del_clone(); // Giai phong vung nho m_clone (unique_ptr.reset())
    cout << "10. Giai phong Snapshot: OK" << endl;

    // 11. Tinh tuong thich
    adv::store<int, std::deque<int>> s_deque = {10, 20, 30};
    s_deque.push_front(5);
    assert(s_deque.front() == 5);
    cout << "11. Chay tren Deque: OK" << endl;
    
    return 0;
}