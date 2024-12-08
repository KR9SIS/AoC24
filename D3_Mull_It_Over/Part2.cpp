#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "small_input.txt";
  my_file.open(filename, std::ios::in);
  if (!my_file.is_open()) {
    std::cout << "File not found: " << filename << std::endl;
    return 1;
  }

  std::regex rgx("mul\\([0-9]{1,3},[0-9]{1,3}\\)");
  std::string file_line;
  std::vector<std::string> matches;
  int sum = 0;
  while (std::getline(my_file, file_line)) {
    auto matches_start =
        std::sregex_iterator(file_line.begin(), file_line.end(), rgx);
    auto matches_end = std::sregex_iterator();

    for (std::sregex_iterator k = matches_start; k != matches_end; ++k) {
      std::smatch match = *k;
      std::string match_str = match.str();
      int open = match_str.find("(");
      int mid = match_str.find(",");
      int close = match_str.find(")");
      // std::cout << match_str << "\nopen: " << open << "\nmid: " << mid
      //           << "\nclose: " << close << "\n";

      int left = std::stoi(match_str.substr(open + 1, open + 1 - mid));
      int right = std::stoi(match_str.substr(mid + 1, mid + 1 - close));
      sum = sum + (left * right);
      // std::cout << "\nleft: " << left << "\nright: " << right << "\n"
      //           << std::endl;
    }
  }
  std::cout << "Total sum is: " << sum << std::endl;
  my_file.close();
  return 0;
}
