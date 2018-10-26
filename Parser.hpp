#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "OperandFactory.hpp"
#include <list>

class Parser {
  typedef void (Parser::*Instr)(void);

public:
  Parser(std::vector<std::pair<eTokenType, std::string>> const &vector);
  virtual ~Parser(void);

private:
  std::vector<std::pair<eTokenType, std::string>> const &_vector;
  std::list<IOperand const *> _list;
  static std::map<std::string, Instr> _instructions;
  static std::map<std::string, eOperandType> _operandTypes;
  std::vector<std::pair<eTokenType, std::string>>::const_iterator _it;
  OperandFactory _factory;

  Parser(void);
  Parser(Parser const &src);

  Parser &operator=(Parser const &rhs);
  void _push(void);
  void _pop(void);
  void _dump(void);
  void _assert(void);
  void _add(void);
  void _sub(void);
  void _mul(void);
  void _div(void);
  void _mod(void);
  void _print(void);
  void _clearReplacedOperands(void);
  static std::map<std::string, Instr> _instantiateInstr(void);
  static std::map<std::string, eOperandType> _instantiateOpTypes(void);
};

#endif