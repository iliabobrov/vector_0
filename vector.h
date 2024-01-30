#pragma once
#define VECTOR_MEMORY_IMPLEMENTED
#include <memory>
#include <exception>
#include <cstdint>
#include <algorithm>

template <class T>
class Vector {
 private:
  uint64_t size_;
  uint64_t capacity_;
  T *bufer_;
  void Delite() {
    if (size_ > 0) {
      std::destroy(begin(), end());
      size_ = 0;
    }
    if (capacity_ > 0) {
      ::operator delete(bufer_);
      capacity_ = 0;
    }
    bufer_ = nullptr;
  }

 public:
  using ValueType = T;
  using Pointer = T *;
  using ConstPointer = const T *;
  using Reference = T &;
  using ConstReference = const T &;
  using RvalReference = T &&;
  using SizeType = size_t;
  using Iterator = T *;
  using ConstIterator = const T *;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  Vector() : size_(0), capacity_(0), bufer_(nullptr) {
  }
  explicit Vector(uint64_t size) {
    size_ = 0;
    capacity_ = 0;
    bufer_ = nullptr;
    if (size == 0) {
      return;
    }
    bufer_ = reinterpret_cast<T *>(::operator new(size * sizeof(T)));
    try {
      std::uninitialized_default_construct(bufer_, bufer_ + size);
      size_ = size;
      capacity_ = size;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }
  Vector(uint64_t size, T value) {
    if (size == 0) {
      size_ = 0;
      capacity_ = 0;
      bufer_ = nullptr;
      return;
    }
    try {
      bufer_ = reinterpret_cast<T *>(::operator new(size * sizeof(T)));
    } catch (...) {
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    try {
      std::uninitialized_fill(bufer_, bufer_ + size, value);
      size_ = size;
      capacity_ = size;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }
  template <class Iter, class = std::enable_if_t<std::is_base_of_v<
                            std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category> > >
  Vector(Iter beg, Iter end) {
    uint64_t size = std::distance(beg, end);
    if (size == 0) {
      size_ = 0;
      capacity_ = 0;
      bufer_ = nullptr;
      return;
    }
    try {
      bufer_ = reinterpret_cast<T *>(::operator new(size * sizeof(T)));
    } catch (...) {
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    try {
      std::uninitialized_copy(beg, end, bufer_);
      size_ = size;
      capacity_ = size;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }
  Vector(std::initializer_list<T> list) {
    uint64_t size = list.size();
    if (size == 0) {
      size_ = 0;
      capacity_ = 0;
      bufer_ = nullptr;
      return;
    }
    try {
      bufer_ = reinterpret_cast<T *>(::operator new(size * sizeof(T)));
    } catch (...) {
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    try {
      std::uninitialized_copy(list.begin(), list.end(), bufer_);
      size_ = size;
      capacity_ = size;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  // правило пяти
  Vector(const Vector &oth) {
    if (oth.size_ == 0) {
      size_ = 0;
      capacity_ = 0;
      bufer_ = nullptr;
      return;
    }
    try {
      bufer_ = reinterpret_cast<T *>(::operator new(oth.capacity_ * sizeof(T)));
    } catch (...) {
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    try {
      std::uninitialized_copy(oth.begin(), oth.end(), bufer_);
      size_ = oth.size_;
      capacity_ = oth.capacity_;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }
  Vector(Vector &&oth) noexcept {
    bufer_ = oth.bufer_;
    size_ = oth.size_;
    capacity_ = oth.capacity_;
    oth.size_ = 0;
    oth.capacity_ = 0;
    oth.bufer_ = nullptr;
  }
  Vector operator=(const Vector &oth) {
    if (this == &oth) {
      return *this;
    }
    Delite();
    if (oth.size_ == 0) {
      return *this;
    }
    bufer_ = reinterpret_cast<T *>(::operator new(oth.capacity_ * sizeof(T)));
    try {
      std::uninitialized_copy(oth.begin(), oth.end(), bufer_);
      size_ = oth.size_;
      capacity_ = oth.capacity_;
    } catch (...) {
      ::operator delete(bufer_);
      bufer_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    return *this;
  }

  Vector &operator=(Vector &&oth) noexcept {
    if (this == &oth) {
      return *this;
    }
    Delite();
    Swap(oth);
    return *this;
  }

  ~Vector() {
    Delite();
  }
  // правило пяти

  size_t Size() const {
    return size_;
  }
  size_t Capacity() const {
    return capacity_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  const T &operator[](uint64_t num) const {
    return bufer_[num];
  }
  T &operator[](uint64_t num) {
    return bufer_[num];
  }
  const T &At(uint64_t num) const {
    if (size_ <= num) {
      throw std::out_of_range("bulat_geniy");
    }
    return bufer_[num];
  }
  T &At(uint64_t num) {
    if (size_ <= num) {
      throw std::out_of_range("bulat_geniy");
    }
    return bufer_[num];
  }
  const T &Front() const {
    return bufer_[0];
  }
  T &Front() {
    return bufer_[0];
  }
  const T &Back() const {
    return bufer_[size_ - 1];
  }
  T &Back() {
    return bufer_[size_ - 1];
  }
  const T *Data() const {
    return bufer_;
  }
  T *Data() {
    return bufer_;
  }
  void Swap(Vector &oth) {
    std::swap(bufer_, oth.bufer_);
    std::swap(capacity_, oth.capacity_);
    std::swap(size_, oth.size_);
  }

  void Resize(uint64_t new_size) {
    if (new_size == size_) {
      return;
    }
    if (new_size < size_) {
      std::destroy(bufer_ + new_size, bufer_ + size_);
      size_ = new_size;
      return;
    }
    if (new_size <= capacity_) {
      std::uninitialized_default_construct(bufer_ + size_, bufer_ + new_size);
      size_ = new_size;
      return;
    }
    if (new_size > capacity_) {
      auto new_bufer = reinterpret_cast<T *>(::operator new(new_size * sizeof(T)));
      try {
        if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
          std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
        } else {
          std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
        }
      } catch (...) {
        ::operator delete(new_bufer);
        throw;
      }
      try {
        std::uninitialized_default_construct(new_bufer + size_, new_bufer + new_size);
        Delite();
        bufer_ = new_bufer;
        size_ = new_size;
        capacity_ = new_size;
      } catch (...) {
        std::destroy(new_bufer, new_bufer + size_);
        ::operator delete(new_bufer);
        throw;
      }
    }
  }
  void Resize(uint64_t new_size, T value) {
    if (new_size == size_) {
      return;
    }
    if (new_size < size_) {
      std::destroy(bufer_ + new_size, bufer_ + size_);
      size_ = new_size;
      return;
    }
    if (new_size <= capacity_) {
      std::uninitialized_fill(bufer_ + size_, bufer_ + new_size, value);
      size_ = new_size;
      return;
    }
    if (new_size > capacity_) {
      auto new_bufer = reinterpret_cast<T *>(::operator new(new_size * sizeof(T)));
      try {
        if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
          std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
        } else {
          std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
        }
      } catch (...) {
        ::operator delete(new_bufer);
        throw;
      }
      try {
        std::uninitialized_fill(new_bufer + size_, new_bufer + new_size, value);
        Delite();
        bufer_ = new_bufer;
        size_ = new_size;
        capacity_ = new_size;
      } catch (...) {
        std::destroy(new_bufer, new_bufer + size_);
        ::operator delete(new_bufer);
        throw;
      }
    }
  }
  void Reserve(uint64_t new_cap) {
    if (capacity_ >= new_cap) {
      return;
    }
    auto new_bufer = reinterpret_cast<T *>(::operator new(new_cap * sizeof(T)));
    try {
      if (size_ > 0) {
        if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
          std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
        } else {
          std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
        }
      }
      capacity_ = new_cap;
      if (size_ > 0) {
        std::destroy(begin(), end());
      }
      if (capacity_ > 0) {
        ::operator delete(bufer_);
      }
      bufer_ = new_bufer;
    } catch (...) {
      ::operator delete(new_bufer);
      throw;
    }
  }
  void ShrinkToFit() {
    if (capacity_ == size_) {
      return;
    }
    if (size_ == 0) {
      Delite();
      return;
    }
    auto new_bufer = reinterpret_cast<T *>(::operator new(size_ * sizeof(T)));
    try {
      if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
        std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
      } else {
        std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
      }
      std::destroy(bufer_, bufer_ + size_);
      if (capacity_ > 0) {
        ::operator delete(bufer_);
      }
      capacity_ = size_;
      bufer_ = new_bufer;
    } catch (...) {
      ::operator delete(new_bufer);
      throw;
    }
  }
  void Clear() {
    if (size_ > 0) {
      std::destroy(begin(), end());
    }
    size_ = 0;
  }
  void PushBack(const T &value) {
    if (size_ + 1 <= capacity_) {
      new (bufer_ + size_) T(value);
      ++size_;
      return;
    }
    auto new_bufer = reinterpret_cast<T *>(::operator new((capacity_ + 1) * 2 * sizeof(T)));
    try {
      if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
        std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
      } else {
        std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
      }
    } catch (...) {
      ::operator delete(new_bufer);
      throw;
    }
    try {
      new (new_bufer + size_) T(value);
      if (size_ > 0) {
        std::destroy(begin(), end());
      }
      if (capacity_ > 0) {
        ::operator delete(bufer_);
      }
      size_ = size_ + 1;
      capacity_ = (capacity_ + 1) * 2;
      bufer_ = new_bufer;
    } catch (...) {
      std::destroy(new_bufer, new_bufer + size_);
      ::operator delete(new_bufer);
      throw;
    }
  }
  void PushBack(T &&value) {
    if (size_ + 1 <= capacity_) {
      new (bufer_ + size_) T(std::move(value));
      ++size_;
      return;
    }
    auto new_bufer = reinterpret_cast<T *>(::operator new((capacity_ + 1) * 2 * sizeof(T)));
    try {
      if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
        std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
      } else {
        std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
      }
    } catch (...) {
      ::operator delete(new_bufer);
      throw;
    }
    try {
      new (new_bufer + size_) T(std::move(value));
      if (size_ > 0) {
        std::destroy(begin(), end());
      }
      if (capacity_ > 0) {
        ::operator delete(bufer_);
      }
      size_ = size_ + 1;
      capacity_ = (capacity_ + 1) * 2;
      bufer_ = new_bufer;
    } catch (...) {
      std::destroy(new_bufer, new_bufer + size_);
      ::operator delete(new_bufer);
      throw;
    }
  }
  template <class... Args>
  void EmplaceBack(Args &&... args) {
    if (size_ + 1 <= capacity_) {
      new (bufer_ + size_) T(std::forward<Args>(args)...);
      ++size_;
      return;
    }
    auto new_bufer = reinterpret_cast<T *>(::operator new((capacity_ + 1) * 2 * sizeof(T)));
    try {
      if constexpr (std::is_nothrow_move_constructible<T>::value) {  // NOLINT
        std::uninitialized_move(bufer_, bufer_ + size_, new_bufer);
      } else {
        std::uninitialized_copy(bufer_, bufer_ + size_, new_bufer);
      }
    } catch (...) {
      ::operator delete(new_bufer);
      throw;
    }
    try {
      new (new_bufer + size_) T(std::forward<Args>(args)...);
      if (size_ > 0) {
        std::destroy(begin(), end());
      }
      if (capacity_ > 0) {
        ::operator delete(bufer_);
      }
      size_ = size_ + 1;
      capacity_ = (capacity_ + 1) * 2;
      bufer_ = new_bufer;
    } catch (...) {
      std::destroy(new_bufer, new_bufer + size_);
      ::operator delete(new_bufer);
      throw;
    }
  }
  void PopBack() {
    if (size_ == 0) {
      return;
    }
    std::destroy(bufer_ + size_ - 1, bufer_ + size_);
    --size_;
  }
  Iterator begin() {  // NOLINT
    return static_cast<T *>(bufer_);
  }
  Iterator end() {  // NOLINT
    return static_cast<T *>(bufer_ + size_);
  }
  ConstIterator begin() const {  // NOLINT
    return static_cast<const T *>(bufer_);
  }
  ConstIterator end() const {  // NOLINT
    return static_cast<const T *>(bufer_ + size_);
  }
  ConstIterator cbegin() const {  // NOLINT
    return static_cast<const T *>(bufer_);
  }
  ConstIterator cend() const {  // NOLINT
    return static_cast<const T *>(bufer_ + size_);
  }
  ReverseIterator rbegin() {  // NOLINT
    return static_cast<std::reverse_iterator<Iterator> >(bufer_ + size_);
  }
  ReverseIterator rend() {  // NOLINT
    return static_cast<std::reverse_iterator<Iterator> >(bufer_);
  }
  ConstReverseIterator rbegin() const {  // NOLINT
    return static_cast<std::reverse_iterator<ConstIterator> >(bufer_ + size_);
  }
  ConstReverseIterator rend() const {  // NOLINT
    return static_cast<std::reverse_iterator<ConstIterator> >(bufer_);
  }
  ConstReverseIterator crbegin() const {  // NOLINT
    return static_cast<std::reverse_iterator<ConstIterator> >(bufer_ + size_);
  }
  ConstReverseIterator crend() const {  // NOLINT
    return static_cast<std::reverse_iterator<ConstIterator> >(bufer_);
  }
};

template <class T>
bool operator<(const Vector<T> &a, const Vector<T> &b) {
  for (uint64_t i = 0; i < std::min(a.Size(), b.Size()); ++i) {
    if (a[i] > b[i]) {
      return false;
    }
    if (a[i] < b[i]) {
      return true;
    }
  }
  return (a.Size() < b.Size());
}

template <class T>
bool operator>(const Vector<T> &a, const Vector<T> &b) {
  for (uint64_t i = 0; i < std::min(a.Size(), b.Size()); ++i) {
    if (a[i] < b[i]) {
      return false;
    }
    if (a[i] > b[i]) {
      return true;
    }
  }
  return (a.Size() > b.Size());
}

template <class T>
bool operator<=(const Vector<T> &a, const Vector<T> &b) {
  return a < b || a == b;
}

template <class T>
bool operator>=(const Vector<T> &a, const Vector<T> &b) {
  return a > b || a == b;
}

template <class T>
bool operator==(const Vector<T> &a, const Vector<T> &b) {
  if (a.Size() != b.Size()) {
    return false;
  }
  for (uint64_t i = 0; i < a.Size(); ++i) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

template <class T>
bool operator!=(const Vector<T> &a, const Vector<T> &b) {
  return !(a == b);
}