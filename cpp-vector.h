#ifndef VECTOR_H
#define VECTOR_H


#include<iostream>
#include<stdexcept>
#include <initializer_list>
#include<algorithm>
#include <ostream>

template <typename T>
class Vector {
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    size_t sz;
    size_t max_sz;
    pointer values;

    static constexpr size_t min_capacity = 5;


public:


    Vector() : sz(0), max_sz(min_capacity), values(new value_type[min_capacity]) {};

    Vector(const Vector& other) : sz(other.sz), max_sz(other.max_sz), values(new value_type[other.max_sz]) {
        std::copy(other.values, other.values + other.sz, values);
    };

    Vector(size_t num) : sz(0), max_sz(num), values(new value_type[num]) {};

    Vector(std::initializer_list<value_type> list) : sz(list.size()), max_sz(list.size()), values(new value_type[list.size()]) {
        std::copy(list.begin(), list.end(), values);
    }





    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[]values;
            sz = other.sz;
            max_sz = other.max_sz;
            values = new value_type[max_sz];
            std::copy(other.values, other.values + other.size(), values);
        }
        return *this;
    }


    ~Vector() {
        delete[]values;
    }





    size_type size() const {
        return sz;
    }


    bool empty() const {
        if (sz == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    void clear() {
        sz = 0;
    }

    void reserve(size_type n) {
        if (max_sz < n) {

            pointer new_vec = new value_type[n];
            std::copy(values, values + sz, new_vec);
            delete[]values;
            values = new_vec;
            max_sz = n;
        }
    }


    void shrink_to_fit() {
        if (sz < max_sz) {
            pointer new_vec = new value_type[sz];
            std::copy(values, values + sz, new_vec);
            delete[]values;
            values = new_vec;
            max_sz = sz;
        }
    }



    void push_back(value_type x) {
        if (sz >= max_sz) {
            reserve(std::max(static_cast<size_type>(1), (2 * max_sz)));
        }
        values[sz++] = x;
    }




    void pop_back() {


        if (empty()) {
            throw std::runtime_error("false");
        }

        --sz;
    }


    reference operator[](size_type index) {
        if (index >= sz) {
            throw std::runtime_error("False");
        }
        return values[index];
    }

    const_reference operator[](size_type index) const {
        if (index >= sz) {
            throw std::runtime_error("False");
        }
        return values[index];
    }

    size_type capacity() const {
        return max_sz;
    }


    friend std::ostream& operator<<(std::ostream& o, const Vector& v) {
        o << "[";
        for (int i = 0; i < v.sz; i++) {

            o << v.values[i];

            if (i < v.sz - 1) {
                o << ", ";
            }
        }

        o << "]";

        return o;
    }


    class Iterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
        friend class ConstIterator;

    private:
        pointer ptr;

    public:

        Iterator() : ptr(nullptr) {};
        Iterator(pointer p) : ptr(p) {};

        reference operator*() const {
            return *ptr;
        }
        pointer operator->() const {
            return ptr;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const ConstIterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const ConstIterator& other) const {
            return ptr != other.ptr;
        }

        iterator& operator++() {
            ++ptr;
            return *this;
        }

        iterator operator++(int) {
            Iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        operator const_iterator() const {
            return ConstIterator(ptr);
        }
    };




    class ConstIterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;
        friend class Iterator;

    public:

        ConstIterator() : ptr(nullptr) {};
        ConstIterator(pointer p) : ptr(p) {};

        reference operator*() const {
            return *ptr;
        }

        pointer operator->() const {
            return ptr;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const ConstIterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const ConstIterator& other) const {
            return ptr != other.ptr;
        }


        const_iterator& operator++() {
            ++ptr;
            return *this;
        }

        const_iterator operator++(int) {
            ConstIterator tmp = *this;
            ++ptr;
            return tmp;
        }

        friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
            return lop.ptr - rop.ptr;
        }

    };



    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::runtime_error("Iterator out of bounds");

        size_type current{ static_cast<size_type>(diff) };

        if (sz >= max_sz)
            reserve(max_sz * 2 + 1); // Attention: special case if no minimum size is defined

        for (auto i{ sz }; i-- > current;)
            values[i + 1] = values[i];

        values[current] = val;
        ++sz;

        return iterator{ values + current };
    }

    iterator erase(const_iterator pos) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::runtime_error("Iterator out of bounds");

        size_type current{ static_cast<size_type>(diff) };

        for (auto i{ current }; i < sz - 1; ++i)
            values[i] = values[i + 1];

        --sz;

        return iterator{ values + current };
    }




    iterator begin() {
        return iterator(values);
    }
    iterator end() {
        return iterator(values + sz);
    }
    const_iterator begin() const {
        return const_iterator(values);
    }
    const_iterator end() const {
        return const_iterator(values + sz);
    }
};

#endif