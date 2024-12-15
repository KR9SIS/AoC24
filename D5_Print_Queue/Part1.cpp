#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "small_input.txt";
  my_file.open(filename, std::ios::in);
  if (!my_file.is_open()) {
    std::cout << "File not found: " << filename << std::endl;
    return 1;
  }

  PriorityQueue p_queue;
  std::string file_line;
  int p_page, c_page;
  PageNode *p, *c;
  while (std::getline(my_file, file_line)) {
    if (file_line.empty()) {
      break;
    }
    int delim = file_line.find("|");
    p_page = std::stoi(file_line.substr(0, delim));
    c_page = std::stoi(file_line.erase(0, delim + 1));
    p = p_queue.find(p_page);
    c = p_queue.find(c_page);
  }
  while (std::getline(my_file, file_line)) {
    std::cout << file_line << "\n";
  }
  std::cout << std::endl;
  my_file.close();
  return 0;
}
