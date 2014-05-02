#include <iostream>
#include <iomanip>
#include "rb_tree.h"

int main(int argc, char* argv[]) {
  std::cout << "Red Black Tree" << std::endl;
  Tree t;
  t.insertValue(1);
  t.insertValue(2);
  // insertValue(3);
  // t.insertValue(4);
  // t.insertValue(5);

  // std::cout << "Count of Nodes: " << t.countNodes(t.getRoot()) << std::endl;

  std::cout << std::endl;

  return 0;
}
