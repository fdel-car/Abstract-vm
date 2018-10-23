#include "Lexer.hpp"
#include "Operand.hpp"
#include <stack>

int main(int argc, char **argv) {
  // std::stack<IOperand *> stack;
  // IOperand *operand = new Operand<float>(45.2f, Float);
  // stack.push(operand);
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      Lexer lexer(argv[i]);
    }
  }
  return 0;
}