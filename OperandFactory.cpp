#include "OperandFactory.hpp"
#include <limits>

OperandFactory::OperandFactory(void) {}

OperandFactory::~OperandFactory(void) {}

IOperand const *OperandFactory::_createInt8(std::string const &value) const {
  int tmp = std::stoi(value);
  if (std::numeric_limits<char>::min() > tmp ||
      std::numeric_limits<char>::max() < tmp) {
    throw std::out_of_range("stoc: out of range");
  }
  return new Operand<char>(static_cast<char>(tmp), Int8, value);
}

IOperand const *OperandFactory::_createInt16(std::string const &value) const {
  int tmp = std::stoi(value);
  if (std::numeric_limits<short>::min() > tmp ||
      std::numeric_limits<short>::max() < tmp) {
    throw std::out_of_range("stos: out of range");
  }
  return new Operand<short>(static_cast<short>(tmp), Int16, value);
}

IOperand const *OperandFactory::_createInt32(std::string const &value) const {
  return new Operand<int>(std::stoi(value), Int32, value);
}

IOperand const *OperandFactory::_createFloat(std::string const &value) const {
  return new Operand<float>(std::stof(value), Float, value);
}

IOperand const *OperandFactory::_createDouble(std::string const &value) const {
  return new Operand<double>(std::stod(value), Double, value);
}

IOperand const *OperandFactory::createOperand(eOperandType type,
                                              std::string const &value) const {
  Factory factory = _factories[type];
  return (this->*factory)(value);
}

std::map<eOperandType, OperandFactory::Factory>
OperandFactory::_instantiateFactories(void) {
  std::map<eOperandType, OperandFactory::Factory> map;
  map.insert(std::make_pair(Int8, &OperandFactory::_createInt8));
  map.insert(std::make_pair(Int16, &OperandFactory::_createInt16));
  map.insert(std::make_pair(Int32, &OperandFactory::_createInt32));
  map.insert(std::make_pair(Float, &OperandFactory::_createFloat));
  map.insert(std::make_pair(Double, &OperandFactory::_createDouble));
  return map;
}

std::map<eOperandType, OperandFactory::Factory> OperandFactory::_factories =
    OperandFactory::_instantiateFactories();
