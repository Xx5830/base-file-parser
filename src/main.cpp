#include <cstdint>
#include <fstream>
#include <iostream>

class ArrayChar {
  const char* kStr;
  size_t size_str;

 public:
  ArrayChar(const char* str) {
    size_str = GetLength(str);
    kStr = Copy(kStr);
  }
  ArrayChar(const ArrayChar* str) {
    size_str = str->GetLength();
    kStr = Copy(str->ToChar());
  }
  ~ArrayChar() { delete[] kStr; }

  size_t GetLength() const { return size_str; }

  char GetValue(uint32_t index) const { return kStr[index]; }

  ArrayChar* Copy() const {
    size_t size_str = GetLength();

    char* new_str = new char[size_str];
    for (int index_str = 0; index_str < size_str; index_str++) {
      new_str[index_str] = GetValue(index_str);
    }

    
    ArrayChar* result = new ArrayChar(new_str);
    delete[new_str];
    return result;
  }

  bool equal(const ArrayChar* another) const {
    if (GetLength() != another->GetLength()) return false;

    return another->hasPrefix(this) && hasPrefix(another);
  }

  bool hasPrefix(const ArrayChar* prefix) const {
    if (GetLength() < prefix->GetLength()) {
      return false;
    }

    for (int my_index = 0; my_index < prefix->GetLength(); my_index++) {
      if (GetValue(my_index) != prefix->GetValue(my_index)) {
        return false;
      }
    }

    return true;
  }

  char* ToChar() const { return ArrayChar::Copy(kStr); }

  ArrayChar* Trim(){
    int left = 0;
    while (left < GetLength() && GetValue(left) == ' ') {
      ++left;
    }

    int right = (int)GetLength() - 1;
    if (right >= 0 && GetValue(right) == ' '){
      --right;
    }

    if (right < left){
      return new ArrayChar("");
    }

    char* str_new = new char[right - left + 1];

    for (int my_index = left, index_result; my_index <= right; my_index++, index_result++){
      str_new[index_result] = GetValue(my_index);
    }

    ArrayChar* str_arr = new ArrayChar(str_new);
    delete[] str_new;

    return str_arr;
  }

  static size_t GetLength(const char* kStr) {
    size_t size_str = 0;
    while (kStr != '\0') {
      ++size_str;
    }

    return size_str;
  }

  static char* Copy(const char* str) {
    size_t size_str = GetLength(str);

    char* new_str = new char[size_str];
    for (int index_str = 0; index_str < size_str; index_str++) {
      new_str[index_str] = str[index_str];
    }

    return new_str;
  }
};

class Trie {
  struct Node {
    ArrayChar* key;
    Node* next;

    ~Node() {
      if (next) {
        delete[] next;
      }
    }
  };
  Node* kHead;
  const size_t kSizeNext = 36;

 public:
  Trie() { kHead = new Node(); }

  uint32_t SymbolToIndex(char symbol) const {
    if (symbol >= 'a' && symbol <= 'z') {
      return symbol - 'a';
    } else if (symbol >= '0' && symbol <= '9') {
      return 26 + (symbol - '0');
    } else {
      return 36;
    }
  }

  Node* GetNode(const ArrayChar* kStr) {
    Node* my_node = kHead;
    size_t size_str = kStr->GetLength();

    for (int i = 0; i < size_str; i++) {
      char current_symbol = kStr->GetValue(i);
      uint32_t current_symbol_index = SymbolToIndex(current_symbol);

      if (!my_node->next) {
        my_node->next = new Node[kSizeNext];
      }

      my_node = &my_node->next[current_symbol_index];
    }

    return my_node;
  }

  void add(const ArrayChar* kStr, const ArrayChar* kKey) {
    Node* my_node = GetNode(kStr);

    if (my_node->key != nullptr) {
      delete my_node->key;
    }

    my_node->key = kKey->Copy();
  }

  ArrayChar* get(const ArrayChar* kStr) {
    Node* my_node = GetNode(kStr);

    return my_node->key->Copy();
  }
};

struct Parser {
  static ArrayChar* GetCompileKey(int size_array_str,
                                  const ArrayChar** kArrayStr,
                                  const ArrayChar* kShortPattern,
                                  const ArrayChar* kLongPattern) {
    ArrayChar* result_key = nullptr;

    for (int index_array_str = 0; index_array_str < size_array_str; index_array_str++) {
      const ArrayChar* kCurrentStr = kArrayStr[index_array_str];

      if (kCurrentStr->equal(kShortPattern)) {
        result_key = kArrayStr[index_array_str + 1]->Copy();

        break;
      } else if (kCurrentStr->hasPrefix(kLongPattern)) {
        if (!kArrayStr[index_array_str]->equal(kLongPattern)) {
          size_t size_key =
              kArrayStr[index_array_str]->GetLength() - kLongPattern->GetLength();
          char* key = new char(size_key);

          for (int index_str = kLongPattern->GetLength(), index_key = 0;
               index_str < kArrayStr[index_array_str]->GetLength(); index_str++, index_key++) {
            key[index_key] = kArrayStr[index_array_str]->GetValue(index_str);
          }
        }

        break;
      }
    }

    return result_key;
  }

  static bool CorrectlyParamKeys(const ArrayChar* kPathTemplate,
                                 const ArrayChar* kPathData,
                                 const ArrayChar* kPathOutput) {
    if (!kPathTemplate || !kPathData) {
      return false;
    }
    if (kPathData->GetLength() == 0 || kPathData->GetLength() == 0){
      return false;
    }
    if (kPathData && kPathOutput->GetLength() == 0) {
      return false;
    }

    return true;
  }

  static bool CorrectlyFiles(const std::ifstream* kFileTemplate, const std::ifstream* kFileData, const std::ofstream* kFileOutPut){
    if (!kFileTemplate->is_open()){
      return false;
    }
    if (!kFileData->is_open()){
      return false;
    }
    if (kFileOutPut != nullptr && !kFileOutPut){
      return false;
    }

    return true;
  }

  void CollectKeyFromFile(std::ifstream *file, Trie* set_keys) {
    char current_line;
    while ((*file) >> current_line){
      
    }


  }
};

int main(int argc, char* argv[]) {
  const auto PrintChar = [](char symbol, std::ofstream* kFileOut) {
    if (kFileOut) {
      (*kFileOut) << symbol;
    } else {
      std::cout << symbol;
    }
  };

  static const std::pair<const ArrayChar, const ArrayChar> kCompilePattern[]{
      {ArrayChar("-t"), ArrayChar("--template=")},
      {ArrayChar("-d"), ArrayChar("--data=")},
      {ArrayChar("-o"), ArrayChar("--output=")}};

  const ArrayChar** kArrayStr = new ArrayChar*[argc];
  size_t size_array_str = argc;

  std::ifstream* file_template = nullptr;
  std::ifstream* file_data = nullptr;
  std::ofstream* file_output = nullptr;

  ArrayChar* path_template = Parser::GetCompileKey(size_array_str, kArrayStr,
                                                   &kCompilePattern[0].first,
                                                   &kCompilePattern[0].second);

  ArrayChar* path_data = Parser::GetCompileKey(size_array_str, kArrayStr,
                                               &kCompilePattern[1].first,
                                               &kCompilePattern[1].second);

  ArrayChar* path_output = Parser::GetCompileKey(size_array_str, kArrayStr,
                                                 &kCompilePattern[2].first,
                                                 &kCompilePattern[2].second);

  if (!Parser::CorrectlyParamKeys(path_template, path_data, path_output)) {
    std::cerr << "Parsing error" << std::endl;
    return 2;
  }

  file_template = new std::ifstream(path_template->ToChar());
  file_data = new std::ifstream(path_data->ToChar());
  if (path_output) {
    file_output = new std::ofstream(path_output->ToChar());
  }

  if (!Parser::CorrectlyFiles(file_template, file_data, file_output)){
    std::cerr << "Open file error" << std::endl;;
    return 3;
  }

  
}
