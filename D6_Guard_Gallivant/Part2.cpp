#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class Grid {

  enum Direction {
    up = '^',
    right = '>',
    down = 'v',
    left = '<',
  };

  Direction direction = Direction::up;

  bool within_bounds(int x_mod, int y_mod) {
    if ((0 <= this->guard_coords.first + x_mod &&
         this->guard_coords.first + x_mod < this->width &&
         0 <= this->guard_coords.second + y_mod &&
         this->guard_coords.second + y_mod < this->grid.size())) {
      return true;
    }
    return false;
  };

  bool patrol_direction(int x_mod, int y_mod) {
    while (within_bounds(x_mod, y_mod) &&
           (this->grid[this->guard_coords.second + y_mod]
                      [this->guard_coords.first + x_mod] != '#')) {

      std::swap(this->grid[this->guard_coords.second][this->guard_coords.first],
                this->grid[this->guard_coords.second + y_mod]
                          [this->guard_coords.first + x_mod]);

      this->guard_coords.first = this->guard_coords.first + x_mod;
      this->guard_coords.second = this->guard_coords.second + y_mod;

      std::tuple<int, int, char> coords = {
          this->guard_coords.first, this->guard_coords.second, direction};
      if (std::binary_search(this->guard_positions.begin(),
                             this->guard_positions.end(), coords)) {
        return true;
      }
      this->guard_positions.insert(
          {this->guard_coords.first, this->guard_coords.second, direction});
    };
    return false;
  };

public:
  std::set<std::tuple<int, int, char>> guard_positions;
  std::vector<std::vector<char>> grid;
  int width = 0;
  std::pair<int, int> guard_coords;

  bool patrol() {
    int x_mod = 0;
    int y_mod = 0;
    bool loop = false;
    // while guard within grid bounds
    while (within_bounds(x_mod, y_mod)) {

      print_grid();

      switch (direction) {
      case Direction::up:
        x_mod = 0;
        y_mod = -1;
        this->grid[this->guard_coords.second][this->guard_coords.first] = up;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = Direction::right;
        break;
      case Direction::right:
        x_mod = 1;
        y_mod = 0;
        this->grid[this->guard_coords.second][this->guard_coords.first] = right;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = Direction::down;
        break;
      case Direction::down:
        x_mod = 0;
        y_mod = 1;
        this->grid[this->guard_coords.second][this->guard_coords.first] = down;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = Direction::left;
        break;
      case Direction::left:
        x_mod = -1;
        y_mod = 0;
        this->grid[this->guard_coords.second][this->guard_coords.first] = left;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = Direction::up;
        break;
      }
      if (loop) {
        return true;
      };
    }
    return false;
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
              << this->guard_coords.second << "\n"
              << std::endl;
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
  int loops = 0;
  // for (int row = 0; row < grid.grid.size(); row++) {
  //   for (int col = 0; col < grid.width; col++) {
  //     char val = grid.grid[row][col];
  //     if (val == '#' || val == '^') {
  //       continue;
  //     } else {
  //       grid.grid[row][col] = '#';
  //       if (grid.patrol()) {
  //         loops++;
  //       };
  //       grid.grid[row][col] = val;
  //     }
  //   }
  // }
  // std::cout << "The guard looped at " << loops << " positions" << std::endl;
  grid.patrol();
  std::cout << grid.guard_positions.size() << std::endl;
  return 0;
}
