#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class Grid {

  char direction_arr[4]{
      '^',
      '>',
      'v',
      '<',
  };

  char direction;

  bool within_bounds(int x_mod, int y_mod) {
    if (0 <= this->guard_coords.first + x_mod &&
        this->guard_coords.first + x_mod < this->width &&
        0 <= this->guard_coords.second + y_mod &&
        this->guard_coords.second + y_mod < this->grid.size()) {
      return true;
    }
    return false;
  };

  bool patrol_direction(int x_mod, int y_mod) {
    while (within_bounds(x_mod, y_mod) &&
           (this->grid[this->guard_coords.second + y_mod]
                      [this->guard_coords.first + x_mod] != '#') &&
           (this->grid[this->guard_coords.second + y_mod]
                      [this->guard_coords.first + x_mod] != 'O')) {

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
    this->guard_positions = {};
    this->direction = this->direction_arr[0];

    // while guard within grid bounds
    while (within_bounds(x_mod, y_mod)) {

      switch (this->direction) {
      case '^':
        x_mod = 0;
        y_mod = -1;
        this->grid[this->guard_coords.second][this->guard_coords.first] =
            direction;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = direction_arr[1];
        break;
      case '>':
        x_mod = 1;
        y_mod = 0;
        this->grid[this->guard_coords.second][this->guard_coords.first] =
            direction;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = direction_arr[2];
        break;
      case 'v':
        x_mod = 0;
        y_mod = 1;
        this->grid[this->guard_coords.second][this->guard_coords.first] =
            direction;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = direction_arr[3];
        break;
      case '<':
        x_mod = -1;
        y_mod = 0;
        this->grid[this->guard_coords.second][this->guard_coords.first] =
            direction;
        loop = patrol_direction(x_mod, y_mod);
        this->direction = direction_arr[0];
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

  std::vector<std::pair<int, int>> loops;
  std::pair<int, int> guard_start_pos = {grid.guard_coords.first,
                                         grid.guard_coords.second};

  bool loop = grid.patrol();
  if (loop) {
    std::cout << "true" << std::endl;
    grid.print_grid();
    return 0;
  }
  grid.grid[guard_start_pos.second][guard_start_pos.first] = '^';
  grid.grid[grid.guard_coords.second][grid.guard_coords.first] = '.';
  grid.guard_coords.first = guard_start_pos.first;
  grid.guard_coords.second = guard_start_pos.second;
  std::set<std::pair<int, int>> first_route;
  for (std::tuple<int, int, char> pos : grid.guard_positions) {
    first_route.insert({std::get<0>(pos), std::get<1>(pos)});
  }

  for (std::tuple<int, int> pos : first_route) {
    // std::cout << "Start new iteration\n";
    // grid.print_grid();
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);
    char val = grid.grid[row][col];
    grid.grid[row][col] = 'O';
    // std::cout << "Place obstacle\n";
    // grid.print_grid();

    if (grid.patrol()) {
      loops.push_back({row, col});
    };
    // std::cout << "After patrol\n";
    // grid.print_grid();

    grid.grid[guard_start_pos.second][guard_start_pos.first] = '^';
    grid.grid[grid.guard_coords.second][grid.guard_coords.first] = '.';
    grid.guard_coords.first = guard_start_pos.first;
    grid.guard_coords.second = guard_start_pos.second;
    // std::cout << "After guard pos cleanup\n";
    // grid.print_grid();
    grid.grid[row][col] = val;
    // grid.print_grid();
    // std::cout << "After obstacle cleanup\n";
    // std::cout << "\n\n";
  }
  std::cout << "The guard looped at " << loops.size() << " positions"
            << std::endl;

  return 0;
}
