#include <cstdint>
#include <fstream>
#include <iostream>

void PrintChar(char symbol, std::ofstream* kFileOut) {
  if (kFileOut) {
    (*kFileOut) << symbol;
  } else {
    std::cout << symbol;
  }
};

class ArrayChar {
  char* str;
  size_t size_str;

 public:
  ArrayChar(const char* str) {
    size_str = GetLength(str);
    str = Copy(str);
  }
  ArrayChar(const ArrayChar* str) {
    size_str = str->GetLength();
    str = Copy(str->ToChar());
  }
  ArrayChar(size_t size) {
    size_str = size;
    str = new char[size];
  }
  ~ArrayChar() { delete[] str; }

  size_t GetLength() const { return size_str; }

  char GetValue(uint32_t index) const { return str[index]; }

  void SetValue(uint32_t index, char value) { str[index] = value; }

  ArrayChar* Copy() const {
    size_t size_str = GetLength();

    char* new_str = new char[size_str];
    for (int index_str = 0; index_str < size_str; index_str++) {
      new_str[index_str] = GetValue(index_str);
    }

    ArrayChar* result = new ArrayChar(new_str);
    delete[] new_str;
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

  char* ToChar() const { return ArrayChar::Copy(str); }

  /* std::pair<size_t, ArrayChar**> Parse(char separator) const {
    uint32_t count_arr = 1;
    for (uint32_t index_str = 0; index_str < GetLength(); index_str++){
      if (GetValue(index_str) == separator){
        ++count_arr;
      }
    }

    ArrayChar** arrays_result = new ArrayChar*[count_arr];
    char* str_current =  ;
    for (uint32_t index_str = 0; index_str < GetLength(); index_str++){

    }
  } */

  ArrayChar* Trim() {
    int left = 0;
    while (left < GetLength() && GetValue(left) == ' ') {
      ++left;
    }

    int right = (int)GetLength() - 1;
    if (right >= 0 && GetValue(right) == ' ') {
      --right;
    }

    if (right < left) {
      return new ArrayChar("");
    }

    char* str_new = new char[right - left + 1];

    for (int my_index = left, index_result; my_index <= right;
         my_index++, index_result++) {
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
  Node* head;
  const size_t kSizeNext = 36;

 public:
  Trie() { head = new Node(); }

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
    Node* my_node = head;
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

  void Add(const ArrayChar* kStr, const ArrayChar* kKey) {
    Node* my_node = GetNode(kStr);

    if (my_node->key != nullptr) {
      delete my_node->key;
    }

    my_node->key = kKey->Copy();
  }

  ArrayChar* Get(const ArrayChar* kStr) {
    Node* my_node = GetNode(kStr);

    if (my_node->key == nullptr) {
      return nullptr;
    }

    return my_node->key->Copy();
  }
};

struct Parser {
  static ArrayChar* GetCompileKey(int size_array_str,
                                  const ArrayChar** kArrayStr,
                                  const ArrayChar* kShortPattern,
                                  const ArrayChar* kLongPattern) {
    ArrayChar* result_key = nullptr;

    for (int index_array_str = 0; index_array_str < size_array_str;
         index_array_str++) {
      const ArrayChar* kCurrentStr = kArrayStr[index_array_str];

      if (kCurrentStr->equal(kShortPattern)) {
        result_key = kArrayStr[index_array_str + 1]->Copy();

        break;
      } else if (kCurrentStr->hasPrefix(kLongPattern)) {
        if (!kArrayStr[index_array_str]->equal(kLongPattern)) {
          size_t size_key = kArrayStr[index_array_str]->GetLength() -
                            kLongPattern->GetLength();
          char* key = new char(size_key);

          for (int index_str = kLongPattern->GetLength(), index_key = 0;
               index_str < kArrayStr[index_array_str]->GetLength();
               index_str++, index_key++) {
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
    if (kPathData->GetLength() == 0 || kPathData->GetLength() == 0) {
      return false;
    }
    if (kPathData && kPathOutput->GetLength() == 0) {
      return false;
    }

    return true;
  }

  static bool CorrectlyFiles(const std::ifstream* kFileTemplate,
                             const std::ifstream* kFileData,
                             const std::ofstream* kFileOutPut) {
    if (!kFileTemplate->is_open()) {
      return false;
    }
    if (!kFileData->is_open()) {
      return false;
    }
    if (kFileOutPut != nullptr && !kFileOutPut) {
      return false;
    }

    return true;
  }

  void CollectKeyFromFile(std::ifstream* file, Trie* set_keys) {
    size_t kSizeBuff = 256;
    char buff[kSizeBuff];
    ArrayChar line(kSizeBuff);

    while (file->getline(buff, kSizeBuff)) {
      for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
           index++) {
        line.SetValue(index, buff[index]);
      }

      ArrayChar* line_trim = line.Trim();

      if ((line_trim->GetLength() > 0 && line_trim->GetValue(0) == '#') ||
          (line_trim->GetLength() > 1 &&
           line_trim->GetValue(0) == line_trim->GetValue(1) &&
           line_trim->GetValue(2) == '/')) {
        continue;
      } else if (line_trim->GetLength() < 3) {
        std::cerr << "error \"key=value\" format\n";
        exit(5);
      }

      uint32_t index_sign_equal = -1;

      for (uint32_t index = 0; index < line_trim->GetLength(); index++) {
        if (line_trim->GetValue(index) == '=') {
          index_sign_equal = index;
          break;
        }
      }

      if (index_sign_equal <= 0 ||
          index_sign_equal + 1 == line_trim->GetLength()) {
        std::cerr << "error \"key=value\" format\n";
        exit(5);
      }

      ArrayChar key(index_sign_equal);
      ArrayChar value(line_trim->GetLength() - index_sign_equal - 1);

      for (uint32_t index = 0; index < index_sign_equal; index++) {
        key.SetValue(index, line_trim->GetValue(index));
      }
      for (uint32_t index = index_sign_equal + 1;
           index < line_trim->GetLength(); index++) {
        value.SetValue(index, line_trim->GetValue(index));
      }

      set_keys->add(&key, value.Copy());

      delete line_trim;
    }

    // final processing buffer
    {
      for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
           index++) {
        line.SetValue(index, buff[index]);
      }

      ArrayChar* line_trim = line.Trim();

      if ((line_trim->GetLength() > 0 && line_trim->GetValue(0) == '#') ||
          (line_trim->GetLength() > 1 &&
           line_trim->GetValue(0) == line_trim->GetValue(1) &&
           line_trim->GetValue(2) == '/')) {
        return;
      } else if (line_trim->GetLength() < 3) {
        std::cerr << "error \"key=value\" format\n";
        exit(5);
      }

      uint32_t index_sign_equal = -1;

      for (uint32_t index = 0; index < line_trim->GetLength(); index++) {
        if (line_trim->GetValue(index) == '=') {
          index_sign_equal = index;
          break;
        }
      }

      if (index_sign_equal <= 0 ||
          index_sign_equal + 1 == line_trim->GetLength()) {
        std::cerr << "error \"key=value\" format\n";
        exit(5);
      }

      ArrayChar key(index_sign_equal);
      ArrayChar value(line_trim->GetLength() - index_sign_equal - 1);

      for (uint32_t index = 0; index < index_sign_equal; index++) {
        key.SetValue(index, line_trim->GetValue(index));
      }
      for (uint32_t index = index_sign_equal + 1;
           index < line_trim->GetLength(); index++) {
        value.SetValue(index, line_trim->GetValue(index));
      }

      set_keys->add(&key, value.Copy());

      delete line_trim;
    }
  }

  void ReadAndReplaceKeysAndOutputFile(std::ifstream* file_template,
                                       std::ofstream* file_output,
                                       Trie* set_keys) {
    struct ReadMachin {
      Trie* set_keys;
      ArrayChar* buff_for_key;
      enum class State { text, startkey, key, finishkey };
      State mystate;
      size_t size_buff;
      size_t current_size_buff;
      char start_separator_symbol;
      char finish_separator_symbol;
      std::ofstream* file_output;

      ReadMachin(char start_separator_symbol, char finish_separator_symbol,
                 size_t size_buff, Trie* set_keys, std::ofstream* file_output) {
        this->size_buff = size_buff;
        buff_for_key = new ArrayChar(this->size_buff);
        mystate = State::text;
        this->start_separator_symbol = start_separator_symbol;
        this->finish_separator_symbol = finish_separator_symbol;
        current_size_buff = 0;
        this->set_keys = set_keys;
        this->file_output = file_output;
      }
      ~ReadMachin() { delete buff_for_key; }

      bool add(char symbol) {
        if (mystate == State::key) {
          if ((symbol >= 'a' && symbol <= 'z') ||
              (symbol >= 'A' && symbol <= 'Z') ||
              (symbol >= '0' && symbol <= '9') || symbol == ' ') {
            buff_for_key->SetValue(current_size_buff, symbol);
            buff_for_key->SetValue(++current_size_buff, '\0');
            return true;
          } else if (symbol == finish_separator_symbol) {
            mystate = State::finishkey;
            return true;
          } else {
            return false;
          }
        } else if (mystate == State::startkey) {
          if (symbol != start_separator_symbol) {
            PrintChar(start_separator_symbol, file_output);
            PrintChar(symbol, file_output);
            mystate = State::text;
          }
        } else if (mystate == State::finishkey) {
          if (symbol == finish_separator_symbol) {
            ArrayChar key(current_size_buff);
            for (uint32_t index = 0; index < current_size_buff; index++) {
              key.SetValue(index, buff_for_key->GetValue(index));
            }

            ArrayChar* key_trim = key.Trim();

            ArrayChar* value = set_keys->Get(key_trim);

            if (value == nullptr) {
              return false;
            }

            for (uint32_t index = 0; index < value->GetLength(); index++) {
              PrintChar(value->GetValue(index), file_output);
            }

            current_size_buff = 0;
            mystate = State::text;
            delete key_trim;
            return true;
          } else {
            return false;
          }
        } else {
          if (symbol == start_separator_symbol) {
            mystate = State::startkey;
          }
        }
      }
    };

    size_t kSizeBuff = 256;
    char buff[kSizeBuff];
    ReadMachin mashine('{', '}', kSizeBuff, set_keys, file_output);

    while (file_template->read(buff, kSizeBuff)) {
      for (uint32_t index = 0; index < kSizeBuff; index++){
        bool result = mashine.add(buff[index]);

        if (!result){
          std::cerr << "data file has broblem with key\n";
          exit(4);
        }
      }
    }

    for (uint32_t index = 0; buff[index] != '\0'; index++){
      bool result = mashine.add(buff[index]);

        if (!result){
          std::cerr << "data file has broblem with key\n";
          exit(4);
        }
    }
  }
};

// Это худший код что я писал, спасибо за то что запретили все что делает кодера
// счастливым
int main(int argc, char* argv[]) {
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

  if (!Parser::CorrectlyFiles(file_template, file_data, file_output)) {
    std::cerr << "Open file error" << std::endl;
    ;
    return 3;
  }
}
