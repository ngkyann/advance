🚀 Advance C++ Library (adv::)
Advance là một hệ sinh thái các thư viện C++ Modern Header-only, được xây dựng với mục tiêu: "Nhanh hơn - Ít lỗi hơn - Thân thiện hơn".

Dự án này cung cấp các công cụ và cấu trúc dữ liệu cải tiến, giúp lập trình viên C++ xử lý các tác vụ phức tạp bằng những cú pháp ngắn gọn và an toàn, mang lại trải nghiệm giống như các ngôn ngữ bậc cao (Python, JavaScript) nhưng vẫn giữ nguyên hiệu năng của C++.

✨ Triết lý thiết kế (v2.0)
Header-only: Chỉ cần include là dùng, không cần cấu hình build phức tạp.

Tính đa hình Container: adv::store giờ đây có thể tùy biến lớp lưu trữ cơ sở (mặc định là std::vector, nhưng hỗ trợ cả std::deque và std::list).

Quản lý lỗi Hybrid: Hệ thống adv::Errors cho phép chuyển đổi linh hoạt giữa việc ném ngoại lệ (throw) hoặc trả về mã thoát (exit code) tùy theo nhu cầu hệ thống.

An toàn dữ liệu: Tích hợp cơ chế clone (sử dụng unique_ptr) để sao lưu và phục hồi trạng thái dữ liệu tức thời.

📦 Các thành phần chính
1. adv::store<T, Container> (v2.0 - Current)
Cấu trúc danh sách cải tiến với các tính năng vượt trội so với STL truyền thống:

Slicing (Cắt lát): Hỗ trợ toán tử operator() để trích xuất mảng con: s(start, end, step).

Truy cập linh hoạt: Hỗ trợ chỉ số âm (Python-style) như at(-1) để lấy phần tử cuối cùng.

Quản lý bản sao (Cloning): * set_clone(): Tạo điểm lưu trữ hiện tại.

get_clone(): Khôi phục về trạng thái đã lưu.

swap_clone(): Hoán vị nhanh giữa dữ liệu thực và bản sao.

Biến đổi kiểu (Conversion): Tích hợp sẵn to_int(), to_double(), to_char(), và to_string() với khả năng tự động xử lý chuỗi và số.

Hàm tiện ích: * join(): Nối các phần tử thành chuỗi với định dạng tùy chỉnh.

range(): Khởi tạo mảng số theo bước nhảy (giống range trong Python).

filter() & transform(): Xử lý dữ liệu theo phong cách Functional Programming.

🛠 Cách sử dụng
Yêu cầu: C++17 trở lên.

C++
#include "store.hpp"
#include <iostream>

int main() {
    // Khởi tạo store với vector
    adv::store<int> s = {1, 2, 3, 4, 5};

    // Sử dụng Slicing: lấy các phần tử từ index 0 đến 4, bước nhảy 2
    auto sub = s(0, 4, 2); // Result: [1, 3]

    // Truy cập chỉ số âm
    std::cout << s.at(-1); // In ra 5

    // Chuyển đổi và nối chuỗi
    std::string str = s.join(" -> "); 
    std::cout << str; // "1 -> 2 -> 3 -> 4 -> 5"

    return 0;
}
⚖️ Bản quyền và bảo mật
License: MIT License.

Security: SLSA Level 3 Certified.

Product ID: 2Y+5$ZR.4÷>·9|X (Phiên bản 2.0).

Mỗi sản phẩm thuộc adv:: sở hữu một mã định danh duy nhất để ngăn chặn việc tái bản và giả mạo mã nguồn.