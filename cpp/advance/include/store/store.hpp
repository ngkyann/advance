//Tên: store
//Phiên bản: 2.1.0 (C++17 Optimized)
#pragma once

// ===========================
// Minimal Headers (C++17)
// ===========================
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
#include <memory>
#include <optional>
#include <cmath>
#include <sstream>

namespace adv
{
// ===========================
// Error Management (Optimized)
// ===========================
class errors
{
private:
    bool m_exit_code_mode = false;
    short m_last_error_code = 0;

public:
    // Inline all error functions for better optimization
    void set_exit_code_mode(bool mode) noexcept { m_exit_code_mode = mode; }
    
    [[nodiscard]] bool get_exit_code_mode() const noexcept { return m_exit_code_mode; }
    
    [[nodiscard]] short get_last_error_code() const noexcept { return m_last_error_code; }

    // Exception throwing methods
    [[noreturn]] void throw_out_of_range(std::string_view msg = "Error: Out of range") const
    {
        throw std::out_of_range(std::string(msg));
    }

    [[noreturn]] void throw_invalid_argument(std::string_view msg = "Error: Invalid argument") const
    {
        throw std::invalid_argument(std::string(msg));
    }

    [[noreturn]] void throw_runtime_error(std::string_view msg = "Error: Runtime error") const
    {
        throw std::runtime_error(std::string(msg));
    }

    // Error code methods
    short get_out_of_range_code() noexcept { m_last_error_code = -1; return -1; }
    short get_invalid_argument_code() noexcept { m_last_error_code = -2; return -2; }
    short get_runtime_error_code() noexcept { m_last_error_code = -3; return -3; }
};

// ===========================
// Store Class (C++17 Optimized)
// ===========================
template<typename T, typename Container = std::vector<T>>
class store
{
private:
    Container m_data;
    std::unique_ptr<Container> m_clone;
    thread_local static errors s_error;

    // Helper to normalize negative indices
    [[nodiscard]] constexpr long long normalize_index(long long pos) const noexcept
    {
        return pos < 0 ? static_cast<long long>(m_data.size()) + pos : pos;
    }

    // Check bounds
    [[nodiscard]] bool is_valid_index(long long pos) const noexcept
    {
        long long normalized = normalize_index(pos);
        return normalized >= 0 && normalized < static_cast<long long>(m_data.size());
    }

public:
    // Type aliases
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;

    // ===== Constructors =====
    store() = default;
    ~store() = default;

    explicit store(size_t size) : m_data(size) {}
    
    store(size_t size, const T& value) : m_data(size, value) {}
    
    store(std::initializer_list<T> list) : m_data(list) {}

    // Range constructor using enable_if
    template<typename Iterator, typename = std::enable_if_t<!std::is_arithmetic_v<Iterator>>>
    store(Iterator begin, Iterator end) : m_data(begin, end) {}

    // Range-based constructor
    template<typename Range, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Range>, store>>>
    explicit store(const Range& range) : m_data(range.begin(), range.end()) {}

    // Copy constructor
    store(const store& other) : m_data(other.m_data)
    {
        if (other.m_clone)
            m_clone = std::make_unique<Container>(*other.m_clone);
    }

    // Move constructor
    store(store&&) noexcept = default;

    // Copy assignment
    store& operator=(const store& other)
    {
        if (this != &other)
        {
            m_data = other.m_data;
            m_clone = other.m_clone ? std::make_unique<Container>(*other.m_clone) : nullptr;
        }
        return *this;
    }

    // Move assignment
    store& operator=(store&&) noexcept = default;

    // Initializer list assignment
    store& operator=(std::initializer_list<T> list)
    {
        m_data = list;
        return *this;
    }

    // ===== Assignment Methods =====
    void assign(std::initializer_list<T> list) { m_data.assign(list); }
    
    void assign(size_t size, const T& value) { m_data.assign(size, value); }

    template<typename Iterator, typename = std::enable_if_t<!std::is_arithmetic_v<Iterator>>>
    void assign(Iterator begin, Iterator end) { m_data.assign(begin, end); }

    // ===== Element Access =====
    [[nodiscard]] T& at(long long pos)
    {
        long long normalized = normalize_index(pos);
        if (!is_valid_index(pos))
            s_error.throw_out_of_range("Index out of range");
        
        if constexpr (std::is_same_v<Container, std::list<T>>)
            return *std::next(m_data.begin(), normalized);
        else
            return m_data[normalized];
    }

    [[nodiscard]] const T& at(long long pos) const
    {
        long long normalized = normalize_index(pos);
        if (!is_valid_index(pos))
            s_error.throw_out_of_range("Index out of range");
        
        if constexpr (std::is_same_v<Container, std::list<T>>)
            return *std::next(m_data.begin(), normalized);
        else
            return m_data[normalized];
    }

    [[nodiscard]] T& operator[](long long pos) noexcept
    {
        long long normalized = normalize_index(pos);
        if constexpr (std::is_same_v<Container, std::list<T>>)
            return *std::next(m_data.begin(), normalized);
        else
            return m_data[normalized];
    }

    [[nodiscard]] const T& operator[](long long pos) const noexcept
    {
        long long normalized = normalize_index(pos);
        if constexpr (std::is_same_v<Container, std::list<T>>)
            return *std::next(m_data.begin(), normalized);
        else
            return m_data[normalized];
    }

    [[nodiscard]] const T& front() const
    {
        if (m_data.empty())
            s_error.throw_out_of_range("store is empty");
        return m_data.front();
    }

    [[nodiscard]] const T& back() const
    {
        if (m_data.empty())
            s_error.throw_out_of_range("store is empty");
        return m_data.back();
    }

    [[nodiscard]] const T& max() const
    {
        if (m_data.empty())
            s_error.throw_out_of_range("store is empty");
        return *std::max_element(m_data.begin(), m_data.end());
    }

    [[nodiscard]] const T& min() const
    {
        if (m_data.empty())
            s_error.throw_out_of_range("store is empty");
        return *std::min_element(m_data.begin(), m_data.end());
    }

    [[nodiscard]] std::pair<const T&, const T&> max_min() const
    {
        if (m_data.empty())
            s_error.throw_out_of_range("store is empty");
        auto [min_it, max_it] = std::minmax_element(m_data.begin(), m_data.end());
        return {*max_it, *min_it};
    }

    // ===== Size & Capacity =====
    [[nodiscard]] size_t size() const noexcept { return m_data.size(); }
    
    [[nodiscard]] bool empty() const noexcept { return m_data.empty(); }

    void reserve(size_t capacity) noexcept
    {
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            m_data.reserve(capacity);
    }

    void resize(size_t new_size)
    {
        m_data.resize(new_size);
    }

    void resize(size_t new_size, const T& value)
    {
        m_data.resize(new_size, value);
    }

    void clear() noexcept { m_data.clear(); }

    // ===== Type Conversion =====
    explicit operator std::vector<T>() const
    {
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            return m_data;
        else
            return std::vector<T>(m_data.begin(), m_data.end());
    }

    explicit operator std::deque<T>() const
    {
        if constexpr (std::is_same_v<Container, std::deque<T>>)
            return m_data;
        else
            return std::deque<T>(m_data.begin(), m_data.end());
    }

    explicit operator std::list<T>() const
    {
        if constexpr (std::is_same_v<Container, std::list<T>>)
            return m_data;
        else
            return std::list<T>(m_data.begin(), m_data.end());
    }

    // ===== Iterators =====
    [[nodiscard]] iterator begin() noexcept { return m_data.begin(); }
    [[nodiscard]] iterator end() noexcept { return m_data.end(); }
    [[nodiscard]] const_iterator begin() const noexcept { return m_data.begin(); }
    [[nodiscard]] const_iterator end() const noexcept { return m_data.end(); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    [[nodiscard]] const_iterator cend() const noexcept { return m_data.cend(); }
    [[nodiscard]] reverse_iterator rbegin() noexcept { return m_data.rbegin(); }
    [[nodiscard]] reverse_iterator rend() noexcept { return m_data.rend(); }
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
    [[nodiscard]] const_reverse_iterator crend() const noexcept { return m_data.crend(); }

    // ===== Removal Operations =====
    void pop_front()
    {
        if (m_data.empty())
            s_error.throw_out_of_range("Cannot pop_front from empty store");
        
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            m_data.erase(m_data.begin());
        else
            m_data.pop_front();
    }

    void pop_back()
    {
        if (m_data.empty())
            s_error.throw_out_of_range("Cannot pop_back from empty store");
        m_data.pop_back();
    }

    void remove_at(size_t pos)
    {
        if (pos >= m_data.size())
            s_error.throw_out_of_range("Index out of range");
        m_data.erase(std::next(m_data.begin(), pos));
    }

    // ===== Insertion =====
    void insert(size_t pos, const T& value)
    {
        if (pos > m_data.size())
            s_error.throw_out_of_range("Index out of range");
        m_data.insert(std::next(m_data.begin(), pos), value);
    }

    void insert(size_t pos, T&& value)
    {
        if (pos > m_data.size())
            s_error.throw_out_of_range("Index out of range");
        m_data.insert(std::next(m_data.begin(), pos), std::move(value));
    }

    // ===== Modification =====
    void replace_at(size_t pos, const T& value)
    {
        if (pos >= m_data.size())
            s_error.throw_out_of_range("Index out of range");
        
        if constexpr (std::is_same_v<Container, std::list<T>>)
            *std::next(m_data.begin(), pos) = value;
        else
            m_data[pos] = value;
    }

    void replace_all(const T& old_value, const T& new_value)
    {
        std::replace(m_data.begin(), m_data.end(), old_value, new_value);
    }

    void fill(const T& value)
    {
        std::fill(m_data.begin(), m_data.end(), value);
    }

    void reverse()
    {
        if constexpr (std::is_same_v<Container, std::list<T>>)
            m_data.reverse();
        else
            std::reverse(m_data.begin(), m_data.end());
    }

    void swap(store& other) noexcept
    {
        m_data.swap(other.m_data);
        m_clone.swap(other.m_clone);
    }

    // ===== Addition Operations =====
    void push_front(const T& value)
    {
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            m_data.insert(m_data.begin(), value);
        else
            m_data.push_front(value);
    }

    void push_front(T&& value)
    {
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            m_data.insert(m_data.begin(), std::move(value));
        else
            m_data.push_front(std::move(value));
    }

    template<typename Containers>
    void push_front(const Containers& container)
    {
        m_data.insert(m_data.begin(), container.begin(), container.end());
    }

    template<typename Containers>
    std::enable_if_t<!std::is_same_v<std::decay_t<Containers>, T>>
    push_front(Containers&& container)
    {
        m_data.insert(m_data.begin(),
                     std::make_move_iterator(container.begin()),
                     std::make_move_iterator(container.end()));
    }

    void push_front(std::initializer_list<T> list)
    {
        m_data.insert(m_data.begin(), list.begin(), list.end());
    }

    void push_back(const T& value)
    {
        m_data.push_back(value);
    }

    void push_back(T&& value)
    {
        m_data.push_back(std::move(value));
    }

    template<typename C, typename = std::void_t<decltype(std::declval<C>().begin())>>
	void push_back(const C& container)
	{
	    m_data.insert(m_data.end(), container.begin(), container.end());
	}

    template <typename Containers>
	auto push_back(Containers&& container) 
	    -> std::enable_if_t<
	        !std::is_same_v<std::decay_t<Containers>, T> && 
	        !std::is_convertible_v<Containers, T>,
	        void
	    > 
	{
	    using std::begin;
	    using std::end;
	
	    m_data.insert(m_data.end(),
	                  std::make_move_iterator(begin(container)),
	                  std::make_move_iterator(end(container)));
	}

    void push_back(std::initializer_list<T> list)
    {
        m_data.insert(m_data.end(), list.begin(), list.end());
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        return m_data.emplace_back(std::forward<Args>(args)...);
    }

    template<typename... Args>
    T& emplace_front(Args&&... args)
    {
        if constexpr (std::is_same_v<Container, std::vector<T>>)
            return *m_data.emplace(m_data.begin(), std::forward<Args>(args)...);
        else
        {
            m_data.emplace_front(std::forward<Args>(args)...);
            return m_data.front();
        }
    }

    // ===== Searching (Optimized) =====
    [[nodiscard]] bool contains(const T& value) const noexcept
    {
        return std::find(m_data.begin(), m_data.end(), value) != m_data.end();
    }

    template<typename Pred>
    [[nodiscard]] bool any_of(Pred pred) const noexcept
    {
        return std::any_of(m_data.begin(), m_data.end(), pred);
    }

    [[nodiscard]] bool any_of(const T& value) const noexcept
    {
        return std::any_of(m_data.begin(), m_data.end(),
                          [&value](const T& elem) { return elem == value; });
    }

    template<typename Pred>
    [[nodiscard]] bool all_of(Pred pred) const noexcept
    {
        return std::all_of(m_data.begin(), m_data.end(), pred);
    }

    [[nodiscard]] bool all_of(const T& value) const noexcept
    {
        return std::all_of(m_data.begin(), m_data.end(),
                          [&value](const T& elem) { return elem == value; });
    }

    template<typename Pred>
    [[nodiscard]] bool none_of(Pred pred) const noexcept
    {
        return std::none_of(m_data.begin(), m_data.end(), pred);
    }

    [[nodiscard]] bool none_of(const T& value) const noexcept
    {
        return std::none_of(m_data.begin(), m_data.end(),
                           [&value](const T& elem) { return elem == value; });
    }

    // ===== Counting (Optimized) =====
    [[nodiscard]] size_t count(const T& value) const noexcept
    {
        return std::count(m_data.begin(), m_data.end(), value);
    }

    template<typename Pred>
    [[nodiscard]] size_t count_if(Pred pred) const noexcept
    {
        return std::count_if(m_data.begin(), m_data.end(), pred);
    }

    // ===== Find All (Optimized) =====
    [[nodiscard]] std::vector<size_t> find_all_of(const T& value) const
    {
        std::vector<size_t> positions;
        positions.reserve(m_data.size() / 2);

        size_t idx = 0;
        for (const auto& elem : m_data)
        {
            if (elem == value)
                positions.push_back(idx);
            ++idx;
        }
        positions.shrink_to_fit();
        return positions;
    }

    template<typename Pred>
    [[nodiscard]] std::vector<size_t> find_all_if(Pred pred) const
    {
        std::vector<size_t> positions;
        positions.reserve(m_data.size() / 2);

        size_t idx = 0;
        for (const auto& elem : m_data)
        {
            if (pred(elem))
                positions.push_back(idx);
            ++idx;
        }
        positions.shrink_to_fit();
        return positions;
    }

    // ===== Transform & Filter =====
    template<typename Func>
    void transform(Func func)
    {
        std::transform(m_data.begin(), m_data.end(), m_data.begin(), func);
    }

    template<typename Pred>
    [[nodiscard]] store filter(Pred pred) const
    {
        store result;
        std::copy_if(m_data.begin(), m_data.end(), 
                    std::back_inserter(result.m_data), pred);
        return result;
    }

    // ===== Sorting (Optimized) =====
    void sort(bool ascending = true)
    {
        if (m_data.size() < 2) return;

        if constexpr (std::is_same_v<Container, std::vector<T>>)
        {
            if (ascending)
                std::sort(m_data.begin(), m_data.end());
            else
                std::sort(m_data.begin(), m_data.end(), std::greater<T>());
        }
        else
        {
            // For non-vector containers, copy to vector, sort, and copy back
            std::vector<T> temp(std::make_move_iterator(m_data.begin()),
                               std::make_move_iterator(m_data.end()));
            if (ascending)
                std::sort(temp.begin(), temp.end());
            else
                std::sort(temp.begin(), temp.end(), std::greater<T>());
            m_data.assign(std::make_move_iterator(temp.begin()),
                         std::make_move_iterator(temp.end()));
        }
    }

    template<typename Compare>
    void sort(Compare comp)
    {
        if (m_data.size() < 2) return;

        if constexpr (std::is_same_v<Container, std::vector<T>>)
        {
            std::sort(m_data.begin(), m_data.end(), comp);
        }
        else
        {
            std::vector<T> temp(std::make_move_iterator(m_data.begin()),
                               std::make_move_iterator(m_data.end()));
            std::sort(temp.begin(), temp.end(), comp);
            m_data.assign(std::make_move_iterator(temp.begin()),
                         std::make_move_iterator(temp.end()));
        }
    }

    void unique(bool auto_sort = true)
    {
        if (m_data.empty()) return;
        if (auto_sort) sort();

        if constexpr (std::is_same_v<Container, std::list<T>>)
        {
            m_data.unique();
        }
        else
        {
            auto it = std::unique(m_data.begin(), m_data.end());
            m_data.erase(it, m_data.end());
        }
    }

    // ===== String Operations (Optimized) =====
    [[nodiscard]] std::string join(std::string_view separator = "") const
    {
        if (m_data.empty()) return "";

        std::string result;
        bool first = true;

        for (const auto& item : m_data)
        {
            if (!first) result += separator;
            
            if constexpr (std::is_same_v<T, std::string>)
                result += item;
            else if constexpr (std::is_arithmetic_v<T>)
                result += std::to_string(item);
            else
            {
                std::ostringstream oss;
                oss << item;
                result += oss.str();
            }
            first = false;
        }
        return result;
    }

    template<typename Func>
    [[nodiscard]] std::string join(std::string_view separator, Func converter) const
    {
        if (m_data.empty()) return "";

        std::string result;
        bool first = true;

        for (const auto& item : m_data)
        {
            if (!first) result += separator;
            result += converter(item);
            first = false;
        }
        return result;
    }

    // ===== Slicing (Optimized) =====
    [[nodiscard]] store operator()(int start, int end, int step) const
    {
        if (step == 0) return store();
        if (step == 1) return (*this)(start, end);

        int size = static_cast<int>(m_data.size());
        store result;

        if (step > 0)
        {
            int s = (start >= 0) ? start : size + start;
            int e = (end >= 0) ? end : size + end;
            s = std::clamp(s, 0, size);
            e = std::clamp(e, 0, size);

            for (int i = s; i < e; i += step)
                result.push_back(m_data[i]);
        }
        else
        {
            int pos_step = -step;
            int s = (end >= 0) ? end + 1 : size + end + 1;
            int e = (start >= 0) ? start + 1 : size + start + 1;
            s = std::clamp(s, 0, size);
            e = std::clamp(e, 0, size);

            for (int i = s; i < e; i += pos_step)
                result.push_back(m_data[i]);
            result.reverse();
        }
        return result;
    }

    [[nodiscard]] store operator()(int start, int end) const
    {
        int size_raw = static_cast<int>(m_data.size());
        int s = (start >= 0) ? start : size_raw + start;
        int e = (end >= 0) ? end : size_raw + end;
        s = std::clamp(s, 0, size_raw);
        e = std::clamp(e, 0, size_raw);

        store result;
        if (s < e)
        {
            if constexpr (std::is_same_v<Container, std::vector<T>>)
                result.m_data.assign(m_data.begin() + s, m_data.begin() + e);
            else
                result.m_data.assign(std::next(m_data.begin(), s),
                                   std::next(m_data.begin(), e));
        }
        return result;
    }

    // ===== Type Conversion (Optimized) =====
    [[nodiscard]] store<long long> to_ll() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion");

        store<long long> result;
        result.reserve(m_data.size());

        for (const auto& val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                try { result.push_back(std::stoll(val)); }
                catch (...) { s_error.throw_runtime_error("Conversion failed"); }
            }
            else
                result.push_back(static_cast<long long>(val));
        }
        return result;
    }

    [[nodiscard]] store<double> to_double() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion");

        store<double> result;
        result.reserve(m_data.size());

        for (const auto& val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                try { result.push_back(std::stod(val)); }
                catch (...) { s_error.throw_runtime_error("Conversion failed"); }
            }
            else
                result.push_back(static_cast<double>(val));
        }
        return result;
    }

    [[nodiscard]] store<char> to_char() const
    {
        static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>,
                      "Type must be arithmetic or string for conversion");

        store<char> result;
        result.reserve(m_data.size());

        for (const auto& val : m_data)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                if (!val.empty())
                    result.m_data.insert(result.m_data.end(), val.begin(), val.end());
                else
                    result.push_back('\0');
            }
            else
                result.push_back(static_cast<char>(val));
        }
        return result;
    }

    [[nodiscard]] store<std::string> to_str() const
    {
        store<std::string> result;
        result.reserve(m_data.size());

        for (const auto& val : m_data)
        {
            if constexpr (std::is_arithmetic_v<T>)
                result.push_back(std::to_string(val));
            else if constexpr (std::is_same_v<T, char>)
                result.push_back(std::string(1, val));
            else if constexpr (std::is_same_v<T, std::string>)
                result.push_back(val);
            else
            {
                std::ostringstream oss;
                oss << val;
                result.push_back(oss.str());
            }
        }
        return result;
    }

    // ===== Clone Management =====
    void set_clone()
    {
        m_clone = std::make_unique<Container>(m_data);
    }

    [[nodiscard]] std::optional<store> clone() const
    {
        if (!m_clone) return std::nullopt;
        store result;
        result.m_data = *m_clone;
        return result;
    }

    void delete_clone() noexcept { m_clone.reset(); }

    void swap_clone()
    {
        if (!m_clone)
            s_error.throw_runtime_error("No clone to swap");
        std::swap(m_data, *m_clone);
    }

    void restore_clone()
    {
        if (!m_clone)
            s_error.throw_runtime_error("No clone to restore");
        m_data = *m_clone;
    }

    // ===== Comparison Operators =====
    [[nodiscard]] bool operator<(const store& rhs) const noexcept
    {
        return std::lexicographical_compare(m_data.begin(), m_data.end(),
                                          rhs.m_data.begin(), rhs.m_data.end());
    }

    [[nodiscard]] bool operator>(const store& rhs) const noexcept
    {
        return rhs < *this;
    }

    [[nodiscard]] bool operator<=(const store& rhs) const noexcept
    {
        return !(rhs < *this);
    }

    [[nodiscard]] bool operator>=(const store& rhs) const noexcept
    {
        return !(*this < rhs);
    }

    [[nodiscard]] bool operator==(const store& rhs) const noexcept
    {
        return m_data.size() == rhs.m_data.size() &&
               std::equal(m_data.begin(), m_data.end(), rhs.m_data.begin());
    }

    [[nodiscard]] bool operator!=(const store& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    // ===== Static Range Methods =====
    template<typename Pred = std::true_type>
    [[nodiscard]] static std::vector<T> range(T start, T stop, T step = 1, Pred pred = {})
    {
        static_assert(std::is_arithmetic_v<T>, "Range supports only numeric types");

        if (step == 0) return {};

        std::vector<T> result;
        double dist = std::abs(static_cast<double>(stop) - static_cast<double>(start));
        size_t count = static_cast<size_t>(std::ceil(dist / std::abs(static_cast<double>(step))));
        result.reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            T current = start + static_cast<T>(i) * step;
            if (step > 0 ? (current >= stop) : (current <= stop))
                break;

            if constexpr (std::is_same_v<Pred, std::true_type> ||
                         std::is_invocable_v<Pred, T>)
            {
                if constexpr (std::is_same_v<Pred, std::true_type>)
                    result.push_back(current);
                else if (pred(current))
                    result.push_back(current);
            }
        }
        return result;
    }

    [[nodiscard]] static std::vector<T> range(T stop)
    {
        return range(static_cast<T>(0), stop, static_cast<T>(1));
    }
};

// Template static member initialization
template<typename T, typename Container>
thread_local errors store<T, Container>::s_error;

// ===========================
// Non-member Operators
// ===========================

template<typename T, typename Container>
std::ostream& operator<<(std::ostream& os, const store<T, Container>& s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        os << s[i];
        if (i + 1 < s.size()) os << ' ';
    }
    return os;
}

template<typename T, typename Container>
std::istream& operator>>(std::istream& is, store<T, Container>& s)
{
    if (s.size() == 0)
        throw std::runtime_error("Store size is 0");

    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!(is >> s[i])) break;
    }
    return is;
}

// ===========================
// Global Range Functions
// ===========================

template<typename T, typename Container, typename Pred>
[[nodiscard]] inline std::vector<T> range(T start, T stop, T step, Pred pred)
{
    return store<T, Container>::range(start, stop, step, pred);
}

template<typename T, typename Container>
[[nodiscard]] inline std::vector<T> range(T start, T stop, T step = 1)
{
    return store<T, Container>::range(start, stop, step);
}

template<typename T, typename Container>
[[nodiscard]] inline std::vector<T> range(T stop)
{
    return store<T, Container>::range(stop);
}

} // namespace adv
