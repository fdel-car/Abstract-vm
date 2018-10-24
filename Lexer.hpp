#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>

std::string const InstrList[] = {"push", "pop", "dump", "assert", "add", "sub",
                                 "mul",  "div", "mod",  "print",  "exit"};

std::string const OperandList[] = {"int8", "int16", "int32", "float", "double"};

enum eTokenType { Instruction, OperandType, Value };

template <typename T>
bool isInArray(T elem, const T *array, size_t length) {
  for (size_t i = 0; i < length; i++)
    if (elem == array[i]) return true;
  return false;
}

class Lexer {
 public:
  Lexer(std::string const &filename);
  virtual ~Lexer(void);

  std::vector<std::pair<eTokenType, std::string>> const getVector(void) const;
  std::string const getFilename(void) const;
  bool isValid(void) const;

 private:
  std::vector<std::pair<eTokenType, std::string>> _vector;
  std::string const _filename;
  bool _isValid;

  Lexer(void);
  Lexer(Lexer const &src);

  Lexer &operator=(Lexer const &rhs);
};

#endif