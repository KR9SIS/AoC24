#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<int>, std::vector<int>> readFile(std::string filename) {
  std::fstream myFile;
  myFile.open(filename, std::ios::in);
  if (!myFile.is_open()) {
    throw std::runtime_error("File not found: " + filename);
  }

  std::string fileLine;
  std::string delim = "   ";
  std::vector<int> left;
  std::vector<int> right;

  while (getline(myFile, fileLine)) {
    char idx = fileLine.find(delim);
    int left_num = std::stoi(fileLine.substr(0, idx));
    fileLine.erase(0, idx + delim.length());
    int right_num = std::stoi(fileLine);
    left.push_back(left_num);
    right.push_back(right_num);
  }

  myFile.close();
  return std::make_tuple(left, right);
}

int main(int argc, char *argv[]) {

  std::tuple<std::vector<int>, std::vector<int>> tup;
  try {
    tup = readFile("input_p1.txt");
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::vector<int> left = std::get<0>(tup);
  std::vector<int> right = std::get<1>(tup);
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  int sum = 0;
  for (int i = 0; i < left.size(); i++) {
    sum = sum + std::abs(left[i] - right[i]);
  }
  std::cout << "The total distance is: " << sum << std::endl;

  return 0;
}
