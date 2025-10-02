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
    this->str = ArrayChar::Copy(str);
  }
  ArrayChar(const ArrayChar* str) {
    size_str = str->GetLength();
    ArrayChar* current = str->Copy();
    this->str = current->ToChar();
    delete current;
  }
  ArrayChar(size_t size) {
    size_str = size;
    str = new char[size + 1];
    str[size] = '\0';
  }
  ~ArrayChar() { delete[] str; }

  size_t GetLength() const { return size_str; }

  char GetValue(uint32_t index) const { return str[index]; }

  void SetValue(uint32_t index, char value) { str[index] = value; }

  ArrayChar* Copy() const {
    size_t size_str = GetLength();

    ArrayChar* new_str = new ArrayChar(size_str);
    for (uint32_t index_str = 0; index_str < size_str; index_str++) {
      new_str->SetValue(index_str, GetValue(index_str));
    }

    return new_str;
  }

  bool equal(const ArrayChar* another) const {
    if (this->GetLength() != another->GetLength()) return false;

    return another->hasPrefix(this) && this->hasPrefix(another);
  }

  bool hasPrefix(const ArrayChar* prefix) const {
    if (this->GetLength() < prefix->GetLength()) {
      return false;
    }

    for (uint32_t index_prefix = 0; index_prefix < prefix->GetLength();
         index_prefix++) {
      if (this->GetValue(index_prefix) != prefix->GetValue(index_prefix)) {
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
    int32_t left = 0;
    while (left < (int32_t)this->GetLength() && GetValue(left) == ' ') {
      ++left;
    }

    int32_t right = (int32_t)this->GetLength() - 1;
    while (right >= 0 && this->GetValue(right) == ' ') {
      --right;
    }

    if (right < left) {
      return new ArrayChar("");
    }

    ArrayChar* trim_str = new ArrayChar(right - left + 1);

    for (uint32_t index_str = left, index_trim_str = 0; (int32_t)index_str <= right;
         index_str++, index_trim_str++) {
      trim_str->SetValue(index_trim_str, this->GetValue(index_str));
    }

    return trim_str;
  }

  static size_t GetLength(const char* kStr) {
    size_t size_str = 0;
    while (kStr[size_str] != '\0') {
      ++size_str;
    }

    return size_str;
  }

  static char* Copy(const char* str) {
    size_t size_str = GetLength(str);

    char* new_str = new char[size_str + 1];
    new_str[size_str] = '\0';
    for (uint32_t index_str = 0; index_str < size_str; index_str++) {
      new_str[index_str] = str[index_str];
    }

    return new_str;
  }
};

class ListMap {
  struct Node{
    ArrayChar* key;
    ArrayChar* value;
    Node* next;
    Node* prev;

    Node(const ArrayChar* kKey, const ArrayChar* kValue){
      key = kKey->Copy();
      value = kValue->Copy();
      next = nullptr;
      prev = nullptr;
    }
    ~Node(){
      delete key;
      delete value;
      if (next){
        delete next;
      }
      delete prev;
    }
  };

  Node* head;
  Node* tail;

  Node* GetNode(const ArrayChar* kKey){
    Node* current_node = head;
    while(current_node->next != nullptr){
      current_node = current_node->next;
      if (kKey->equal(current_node->key)){
        return current_node;
      }
    }

    return nullptr;
  }
  public:

  ListMap(){
    ArrayChar *current_key = new  ArrayChar("#$#");
    ArrayChar *current_value = new ArrayChar("");
    head = new Node(current_key, current_value);
    tail = head;
    tail->prev = tail;

    delete current_key;
    delete current_value;
  }

  void Add(const ArrayChar* kKey, const ArrayChar* kValue) {
    Node* current_node = GetNode(kKey);

    if (current_node != nullptr){
      delete current_node->value;
      current_node->value = kValue->Copy();
    } else{
      Node *new_node = new Node(kKey, kValue);
      new_node->prev = tail;
      tail->next = new_node;
      tail = new_node;
    }
  }

  ArrayChar* Get(const ArrayChar* kKey){
    Node* current_node = GetNode(kKey);
    if (current_node != nullptr) {
      return current_node->value->Copy();
    }
    else{
      return nullptr;
    }
  }
};

struct Parser {
  static ArrayChar* GetCompileKey(size_t count_arguments,
                                  ArrayChar** arguments_arr,
                                  const ArrayChar* kShortPattern,
                                  const ArrayChar* kLongPattern) {
    ArrayChar* result_key = nullptr;

    for (uint32_t index_argument = 0; index_argument < count_arguments;
         index_argument++) {
      const ArrayChar* kCurrentStr = arguments_arr[index_argument];

      if (kCurrentStr->equal(kShortPattern)) {
        if (index_argument + 1 != count_arguments) {
          result_key = arguments_arr[index_argument + 1]->Copy();
        }
        break;
      } else if (kCurrentStr->hasPrefix(kLongPattern)) {
        if (!arguments_arr[index_argument]->equal(kLongPattern)) {
          size_t size_key = arguments_arr[index_argument]->GetLength() -
                            kLongPattern->GetLength();
          result_key = new ArrayChar(size_key);

          for (uint32_t index_str = kLongPattern->GetLength(), index_key = 0;
               index_str < arguments_arr[index_argument]->GetLength();
               index_str++, index_key++) {
            result_key->SetValue(
                index_key, arguments_arr[index_argument]->GetValue(index_str));
          }
        }

        break;
      }
    }

    return result_key;
  }

  static std::pair<bool, const char*> CorrectlyParamKeys(
      const ArrayChar* kPathTemplate, const ArrayChar* kPathData,
      const ArrayChar* kPathOutput) {
    if (!kPathTemplate || !kPathData) {
      if (!kPathTemplate) {
        return {false, "don't find template file"};
      } else {
        return {false, "don't find data file"};
      }
    }
    if (kPathData->GetLength() == 0 || kPathTemplate->GetLength() == 0) {
      if (kPathData->GetLength() == 0) {
        return {false, "don't correct path for data file"};
      } else {
        return {false, "don'y correct path for template file"};
      }
    }
    if (kPathOutput != nullptr && kPathOutput->GetLength() == 0) {
      return {false, "don't correct path for output file"};
    }

    return {true, nullptr};
  }

  static std::pair<bool, const char*> CorrectlyFiles(
      const std::ifstream* kFileTemplate, const std::ifstream* kFileData,
      const std::ofstream* kFileOutPut) {
    if (!kFileTemplate->is_open()) {
      return {false, "don't find template file"};
    }
    if (!kFileData->is_open()) {
      return {false, "don't find data file}"};
    }
    if (kFileOutPut != nullptr && !kFileOutPut) {
      return {false,
              "the file path was passed but don't find output file along this "
              "path"};
    }

    return {true, nullptr};
  }

  static void CollectKeyFromFile(std::ifstream* file, ListMap* set_keys) {
    size_t kSizeBuff = 1024;
    char buff[kSizeBuff];
    ArrayChar line(kSizeBuff);
    for (uint32_t index = 0; index < line.GetLength(); index++) {
      line.SetValue(index, ' ');
    }

    while (file->getline(buff, kSizeBuff)) {
      for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
           index++) {
        line.SetValue(index, buff[index]);
      }

      ArrayChar* line_trim = line.Trim();

      if ((line_trim->GetLength() > 0 && line_trim->GetValue(0) == '#') ||
          (line_trim->GetLength() > 1 &&
           line_trim->GetValue(0) == line_trim->GetValue(1) &&
           line_trim->GetValue(1) == '/')) {
        for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
             index++) {
          line.SetValue(index, ' ');
        }
        delete line_trim;
        continue;
      } else if (line_trim->GetLength() == 0) {
        for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
             index++) {
          line.SetValue(index, ' ');
        }
        delete line_trim;
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

      for (uint32_t index_value = 0; index_value < index_sign_equal;
           index_value++) {
        key.SetValue(index_value, line_trim->GetValue(index_value));
      }
      for (uint32_t index_line_trim = index_sign_equal + 1, index_value = 0;
           index_line_trim < line_trim->GetLength();
           index_line_trim++, index_value++) {
        value.SetValue(index_value, line_trim->GetValue(index_line_trim));
      }

      ArrayChar key_trim = key.Trim();
      ArrayChar value_trim = value.Trim();

      // std::cout << "key: " << key_trim.ToChar() << std::endl;
      // std::cout << "value: " << value_trim.ToChar() << std::endl;
      set_keys->Add(&key_trim, &value_trim);

      for (uint32_t index = 0; index < kSizeBuff && buff[index] != '\0';
           index++) {
        line.SetValue(index, ' ');
      }
      delete line_trim;
    }

    // final processing buffer
    {
      for (uint32_t index = 0; index < line.GetLength(); index++) {
        line.SetValue(index, ' ');
      }
      for (uint32_t index = 0; index < kSizeBuff && index < file->gcount();
           index++) {
        line.SetValue(index, buff[index]);
      }

      ArrayChar* line_trim = line.Trim();
      if (line_trim->GetLength() != 0) {
        if ((line_trim->GetLength() > 0 && line_trim->GetValue(0) == '#') ||
            (line_trim->GetLength() > 1 &&
             line_trim->GetValue(0) == line_trim->GetValue(1) &&
             line_trim->GetValue(1) == '/')) {
          delete line_trim;
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

        for (uint32_t index_value = 0; index_value < index_sign_equal;
             index_value++) {
          key.SetValue(index_value, line_trim->GetValue(index_value));
        }
        for (uint32_t index_line_trim = index_sign_equal + 1, index_value = 0;
             index_line_trim < line_trim->GetLength();
             index_line_trim++, index_value++) {
          value.SetValue(index_value, line_trim->GetValue(index_line_trim));
        }

        set_keys->Add(&key, value.Copy());
      }

      delete line_trim;
    }
  }

  static void ReadAndReplaceKeysAndOutputFile(std::ifstream* file_template,
                                              std::ofstream* file_output,
                                              ListMap* set_keys) {
    struct ReadMachin {
      ListMap* set_keys;
      ArrayChar* buff_for_key;
      enum class State { text, startkey, key, finishkey };
      State mystate;
      size_t size_buff;
      size_t current_size_buff;
      char start_separator_symbol;
      char finish_separator_symbol;
      std::ofstream* file_output;

      ReadMachin(char start_separator_symbol, char finish_separator_symbol,
                 size_t size_buff, ListMap* set_keys, std::ofstream* file_output) {
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
              (symbol >= '0' && symbol <= '9') || symbol == ' ' ||
              symbol == '_') {
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
          if (symbol == start_separator_symbol) {
            mystate = State::key;
          } else {
            PrintChar(start_separator_symbol, file_output);
            PrintChar(symbol, file_output);
            mystate = State::text;
          }

          return true;
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
          } else {
            PrintChar(symbol, file_output);
          }

          return true;
        }
      }
    };

    size_t kSizeBuff = 1024;
    char buff[kSizeBuff];
    ReadMachin machine('{', '}', kSizeBuff, set_keys, file_output);

    while (file_template->read(buff, kSizeBuff)) {
      for (uint32_t index = 0; index < kSizeBuff; index++) {
        bool result = machine.add(buff[index]);

        if (!result) {
          std::cerr << "data file has broblem with key\n";
          exit(4);
        }
      }
    }

    for (uint32_t index = 0; index < file_template->gcount(); index++) {
      bool result = machine.add(buff[index]);

      if (!result) {
        std::cerr << "\ndata file has broblem with key\n";
        exit(4);
      }
    }
  }
};

int main(int argc, char* argv[]) {
  const ArrayChar** kCompilePattern = new const ArrayChar*[6];
  kCompilePattern[0] = new ArrayChar("-t");
  kCompilePattern[1] = new ArrayChar("--template=");
  kCompilePattern[2] = new ArrayChar("-d");
  kCompilePattern[3] = new ArrayChar("--data=");
  kCompilePattern[4] = new ArrayChar("-o");
  kCompilePattern[5] = new ArrayChar("--output=");

  size_t argument_arr_size = argc;
  ArrayChar** argument_arr_str = new ArrayChar*[argc];

  for (uint32_t index = 0; index < argument_arr_size; index++) {
    argument_arr_str[index] = new ArrayChar(argv[index]);
  }

  std::ifstream* file_template = nullptr;
  std::ifstream* file_data = nullptr;
  std::ofstream* file_output = nullptr;

  ArrayChar* path_template =
      Parser::GetCompileKey(argument_arr_size, argument_arr_str,
                            kCompilePattern[0], kCompilePattern[1]);

  ArrayChar* path_data =
      Parser::GetCompileKey(argument_arr_size, argument_arr_str,
                            kCompilePattern[2], kCompilePattern[3]);

  ArrayChar* path_output =
      Parser::GetCompileKey(argument_arr_size, argument_arr_str,
                            kCompilePattern[4], kCompilePattern[5]);

  std::pair<bool, const char*> correctly_path =
      Parser::CorrectlyParamKeys(path_template, path_data, path_output);
  if (!correctly_path.first) {
    std::cerr << "Parsing error: " << correctly_path.second << std::endl;
    return 2;
  }

  file_template = new std::ifstream(path_template->ToChar());
  file_data = new std::ifstream(path_data->ToChar());
  if (path_output) {
    file_output = new std::ofstream(path_output->ToChar());
  }

  std::pair<bool, const char*> correctly_file =
      Parser::CorrectlyFiles(file_template, file_data, file_output);
  if (!correctly_file.first) {
    std::cerr << "Open file error: " << correctly_file.second << std::endl;
    return 3;
  }

  ListMap* set_keys = new ListMap();

  Parser::CollectKeyFromFile(file_data, set_keys);
  Parser::ReadAndReplaceKeysAndOutputFile(file_template, file_output, set_keys);

  delete set_keys;

  for (uint32_t index = 0; index < 6; index++) {
    delete kCompilePattern[index];
  }
  delete[] kCompilePattern;

  for (uint32_t index = 0; index < argument_arr_size; index++) {
    if (argument_arr_str[index]) {
      delete argument_arr_str[index];
    }
  }
  delete[] argument_arr_str;

  file_template->close();
  file_data->close();
  if (file_output) {
    file_output->close();
  }
}
