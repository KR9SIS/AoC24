#include <fstream>
#include <iostream>
#include <string>
int readFile(std::string filename) {

  std::fstream myFile;
  myFile.open(filename, std::ios::in);
  if (myFile.is_open()) {
    std::string fileLine;
    std::string delim = "   ";
    while (getline(myFile, fileLine)) {
      std::cout << fileLine << std::endl;
    }

    myFile.close();
  }
}

int main(int argc, char *argv[]) { return 0; }
