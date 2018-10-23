#include "Lexer.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <locale>
#include <regex>

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

Lexer::Lexer(std::string const &filename)
    : _filename(filename), _isValid(true) {
  std::ifstream ifs;
  ifs.open(_filename);
  std::string line;
  size_t lineIndex = 0;

  if (ifs.is_open()) {
    while (getline(ifs, line)) {
      lineIndex++;
      // Remove everything after the semicolon
      size_t semicolon = line.find(';');
      line = line.substr(0, semicolon);
      trim(line);

      if (!line.empty()) {
        // Create Instruction token if there is no syntax error
        size_t space = line.find(' ');
        std::string instr = line.substr(0, space);
        try {
          if (isInArray<std::string>(instr, InstrList, InstrListLength))
            _vector.push_back(std::make_pair(Instruction, instr));
          else
            throw Lexer::InvalidInstruction();
        } catch (Lexer::InvalidInstruction &err) {
          std::cout << "Lexer: line " << lineIndex
                    << ", invalid instruction found (" << instr << ")."
                    << std::endl;
          _isValid = false;
        }

        if (instr == "assert" || instr == "push") {
          // Create OperandType token if there is no syntax error
          size_t openingBracket = line.find('(', space);
          std::string type =
              line.substr(space + 1, openingBracket - (space + 1));
          if (isInArray<std::string>(type, OperandList, OperandListLength))
            _vector.push_back(std::make_pair(OperandType, type));

          // Create Value token if there is no syntax error
          size_t closingBracket = line.find(')', openingBracket);
          std::string value = line.substr(
              openingBracket + 1, closingBracket - (openingBracket + 1));
          if (type == "float" || type == "double") {
            if (std::regex_match(value, std::regex("[-]?[0-9]+.[0-9]+")))
              _vector.push_back(std::make_pair(Value, value));
          } else if (std::regex_match(value, std::regex("[-]?[0-9]+")))
            _vector.push_back(std::make_pair(Value, value));
        }
      }
    }
  } else {
    std::cout << _filename << ": Unable to open the file." << std::endl;
  }
}

Lexer::Lexer(Lexer const &src) { *this = src; }

Lexer::~Lexer(void) {}

Lexer &Lexer::operator=(Lexer const &rhs) {
  return *this;
  (void)rhs;
}