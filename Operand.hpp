#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "IOperand.hpp"
#include <limits>

template <typename T> class Operand : public IOperand {
public:
  Operand(T value, eOperandType type)
      : _value(value), _type(type), _str(std::to_string(_value)) {}
  Operand(T value, eOperandType type, std::string const &str)
      : _value(value), _type(type), _str(str) {}
  Operand(Operand const &src)
      : _value(src.getValue()), _type(src.getType()), _str(src.toString()) {}
  virtual ~Operand(void) {}

  IOperand const *operator+(IOperand const &rhs) const {
    T rhsValue = _getRhsValue(rhs);
    if ((rhsValue > 0) && (_value > std::numeric_limits<T>::max() - rhsValue))
      throw std::overflow_error(
          " " + this->toString() + " and " + rhs.toString() +
          "\033[0m, it caused an \033[31;1moverflow\033[0m.");
    if ((rhsValue < 0) &&
        (_value < std::numeric_limits<T>::lowest() - rhsValue))
      throw std::underflow_error(
          " " + this->toString() + " and " + rhs.toString() +
          "\033[0m, it caused an \033[31;1munderflow\033[0m.");
    return new Operand<T>(_value + rhsValue, _type);
  }

  T getValue(void) const { return _value; }
  int getPrecision(void) const { return static_cast<int>(_type); }
  eOperandType getType(void) const { return _type; }
  std::string const &toString(void) const { return _str; }

private:
  T _value;
  eOperandType _type;
  std::string const _str;

  Operand(void);
  Operand &operator=(Operand const &rhs);

  T _getRhsValue(IOperand const &rhs) const {
    switch (rhs.getType()) {
    case Int8:
      return dynamic_cast<Operand<char> const &>(rhs).getValue();
    case Int16:
      return dynamic_cast<Operand<short> const &>(rhs).getValue();
    case Int32:
      return dynamic_cast<Operand<int> const &>(rhs).getValue();
    case Float:
      return dynamic_cast<Operand<float> const &>(rhs).getValue();
    case Double:
      return dynamic_cast<Operand<double> const &>(rhs).getValue();
    }
  }
};

#endif