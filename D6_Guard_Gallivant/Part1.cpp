#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Grid {
  std::vector<std::vector<char>> grid;
  int width = 0;
  std::pair<int, int> guard_coords;

public:
  void insert_row(std::string new_row) {
    if (this->width == 0) {
      this->width = new_row.size();
    } else if (new_row.size() != this->width) {
      std::cout << new_row << "\nNew row length of " << new_row.size()
                << "\ndoes not fit in grid with row length of " << this->width
                << std::endl;
    }
    std::vector<char> row;
    for (int i = 0; i < new_row.size(); i++) {
      char c = new_row[i];
      if (c == '^') {
        this->guard_coords.first = i;
        this->guard_coords.second = this->grid.size();
      }
      row.push_back(c);
    }
    this->grid.push_back(row);
  }

  void print_grid() {
    for (int row = 0; row < this->grid.size(); row++) {
      for (int col = 0; col < this->width; col++) {
        std::cout << this->grid[row][col] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "Guard is at " << this->guard_coords.first << ", "
              << this->guard_coords.second << std::endl;
  }
};
int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "small_input.txt";
  my_file.open(filename, std::ios::in);
  if (!my_file.is_open()) {
    std::cout << "File not found: " << filename << std::endl;
    return 1;
  }

  Grid grid = Grid();
  std::string line;
  while (std::getline(my_file, line)) {
    grid.insert_row(line);
  }
  my_file.close();

  grid.print_grid();
  return 0;
}
