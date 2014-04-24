#include <iostream>
#include "rb_tree.h"

int main(int argc, char* argv[]){
    std::cout << "Red Black Tree" << std::endl;
    Tree t;
    t.insertValue(1);
    t.insertValue(2);
    t.insertValue(3);
    t.insertValue(4);
    t.insertValue(5);

    return 0;
}
