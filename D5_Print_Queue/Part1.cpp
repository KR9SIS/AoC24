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
    // std::cout << "in help u_p\n"
    //           << p->page << ", " << p->priority << "\n"
    //           << c->page << ", " << c->priority << "\n";
    if (p->priority < c->priority) {
      return;
    } else {
      c->priority = p->priority + 1;
      for (int i = 0; i < c->children.size(); i++) {
        help_update_priority(c, c->children[i]);
      }
    }
  }

  void quick_print(std::string name, int priority, int page, int idx,
                   int size) {
    std::cout << name << "\n  priority: " << priority << "\n  page: " << page
              << "\n  idx: " << idx << "\n  size:" << size << std::endl;
  }
  void help_heapify(int i, int n) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // if (left < n) {
    //   std::cout << "left cmp\n";
    //   quick_print("smallest", this->queue[smallest]->priority,
    //               this->queue[smallest]->page, smallest, n);
    //   std::cout << "\n";
    //   quick_print("left", this->queue[left]->priority,
    //   this->queue[left]->page,
    //               left, n);
    //   std::cout << "\n";
    //   quick_print("right", this->queue[right]->priority,
    //               this->queue[right]->page, right, n);
    //   std::cout << "\n";
    // } else {
    //   std::cout << "left " << left << " > " << n << " size";
    // }
    if (left < n &&
        &this->queue[left]->priority < &this->queue[smallest]->priority) {
      smallest = left;
      // std::cout << "True\n";
    }
    // if (right < n) {
    //   std::cout << "right cmp\n";
    //   quick_print("smallest", this->queue[smallest]->priority,
    //               this->queue[smallest]->page, smallest, n);
    //
    //   std::cout << "\n";
    //   quick_print("left", this->queue[left]->priority,
    //   this->queue[left]->page,
    //               left, n);
    //   std::cout << "\n";
    //   quick_print("right", this->queue[right]->priority,
    //               this->queue[right]->page, right, n);
    //
    // } else {
    //   std::cout << "left " << left << " > " << n << " size";
    // }
    if (right < n &&
        &this->queue[right]->priority < &this->queue[smallest]->priority) {
      smallest = right;
      // std::cout << "True\n";
    }
    // std::cout << "\n-------------------------\n";

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

  PageNode *find(int page) {
    for (int i = 0; i < this->queue.size(); i++) {
      if (this->queue[i]->page == page) {
        return this->queue[i];
      }
    }
    return nullptr;
  };

  void update_priority(PageNode *p, PageNode *c) {
    // std::cout << "before update_priority\n"
    //           << p->page << ", " << p->priority << "\n"
    //           << c->page << ", " << c->priority << "\n";
    this->help_update_priority(p, c);
    // std::cout << "after update_priority\n"
    //           << p->page << ", " << p->priority << "\n"
    //           << c->page << ", " << c->priority << "\n";
    std::vector<PageNode *> old_queue = this->queue;
    this->heapify();
    std::cout << "heap before heapify:\n";
    print_queue(old_queue);
    std::cout << "heap after heapify:\n";
    print_queue(this->queue);
    std::cout << "-----------------------------\n" << std::endl;
  }

  void print_queue(std::vector<PageNode *> queue) {
    for (int i = 0; i < queue.size(); i++) {
      std::cout << queue[i]->page << " " << queue[i]->priority << "\n";
    }
    std::cout << "\n" << std::endl;
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
    // p_queue.print_queue(this->queue);
  }
  while (std::getline(my_file, file_line)) {
    std::cout << file_line << "\n";
  }
  std::cout << std::endl;
  my_file.close();
  return 0;
}
