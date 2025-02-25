//
// Created by kwakujosh on 2/23/25.
//
// dynamically sized array
// should automatically adjust its size if its getting full
    // should multiply its size by 1.5 on adjustment
// should be able to hold any type
// should have random access using the [] operator

#include <iostream>
#include <cmath>
#include <memory>
#include <cstring>
#include <utility>

typedef unsigned long long list_size;

template <typename T>
class List {
public:
    List() {
        _data = std::make_unique<T>(_capacity);
    }

    explicit List(const list_size size): _capacity{size} {
        _data = std::make_unique<T[]>(_capacity);
    }

    List(const list_size size, T val): _size{size}, _capacity{size} {
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

    List(const List& other): _size{other.size()}, _capacity{other.capacity()} {
        _data = std::make_unique<T[]>(_capacity);
        // should prolly use iterators but for now, just a basic loop
        for (list_size i = 0; i < _size; i++) {
            *(_data.get() + i) = *(other._data.get() + i);
        }
    }

    List &operator=(const List& other) {
        if (this == &other) return *this;
        _size = other.size();
        _capacity = other.capacity();
        _data.reset();
        _data = std::make_unique<T[]>(_capacity);
        // should prolly use iterators but for now, just a basic loop
        for (list_size i = 0; i < _size; i++) {
            *(_data.get() + i) = *(other._data.get() + i);
        }
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
private:
    list_size _size = 0;
    list_size _capacity = 2;
    std::unique_ptr<T[]> _data; // T* _data = new T[]();

    void resize() {
        const list_size new_capacity = _capacity << 1;
        auto new_data = std::make_unique<T[]>(new_capacity);
        memcpy(new_data.get(), _data.get(), _size * sizeof(T));
        _data = std::move(new_data);
    }
};
int main() {
    List<int> list(10, 10);
    // list.append(3);
    // list[0] = 20;
    // std::cout << list[0] << '\n';
    //
    // List<std::string> strs = {"kwaku", "fritz", "george"};
    // std::cout << strs[0] << '\n';
    List<int> test = {1, 2, 3};
    test = list;
    test[2] = 20;
    std::cout << test[2] << '\n';
    std::cout << list[2] << '\n';
}
