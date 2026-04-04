//Tên: store
//Phiên bản: 2.0
#pragma once
//STL
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <utility>
#include <deque>
#include <list>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <sstream>
#include <cmath>
#include <memory>
namespace adv
{
// =======================
// Lop quan li loi
// =======================
class errors
{
  private:
    bool m_exit_code_mode = false; // false: nem loi, true: tra ve ma loi
    short m_last_error_code = 0;     // Kiem tra ma loi cuoi

  public:
    // =======================
    // Cau hinh che do
    // =======================
    
    /// @brief Dat che do ma thoat
    /// @param mode true cho ma loi, false cho ngoai le
    void has_exit_code(bool mode) noexcept
    {
        m_exit_code_mode = mode;
    }

    /// @brief Lay che do hien tai
    /// @return true neu che do ma loi
    bool get_exit_code_mode() const noexcept
    {
        return m_exit_code_mode;
    }

    /// @brief Lay ma loi cuoi cung
    /// @return Tra ve ma loi cuoi cung
    short get_last_error_code() const noexcept
    {
        return m_last_error_code;
    }

    // =======================
    // Phuong thuc loi - Phien ban ngoai le (khong tra ve)
    // =======================

    /// @brief Nem ngoai le out_of_range
    [[noreturn]] void throw_out_of_range(const std::string &msg = "Error: Out of range") const
    {
        throw std::out_of_range(msg);
    }

    /// @brief Nem ngoai le invalid_argument
    [[noreturn]] void throw_invalid_argument(const std::string &msg = "Error: Invalid argument") const
    {
        throw std::invalid_argument(msg);
    }

    /// @brief Nem ngoai le runtime_error
    [[noreturn]] void throw_runtime_error(const std::string &msg = "Error: Runtime error") const
    {
        throw std::runtime_error(msg);
    }

    // =======================
    // Phuong thuc loi - Phien ban ma loi (tra ve short)
    // =======================

    /// @brief Tra ve ma loi out_of_range
    /// @return Ma loi -1
    short return_out_of_range() noexcept
    {
        m_last_error_code = -1;
        return -1;
    }

    /// @brief Tra ve ma loi invalid_argument
    /// @return Ma loi -2
    short return_invalid_argument() noexcept
    {
        m_last_error_code = -2;
        return -2;
    }

    /// @brief Tra ve ma loi runtime_error
    /// @return Ma loi -3
    int return_runtime_error() noexcept
    {
        m_last_error_code = -3;
        return -3;
    }

    // =======================
    // Phuong thuc loi duy nhat (che do tu nhan dien)
    // =======================

    /// @brief Xu ly out_of_range
    /// @return Ma loi neu trong che do ma loi, nguoc lai ngoai le
    [[noreturn]] short out_of_range(const std::string &msg = "")
    {
        if (m_exit_code_mode)
        {
            std::exit(return_out_of_range());
        }
        throw_out_of_range(msg);
    }

    /// @brief Xu ly invalid_argument
    /// @return Ma loi neu trong che do ma loi, nguoc lai ngoai le
    [[noreturn]] short invalid_argument(const std::string &msg = "")
    {
        if (m_exit_code_mode)
        {
            std::exit(return_invalid_argument());
        }
        throw_invalid_argument(msg);
    }

    /// @brief Xu ly runtime_error
    /// @return Ma loi neu trong che do ma loi, nguoc lai ngoai le
    [[noreturn]] short runtime_error(const std::string &msg = "")
    {
        if (m_exit_code_mode)
        {
            std::exit(return_runtime_error());
        }
        throw_runtime_error(msg);
    }

    // =======================
    // Phuong thuc loi khong thoat (cho cac ham tra ve ma loi)
    // =======================

    /// @brief Lay ma loi out_of_range
    /// @return Ma loi -1
    short get_out_of_range_code() noexcept
    {
        m_last_error_code = -1;
        return -1;
    }

    /// @brief Lay ma loi invalid_argument
    /// @return Ma loi -2
    short get_invalid_argument_code() noexcept
    {
        m_last_error_code = -2;
        return -2;
    }

    /// @brief Lay ma loi runtime_error
    /// @return Ma loi -3
    int get_runtime_error_code() noexcept
    {
        m_last_error_code = -3;
        return -3;
    }
};

// =======================
// Lop khuon mau store
// =======================
template <typename T, typename Container = std::vector<T>>
class store
{
  private:
    Container m_data;	//Luu tru noi bo
    std::unique_ptr<Container> m_clone;    //Ban sao noi bo
    thread_local inline static errors s_error;	//Quan li loi
    struct AlwaysTrueTag {};	//Cau truc rong

  public:
    /// Cac loai con tro/kieu
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;
    using pointer = typename Container::pointer;
    using const_pointer = typename Container::const_pointer;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;
    using value_type = T;

    // =======================
    // Khoi tao & Xoa
    // =======================

    /// @brief Khoi tao mac dinh
    store() = default;

    /// @brief Khoi tao voi kich thuoc
    /// @param Kich thuoc ban dau size
    explicit store(std::size_t size) : m_data(size) {}

    /// @brief Khoi tao voi kich thuoc va gia tri ban dau
    /// @param Kich thuoc ban dau size
    /// @param Gia tri ban dau value
    explicit store(std::size_t size, const T& value) : m_data(size, value) {}

    /// @brief Khoi tao voi danh sach khoi tao
    /// @param Danh sach khoi tao khoi tao cac gia tri phan tu
    store(std::initializer_list<T> list) : m_data(list) {}

    /// @brief Khoi tao voi con tro lap pham vi
    /// @tparam Kieu con tro lap
    /// @param Con tro lap bat dau begin
    /// @param Con tro lap ket thuc end
    template <typename Iterator, 
          typename = std::enable_if_t<!std::is_arithmetic_v<Iterator>>>
	store(Iterator begin, Iterator end) : m_data(begin, end) {}

    /// @brief Khoi tao voi pham vi
    /// @tparam Kieu pham vi
    /// @param Pham vi dau vao
    template <typename Range,
          typename = std::enable_if_t<!std::is_same_v<std::decay_t<Range>, store>>>
	explicit store(const Range &range) : m_data(range.begin(), range.end()) {}


    /// @brief Sao chep khoi tao
    store(const store &other) : m_data(other.m_data) 
    {
    	if(other.m_clone)
    	{
    		m_clone = std::make_unique<Container>(*other.m_clone);
		}
	}

    /// @brief Di chuyen khoi tao
    store(store &&) noexcept = default;

    /// @brief Sao chep gan
    store &operator=(const store &other)
    {
    	if (this != &other) {
            m_data = other.m_data;
            if (other.m_clone) {
                m_clone = std::make_unique<Container>(*other.m_clone);
            } else {
                m_clone.reset();
            }
        }
        return *this;
	}

    /// @brief Di chuyen gan
    store &operator=(store &&) noexcept = default;
    
    /// @brief Gan bang list
    store &operator=(std::initializer_list<T> list) {
	    m_data = list;
	    return *this;
	}

    /// @brief Gan theo danh sach khoi tao
    /// @param Danh sach khoi tao list
    void assign(std::initializer_list<T> list)
    {
        m_data.assign(list);
    }

    /// @brief Gan theo theo kich thuoc va gia tri
    /// @param Kich thuoc gan size
    /// @param Gia tri gan value
    void assign(std::size_t size, const T &value)
    {
        m_data.assign(size, value);
    }

    /// @brief Gan theo con tro
    /// @tparam Kieu con tro lap Iterator
    /// @param Con tro dau begin
    /// @param Con tro cuoi end
    template <typename Iterator, 
          typename = std::enable_if_t<!std::is_arithmetic_v<Iterator>>>
    void assign(Iterator begin, Iterator end)
    {
        m_data.assign(begin, end);
    }

    // =======================
    // Truy cap phan tu
    // =======================

    /// @brief Truy cap phan tu voi kiem tra
    /// @param Vi tri kiem tra pos
    /// @return Tham chieu den phan tu tai vi tri
    /// @throws std::out_of_range neu vi tri khong hop le
    T &at(long long pos)
    {
        if (pos >= m_data.size() || pos < -m_data.size())
        {
            s_error.throw_out_of_range("Index " + std::to_string(pos) + " out of range");
        }
        if (pos < 0)
            pos += m_data.size();
	    if constexpr (std::is_same_v<Container, std::list<T>>) {
	        return *std::next(m_data.begin(), pos);
	    } else {
	        return m_data[pos];
	    }
    }

    /// @brief Truy cap hang so phan tu voi kiem tra
    /// @param Vi tri kiem tra pos
    /// @return Tham chieu hang so den phan tu tai vi tri
    /// @throws std::out_of_range neu vi tri khong hop le
    const T &at(long long pos) const
    {
        if (pos >= m_data.size() || pos < -m_data.size())
        {
            s_error.throw_out_of_range("Index " + std::to_string(pos) + " out of range");
        }
        if (pos < 0)
            pos += m_data.size();
        if constexpr (std::is_same_v<Container, std::list<T>>) {
	        return *std::next(m_data.begin(), pos);
	    } else {
	        return m_data[pos];
	    }
    }

    /// @brief Truy cap phan tu voi khong kiem tra
    /// @param Vi tri kiem tra pos
    /// @return Tham chieu den phan tu tai vi tri
    T &operator[](long long pos) noexcept
    {
        if (pos < 0)
            pos += m_data.size();
        if constexpr (std::is_same_v<Container, std::list<T>>) {
	        return *std::next(m_data.begin(), pos);
	    } else {
	        return m_data[pos];
	    }
    }

    /// @brief Truy cap hang so phan tu voi khong kiem tra
    /// @param Vi tri kiem tra pos
    /// @return Tham chieu hang so den phan tu tai vi tri
    const T &operator[](long long pos) const noexcept
    {
        if (pos < 0)
            pos += m_data.size();
        if constexpr (std::is_same_v<Container, std::list<T>>) {
	        return *std::next(m_data.begin(), pos);
	    } else {
	        return m_data[pos];
	    }
    }

    /// @brief Lay phan tu dau tien
    /// @return Tham chieu hang den phan tu dau tien
    /// @throws std::out_of_range neu store rong
    const T &front() const
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("store is empty");
        }
        return m_data.front();
    }

    /// @brief Lay phan tu cuoi
    /// @return Tham chieu hang den phan tu cuoi
    /// @throws std::out_of_range neu store rong
    const T &back() const
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("store is empty");
        }
        return m_data.back();
    }

    /// @brief Lay phan tu lon nhat
    /// @return Tham chieu hang den phan tu lon nhat
    /// @throws std::out_of_range neu store rong
    const T &max() const
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("store is empty");
        }
        return *std::max_element(m_data.begin(), m_data.end());
    }

    /// @brief Lay phan tu nho nhat
    /// @return Tham chieu hang den phan tu nho nhat
    /// @throws std::out_of_range neu store rong
    const T &min() const
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("store is empty");
        }
        return *std::min_element(m_data.begin(), m_data.end());
    }
	
	/// @brief Lay cap gia tri lon nhat va nho nhat
    /// @return Tra ve cap gia tri lon nhat va nho nhat
    /// @throws std::out_of_range neu store rong
    const std::pair<T, T> max_min()
    {
    	if (m_data.empty())
        {
            s_error.throw_out_of_range("store is empty");
        }
        auto value = std::minmax_element(m_data.begin(), m_data.end());
    	return {*value.first, *value.second};
	}

    // =======================
    // Dung luong
    // =======================

    /// @brief Lay kich thuoc hien tai
    /// @return So phan tu trong store
    size_t size() const noexcept
    {
        return m_data.size();
    }

    /// @brief Kiem tra neu store rong
    /// @return true neu store rong, nguoc lai false
    bool empty() const noexcept
    {
        return m_data.empty();
    }

    // =======================
    // Toan tu chuyen doi
    // =======================

    /// @brief Chuyen doi qua vector
    /// @return vector chua cac phan tu cua store
    operator std::vector<T>() const
    {
	    if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        return m_data;
	    } else {
	       	return std::vector<T>(m_data.begin(), m_data.end());
	    }
	}

	/// @brief Chuyen doi qua deque
    /// @return deque chua cac phan tu cua store
	operator std::deque<T>() const
    {
	    if constexpr (std::is_same_v<Container, std::deque<T>>) {
	        return m_data;
	    } else {
	       	return std::deque<T>(m_data.begin(), m_data.end());
	    }
	}

	/// @brief Chuyen doi qua list
    /// @return list chua cac phan tu cua store
    operator std::list<T>() const
    {
	    if constexpr (std::is_same_v<Container, std::list<T>>) {
	        return m_data;
	    } else {
	       	return std::list<T>(m_data.begin(), m_data.end());
	    }
	}

    // =======================
    // Con tro dau/cuoi
    // =======================

    iterator begin() noexcept { return m_data.begin(); }
    iterator end() noexcept { return m_data.end(); }
    iterator begin() const noexcept { return m_data.begin(); }
    iterator end() const noexcept { return m_data.end(); }
    const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    const_iterator cend() const noexcept { return m_data.cend(); }
    reverse_iterator rbegin() noexcept { return m_data.rbegin(); }
    reverse_iterator rend() noexcept { return m_data.rend(); }
    reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
    reverse_iterator rend() const noexcept { return m_data.rend(); }
    const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
    const_reverse_iterator crend() const noexcept { return m_data.crend(); }

    // =======================
    // Cac ham co ban
    // =======================

	/// @brief Cap phat bo nho store
	/// @param So luong bo nho size
	void reserve(size_t size)
	{
		if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        m_data.reserve(size);
	    }
	}
    /// @brief Thay doi kich thuoc store
    /// @param Kich thuoc moi cua store new_size
    void resize(size_t new_size)
    {
        m_data.resize(new_size);
    }

    /// @brief Thay doi kich thuoc store voi gia tri
    /// @param Kich thuoc moi cua store new_size
    /// @param Gia tri value
    void resize(size_t new_size, const T& value)
    {
    	m_data.resize(new_size, value);
	}

    /// @brief Xoa toan bo phan tu
    void clear() noexcept
    {
        m_data.clear();
    }

    /// @brief Xoa phan tu dau
    /// @throws std::out_of_range neu mang rong
    void pop_front()
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("Cannot pop_front from empty store");
        }
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        m_data.erase(m_data.begin());
	    } else {
	        m_data.pop_front();
	    }
    }

    /// @brief Xoa phan tu cuoi
    /// @throws std::out_of_range neu mang rong
    void pop_back()
    {
        if (m_data.empty())
        {
            s_error.throw_out_of_range("Cannot pop_back from empty store");
        }
        m_data.pop_back();
    }

    /// @brief Xoa phan tu tai vi tri
    /// @param Vi tri xoa pos
    /// @throws std::out_of_range neu vi tri khong hop le
    void remove_at(size_t pos)
    {
        if (pos >= m_data.size())
        {
            s_error.throw_out_of_range("Index " + std::to_string(pos) + " out of range");
        }
        m_data.erase(std::next(m_data.begin(), pos));
    }

    /// @brief Chen phan tu tai vi tri
    /// @param Vi tri chen pos
    /// @param Gia tri chen value
    /// @throws std::out_of_range neu vi tri khong hop le
    void insert(size_t pos, const T &value)
    {
        if (pos > m_data.size())
        {
            s_error.throw_out_of_range("Index " + std::to_string(pos) + " out of range");
        }
        m_data.insert(std::next(m_data.begin(), pos), value);
    }

    /// @brief Thay the phan tu tai vi tri
    /// @param Vi tri thay the pos
    /// @param Gia tri moi value
    /// @throws std::out_of_range neu vi tri khong hop le
    void replace_at(size_t pos, const T &value)
    {
        if (pos >= m_data.size())
        {
            s_error.throw_out_of_range("Index " + std::to_string(pos) + " out of range");
        }
        if constexpr (std::is_same_v<Container, std::list<T>>) {
	        *std::next(m_data.begin(), pos) = value;
	    } else {
	        m_data[pos] = value;
	    }
    }

    /// @brief Thay the tat ca gia tri cua mot gia tri
    /// @param Gia tri can thay the old_value
    /// @param Gia tri moi new_value
    void replace_all(const T &old_value, const T &new_value)
    {
        std::replace(m_data.begin(), m_data.end(), old_value, new_value);
    }

    /// @brief Lap day store voi gia tri
    /// @param Gia tri lap day value
    void fill(const T &value)
    {
        std::fill(m_data.begin(), m_data.end(), value);
    }

    /// @brief Dao nguoc cac phan tu trong mang
    void reverse()
    {
        if constexpr (std::is_same_v<Container, std::list<T>>) {
	        m_data.reverse();
	    } else {
	        std::reverse(m_data.begin(), m_data.end());
	    }
    }

    /// @brief Hoan doi phan tu voi store khac
    /// @param Store khac muon thay doi other
    void swap(store<T, Container> &other) noexcept
    {
        m_data.swap(other.m_data);
    }

    // =======================
    // Ham in nang cao
    // =======================
    /// @brief In cac phan tu thanh chuoi theo dinh dang
    /// @param Dinh dang cua chuoi format
    std::string join(const std::string &format) const
	{
	    if (m_data.empty()) return "";
	    std::ostringstream ss;
	    bool first = true;
	    for (const auto &item : m_data)
	    {
	        if (!first) ss << format;
	        ss << item;
	        first = false;
	    }
	    return ss.str();
	}
	
	/// @brief In cac phan tu thanh chuoi theo dinh dang
    /// @param Dinh dang cua chuoi format
    /// @tparam Chuyen doi linh hoat voi converter
	template <typename Func>
	std::string join(const std::string &format, Func converter) const {
	    if (m_data.empty()) return "";
	    std::ostringstream ss;
	    bool first = true;
	    for (const auto &item : m_data) {
	        if (!first) ss << format;
	        ss << converter(item); 
	        first = false;
	    }
	    return ss.str();
	}

    // =======================
    // operator() - cat lat
    // =======================

    /// @brief Cat lat 3 tham so
    /// @param Vi tri bat dau start
    /// @param Vi tri ket thuc end
    /// @param Buoc nhay step
    /// @return Store moi voi cac gia tri duoc cat
    store<T, Container> operator()(int start, int end, int step) const {
	    if (step == 0) return store<T, Container>();
	    if (step == 1) return (*this)(start, end);
	    
	    int size = static_cast<int>(m_data.size());
	    store<T, Container> result;
	    if (step > 0) {
	        int s = (start >= 0) ? start : size + start;
	        int e = (end >= 0)   ? end   : size + end;
	        s = std::clamp(s, 0, size);
	        e = std::clamp(e, 0, size);
	
	        for (int i = s; i < e; i += step) {
	            result.push_back(m_data[i]);
	        }
	    } 
	    else {
	        int pos_step = -step;
	        int s = (end >= 0)   ? end + 1 : size + end + 1;
	        int e = (start >= 0) ? start + 1 : size + start + 1;
	        s = std::clamp(s, 0, size);
	        e = std::clamp(e, 0, size);
	        for (int i = s; i < e; i += pos_step) {
	            result.push_back(m_data[i]);
	        }
	        result.reverse();
	    }
	
	    return result;
	}
	
	/// @brief Cắt lát nhanh 2 tham so
	/// @param Vi tri bat dau start
    /// @param Vi tri ket thuc end
    /// @return Store moi voi cac gia tri duoc cat
	store<T, Container> operator()(int start, int end) const {
	    int size_raw = static_cast<int>(m_data.size());
	    int s = (start >= 0) ? start : size_raw + start;
	    int e = (end >= 0) ? end : size_raw + end;
	    s = std::clamp(s, 0, size_raw);
	    e = std::clamp(e, 0, size_raw);
	    store<T, Container> result;
	    if (s < e) {
	        result.m_data.assign(std::next(m_data.begin(), s), std::next(m_data.begin(), e));
	    }
	    return result;
	}

    // =======================
    // count - Dem so lan xuat hien gia tri
    // =======================

    /// @brief Dem so lan xuat hien gia tri
    /// @param Gia tri de dem value
    /// @return So lan xuat hien
    size_t count(const T &value) const noexcept
    {
        return std::count(m_data.begin(), m_data.end(), value);
    }

    // =======================
    // count_if - Dem phan tu thoa man dieu kien
    // =======================

    /// @brief Dem phan tu thoa man dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kien pred
    /// @return So phan tu thoa man dieu kien
    template <typename Pred>
    size_t count_if(Pred pred) const
    {
        return std::count_if(m_data.begin(), m_data.end(), pred);
    }

    // =======================
    // Them phan tu
    // =======================

    /// @brief Them container vao dau
    /// @tparam Kieu vat chua Container
    /// @param Vat chua de them container
    template <typename Containers>
	void push_front(const Containers &other)
	{
	    m_data.insert(m_data.begin(), other.begin(), other.end());
	}

    /// @brief Them di chuyen container vao dau
    /// @tparam Kieu vat chua Container
    /// @param Vat chua de di chuyen container
	template <typename Containers>
	auto push_front(Containers &&container) 
	    -> std::enable_if_t<!std::is_same_v<std::decay_t<Containers>, T>, void> 
	{
	    m_data.insert(m_data.begin(),
	                  std::make_move_iterator(std::begin(container)),
	                  std::make_move_iterator(std::end(container)));
	}

    /// @brief Them danh sach khoi tao vao dau
    /// @param Danh sach khoi tao de them list
    void push_front(std::initializer_list<T> i_list)
    {
        m_data.insert(m_data.begin(), i_list.begin(), i_list.end());
    }

    /// @brief Them gia tri vao dau
    /// @param Gia tri de them value
    void push_front(const T &value)
	{
	    if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        m_data.insert(m_data.begin(), value);
	    } else {
	        m_data.push_front(value);
	    }
	}

    /// @brief Them di chuyen gia tri vao dau
    /// @param Gia tri de di chuyen value
    void push_front(T &&value)
	{
	    if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        m_data.insert(m_data.begin(), std::move(value));
	    } else {
	        m_data.push_front(std::move(value));
	    }
	}

    /// @brief Them container vao cuoi
    /// @tparam Kieu vat chua Container
    /// @param Vat chua de them container
    template <typename Containers>
	void push_back(const Containers &container)
	{
	    m_data.insert(m_data.end(), container.begin(), container.end());
	}

	/// @brief Them di chuyen container vao cuoi
    /// @tparam Kieu vat chua Container
    /// @param Vat chua de di chuyen container
	template <typename Containers>
	auto push_back(Containers &&container) 
	    -> std::enable_if_t<!std::is_same_v<std::decay_t<Containers>, T>, void> 
	{
	    m_data.insert(m_data.end(),
	                  std::make_move_iterator(std::begin(container)),
	                  std::make_move_iterator(std::end(container)));
	}

    /// @brief Them danh sach khoi tao vao cuoi
    /// @param Danh sach khoi tao de them list
    void push_back(std::initializer_list<T> i_list)
    {
        m_data.insert(m_data.end(), i_list.begin(), i_list.end());
    }

    /// @brief Them gia tri vao cuoi
    /// @param Gia tri de them value
    void push_back(const T &value)
    {
        m_data.push_back(value);
    }

    /// @brief Them di chuyen gia tri vao dau
    /// @param Gia tri de di chuyen value
    void push_back(T &&value)
    {
        m_data.push_back(std::move(value));
    }

    /// @brief Dat phan tu vao cuoi
    /// @tparam Kieu doi so Args
    /// @param Cac loai doi so args
    template <typename... Args>
	T& emplace_back(Args &&... args) {
	    m_data.emplace_back(std::forward<Args>(args)...);
	    return m_data.back();
	}

    /// @brief Dat phan tu vao dau
    /// @tparam Kieu doi so Args
    /// @param Cac loai doi so args
    template <typename... Args>
	T& emplace_front(Args &&... args) {
	    if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        return *m_data.emplace(m_data.begin(), std::forward<Args>(args)...);
	    } else {
	        m_data.emplace_front(std::forward<Args>(args)...);
	        return m_data.front();
	    }
	}

    // =======================
    // Tim kiem & kiem tra
    // =======================

    /// @brief Kiem tra neu store chua gia tri
    /// @param Gia tri de tim kiem value
    /// @return true neu tim thay gia tri, nguoc lai false
    bool contains(const T &value) const
    {
        return std::find(m_data.begin(), m_data.end(), value) != m_data.end();
    }

    /// @brief Kiem tra neu bat ky phan tu thoa man dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kien pred
    /// @return true neu bat ky phan tu thoa man dieu kien
    template <typename Pred>
    bool any_of(Pred pred) const
    {
        return std::any_of(m_data.begin(), m_data.end(), pred);
    }

    /// @brief Kiem tra neu bat ky phan tu bang gia tri
    /// @param Gia tri de so sanh value
    /// @return true neu bat ky phan tu bang gia tri
    bool any_of(const T &value) const
    {
        return std::any_of(m_data.begin(), m_data.end(),
                           [&](const T &ele) { return ele == value; });
    }

    /// @brief Kiem tra neu tat ca phan tu thoa man dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kien pred
    /// @return true neu tat ca phan tu thoa man dieu kien
    template <typename Pred>
    bool all_of(Pred pred) const
    {
        return std::all_of(m_data.begin(), m_data.end(), pred);
    }

    /// @brief Kiem tra neu tat ca phan tu bang gia tri
    /// @param Gia tri de so sanh value
    /// @return true neu tat ca phan tu bang gia tri
    bool all_of(const T &value) const
    {
        return std::all_of(m_data.begin(), m_data.end(),
                           [&](const T &ele) { return ele == value; });
    }

    /// @brief Kiem tra neu khong phan tu nao thoa man dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kien pred
    /// @return true neu khong phan tu nao thoa man dieu kien
    template <typename Pred>
    bool none_of(Pred pred) const
    {
        return std::none_of(m_data.begin(), m_data.end(), pred);
    }

    /// @brief Kiem tra neu khong phan tu nao bang gia tri
    /// @param Gia tri de so sanh value
    /// @return true neu khong phan tu nao bang gia tri
    bool none_of(const T &value) const
    {
        return std::none_of(m_data.begin(), m_data.end(),
                            [&](const T &ele) { return ele == value; });
    }

    /// @brief Tim tat ca vi tri cua gia tri
    /// @param Gia tri de tim value
    /// @return vector cac vi tri xuat hien
	std::vector<size_t> find_all_of(const T &value) const
	{
	    std::vector<size_t> positions;
	    positions.reserve(m_data.size());

	    size_t current_index = 0;
	    for (const auto &item : m_data)
	    {
	        if (item == value)
	        {
	            positions.push_back(current_index);
	        }
	        current_index++;
	    }

	    positions.shrink_to_fit();

	    return positions;
	}

    /// @brief Tim tat ca vi tri phan tu thoa man dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kien pred
    /// @return vector cac vi tri thoa man dieu kien
    template <typename Pred>
	std::vector<size_t> find_all_if(Pred pred) const
	{
	    std::vector<size_t> positions;
	    positions.reserve(m_data.size());

	    std::size_t current_idx = 0;

	    for (const auto &item : m_data)
	    {
	        if (pred(item))
	        {
	            positions.push_back(current_idx);
	        }
	        current_idx++;
	    }

	    positions.shrink_to_fit();

	    return positions;
	}
	
    // =======================
    // Bien doi & Loc
    // =======================

    /// @brief Bien doi cac phan tu tai cho
    /// @tparam Kieu ham Func
    /// @param Ham bien doi func
    template <typename Func>
    void transform(Func func)
    {
        std::transform(m_data.begin(), m_data.end(), m_data.begin(), func);
    }

    /// @brief Loc cac phan tu theo dieu kien
    /// @tparam Kieu dieu kien Pred
    /// @param Ham dieu kein pred
    /// @return store moi voi cac phan tu duoc loc
    template <typename Pred>
    store<T, Container> filter(Pred pred) const
    {
        store<T, Container> result;
        for (const auto &elem : m_data)
        {
            if (pred(elem))
            {
                result.push_back(elem);
            }
        }
        return result;
    }

    // =======================
    // Sap xep
    // =======================

    /// @brief Sap xep phan tu
    /// @param Sap xep theo thu tu tang dan ascending (mac dinh: true)
    void sort(bool ascending = true) {
	    if (m_data.size() < 2) return;
	    if constexpr (std::is_same_v<Container, std::vector<T>>) {
	        if (ascending) {
	            std::sort(m_data.begin(), m_data.end());
	        } else {
	            std::sort(m_data.begin(), m_data.end(), std::greater<T>());
	        }
	    }
	    else {
	        std::vector<T> temp(std::make_move_iterator(m_data.begin()),
	                            std::make_move_iterator(m_data.end()));

	        if (ascending) {
	            std::sort(temp.begin(), temp.end());
	        } else {
	            std::sort(temp.begin(), temp.end(), std::greater<T>());
	        }
	        m_data.assign(std::make_move_iterator(temp.begin()),
	                      std::make_move_iterator(temp.end()));
	    }
	}

    /// @brief Sap xep voi bo so sanh tuy chinh
    /// @tparam Kieu so sanh Compare
    /// @param Ham so sanh comp
    template <typename Compare>
	void sort(Compare comp)
	{
	    if (m_data.size() < 2) return;

	    if constexpr (std::is_same_v<Container, std::vector<T>>) {

	        std::sort(m_data.begin(), m_data.end(), comp);
	    }
	    else {

	        std::vector<T> temp(std::make_move_iterator(m_data.begin()),
	                            std::make_move_iterator(m_data.end()));

	        std::sort(temp.begin(), temp.end(), comp);
	        m_data.assign(std::make_move_iterator(temp.begin()),
	                      std::make_move_iterator(temp.end()));
	    }
	}

    /// @brief Xoa bo phan tu trung nhau
    /// @param Sap xep truoc khi xoa auto_sort (mac dinh: true)
    void unique(bool auto_sort = true)
    {
    	if (m_data.empty()) return;
        if (auto_sort)
        {
            sort();
        }
        if constexpr (std::is_same_v<Container, std::list<T>>) {
        	m_data.unique();
		}else {
			auto it = std::unique(m_data.begin(), m_data.end());
       		m_data.erase(it, m_data.end());
		}
    }

    // =======================
    // Chuyen doi kieu
    // =======================

    /// @brief Chuyen doi thanh store so nguyen
    /// @return store moi voi gia tri so nguyen
    store<long long> to_ll() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion to int");

        store<long long> result;
        result.reserve(m_data.size());
        for (const auto &val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                try
                {
                    result.push_back(std::stoll(val));
                }
                catch (...)
                {
                    s_error.throw_runtime_error();
                }
            }
            else
            {
                result.push_back(static_cast<long long>(val));
            }
        }
        return result;
    }

    /// @brief Chuyen doi thanh store so thuc
    /// @return store moi voi gia tri so thuc
    store<double> to_double() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion to double");

        store<double> result;
        result.reserve(m_data.size());
        for (const auto &val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                try
                {
                    result.push_back(std::stod(val));
                }
                catch (...)
                {
                    s_error.throw_runtime_error();
                }
            }
            else
            {
                result.push_back(static_cast<double>(val));
            }
        }
        return result;
    }

    /// @brief Chuyen doi thanh store ki tu
    /// @return store moi voi gia tri ki tu
    store<char> to_char() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion to char");

        store<char> result;
        result.reserve(m_data.size());
        for (const auto &val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                if (!val.empty())
                {
                    result.m_data.insert(result.m_data.end(), val.begin(), val.end());
                }
                else
                {
                    result.push_back('\0');
                }
            }
            else
            {
                result.push_back(static_cast<char>(val));
            }
        }
        return result;
    }

    /// @brief Chuyen doi thanh store chuoi
    /// @return store moi voi gia tri chuoi
    store<std::string> to_str() const
    {
        store<std::string> result;
        result.reserve(m_data.size());
        for (const auto &val : m_data)
        {
            if constexpr (std::is_arithmetic_v<T>)
            {
                result.push_back(std::to_string(val));
            }
            else if constexpr (std::is_same_v<T, char>)
            {
                result.push_back(string(1, val));
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                result.push_back(val);
            }
            else
            {
                std::ostringstream oss;
                oss << val;
                result.push_back(oss.str());
            }
        }
        return result;
    }

    // =======================
    // Phuong thuc ban sao
    // =======================

    /// @brief Dat ban sao luu
    void set_clone() {
        if (!m_clone) {
            m_clone = std::make_unique<Container>(m_data);
        } else {
            *m_clone = m_data;
        }
    }

    /// @brief Lay ban sao luu
    store<T, Container> clone()
    {
        if (m_clone.empty())
            s_error.throw_runtime_error("Clone is empty");
        store<T, Container> temp;
        temp.m_data = m_clone;
        return temp;
    }

    /// @brief Dat lai ban sao luu
    void del_clone() {
        m_clone.reset();
    }
    
    /// @brief Hoan doi ban sao va store
    void swap_clone() {
        if (!m_clone) {
            s_error.throw_runtime_error("Khong co gi de swap!");
            return;
        }
        std::swap(m_data, *m_clone);
    }

    /// @brief Dat store thanh ban sao
    void get_clone() {
        if (!m_clone) {
            s_error.throw_runtime_error("Chua co ban sao nao duoc tao!");
            return;
        }
        m_data = *m_clone;
    }
    
    // =======================
    // Toan tu so sanh
    // =======================
    /// @brief So sánh nhỏ hơn
    bool operator<(const store<T> &rhs)
    {
        return std::lexicographical_compare(this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    /// @brief So sánh lớn hơn
    bool operator>(const store<T> &rhs)
    {
        return rhs < this;
    }

    /// @brief SO sánh nhỏ hơn hoặc bằng
    bool operator<=(const store<T> &rhs)
    {
        return !(rhs < this);
    }

    /// @brief So sánh lớn hơn hoặc bằng
    bool operator>=(const store<T> &rhs)
    {
        return !(this < rhs);
    }

    /// @brief Toan tu bang
    bool operator==(const store<T> &rhs)
    {
        if (this->size() != rhs.size())
            return false;
        return std::equal(this->begin(), this->end(), rhs.begin());
    }

    /// @brief Toan tu khac
    bool operator!=(const store<T> &rhs)
    {
        return !(this == rhs);
    }

    // =======================
    // Ham tao pham vi
    // =======================

    /// @brief Tao mang co hoac khong dieu kien
    template <typename Pred = AlwaysTrueTag>
    static std::vector<T> range(T start, T stop, T step = 1, Pred pred = {})
    {
        static_assert(std::is_arithmetic_v<T>, "Range supports only digital data types.");

        if (step == 0)
            return {};

        std::vector<T> result;
        double dist = std::abs(static_cast<double>(stop) - static_cast<double>(start));
        size_t count = static_cast<size_t>(std::ceil(dist / std::abs(static_cast<double>(step))));

        result.reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            T current = start + static_cast<T>(i) * step;

            if (step > 0 ? (current >= stop) : (current <= stop))
                break;

            if constexpr (std::is_same_v<Pred, AlwaysTrueTag>)
            {
                result.push_back(current);
            }
            else
            {
                if (pred(current))
                    result.push_back(current);
            }
        }
        return result;
    }

    /// @brief Tao mang tu 0
    static std::vector<T> range(T stop)
    {
        return range(static_cast<T>(0), stop, static_cast<T>(1));
    }
};

// =======================
// Toan tu khong phai thanh vien
// =======================

/// @brief Toan tu dau ra
template <typename T, typename Container>
std::ostream &operator<<(std::ostream &os, const store<T, Container> &s)
{
    for (std::size_t i = 0; i < s.size(); ++i)
    {
        os << s[i] << ' ';
    }
    return os;
}
/// @brief Toan tu dau vao
template <typename T, typename Container>
std::istream &operator>>(std::istream &is, store<T, Container> &s)
{
    if (s.size() == 0)
    {
        throw std::runtime_error("Store size is 0");
    }
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!(is >> s[i]))
            break;
    }
    return is;
}

template <typename T, typename Container, typename Pred>
inline std::vector<T> range(T start, T stop, T step, Pred pred)
{
    return store<T, Container>::range(start, stop, step,pred);
}

template <typename T, typename Container>
inline std::vector<T> range(T start, T stop, T step = 1)
{
    return store<T, Container>::range(start, stop, step);
}

template <typename T, typename Container>
inline std::vector<T> range(T stop)
{
    return store<T, Container>::range(static_cast<T>(0), stop, static_cast<T>(1));
}
} // namespace adv
