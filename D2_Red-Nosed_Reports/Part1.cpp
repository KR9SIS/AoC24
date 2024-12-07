#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool is_safe(std::string direction) {
  // some code
  // Some more code
  return false;
}

int main(int argc, char *argv[]) {
  std::string filename = "small_input.txt";
  std::fstream myFile;
  myFile.open(filename, std::ios::in);
  if (!myFile.is_open()) {
    std::cout << "File not found: " << filename;
    return 1;
    // throw std::runtime_error("File not found: " + filename);
  }
  std::string fileLine;
  std::string num;
  int safeReports = 0;
  bool safe;

  while (getline(myFile, fileLine)) {
    std::stringstream fileStream(fileLine);
    std::vector<int> report;
    while (getline(fileStream, num, ' ')) {
      report.push_back(std::stoi(num));
    };
    if (report[0] > report[1]) {
      if (is_safe("decr")) {
        safeReports++;
      };
    } else if (is_safe("incr")) {
      safeReports++;
    }
    for (int i = 0; i < report.size() - 1; i++) {
      std::cout << report[i] << ", ";
    }
    std::cout << report[report.size() - 1] << std::endl;
  }

  myFile.close();
  return 0;
}
