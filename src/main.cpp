#include <cstdint>
#include <iostream>

namespace cont {
template <typename T>
class Iterator {
  virtual bool HaveNext();
  virtual void Next();
  virtual T get();
};

template <typename T>
class Collection {
 public:
  uint32_t size;

  Collection(uint32_t _size) { size = _size; }
  virtual Iterator<T> begin() = 0;

  virtual T get(uint32_t index) = 0;

  virtual void set(uint32_t index, T value) = 0;

  virtual void insert(uint32_t index) = 0;

  virtual void erase(uint32_t index) = 0;
  virtual uint32_t length() {return size;};
};

template <typename T>
class List : public Collection<T> {
 private:
  T* arr;
  uint32_t capacity;
  const uint32_t kCapacityMultiplier = 2;

 public:
  List(uint32_t _size, T element = T()) : Collection<T>(_size) {
    capacity = this->size * kCapacityMultiplier;
    arr = new T[capacity];

    for (size_t i = 0; i < capacity; i++) {
      arr[i] = element;
    }
  }

  T get(uint32_t index) {
    return arr[index];
  }
};

};  // namespace cont

int main(int, char**) { cont::List<int> mas(5, 1); }
