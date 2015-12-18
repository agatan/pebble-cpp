#include <pebble/syntax/parse.hpp>
#include <pebble/utils/stringify.hpp>

#include <iostream>
#include <string>

int main()
{
  std::string str;

  std::getline(std::cin, str);

  auto maybe_ast = pebble::syntax::parse_expression(str);

  if (!maybe_ast) {
    std::cerr << "Parse failed." << std::endl;
    return 1;
  }

  auto ast = *maybe_ast;

  std::cout << pebble::utils::stringify(ast) << std::endl;
}
