#ifndef OPERAND_HPP
#define OPERAND_HPP

#include <math.h>
#include <limits>
#include <regex>
#include "OperandFactory.hpp"

// // Type list, this could have been a solution if I didn't use a template
// template <eOperandType eType>
// struct typeList;

// template <>
// struct typeList<Int8> {
//   typedef char type;
// };
// template <>
// struct typeList<Int16> {
//   typedef short type;
// };
// template <>
// struct typeList<Int32> {
//   typedef int type;
// };
// template <>
// struct typeList<Float> {
//   typedef float type;
// };
// template <>
// struct typeList<Double> {
//   typedef double type;
// };

template <typename T>
void addOverflow(T a, std::string const &lhs, T b, std::string const &rhs) {
  if (((b > 0) && (a > std::numeric_limits<T>::max() - b)) ||
      ((b < 0) && (a < std::numeric_limits<T>::lowest() - b)))
    throw std::overflow_error(
        " " + lhs + " and " + rhs +
        "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
}

template <typename T>
void subOverflow(T a, std::string const &lhs, T b, std::string const &rhs) {
  if (((b < 0) && (a > std::numeric_limits<T>::max() + b)) ||
      ((b > 0) && (a < std::numeric_limits<T>::lowest() + b)))
    throw std::overflow_error(
        " " + lhs + " and " + rhs +
        "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
}

template <typename T>
T abs(const T &nbr) {
  return nbr < 0 ? -nbr : nbr;
}

template <typename T>
void mulOverflow(T a, std::string const &lhs, T b, std::string const &rhs) {
  if ((abs(a) > std::numeric_limits<T>::max() / abs(b)) ||
      ((a == -1) && (b == std::numeric_limits<T>::lowest())) ||
      ((b == -1) && (a == std::numeric_limits<T>::lowest())))
    throw std::overflow_error(
        " " + lhs + " and " + rhs +
        "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
}

template <typename T>
void divOverflow(T a, std::string const &lhs, T b, std::string const &rhs) {
  if (a == std::numeric_limits<T>::lowest() && b == -1)
    throw std::overflow_error(
        " " + lhs + " and " + rhs +
        "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
  if (b < 1.0 && b > 0.0) {
    if ((abs(a) > std::numeric_limits<T>::max() / abs(1 / b)))
      throw std::overflow_error(
          " " + lhs + " and " + rhs +
          "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
  }
}

class DivisionByZero : public std::runtime_error {
 public:
  DivisionByZero() : std::runtime_error("division by zero.") {}
};

template <typename T>
class Operand : public IOperand {
 public:
  Operand(T value, eOperandType type)
      : _value(value),
        _type(type),
        _str(std::regex_replace(std::to_string(_value), _regex, "$1")) {}
  Operand(Operand const &src)
      : _value(src.getValue()), _type(src.getType()), _str(src.toString()) {}
  virtual ~Operand(void) {}

  IOperand const *operator+(IOperand const &rhs) const {
    if (rhs.getPrecision() > getPrecision()) return rhs + *this;
    T rhsValue = _getRhsValue(rhs);
    addOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return _factory.createOperand(_type, std::to_string(_value + rhsValue));
  }

  IOperand const *operator-(IOperand const &rhs) const {
    double rhsValue = _getRhsValue(rhs);
    if (rhs.getPrecision() > getPrecision()) {
      switch (rhs.getType()) {
        case Int8:
          break;
        case Int16:
          subOverflow<short>(_value, _str, rhsValue, rhs.toString());
        case Int32:
          subOverflow<int>(_value, _str, rhsValue, rhs.toString());
        case Float:
          subOverflow<float>(_value, _str, rhsValue, rhs.toString());
        case Double:
          subOverflow<double>(_value, _str, rhsValue, rhs.toString());
      }
    } else
      subOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return _factory.createOperand(
        rhs.getPrecision() > getPrecision() ? rhs.getType() : _type,
        std::to_string(_value - rhsValue));
  }

  IOperand const *operator*(IOperand const &rhs) const {
    if (rhs.getPrecision() > getPrecision()) return rhs * *this;
    T rhsValue = _getRhsValue(rhs);
    mulOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return _factory.createOperand(_type, std::to_string(_value * rhsValue));
  }

  IOperand const *operator/(IOperand const &rhs) const {
    double rhsValue = _getRhsValue(rhs);
    if (rhsValue == 0) throw DivisionByZero();
    if (rhs.getPrecision() > getPrecision()) {
      switch (rhs.getType()) {
        case Int8:
          break;
        case Int16:
          divOverflow<short>(_value, _str, rhsValue, rhs.toString());
        case Int32:
          divOverflow<int>(_value, _str, rhsValue, rhs.toString());
        case Float:
          divOverflow<float>(_value, _str, rhsValue, rhs.toString());
        case Double:
          divOverflow<double>(_value, _str, rhsValue, rhs.toString());
      }
    } else
      divOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return _factory.createOperand(
        rhs.getPrecision() > getPrecision() ? rhs.getType() : _type,
        std::to_string(_value / rhsValue));
  }

  IOperand const *operator%(IOperand const &rhs) const {
    double rhsValue = _getRhsValue(rhs);
    if (rhsValue == 0) throw DivisionByZero();
    return _factory.createOperand(
        rhs.getPrecision() > getPrecision() ? rhs.getType() : _type,
        std::to_string(fmod(_value, rhsValue)));
  }

  T getValue(void) const { return _value; }
  int getPrecision(void) const { return static_cast<int>(_type); }
  eOperandType getType(void) const { return _type; }
  std::string const &toString(void) const { return _str; }

 private:
  T _value;
  eOperandType const _type;
  std::string const _str;
  static std::regex const _regex;
  OperandFactory _factory;

  Operand(void);
  Operand &operator=(Operand const &rhs);

  double _getRhsValue(IOperand const &rhs) const {
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

template <typename T>
std::regex const Operand<T>::_regex = std::regex("^(-?\\d+\\.\\d*?\\d)0+$");

#endif