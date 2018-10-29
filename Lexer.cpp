#include "Lexer.hpp"
#include <algorithm>
#include <cctype>
// #include <fstream>
#include <locale>
#include <regex>
#include <sstream>

static inline void ltrim(std::string &str) {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
              return !std::isspace(ch);
            }));
}
static inline void rtrim(std::string &str) {
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](int ch) { return !std::isspace(ch); })
                .base(),
            str.end());
}
static inline void trim(std::string &str) {
  ltrim(str);
  rtrim(str);
}
static size_t InstrListLength = sizeof(InstrList) / sizeof(InstrList[0]);
static size_t OperandListLength = sizeof(OperandList) / sizeof(OperandList[0]);

Lexer::Lexer(std::string const &data) : _isValid(true) {
  std::istringstream iss(data);
  std::string line;
  size_t lineIndex = 0;

  while (getline(iss, line)) {
    lineIndex++;

    try {
      // Remove everything after the semicolon
      size_t semicolon = line.find(';');
      line = line.substr(0, semicolon);
      trim(line);

      if (!line.empty()) {
        // Create Instruction token if there is no syntax error
        size_t space = line.find(' ');
        std::string instr = line.substr(0, space);
        if (isInArray<std::string>(instr, InstrList, InstrListLength))
          _vector.push_back(std::make_pair(Instruction, instr));
        else
          throw std::logic_error("invalid instruction => \033[31;1m" + instr +
                                 "\033[0m");

        if (instr == "assert" || instr == "push") {
          // Create OperandType token if there is no syntax error
          size_t openingBracket = line.find('(', space);
          std::string type =
              line.substr(space + 1, openingBracket - (space + 1));

          if (isInArray<std::string>(type, OperandList, OperandListLength))
            _vector.push_back(std::make_pair(OperandType, type));
          else
            throw std::logic_error("invalid operand type => \033[31;1m" + type +
                                   "\033[0m");

          // Create Value token if there is no syntax error
          size_t closingBracket = line.find(')', openingBracket);
          if (closingBracket == std::string::npos)
            throw std::logic_error("no closing bracket found.");
          std::string value = line.substr(
              openingBracket + 1, closingBracket - (openingBracket + 1));
          if (type == "float" || type == "double") {
            if (std::regex_match(value, std::regex("[-]?[0-9]+(.[0-9]+)?")))
              _vector.push_back(std::make_pair(Value, value));
            else
              throw std::invalid_argument(
                  "\033[31;1m" + value +
                  "\033[0m is not a valid floating point value.");
          } else {
            if (std::regex_match(value, std::regex("[-]?[0-9]+")))
              _vector.push_back(std::make_pair(Value, value));
            else
              throw std::invalid_argument(
                  "\033[31;1m" + value +
                  "\033[0m is not a valid integer value.");
          }
          if (line.find(' ', space + 1) != std::string::npos)
            throw std::logic_error(instr + " only takes one parameter.");
          if (line[closingBracket + 1] != 0)
            throw std::logic_error(
                "trailing character(s) after instruction parameter => "
                "\033[31;1m" +
                line.substr(closingBracket + 1) + "\033[0m");
        } else {
          if (space != std::string::npos)
            throw std::logic_error(instr + " takes no parameter.");
        }
      }
    } catch (const std::logic_error &err) {
      std::cout << "Lexer \033[31merror\033[0m: line \033[1m" << lineIndex
                << "\033[0m, " << err.what() << std::endl;
      _isValid = false;
    }
  }

  // Verify that there is an exit instruction somewhere
  std::vector<std::pair<eTokenType, std::string>>::const_iterator it =
      _vector.begin();
  for (; it != _vector.end(); it++) {
    if (it->first == Instruction && it->second == "exit") break;
  }
  if (it == _vector.end()) {
    std::cout << "Lexer \033[31merror\033[0m: \033[1mexit\033[0m instruction "
                 "not found."
              << std::endl;
    _isValid = false;
  }
  if (_vector.size() > 0 && *it != _vector.back())
    std::cout << "Lexer \033[33mwarning\033[0m: instructions found after "
                 "\033[1mexit\033[0m, "
                 "they will be ignored."
              << std::endl;
}

Lexer::~Lexer(void) {}

std::vector<std::pair<eTokenType, std::string>> const Lexer::getVector(
    void) const {
  return _vector;
}

bool Lexer::isValid(void) const { return _isValid; }
