#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

void printVector(std::vector<int> v) {
  for (int i = 0; i < v.size() - 1; i++) {
    std::cout << v[i] << ", ";
  }
  std::cout << v[v.size() - 1] << std::endl;
}

bool is_safe(std::vector<int> report) {
  bool decr;
  if (report[0] > report[1]) {
    decr = true;
  } else {
    decr = false;
  }
  bool safe = true;
  int prev = report[0];
  for (int i = 1; i < report.size(); i++) {
    if (report[i] == prev) {
      return false;
    } else if (decr) {

      // Report levels are decreasing
      if (prev < report[i] || !(report[i] >= prev - 3)) {
        return false;
      }
    } else {
      // Report levels are increasing
      if (prev > report[i] || !(report[i] <= prev + 3)) {
        return false;
      }
    }
    prev = report[i];
  }
  return true;
}
bool probDamper(std::vector<int> report) {
  for (int i = 0; i < report.size(); i++) {
    std::vector<int> test;
    for (int j = 0; j < report.size(); j++) {
      if (i != j) {
        test.push_back(report[j]);
      }
    }
    if (is_safe(test)) {
      return true;
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  std::string filename = "d2_input.txt";
  std::fstream myFile;
  myFile.open(filename, std::ios::in);
  if (!myFile.is_open()) {
    std::cout << "File not found: " << filename;
    return 1;
  }
  std::string fileLine;
  std::string num;
  int safeReports = 0;

  while (getline(myFile, fileLine)) {
    std::stringstream fileStream(fileLine);
    std::vector<int> report;
    while (getline(fileStream, num, ' ')) {
      report.push_back(std::stoi(num));
    };
    if (is_safe(report) || probDamper(report)) {
      safeReports++;
    }
  }
  std::cout << "The number of safe reports is: " << safeReports << std::endl;

  myFile.close();
  return 0;
}
