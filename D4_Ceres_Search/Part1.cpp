#include <fstream>
#include <iostream>
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

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>
search2D(std::vector<std::vector<char>> grid, int row, int col,
         std::string WORD) {
  if (grid[row][col] != WORD[0]) {
    return {};
  }
  const int ROW_LEN = grid.size();
  const int COL_LEN = grid[0].size();
  const int WORD_LEN = WORD.size();
  const std::vector<std::vector<int>> direction = {
      {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
  int curr_x, curr_y;
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> matches = {};

  for (int d = 0; d < direction.size(); d++) {
    for (int offset = 1; offset < WORD_LEN; offset++) {
      curr_x = row + (offset * direction[d][0]);
      curr_y = col + (offset * direction[d][1]);
      if (curr_x < 0 || curr_x > ROW_LEN - 1 || curr_y < 0 ||
          curr_y > COL_LEN - 1 || grid[curr_x][curr_y] != WORD[offset]) {
        break;
      } else if (offset + 1 == WORD_LEN) {
        // std::cout << "Found! " << direction[d][0] << " " << direction[d][1]
        //           << "\n"
        //           << row << " " << col << "\n"
        //           << curr_x << " " << curr_y << std::endl;

        matches.push_back(std::make_pair(std::make_pair(row, col),
                                         std::make_pair(curr_x, curr_y)));
      }
    }
  }

  return matches;
}

int search_word(std::vector<std::vector<char>> grid, std::string WORD) {
  int count = 0;
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> matches;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      matches = search2D(grid, i, j, WORD);
      if (matches.size()) {
        count = count + matches.size();
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

  const std::string WORD = "XMAS";
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
