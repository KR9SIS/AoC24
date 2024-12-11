#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

void print_grid(std::vector<std::vector<char>> grid) {
  std::cout << "  ";
  for (int col = 0; col < grid[0].size(); col++) {
    std::cout << col << " ";
  }
  std::cout << "\n";
  for (int i = 0; i < grid.size(); i++) {
    std::cout << i << " ";
    for (int j = 0; j < grid[i].size(); j++) {
      std::cout << grid[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

// std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>
bool search2D(std::vector<std::vector<char>> grid, int row, int col,
              std::string WORD) {
  if (grid[row][col] != WORD[1]) {
    return false;
  }
  const int ROW_LEN = grid.size();
  const int COL_LEN = grid[0].size();
  const int WORD_LEN = WORD.size();
  const std::vector<std::vector<int>> corners = {
      {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};

  int left_x, left_y, right_x, right_y;

  // std::cout << "\nrow, col: " << row << ", " << col << "\n";

  for (int c = 0; c < 2; c++) {
    left_x = row + corners[c][0];
    left_y = col + corners[c][1];
    right_x = row + corners[c + 2][0];
    right_y = col + corners[c + 2][1];

    // print_grid(grid);
    // std::cout << left_x << ", " << left_y << " : " << right_x << ", " <<
    // right_y
    //           << std::endl;

    if ( // Bounds check
        (left_x < 0 || ROW_LEN - 1 < left_x) ||
        (left_y < 0 || COL_LEN - 1 < left_y) ||
        (right_x < 0 || ROW_LEN - 1 < right_x) ||
        (right_y < 0 || COL_LEN - 1 < right_y) ||
        ( // MAS check
            !((grid[left_x][left_y] == WORD[0] &&
               grid[right_x][right_y] == WORD[2]) ||
              (grid[left_x][left_y] == WORD[2] &&
               grid[right_x][right_y] == WORD[0])))) {
      // std::cout << "false" << std::endl;
      return false;
    }
  }
  // std::cout << "true" << std::endl;
  return true;
}

int search_word(std::vector<std::vector<char>> grid, std::string WORD) {
  int count = 0;
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> matches;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (search2D(grid, i, j, WORD)) {
        count++;
      }
    }
  }
  return count;
}

int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "d4_input.txt";
  my_file.open(filename, std::ios::in);
  if (!my_file.is_open()) {
    std::cout << "File not found: " << filename << std::endl;
    return 1;
  }

  const std::string WORD = "MAS";
  std::vector<std::vector<char>> grid;
  std::string file_line;
  while (getline(my_file, file_line)) {
    std::vector<char> row = {};
    for (int i = 0; i < file_line.size(); i++) {
      row.push_back(file_line[i]);
    }
    grid.push_back(row);
  }
  int count = search_word(grid, WORD);
  std::cout << WORD << " was found " << count << " times in " << filename
            << std::endl;
  my_file.close();
  return 0;
}
