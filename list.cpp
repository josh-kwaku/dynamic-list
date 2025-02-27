//
// Created by kwakujosh on 2/23/25.
//
// A dynamic array

#include <iostream>
#include <cmath>
#include <memory>
#include <cstring>
#include <utility>
#include <iterator>

template <typename Container>
class Iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename Container::value_type;
    using pointer = value_type*;
    using reference = value_type&;

    explicit Iterator(pointer ptr): _ptr{ptr} {}

    reference operator*() const { return *_ptr; }

    pointer operator->() const { return _ptr; }

    // prefix
    Iterator& operator++() {
        _ptr++;
        return *this;
    }
    // postfix
    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // prefix
    Iterator& operator--() {
        _ptr--;
        return *this;
    }
    // postfix
    Iterator operator--(int) {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs._ptr == rhs._ptr;
    }

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return lhs._ptr != rhs._ptr;
    }

    friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
        return lhs._ptr < rhs._ptr;
    }
private:
    pointer _ptr;
};

typedef unsigned long long list_size;

template <typename T>
class List {
public:
    using value_type = T;
    using Iter = Iterator<List<T>>;
    List(): _size{0}, _capacity {2} {
        _data = std::make_unique<T>(_capacity);
    }

    explicit List(const list_size size): _size{0}, _capacity{size} {
        _data = std::make_unique<T[]>(_capacity);
    }

    List(const list_size size, const T& val): _size{size}, _capacity{size} {
        _data = std::make_unique<T[]>(_capacity);
        auto ptr = _data.get();
        for (list_size i = 0; i < _capacity; i++) {
            *(ptr + i) = val;
        }
    }

    List(std::initializer_list<T> init): _size{init.size()}, _capacity{init.size()} {
        _data = std::make_unique<T[]>(_capacity);
        auto itr = init.begin();
        list_size i = 0;
        while (itr != init.end()) {
            *(_data.get() + i++) = *itr;
            ++itr;
        }
    }

    List(List& other): _size{other.size()}, _capacity{other.capacity()} {
        _data = std::make_unique<T[]>(_capacity);
        auto itr = this->begin();
        for (auto& src: other) {
            *itr = src;
            ++itr;
        }
    }

    List(List&& other) noexcept : _size{other.size()}, _capacity{other.capacity()} {
        _data = std::move(other._data);
        other._size = 0;
        other._capacity = 0;
    }

    List &operator=(List& other) {
        if (this == &other) return *this;
        _size = other.size();
        _capacity = other.capacity();
        _data.reset();
        _data = std::make_unique<T[]>(_capacity);
        auto itr = this->begin();
        for (auto& src: other) {
            *itr = src;
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this == &other) return *this;
        _size = other.size();
        _capacity = other.capacity();
        _data = std::move(other._data);
        other._size = 0;
        other._capacity = 0;
        return *this;
    }

    void append(T val) {
        if (_size == _capacity) resize();
        auto ptr = _data.get();
        *(ptr + (_size++)) = val;
    }

    [[nodiscard]] T at(const list_size index) const {
        if (index >= _size) throw std::out_of_range("Provided index is out of range");
        auto ptr = _data.get();
        return *(ptr + index);
    }

    [[nodiscard]] list_size size() const {
        return _size;
    }

    [[nodiscard]] list_size capacity() const {
        return _capacity;
    }

    T& operator[](list_size index) const {
        if (index >= _size) throw std::out_of_range("Provided index is out of range");
        return *(_data.get() + index);
    }

    Iter begin() {
        return Iter{_data.get()};
    }

    Iter end() {
        return Iter{_data.get() + _size};
    }
private:
    list_size _size = 0;
    list_size _capacity = 2;
    std::unique_ptr<T[]> _data;

    void resize() {
        const list_size new_capacity = _capacity << 1;
        auto new_data = std::make_unique<T[]>(new_capacity);
        if constexpr (std::is_trivially_copyable_v<T>) {
            memcpy(new_data.get(), _data.get(), _size * sizeof(T));
        } else {
            for (list_size i = 0; i < _size; i++) {
                new_data[i] = std::move(_data[i]);
            }
        }
        _data = std::move(new_data);
        _capacity = new_capacity;
    }
};
int main() {
    List<std::string> list(10, "10");
    List<std::string> g = {"jamie", "oliver", "twist"};
    List<std::string> h = g;
    h = list;
    // List<int> g = {1,2,4};
    // List<int> h = g;
    // std::cout << g[g.size() - 1] << " " << g.capacity() << '\n';

    // for (auto s: g) {
    //     std::cout << s << '\n';
    // }
    std::reverse(h.begin(), h.end());
    for (auto it = g.begin(); it != g.end(); ++it) {
        std::cout << *it << '\n';
    }
    // list.append(3);
    // list[0] = 20;
    // std::cout << list[0] << '\n';
    //
    // List<std::string> strs = {"kwaku", "fritz", "george"};
    // std::cout << strs[0] << '\n';
    // List<int> test = {1, 2, 3};
    // test = list;
    // test[2] = 20;
    // std::cout << test[2] << '\n';
    // std::cout << list[2] << '\n';
}
