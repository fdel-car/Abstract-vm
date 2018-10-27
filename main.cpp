#include "Lexer.hpp"
#include "Parser.hpp"
#include <fstream>

int main(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::ifstream ifs(argv[i]);
      if (!ifs.is_open())
        std::cout << "Unable to open " << argv[i] << "." << std::endl;
      else {
        std::string fileContent((std::istreambuf_iterator<char>(ifs)),
                                (std::istreambuf_iterator<char>()));
        if (fileContent.length() == 0)
          std::cout << "The file " << argv[i] << " is empty." << std::endl;
        Lexer lexer(fileContent);
        if (lexer.isValid())
          Parser parser(lexer.getVector());
      }
      if (i < argc - 1)
        std::cout << std::endl;
    }
  } else {
    std::string data;
    std::string line;
    while (getline(std::cin, line)) {
      data += line + "\n";
      if (line.find(";;") != std::string::npos)
        break;
    }
    Lexer lexer(data);
    if (lexer.isValid())
      Parser parser(lexer.getVector());
  }
  return 0;
}