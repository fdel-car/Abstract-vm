#include "Parser.hpp"

Parser::Parser(std::vector<std::pair<eTokenType, std::string>> const &vector)
    : _vector(vector), _it(_vector.begin()) {
  try {
    for (; _it != _vector.end(); _it++) {
      if (_it->first == Instruction) {
        if (_it->second == "exit")
          return;
        Parser::Instr instruction = _instructions[_it->second];
        (this->*instruction)();
      }
    }
  } catch (const std::out_of_range &err) {
    std::cout << "Parser error: could not \033[1m" << (_it - 2)->second
              << "\033[0m " << _it->second << ", \033[31;1m" << err.what()
              << "\033[0m." << std::endl;
  } catch (const std::runtime_error &err) {
    std::cout << "Parser error: could not \033[1m" << _it->second << "\033[0m"
              << err.what() << std::endl;
  }
}

Parser::~Parser(void) {
  for (std::list<IOperand const *>::const_iterator it = _list.begin();
       it != _list.end(); it++)
    delete *it;
  _list.clear();
}

void Parser::_push(void) {
  _it++;
  _list.push_front(
      _factory.createOperand(_operandTypes[_it->second], (++_it)->second));
}

void Parser::_pop(void) {
  if (_list.empty())
    throw std::runtime_error(", the stack is empty.");
  delete _list.front();
  _list.pop_front();
}

void Parser::_dump(void) {
  for (std::list<IOperand const *>::const_iterator it = _list.begin();
       it != _list.end(); it++)
    std::cout << (*it)->toString() << std::endl;
}

void Parser::_assert(void) {
  _it++;
  _it++;
}

void Parser::_add(void) {
  if (_list.size() < 2)
    throw std::runtime_error(", the stack has less than two members.");
  std::list<IOperand const *>::iterator it = _list.begin();
  std::advance(it, 1); // Get previous IOperand *
  IOperand const *result = *(*it) + *_list.front();
  _clearReplacedOperands();
  _list.push_front(result);
}

void Parser::_sub(void) {
  if (_list.size() < 2)
    throw std::runtime_error(", the stack has less than two members.");
  std::list<IOperand const *>::iterator it = _list.begin();
  std::advance(it, 1); // Get previous IOperand *
  IOperand const *result = *(*it) - *_list.front();
  _clearReplacedOperands();
  _list.push_front(result);
}

void Parser::_mul(void) {
  if (_list.size() < 2)
    throw std::runtime_error(", the stack has less than two members.");
  std::list<IOperand const *>::iterator it = _list.begin();
  std::advance(it, 1); // Get previous IOperand *
  IOperand const *result = *(*it) * *_list.front();
  _clearReplacedOperands();
  _list.push_front(result);
}

void Parser::_div(void) {}

void Parser::_mod(void) {}

void Parser::_print(void) {
  if ((_list.front())->getType() == Int8) {
    Operand<char> const *operand =
        dynamic_cast<Operand<char> const *>(_list.front());
    if (operand->getValue() < 0)
      throw std::runtime_error(", non ASCII value.");
    std::cout << operand->getValue() << std::endl;
  } else
    throw std::runtime_error(", non Int8 value.");
}

void Parser::_clearReplacedOperands(void) {
  delete _list.front();
  _list.pop_front();
  delete _list.front();
  _list.pop_front();
}

std::map<std::string, Parser::Instr> Parser::_instantiateInstr(void) {
  std::map<std::string, Parser::Instr> map;
  map.insert(std::make_pair("push", &Parser::_push));
  map.insert(std::make_pair("pop", &Parser::_pop));
  map.insert(std::make_pair("dump", &Parser::_dump));
  map.insert(std::make_pair("assert", &Parser::_assert));
  map.insert(std::make_pair("add", &Parser::_add));
  map.insert(std::make_pair("sub", &Parser::_sub));
  map.insert(std::make_pair("mul", &Parser::_mul));
  map.insert(std::make_pair("div", &Parser::_div));
  map.insert(std::make_pair("mod", &Parser::_mod));
  map.insert(std::make_pair("print", &Parser::_print));
  return map;
}

std::map<std::string, eOperandType> Parser::_instantiateOpTypes(void) {
  std::map<std::string, eOperandType> map;
  map.insert(std::make_pair("int8", Int8));
  map.insert(std::make_pair("int16", Int16));
  map.insert(std::make_pair("int32", Int32));
  map.insert(std::make_pair("float", Float));
  map.insert(std::make_pair("double", Double));
  return map;
}

std::map<std::string, Parser::Instr> Parser::_instructions =
    Parser::_instantiateInstr();

std::map<std::string, eOperandType> Parser::_operandTypes =
    Parser::_instantiateOpTypes();