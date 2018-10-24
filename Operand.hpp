#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "IOperand.hpp"

template <typename T>
class Operand : public IOperand {
 public:
  Operand(T value, eOperandType type) : _value(value), _type(type) {}
  Operand(Operand const &src) : _value(src.getValue()), _type(src.getType()) {}
  virtual ~Operand(void) {}

  Operand &operator=(Operand const &rhs) {
    return *this;
    (void)rhs;
  }
  IOperand const *operator+(IOperand const &rhs) const {}

  T getValue(void) const { return _value; }
  int getPrecision(void) const { return static_cast<int>(_type); }
  eOperandType getType(void) const { return _type; }

 private:
  T _value;
  eOperandType _type;
};

#endif