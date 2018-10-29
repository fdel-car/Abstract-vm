#ifndef OPERANDFACTORY_HPP
#define OPERANDFACTORY_HPP

#include <map>
#include "IOperand.hpp"

class OperandFactory {
  typedef IOperand const *(OperandFactory::*Factory)(
      std::string const &value) const;

 public:
  OperandFactory(void);
  virtual ~OperandFactory(void);

  IOperand const *createOperand(eOperandType type,
                                std::string const &value) const;

 private:
  static std::map<eOperandType, Factory> _factories;

  IOperand const *_createInt8(std::string const &value) const;
  IOperand const *_createInt16(std::string const &value) const;
  IOperand const *_createInt32(std::string const &value) const;
  IOperand const *_createFloat(std::string const &value) const;
  IOperand const *_createDouble(std::string const &value) const;
  static std::map<eOperandType, Factory> _instantiateFactories(void);

  OperandFactory(OperandFactory const &src);

  OperandFactory &operator=(OperandFactory const &rhs);
};

#endif