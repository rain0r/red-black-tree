#include "rb_tree.h"

void insert(Tree *t) {
  for (int i = 0; i < 19; i++) {
    int x = rand() % 10;
    if (x > 0) {
      t->insertValue(x);
    }
  }
}

int main(int argc, char* argv[]) {
  std::cout << std::endl;
  std::cout << "Red Black Tree" << std::endl;

  boost::thread_group tree_threads;

  int tcount = 4;
  Tree t;
  int nums[] = { 7, 9, 5, 2, 1, 6, 3, 8, 4 };

  for (int i = 0; i != tcount; i++) {
    boost::thread *t1 = new boost::thread(insert, &t);
    tree_threads.add_thread(t1);
  }

  tree_threads.join_all();

  t.print();

//  bool b = t.deleteValue(4);
//  if (b) {
//    std::cout << "Success";
//  } else {
//    std::cout << "Fail";
//  }
//  std::cout << std::endl;
//
//  t.print();

  return 0;
}
