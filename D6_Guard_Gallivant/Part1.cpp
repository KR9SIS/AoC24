#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Grid {
  std::vector<std::vector<char>> grid;
  int width = 0;
  std::pair<int, int> guard_coords;

  enum Direction {
    up,
    right,
    down,
    left,
  };

  Direction direction = Direction::up;

  void patrol_direction(int x_mod, int y_mod) {
    while ((0 <= this->guard_coords.first + (1 * x_mod) &&
            this->guard_coords.first + (1 * x_mod) < this->width &&
            0 <= this->guard_coords.second + (1 * y_mod) &&
            this->guard_coords.second + (1 * y_mod) < this->grid.size()) &&
           (this->grid[this->guard_coords.second + (1 * y_mod)]
                      [this->guard_coords.first + (1 * x_mod)] != '#')) {

      std::swap(this->grid[this->guard_coords.second][this->guard_coords.first],
                this->grid[this->guard_coords.second + (1 * y_mod)]
                          [this->guard_coords.first + (1 * x_mod)]);

      this->guard_coords.first = this->guard_coords.first + (1 * x_mod);
      this->guard_coords.second = this->guard_coords.second + (1 * y_mod);

      this->guard_positions.insert(
          {this->guard_coords.first, this->guard_coords.second});
    };
  };

public:
  std::set<std::pair<int, int>> guard_positions;
  void patrol() {
    int x_mod = 0;
    int y_mod = 0;
    // while guard within grid bounds
    while (0 <= this->guard_coords.first + (1 * x_mod) &&
           this->guard_coords.first + (1 * x_mod) < this->width &&
           0 <= this->guard_coords.second + (1 * y_mod) &&
           this->guard_coords.second + (1 * y_mod) < this->grid.size()) {
      print_grid();
      switch (direction) {
      case Direction::up:
        x_mod = 0;
        y_mod = -1;
        this->direction = Direction::right;
        break;
      case Direction::right:
        x_mod = 1;
        y_mod = 0;
        this->direction = Direction::down;
        break;
      case Direction::down:
        x_mod = 0;
        y_mod = 1;
        this->direction = Direction::left;
        break;
      case Direction::left:
        x_mod = -1;
        y_mod = 0;
        this->direction = Direction::up;
        break;
      }

      patrol_direction(x_mod, y_mod);
    }
  }

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
  std::string filename = "d6_input.txt";
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

  grid.patrol();
  std::cout << "The guard patrolled " << grid.guard_positions.size()
            << " positions" << std::endl;
  return 0;
}
