#include "Lexer.hpp"
#include "Parser.hpp"

// Don't forget exit lexer error case (no exit in file)
int main(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      Lexer lexer(argv[i]);
      if (lexer.isValid())
        Parser parser(lexer.getVector());
    }
  }
  return 0;
}