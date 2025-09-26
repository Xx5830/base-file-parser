#include <cstdint>
#include <iostream>

class ArrayChar {
  const char* kStr;
  size_t sizeStr;

 public:
  ArrayChar(const char* str) {
    sizeStr = GetLength(str);
    kStr = Copy(kStr);
  }
  ~ArrayChar(){
    delete[] kStr;
  }

  size_t GetLength() const { return sizeStr; }

  char GetValue(uint32_t index) const { return kStr[index]; }

  ArrayChar* Copy() const {
    size_t sizeStr = GetLength();

    char* newStr = new char[sizeStr];
    for (int i = 0; i < sizeStr; i++) {
      newStr[i] = GetValue(i);
    }

    return new ArrayChar(newStr);
  }

  static size_t GetLength(const char* kStr) {
    size_t sizeStr = 0;
    while (kStr != '\0') {
      ++sizeStr;
    }

    return sizeStr;
  }

  static char* Copy(const char* str) {
    size_t sizeStr = GetLength(str);

    char* newStr = new char[sizeStr];
    for (int i = 0; i < sizeStr; i++) {
      newStr[i] = str[i];
    }

    return newStr;
  }
};

class Trie {
  struct Node {
    ArrayChar* key;
    Node* next;
  };
  Node* kHead;
  size_t kSizeNext = 36;

  public:
  Trie() { kHead = new Node(); }

  uint32_t CharToIndex(char cv) const {
    if (cv >= 'a' && cv <= 'z') {
        return cv - 'a';
    }
    else if (cv >= '0' && cv <= '9'){
      return 26 + (cv - '0');
    }
    else{
      return 36;
    }
  }

  Node *GetNode(const ArrayChar *kStr){
    Node* myNode = kHead;
    size_t sizeStr = kStr->GetLength();

    for (int i = 0; i < sizeStr; i++){
      char currentValue = kStr->GetValue(i);

      if (!myNode->next) {
        myNode->next = new Node[kSizeNext];
      }

      myNode = &myNode->next[currentValue];
    }

    return myNode;
  }

  void add(const ArrayChar *kStr, const ArrayChar *kKey) {
    Node *myNode = GetNode(kStr);

    if (myNode->key != nullptr) {
      delete myNode->key;
    }

    myNode->key = kKey->Copy();
  }

  ArrayChar* get(const ArrayChar *kStr) const {
     Node *myNode = GetNode(kStr);

     return myNode->key->Copy();
  }
};

int main(int, char**) {}
