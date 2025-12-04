#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

void print_children(std::vector<int> line_numbers,
                    std::map<int, std::set<int>> rules) {
  for (int i = 0; i < line_numbers.size(); i++) {
    std::set<int> children = rules[line_numbers[i]];
    std::cout << "\n";
    std::cout << line_numbers[i] << ": ";
    for (int kid : children) {
      std::cout << kid << " ";
    }
  }

  std::cout << std::endl;
}

std::vector<int> stov(std::string file_line) {
  std::string num;
  std::vector<int> line_numbers;
  for (int i = 0; i < file_line.size(); i++) {
    if (file_line[i] == ',') {
      line_numbers.push_back(std::stoi(num));
      num.erase();
    } else {
      num += file_line[i];
    }
  }
  line_numbers.push_back(std::stoi(num));
  return line_numbers;
}

bool checkline(std::vector<int> line_numbers,
               std::map<int, std::set<int>> rules) {

  for (int i = line_numbers.size() - 1; i >= 0; i--) {
    for (int j = i - 1; j >= 0; j--) {
      std::set<int> children = rules[line_numbers[j]];
      if (!std::binary_search(children.begin(), children.end(),
                              line_numbers[i])) {
        return false;
      }
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "d5_input.txt";
  my_file.open(filename, std::ios::in);
  if (!my_file.is_open()) {
    std::cout << "File not found: " << filename << std::endl;
    return 1;
  }
  std::string file_line;
  std::map<int, std::set<int>> rules;
  int p_page, c_page;
  while (std::getline(my_file, file_line)) {
    if (file_line.empty()) {
      break;
    }
    int delim = file_line.find("|");
    p_page = std::stoi(file_line.substr(0, delim));
    c_page = std::stoi(file_line.erase(0, delim + 1));
    rules[p_page].insert(c_page);
  }
  int mid_sum = 0;
  while (std::getline(my_file, file_line)) {
    std::vector<int> line_numbers = stov(file_line);
    if (checkline(line_numbers, rules)) {
      mid_sum += line_numbers[((line_numbers.size() - 1) / 2)];
    }
  }
  std::cout << "\nSum of middle page numbers: " << mid_sum << std::endl;
  my_file.close();
  return 0;
}
