#include <iostream>
#include <string>
std::string hi() { return "hello"; }

void printLine(std::string text) {
  //
  std::cout << text << std::endl;
}

void printLine(int value) {
  //
  std::cout << "integer value is: " << value << std::endl;
}

int main() {
  printLine("---------------------------");
  printLine("Hello world!");
  printLine(100);
  printLine("---------------------------");
  
  char x = 'a';
  
  return 0;
}