#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct PageNode {
  int page;
  int priority;
  std::vector<PageNode *> children;
  PageNode *left = nullptr;
  PageNode *right = nullptr;
};

class PriorityQueue {
  std::vector<PageNode *> queue;

  void help_update_priority(PageNode *p, PageNode *c) {
    if (p->priority < c->priority) {
      return;
    } else {
      c->priority = p->priority + 1;
      for (int i = 0; i < c->children.size(); i++) {
        help_update_priority(c, c->children[i]);
      }
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
    if (p && c) {
      p_queue.update_priority(p, c);
    } else if (p && !c) {
      p_queue.insert(p->priority + 1, c_page);
    } else if (!p && c) {
      p_queue.insert(c->priority - 1, p_page);
    } else {
      p = p_queue.insert(0, p_page);
      p_queue.insert(p->priority + 1, c_page);
    }
  }

  // while (std::getline(my_file, file_line)) {
  //   std::cout << file_line << "\n";
  //   // TODO: Implement line checker
  // }

  std::cout << "Insert end\n";
  p_queue.print_queue();
  std::cout << p_queue.size() << " Size\n\n";
  while (p_queue.size()) {
    PageNode *min = p_queue.pop();
    std::cout << min->page << " " << min->priority << "\n";
  }
  std::cout << std::endl;
  my_file.close();
  return 0;
}
