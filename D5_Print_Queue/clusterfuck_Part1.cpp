#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct PageNode {
  int page;
  int priority;
  std::set<PageNode *> children;
};

class PriorityQueue {
  std::vector<PageNode *> queue;

  void help_update_priority(PageNode *p, PageNode *c, std::string indent = "",
                            std::set<int> parents = {}) {
    if ((p->priority < c->priority) || parents.count(c->page)) {
      // std::cout << indent << "ret\n";
      return;
    } else {
      c->priority = p->priority + 1;
      // std::cout << indent << "p: " << p->page << " " << p->priority << "\n"
      //           << indent << "c: " << c->page << " " << c->priority << "\n";
      for (PageNode *child : c->children) {
        // std::cout << indent << "c child: " << child->page << " "
        //           << child->priority << "\n";
        indent += "|  ";
        parents.insert(p->page);
        help_update_priority(c, child, indent, parents);
      }
      // std::cout << indent << "ret\n";
    }
  }

  void help_heapify(int i, int n) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n &&
        this->queue[left]->priority < this->queue[smallest]->priority) {
      smallest = left;
    }
    if (right < n &&
        this->queue[right]->priority < this->queue[smallest]->priority) {
      smallest = right;
    }

    if (smallest != i) {
      std::swap(this->queue[i], this->queue[smallest]);
      help_heapify(smallest, n);
    }
  };

  void heapify() {
    for (int i = this->queue.size() / 2 - 1; i >= 0; i--) {
      help_heapify(i, this->queue.size());
    }
  }

public:
  int size() { return this->queue.size(); }

  PageNode *insert(int priority, int page) {
    PageNode *new_node = new PageNode();
    new_node->page = page;
    new_node->priority = priority;
    this->queue.push_back(new_node);
    int i = this->queue.size() - 1;

    while (i > 0 &&
           this->queue[(i - 1) / 2]->priority > this->queue[i]->priority) {
      std::swap(this->queue[i], this->queue[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
    return new_node;
  };

  PageNode *pop() {
    std::swap(this->queue[0], this->queue[this->queue.size() - 1]);
    PageNode *ret = this->queue[this->queue.size() - 1];
    this->queue.pop_back();
    this->heapify();
    return ret;
  }

  PageNode *find(int page) {
    for (int i = 0; i < this->queue.size(); i++) {
      if (this->queue[i]->page == page) {
        return this->queue[i];
      }
    }
    return nullptr;
  };

  void update_priority(PageNode *p, PageNode *c) {
    this->help_update_priority(p, c);
    std::vector<PageNode *> old_queue = this->queue;
    this->heapify();
  }

  void print_queue() { print_queue(this->queue); }
  void print_queue(std::vector<PageNode *> queue) {
    for (int i = 0; i < queue.size(); i++) {
      std::cout << queue[i]->page << " " << queue[i]->priority << " "
                << queue[i] << "\n";
    }
    std::cout << std::endl;
  }
};

void print_arr(std::vector<int> arr) {
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
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

bool check_line(std::vector<int> p_queue_arr, std::vector<int> line_numbers) {
  std::vector<int> tmp;
  for (int i = 0; i < p_queue_arr.size(); i++) {
    for (int j = 0; j < line_numbers.size(); j++) {
      if (p_queue_arr[i] == line_numbers[j]) {
        tmp.push_back(p_queue_arr[i]);

        continue;
      }
    }
  }

  for (int i = 0; i < line_numbers.size(); i++) {
    for (int j = 0; j < tmp.size(); j++) {
      if (line_numbers[i] == tmp[j]) {
        std::cout << i << " " << j << "\n";
        if (i < j) {
          return false;
        } else {
          continue;
        }
      }
    }
  }
  std::cout << std::endl;
  return true;
};

int main(int argc, char *argv[]) {
  std::fstream my_file;
  std::string filename = "d5_input.txt";
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
    if (p && c) {
      p->children.insert(c);
      p_queue.update_priority(p, c);
    } else if (p && !c) {
      c = p_queue.insert(p->priority + 1, c_page);
      p->children.insert(c);
    } else if (!p && c) {
      p = p_queue.insert(c->priority - 1, p_page);
      p->children.insert(c);
    } else {
      p = p_queue.insert(0, p_page);
      c = p_queue.insert(p->priority + 1, c_page);
      p->children.insert(c);
    }
  }

  std::vector<int> p_queue_arr;
  int p_queue_size = p_queue.size(); // define size here because size will
                                     // decrease with each iteration
  for (int i = 0; i < p_queue_size; i++) {
    PageNode *min = p_queue.pop();
    p_queue_arr.push_back(min->page);
  }
  int right_order_mid = 0;
  while (std::getline(my_file, file_line)) {
    std::vector<int> line_numbers = stov(file_line);

    if (check_line(p_queue_arr, line_numbers)) {
      right_order_mid += line_numbers[line_numbers.size() / 2];
    }
  }
  std::cout << right_order_mid << std::endl;

  my_file.close();
  return 0;
}
