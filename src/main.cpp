#include <whiteSpace.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <array>

void print_usage()
{
  std::cout << "WhiteSpace interpreter" << '\n';
  std::cout << "Usgae: WhiteSpace <file> " << '\n';
  std::cout << "Options: " << '\n';
  std::cout << "\t--help | -h \t print this message" << '\n';
  std::cout << std::endl;
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "[ERROR] too few arguments passed" << std::endl;
    print_usage();
    return 1;
  }
  else if (argc > 3)
  {
    std::cerr << "[ERROR] too many arguments passed" << std::endl;
    print_usage();
    return 1;
  }

  if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
  {
    print_usage();
    return 0;
  }

  std::ifstream fin(argv[1]);
  if (fin.fail())
  {
    std::cerr << "[ERROR] file not found" << std::endl;
    return 1;
  }

  run_script(fin, std::cout, std::cin);

  return 0;
}
