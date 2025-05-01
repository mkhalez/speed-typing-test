#ifndef STRING_H
#define STRING_H

#include <function.h>
#include <QDebug>
#include <memory>
#include <stdexcept>

class MyString {

   private:
    static constexpr size_t npos = -1;
    std::unique_ptr<char[]> data = nullptr;
    size_t length;
    size_t capacity;

    void resize(size_t new_capacity) {
        capacity = new_capacity;
        data = std::make_unique<char[]>(capacity);
    }

    size_t strlen(const char* s) {
        const char* p = s;
        while (*p)
            p++;
        return p - s;
    }

    char* strcpy(char* s1, const char* s2) {
        char* start = s1;
        while ((*s1++ = *s2++) != '\0') {}
        return start;
    }

    char* strncpy(char* dest, const char* src, size_t n) {
        if (n == 0) {
            return dest;
        }

        char* start = dest;


        while (n > 0 && *src != '\0') {
            *dest++ = *src++;
            n--;
        }

        while (n > 0) {
            *dest++ = '\0';
            n--;
        }

        return start;
    }

   public:
    MyString() : length(0), capacity(1) {
        data = std::make_unique<char[]>(1);
        data[0] = '\0';
    }

    MyString(const char* s) {
        if (!s) {
            length = 0;
            capacity = 1;
            data = std::make_unique<char[]>(1);
            data[0] = '\0';
            return;
        }

        length = strlen(s);
        resize(length + 1);
        /*capacity = length + 1;
        data = std::make_unique<char[]>(capacity);*/
        strcpy(data.get(), s);
    }

    MyString& assign(const char* s) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::assign: null pointer passed");
        } else {
            length = strlen(s);
            if (capacity < length + 1) {
                resize(length + 1);
            }

            strcpy(data.get(), s);
            return *this;
        }
    }

    MyString(const MyString& other) {
        if (!other.data) {
            length = 0;
            resize(length + 1);
            /*capacity = 1;
            data = std::make_unique<char[]>(capacity);*/
            data[length] = '\0';
        } else {
            length = other.length;
            resize(other.capacity);
            strcpy(data.get(), other.data.get());
            /*
            capacity = other.capacity;
            data = std::make_unique<char[]>(capacity);
            function.strcpy(data.get(), other.data.get());*/
        }
    }

    MyString(const char* s, int n) {
        if (s == nullptr)
            throw std::invalid_argument("Null pointer");
        if (n < 0)
            throw std::invalid_argument("Negative length");

        if (n == 0) {
            resize(1);
            length = 0;
            data[0] = '\0';
        } else {
            length = 0;
            while (length < static_cast<size_t>(n) && s[length] != '\0') {
                length++;
            }
            resize(length + 1);
            std::copy(s, s + length, data.get());
            data[length] = '\0';
        }
    }

    MyString& assign(const MyString& str) {
        if (this != &str) {
            if (!str.data) {
                length = 0;
                if (capacity < 1) {
                    resize(1);
                    /*
                    data = std::make_unique<char[]>(1);
                    capacity = 1;*/
                }
                data[length] = '\0';
                return *this;
            }

            if (capacity < str.capacity) {
                resize(str.capacity);
                this->length = str.length;
                strcpy(data.get(), str.data.get());
                /*
                auto new_data = std::make_unique<char[]>(str.capacity);
                this->capacity = str.capacity;
                this->length = str.length;
                function.strcpy(new_data.get(), str.data.get());
                data.reset(new_data.release());*/
            } else {
                strcpy(data.get(), str.data.get());
                length = str.length;
            }
        }
        return *this;
    }


    MyString& operator=(const MyString& other) {
        if (this != &other) {

            if (!other.data) {
                length = 0;
                if (capacity < 1) {
                    resize(1);
                    /*data = std::make_unique<char[]>(1);
                    capacity = 1;*/
                }
                data[length] = '\0';
                return *this;
            }

            if (capacity < other.capacity) {
                resize(other.capacity);
                this->length = other.length;
                strcpy(data.get(), other.data.get());
                /*
                auto new_data = std::make_unique<char[]>(other.capacity);
                //std::unique_ptr<char[]> new_data(new char[other.capacity]);
                this->capacity = other.capacity;
                this->length = other.length;
                strcpy(new_data.get(), other.data.get());
                data.reset(new_data.release());*/
            } else {
                strcpy(data.get(), other.data.get());
                length = other.length;
            }
        }

        return *this;
    }

    const char& at(size_t pos) const {
        if (pos >= length) {
            throw std::out_of_range("MyString::at: position out of range");
        }
        return data[pos];
    }

    char& at(size_t pos) {
        if (pos >= length) {
            throw std::out_of_range("MyString::at: position out of range");
        }
        return data[pos];
    }

    char& front() {
        if (length == 0) {
            throw std::out_of_range("MyString::front: string is empty");
        }
        return data[0];
    }

    const char& front() const {
        if (length == 0) {
            throw std::out_of_range("MyString::front: string is empty");
        }
        return data[0];
    }

    char& back() {
        if (length == 0) {
            throw std::out_of_range("MyString::back: string is empty");
        }
        return data[length - 1];
    }

    const char& back() const {
        if (length == 0) {
            throw std::out_of_range("MyString::back: string is empty");
        }
        return data[length - 1];
    }

    const char* get_data() const { return data.get(); }

    char* get_data() { return data.get(); }

    const char* c_str() const { return data.get(); }

    const char& operator[](size_t pos) const noexcept { return data[pos]; }

    char& operator[](size_t pos) noexcept { return data[pos]; }


    friend QDebug operator<<(QDebug debug, const MyString& str) {
        QDebugStateSaver saver(debug);
        debug.nospace();

        if (str.data) {
            debug << str.data.get();
        } else {
            debug << "";
        }

        return debug;
    }

    class iterator {
       private:
        char* ptr;

       public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = char;
        using difference_type = std::ptrdiff_t;
        using pointer = char*;
        using reference = char&;

        explicit iterator(char* ptr = nullptr) : ptr(ptr) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        iterator& operator++() {
            ++ptr;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        iterator& operator--() {
            --ptr;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }

        iterator operator+(difference_type n) const {
            return iterator(ptr + n);
        }
        iterator operator-(difference_type n) const {
            return iterator(ptr - n);
        }
        iterator& operator+=(difference_type n) {
            ptr += n;
            return *this;
        }
        iterator& operator-=(difference_type n) {
            ptr -= n;
            return *this;
        }

        reference operator[](difference_type n) const { return ptr[n]; }

        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator<(const iterator& other) const { return ptr < other.ptr; }
        bool operator>(const iterator& other) const { return ptr > other.ptr; }
        bool operator<=(const iterator& other) const {
            return ptr <= other.ptr;
        }
        bool operator>=(const iterator& other) const {
            return ptr >= other.ptr;
        }

        difference_type operator-(const iterator& other) const {
            return ptr - other.ptr;
        }
    };

    class const_iterator {
       private:
        const char* ptr;

       public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const char;
        using difference_type = std::ptrdiff_t;
        using pointer = const char*;
        using reference = const char&;

        explicit const_iterator(const char* ptr = nullptr) : ptr(ptr) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        const_iterator& operator++() {
            ++ptr;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        const_iterator& operator--() {
            --ptr;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --ptr;
            return tmp;
        }

        const_iterator operator+(difference_type n) const {
            return const_iterator(ptr + n);
        }
        const_iterator operator-(difference_type n) const {
            return const_iterator(ptr - n);
        }
        const_iterator& operator+=(difference_type n) {
            ptr += n;
            return *this;
        }
        const_iterator& operator-=(difference_type n) {
            ptr -= n;
            return *this;
        }

        reference operator[](difference_type n) const { return ptr[n]; }

        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator<(const const_iterator& other) const {
            return ptr < other.ptr;
        }
        bool operator>(const const_iterator& other) const {
            return ptr > other.ptr;
        }
        bool operator<=(const const_iterator& other) const {
            return ptr <= other.ptr;
        }
        bool operator>=(const const_iterator& other) const {
            return ptr >= other.ptr;
        }

        difference_type operator-(const const_iterator& other) const {
            return ptr - other.ptr;
        }
    };

    iterator begin() noexcept { return iterator(data.get()); }

    iterator end() noexcept { return iterator(data.get() + length); }

    const_iterator begin() const noexcept { return const_iterator(data.get()); }

    const_iterator end() const noexcept {
        return const_iterator(data.get() + length);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(data.get());
    }

    const_iterator cend() const noexcept {
        return const_iterator(data.get() + length);
    }


    bool empty() const { return length == 0; }


    size_t size() const { return length; }


    size_t get_length() const { return length; }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(char) - 1;
    }


    void reserve(size_t n) {
        if (capacity < n) {
            auto new_data = std::make_unique<char[]>(n);

            if (data) {
                strcpy(new_data.get(), data.get());
            }

            data.reset(new_data.release());
            capacity = n;
        }
    }


    size_t get_capacity() const { return capacity; }


    void shrink_to_fit() {
        if (capacity == length + 1) {
            return;
        }
        auto new_data = std::make_unique<char[]>(length + 1);
        if (data) {
            strcpy(new_data.get(), data.get());
        }
        data.reset(new_data.release());
        capacity = length + 1;
    }

    void clear() noexcept {
        if (data) {
            data[0] = '\0';
        }
        length = 0;
    }

    MyString& insert(size_t pos, size_t count, char ch) {
        if (pos > length) {
            throw std::out_of_range("MyString::insert: out of range");
        }

        if (count > max_size() - length) {
            throw std::length_error("MyString::insert: count too large");
        }
        if (length + count >= capacity) {
            reserve(length + count + 1);
        }


        for (size_t i = length; i > pos; --i) {
            data[i + count - 1] = data[i - 1];
        }


        for (int i = 0; i < count; i++) {
            data[pos + i] = ch;
        }

        length += count;
        data[length] = '\0';

        return *this;
    }

    MyString& insert(size_t pos, char* s) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::insert: null pointer passed");
        }
        if (pos > length) {
            throw std::out_of_range("MyString::insert: out of range");
        }

        size_t count = strlen(s);

        if (count > max_size() - length) {
            throw std::length_error("MyString::insert: count too large");
        }
        if (length + count >= capacity) {
            reserve(length + count + 1);
        }


        for (size_t i = length; i > pos; --i) {
            data[i + count - 1] = data[i - 1];
        }


        for (int i = 0; i < count; i++) {
            data[pos + i] = s[i];
        }

        length += count;
        data[length] = '\0';

        return *this;
    }

    MyString& insert(size_t pos, const MyString& str) {
        if (this == &str) {
            MyString tmp(str);
            return insert(pos, tmp.data.get());
        }
        if (pos > length) {
            throw std::out_of_range("MyString::insert: pos out of range");
        }
        if (str.length == 0) {
            return *this;
        }

        if (str.length > max_size() - length) {
            throw std::length_error(
                "MyString::insert: resulting string too long");
        }
        if (length + str.length >= capacity) {
            reserve(length + str.length + 1);
        }


        for (size_t i = length; i > pos; --i) {
            data[i + str.length - 1] = data[i - 1];
        }


        for (size_t i = 0; i < str.length; ++i) {
            data[pos + i] = str.data[i];
        }

        length += str.length;
        data[length] = '\0';

        return *this;
    }


    iterator insert(iterator p, char ch) {
        if (p == end()) {
            if (length + 1 >= capacity) {
                reserve(capacity == 0 ? 2 : capacity * 2);
            }
            data[length] = ch;
            data[length + 1] = '\0';
            length++;
            return iterator(data.get() + length - 1);
        }

        if (p < begin() || p > end()) {
            throw std::out_of_range(
                "MyString::insert: invalid iterator position");
        }

        size_t pos = p - begin();

        if (length + 1 >= capacity) {
            size_t offset = p - begin();
            reserve(capacity == 0 ? 2 : capacity * 2);
            p = iterator(data.get() + offset);
        }

        for (size_t i = length; i > pos; --i) {
            data[i] = data[i - 1];
        }

        data[pos] = ch;
        length++;
        data[length] = '\0';

        return iterator(data.get() + pos);
    }


    iterator insert(iterator p, size_t n, char ch) {
        if (n == 0) {
            return p;
        }

        if (p < begin() || p > end()) {
            throw std::out_of_range(
                "MyString::insert: invalid iterator position");
        }

        if (n > max_size() - length) {
            throw std::length_error("MyString::insert: insertion too large");
        }

        size_t pos = p - begin();

        if (length + n >= capacity) {
            size_t new_capacity = std::max(length + n + 1, capacity * 2);
            size_t offset = p - begin();
            reserve(new_capacity);
            p = iterator(data.get() + offset);
            pos = offset;
        }

        for (size_t i = length; i > pos; --i) {
            data[i + n - 1] = data[i - 1];
        }

        for (size_t i = 0; i < n; ++i) {
            data[pos + i] = ch;
        }

        length += n;
        data[length] = '\0';

        return iterator(data.get() + pos);
    }

    template <class InputIt>


    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) {
            return iterator(data.get() + (pos - cbegin()));
        }

        if (pos < cbegin() || pos > cend()) {
            throw std::out_of_range(
                "MyString::insert: invalid iterator position");
        }

        size_t n = 0;
        InputIt it = first;
        while (it != last) {
            ++n;
            ++it;
            if (n > max_size() - length) {
                throw std::length_error(
                    "MyString::insert: insertion too large");
            }
        }

        size_t insert_pos = pos - cbegin();

        if (length + n >= capacity) {
            size_t new_capacity = std::max(length + n + 1, capacity * 2);
            reserve(new_capacity);
        }

        for (size_t i = length; i > insert_pos; --i) {
            data[i + n - 1] = data[i - 1];
        }

        size_t current_pos = insert_pos;
        for (InputIt src = first; src != last; ++src) {
            data[current_pos++] = *src;
        }

        length += n;
        data[length] = '\0';

        return iterator(data.get() + insert_pos);
    }

    MyString& erase(size_t index = 0, size_t count = npos) {
        if (index > length) {
            throw std::out_of_range("MyString::erase: index out of range");
        }

        size_t actual_count = std::min(count, length - index);
        if (actual_count > 0) {
            for (size_t i = index; i < length - actual_count; ++i) {
                data[i] = data[i + actual_count];
            }

            length -= actual_count;
            data[length] = '\0';
        }

        return *this;
    }


    iterator erase(iterator position) {
        if (position == end()) {
            throw std::out_of_range(
                "MyString::erase: cannot erase end iterator");
        }

        if (position < begin() || position >= end()) {
            throw std::out_of_range(
                "MyString::erase: invalid iterator position");
        }

        size_t pos = position - begin();

        for (size_t i = pos; i < length - 1; ++i) {
            data[i] = data[i + 1];
        }

        length--;
        data[length] = '\0';

        return iterator(data.get() + pos);
    }


    iterator erase(iterator first, iterator last) {
        if (first == last)
            return first;

        size_t pos = first - begin();
        size_t count = last - first;

        for (size_t i = pos; i < length - count; ++i) {
            data[i] = data[i + count];
        }

        length -= count;
        data[length] = '\0';
        return iterator(data.get() + pos);
    }


    void push_back(char ch) {
        if (length + 1 >= capacity) {
            reserve(capacity == 0 ? 2 : capacity * 2);
        }

        data[length] = ch;
        length++;
        data[length] = '\0';
    }


    void pop_back() {
        if (length == 0) {
            throw std::out_of_range("MyString::pop_back: string is empty");
        }

        length--;
        data[length] = '\0';
    }

    MyString& append(char ch) {
        if (length + 1 >= capacity) {
            reserve(capacity == 0 ? 2 : capacity * 2);
        }
        data[length] = ch;
        length++;
        data[length] = '\0';
        return *this;
    }

    MyString& append(const char* s) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::append: null pointer passed");
        }

        size_t s_len = strlen(s);
        if (s_len == 0) {
            return *this;
        }
        if (length + s_len >= capacity) {
            reserve(length + s_len + 1);
        }

        strcpy(data.get() + length, s);
        length += s_len;
        return *this;
    }

    MyString& append(const char* s, size_t n) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::append: null pointer passed");
        }

        size_t s_len = strlen(s);
        size_t append_len = std::min(n, s_len);
        if (append_len == 0) {
            return *this;
        }
        if (length + append_len >= capacity) {
            reserve(length + append_len + 1);
        }

        strncpy(data.get() + length, s, append_len);
        length += append_len;
        data[length] = '\0';
        return *this;
    }

    MyString& append(const MyString& str) {
        if (str.length == 0) {
            return *this;
        }
        if (length + str.length >= capacity) {
            reserve(length + str.length + 1);
        }

        strcpy(data.get() + length, str.data.get());
        length += str.length;
        return *this;
    }

    MyString& append(size_t n, char ch) {
        if (n == 0) {
            return *this;
        }
        if (length + n >= capacity) {
            reserve(length + n + 1);
        }


        for (size_t i = 0; i < n; ++i) {
            data[length + i] = ch;
        }

        length += n;
        data[length] = '\0';
        return *this;
    }

    template <class InputIt>
    MyString& append(InputIt first, InputIt last) {
        size_t n = std::distance(first, last);
        if (n == 0) {
            return *this;
        }
        if (length + n >= capacity) {
            reserve(length + n + 1);
        }


        for (; first != last; ++first) {
            data[length++] = *first;
        }

        data[length] = '\0';
        return *this;
    }

    MyString& replace(size_t pos, size_t count, const MyString& str) {
        if (pos > length) {
            throw std::out_of_range("MyString::replace: position out of range");
        }

        size_t actual_count = std::min(count, length - pos);
        size_t new_length = length - actual_count + str.length;

        if (new_length > max_size()) {
            throw std::length_error(
                "MyString::replace: resulting string too long");
        }
        if (new_length >= capacity) {
            reserve(new_length + 1);
        }
        if (str.length != actual_count) {
            for (size_t i = pos + actual_count; i <= length; ++i) {
                data[i + str.length - actual_count] = data[i];
            }
        }


        for (size_t i = 0; i < str.length; ++i) {
            data[pos + i] = str.data[i];
        }

        length = new_length;
        data[length] = '\0';
        return *this;
    }

    MyString& replace(size_t pos, size_t count, const char* s) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::replace: null pointer passed");
        }

        MyString temp(s);
        return replace(pos, count, temp);
    }

    MyString& replace(size_t pos, size_t count, size_t n, char ch) {
        MyString temp;
        temp.append(n, ch);
        return replace(pos, count, temp);
    }

    MyString& replace(iterator first, iterator last, const MyString& str) {
        if (first < begin() || last > end() || first > last) {
            throw std::out_of_range(
                "MyString::replace: invalid iterator range");
        }

        size_t pos = first - begin();
        size_t count = last - first;
        return replace(pos, count, str);
    }

    MyString& replace(iterator first, iterator last, const char* s) {
        if (!s) {
            throw std::invalid_argument(
                "MyString::replace: null pointer passed");
        }

        size_t pos = first - begin();
        size_t count = last - first;
        return replace(pos, count, s);
    }


    size_t copy(char* dest, size_t count, size_t pos = 0) const {
        if (!dest) {
            throw std::invalid_argument(
                "MyString::copy: null destination pointer");
        }

        if (pos > length) {
            throw std::out_of_range("MyString::copy: position out of range");
        }

        size_t available = length - pos;
        size_t actual_count = std::min(count, available);

        if (actual_count > 0) {
            for (size_t i = 0; i < actual_count; ++i) {
                dest[i] = data[pos + i];
            }
        }

        return actual_count;
    }


    void get_resize(size_t new_size, char fill_char = '\0') {
        if (new_size > max_size()) {
            throw std::length_error(
                "MyString::resize: new size exceeds max size");
        }

        if (new_size < length) {
            length = new_size;
            data[length] = '\0';
        } else if (new_size > length) {
            if (new_size >= capacity) {
                reserve(new_size + 1);
            }

            for (size_t i = length; i < new_size; ++i) {
                data[i] = fill_char;
            }

            length = new_size;
            data[length] = '\0';
        }
    }

    void swap(MyString& other) noexcept {
        std::swap(data, other.data);

        std::swap(length, other.length);

        std::swap(capacity, other.capacity);
    }

    size_t find(char ch, size_t pos = 0) const noexcept {
        if (pos >= length) {
            return npos;
        }


        for (size_t i = pos; i < length; ++i) {
            if (data[i] == ch) {
                return i;
            }
        }

        return npos;
    }


    size_t find(const char* s, size_t pos = 0) {
        if (!s) {
            throw std::invalid_argument("MyString::find: null pointer passed");
        }

        size_t s_len = strlen(s);
        if (pos > length || s_len == 0 || s_len > length - pos) {
            return npos;
        }

        for (size_t i = pos; i <= length - s_len; ++i) {
            bool found = true;
            for (size_t j = 0; j < s_len; ++j) {
                if (data[i + j] != s[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }

        return npos;
    }

    size_t find(const MyString& str, size_t pos = 0) const noexcept {
        if (pos > length || str.length == 0 || str.length > length - pos) {
            return npos;
        }


        for (size_t i = pos; i <= length - str.length; ++i) {
            bool found = true;
            for (size_t j = 0; j < str.length; ++j) {
                if (data[i + j] != str.data[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }

        return npos;
    }

    size_t rfind(char ch, size_t pos = npos) const noexcept {
        if (length == 0)
            return npos;
        if (pos >= length) {
            pos = length - 1;
        }


        for (size_t i = pos; i != npos; --i) {
            if (data[i] == ch) {
                return i;
            }
        }

        return npos;
    }


    size_t rfind(const char* s, size_t pos = npos) {
        if (!s) {
            throw std::invalid_argument("MyString::rfind: null pointer passed");
        }

        size_t s_len = strlen(s);
        if (s_len == 0)
            return pos < length ? pos : length;
        if (s_len > length)
            return npos;

        if (pos >= length) {
            pos = length - 1;
        }

        for (size_t i = (pos >= length - s_len ? length - s_len : pos);
             i != npos; --i) {
            bool match = true;
            for (size_t j = 0; j < s_len; ++j) {
                if (data[i + j] != s[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i;
            }
        }

        return npos;
    }

    size_t rfind(const MyString& str, size_t pos = npos) const noexcept {
        if (str.length == 0)
            return pos < length ? pos : length;
        if (str.length > length)
            return npos;
        if (pos >= length) {
            pos = length - 1;
        }

        for (size_t i =
                 (pos >= length - str.length ? length - str.length : pos);
             i != npos; --i) {
            bool match = true;


            for (size_t j = 0; j < str.length; ++j) {
                if (data[i + j] != str.data[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i;
            }
        }

        return npos;
    }

    int compare(const MyString& str) const noexcept {
        size_t min_len = std::min(length, str.length);


        for (size_t i = 0; i < min_len; ++i) {
            if (data[i] != str.data[i]) {
                return static_cast<int>(data[i]) -
                       static_cast<int>(str.data[i]);
            }
        }
        return static_cast<int>(length) - static_cast<int>(str.length);
    }


    int compare(const char* s) {
        if (!s)
            throw std::invalid_argument("MyString::compare: null pointer");

        size_t s_len = strlen(s);
        size_t min_len = std::min(length, s_len);

        for (size_t i = 0; i < min_len; ++i) {
            if (data[i] != s[i]) {
                return static_cast<int>(data[i]) - static_cast<int>(s[i]);
            }
        }
        return static_cast<int>(length) - static_cast<int>(s_len);
    }

    bool contains(char ch) const noexcept { return find(ch) != npos; }


    bool contains(const char* s) const {
        if (!s) {
            throw std::invalid_argument(
                "MyString::contains: null pointer passed");
        }
        return find(s) != npos;
    }

    bool contains(const MyString& str) const noexcept {
        return find(str) != npos;
    }


    MyString substr(size_t pos = 0, size_t count = npos) const {
        if (pos > length) {
            throw std::out_of_range("MyString::substr: position out of range");
        }

        size_t real_count = std::min(count, length - pos);

        MyString result;
        if (real_count > 0) {
            result.reserve(real_count + 1);
            for (size_t i = 0; i < real_count; ++i) {
                result.push_back(data[pos + i]);
            }
        }

        return result;
    }

    MyString& operator+=(char ch) { return append(ch); }

    MyString& operator+=(const char* s) { return append(s); }

    MyString& operator+=(const MyString& str) { return append(str); }

    bool operator==(const MyString& other) const noexcept {
        if (length != other.length)
            return false;
        return compare(other) == 0;
    }

    bool operator!=(const MyString& other) const noexcept {
        return !(*this == other);
    }

    bool operator<(const MyString& other) const noexcept {
        return compare(other) < 0;
    }

    bool operator>(const MyString& other) const noexcept {
        return compare(other) > 0;
    }

    bool operator<=(const MyString& other) const noexcept {
        return compare(other) <= 0;
    }

    bool operator>=(const MyString& other) const noexcept {
        return compare(other) >= 0;
    }

    // Сравнение с C-строками
    bool operator==(const char* s) const {
        if (!s)
            throw std::invalid_argument("MyString::operator==: null pointer");
        return compare(s) == 0;
    }

    bool operator!=(const char* s) const { return !(*this == s); }

    bool operator<(const char* s) const {
        if (!s)
            throw std::invalid_argument("MyString::operator<: null pointer");
        return compare(s) < 0;
    }

    bool operator>(const char* s) const {
        if (!s)
            throw std::invalid_argument("MyString::operator>: null pointer");
        return compare(s) > 0;
    }

    bool operator<=(const char* s) const {
        if (!s)
            throw std::invalid_argument("MyString::operator<=: null pointer");
        return compare(s) <= 0;
    }

    bool operator>=(const char* s) const {
        if (!s)
            throw std::invalid_argument("MyString::operator>=: null pointer");
        return compare(s) >= 0;
    }

    ~MyString() = default;
};

#endif	// STRING_H
