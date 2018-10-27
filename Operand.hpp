#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "IOperand.hpp"
#include <limits>

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

template <typename T> T abs(const T &nbr) { return nbr < 0 ? -nbr : nbr; }

template <typename T>
void mulOverflow(T a, std::string const &lhs, T b, std::string const &rhs) {
  if ((abs(a) > std::numeric_limits<T>::max() / abs(b)) ||
      ((a == -1) && (b == std::numeric_limits<T>::lowest())) ||
      ((b == -1) && (a == std::numeric_limits<T>::lowest())) ||
      (abs(a) < std::numeric_limits<T>::lowest() / abs(b)))
    throw std::overflow_error(
        " " + lhs + " and " + rhs +
        "\033[0m, it would cause an \033[31;1moverflow\033[0m.");
}

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
    double rhsValue = _getRhsValue(rhs);
    if (rhs.getPrecision() > getPrecision()) {
      switch (rhs.getType()) {
      case Int16:
        addOverflow<short>(_value, _str, rhsValue, rhs.toString());
        return new Operand<short>(_value + rhsValue, rhs.getType());
      case Int32:
        addOverflow<int>(_value, _str, rhsValue, rhs.toString());
        return new Operand<int>(_value + rhsValue, rhs.getType());
      case Float:
        addOverflow<float>(_value, _str, rhsValue, rhs.toString());
        return new Operand<float>(_value + rhsValue, rhs.getType());
      case Double:
        addOverflow<double>(_value, _str, rhsValue, rhs.toString());
        return new Operand<double>(_value + rhsValue, rhs.getType());
      default:
        break;
      }
    }
    addOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return new Operand<T>(_value + rhsValue, _type);
  }

  IOperand const *operator-(IOperand const &rhs) const {
    double rhsValue = _getRhsValue(rhs);
    if (rhs.getPrecision() > getPrecision()) {
      switch (rhs.getType()) {
      case Int16:
        subOverflow<short>(_value, _str, rhsValue, rhs.toString());
        return new Operand<short>(_value - rhsValue, rhs.getType());
      case Int32:
        subOverflow<int>(_value, _str, rhsValue, rhs.toString());
        return new Operand<int>(_value - rhsValue, rhs.getType());
      case Float:
        subOverflow<float>(_value, _str, rhsValue, rhs.toString());
        return new Operand<float>(_value - rhsValue, rhs.getType());
      case Double:
        subOverflow<double>(_value, _str, rhsValue, rhs.toString());
        return new Operand<double>(_value - rhsValue, rhs.getType());
      default:
        break;
      }
    }
    subOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return new Operand<T>(_value - rhsValue, _type);
  }

  IOperand const *operator*(IOperand const &rhs) const {
    double rhsValue = _getRhsValue(rhs);
    if (rhs.getPrecision() > getPrecision()) {
      switch (rhs.getType()) {
      case Int16:
        mulOverflow<short>(_value, _str, rhsValue, rhs.toString());
        return new Operand<short>(_value * rhsValue, rhs.getType());
      case Int32:
        mulOverflow<int>(_value, _str, rhsValue, rhs.toString());
        return new Operand<int>(_value * rhsValue, rhs.getType());
      case Float:
        mulOverflow<float>(_value, _str, rhsValue, rhs.toString());
        return new Operand<float>(_value * rhsValue, rhs.getType());
      case Double:
        mulOverflow<double>(_value, _str, rhsValue, rhs.toString());
        return new Operand<double>(_value * rhsValue, rhs.getType());
      default:
        break;
      }
    }
    mulOverflow<T>(_value, _str, rhsValue, rhs.toString());
    return new Operand<T>(_value * rhsValue, _type);
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

#endif