#include <cstdint>
#include <iostream>

namespace contain {
template <typename T>
class Collection;

template <typename T>
class Iterator {
 public:
  virtual bool HasNext() = 0;
  virtual void Next() = 0;
  virtual const T* Get() = 0;
};

// Collection interface for iterable
template <typename T>
class Collection {
 public:
  virtual const T* Get(uint32_t index) = 0;
  virtual void Set(uint32_t index, const T& value) = 0;
  virtual Iterator<T>* GetIterator() = 0;
  virtual uint32_t Length() = 0;
};

// Dynamic list with fix length
template <typename T>
class ArrayList : public Collection<T> {
 private:
  T* arr;
  uint32_t size;

  template <typename U>
  class MyIterator : public Iterator<U> {
   private:
    const ArrayList<T>* kArrayList;
    uint32_t index;

   public:
    MyIterator(const ArrayList<T>*& kArrayList) {
      this->kArrayList = &kArrayList;
    }

    bool HasNext() { return index + 1 < kArrayList->Length(); }
    void Next() { ++index; }
    const T* Get() { return kArrayList->Get(index); }
    ~MyIterator() { delete index; }
  };

 public:
  ArrayList(uint32_t _size) {
    size = _size;

    arr = new T[Length()];
  }
  ~ArrayList() { delete[] arr; }

  const T* Get(uint32_t index) {
    if (index >= Length()) {
      // need return error and log info
    }

    return &arr[index];
  }

  void Set(uint32_t index, const T& value) {
    if (index >= Length()) {
      // need return error and log info
    }

    arr[index] = value;
  }

  Iterator<T>* GetIterator() { return new MyIterator<T>(this); }

  uint32_t Length() { return size; }
};

/* template <typename T>
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

  template <typename T>
  class Vector : public : List{

  };
}; */

};  // namespace contain

int main(int, char**) {
  contain::ArrayList<int> mas(5);

  for (int i = 0; i < 5; i++) {
    mas.Set(i, i + 1);
    std::cout << (*mas.Get(i)) << "\n";
  }
}
