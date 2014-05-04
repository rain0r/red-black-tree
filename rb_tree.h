#ifndef C_BBTREE_H
#define C_BBTREE_H

#include <stdio.h>
#include <boost/thread.hpp>
#include <iostream>
#include <iomanip>

class Tree {
 private:
  enum Color {
    RED = 0,
    BLACK
  };
  boost::mutex mtx_;

  struct Node {
    Node *parent, *left, *right;
    Color color;
    int value;
    bool isNil;
//    boost::mutex mtx_;

    explicit Node(int value_)
        : parent(NULL),
          left(NULL),
          right(NULL),
          color(RED),
          value(value_),
          isNil(false) {
//      mtx_.lock();
    }

    ~Node() {
      parent = NULL;
      left = NULL;
      right = NULL;
      value = 0;
      color = BLACK;
//      mtx_.unlock();
    }

    Node* grandparent() const {
      if (parent->parent != NULL) {
        return parent->parent;
      } else {
        return NULL;
      }
    }

    Node* uncle() const {
      if (grandparent() != NULL) {
        if (grandparent()->left != NULL || grandparent()->right != NULL) {
          if (parent == grandparent()->left)
            return grandparent()->right;
          else
            return grandparent()->left;
        }
      }
      return NULL;
    }

    Node* sibling() const {
      if (this == parent->left)
        return parent->right;
      else
        return parent->left;
    }
  };

  bool isLeftChild(const Node *node) {
    if (node->parent == NULL)
      return false;
    else
      return node->parent->left == node;
  }

  static bool isRightChild(const Node *node) {
    if (node->parent == NULL)
      return false;
    else
      return node->parent->right == node;
  }

  Node* rotateLeft(Node *node) {
    if (node->parent == NULL) {
      return NULL;
    }

    Node *tmpNode = node->right;
    node->right = tmpNode->left;

    if (tmpNode->left == NULL) {
      return NULL;
    }

    if (!tmpNode->left->isNil) {
      tmpNode->left->parent = node;
    }
    tmpNode->parent = node->parent;
    if (node->parent != NULL) {
      if (node->parent->isNil) {
        this->rootPtr = tmpNode;
      }
    } else if (node->parent->left == node) {
      node->parent->left = tmpNode;
    } else {
      node->parent->right = tmpNode;
    }
    tmpNode->left = node;
    node->parent = tmpNode;
    return node;
  }

  Node* rotateRight(Node *node) {
    Node *tmpNode = node->left;
    node->left = tmpNode->right;
    if (!tmpNode->right->isNil) {
      tmpNode->right->parent = node;
    }
    tmpNode->parent = node->parent;
    if (node->parent != NULL) {
      if (node->parent->isNil) {
        this->rootPtr = tmpNode;
      }
    } else if (node->parent->left == node) {
      node->parent->left = tmpNode;
    } else {
      node->parent->right = tmpNode;
    }
    tmpNode->right = node;
    node->parent = tmpNode;
    return node;
  }

  void insert_case1(Node *node) {
    if (node == NULL) {
      return;
    }
    if (node->parent == NULL) {
      node->color = BLACK;
      return;
    } else {
      insert_case2(node);
    }
  }
  void insert_case2(Node *node) {
    if (node == NULL) {
      return;
    }
    if (node->parent->color == BLACK) {
      return;
    } else {
      insert_case3(node);
    }
  }
  void insert_case3(Node *node) {
    if (node == NULL) {
      return;
    }
    if ((node->uncle()) && (node->uncle()->color == RED)) {
      node->parent->color = BLACK;
      node->uncle()->color = BLACK;
      node->grandparent()->color = RED;
      insert_case1(node->grandparent());
    } else {
      insert_case4(node);
    }
  }
  void insert_case4(Node *node) {
    if (node == NULL || node->grandparent() == NULL) {
      return;
    }
    if ((node == node->parent->right)
        && (node->parent == node->grandparent()->left)) {
      this->rotateLeft(node->parent);
      node = node->left;
    } else if (node == node->parent->left
        && node->parent == node->grandparent()->right) {
      this->rotateRight(node->parent);
      node = node->right;
    }
    insert_case5(node);
  }
  void insert_case5(Node *node) {
    //std::cout << "insert_case5(" << node->value << ")" << std::endl;
    if (node == NULL) {
      return;
    }
    node->parent->color = BLACK;
    node->grandparent()->color = RED;
    if (node == node->parent->left
        && node->parent == node->grandparent()->left) {
      this->rotateRight(node->grandparent());
    } else {
      this->rotateLeft(node->grandparent());
    }
  }

  Node *rootPtr;
  int nodeCounter;

 public:
  Tree()
      : rootPtr(NULL) {
    std::cout << "Tree()" << std::endl;
    this->nodeCounter = 0;
  }

  Node * getRootPtr() {
    if (this->rootPtr != NULL) {
      return this->rootPtr;
    } else {
      return NULL;
    }
  }

  void createNilLeaves(Node* node) {
    // Create 2 Nil Leaves
    Node* nil_lft = new Node(NULL);
    nil_lft->color = BLACK;
    nil_lft->isNil = true;
    Node* nil_rght = new Node(NULL);
    nil_rght->color = BLACK;
    nil_rght->isNil = true;

    node->left = nil_lft;
    node->right = nil_rght;
  }

  bool isEmpty() const {
    //std::cout << "isEmpty() returns: " << !rootPtr << std::endl;
    return !rootPtr;
  }

  bool insertValue(int value) {
    //std::cout << "insertValue(" << value << ")" << std::endl;

    if (this->isEmpty()) {
      // Insert the first node of the tree
      // (root node)
      //std::cout << "Adding root Node" << std::endl;
      Node* newNode = new Node(value);
      newNode->color = BLACK;
      this->rootPtr = newNode;
      this->createNilLeaves(this->rootPtr);
      this->nodeCounter += 1;
    } else {
      // check if the value to insert already exists
      if (this->search(value) != NULL) {
        //std::cout << "Value " << value << " already exists" << std::endl;
        return false;
      }

      // The new Node is not the Root
      mtx_.lock();
      if (this->addNewNode(this->rootPtr, value)) {
        //std::cout << "Added new Node with value: " << value << std::endl;
        this->nodeCounter += 1;
      } else {
        //std::cout << "Error adding new Node with value: " << value << std::endl;
        return false;
      }
      mtx_.unlock();
    }

    return true;
  }

  Node* search(int value) {
    //std::cout << "search(" << value << ")" << std::endl;
    return this->searchWalk(this->getRootPtr(), value);
  }

  Node* searchWalk(Node *node, int value) {
    if (node != NULL) {
      if (node->value == value) {
        return node;
      } else {
        if (value < node->value) {
          return searchWalk(node->left, value);
        } else {
          return searchWalk(node->right, value);
        }
      }
    }
    return NULL;
  }

  bool deleteValue(int value) {
    //std::cout << "deleteValue(" << value << ")" << std::endl;
    mtx_.lock();
    Node *toDelete = this->search(value);

    if (toDelete == NULL) {
      return false;
    } else {
      this->deleteOneChild(toDelete);
      return true;
    }
    mtx_.unlock();
    return false;
  }

  bool isLeaf(Node *node) {
    if (node != NULL) {
      if (node->right != NULL && node->left != NULL) {
        if (node->right->isNil && node->left->isNil) {
          return true;
        }
      }
    }
    return false;
  }

  void replaceNode(Node *node1, Node *node2) {
    // node2 replaces node1
    node1 = node2;
    delete node2;
  }

  void delete_case1(Node *node) {
    if (node->parent != NULL) {
      delete_case2(node);
    }
  }

  void delete_case2(Node *node) {
    Node *s = node->sibling();

    if (s->color == RED) {
      node->parent->color = RED;
      s->color = BLACK;
      if (node == node->parent->left) {
        rotateLeft(node->parent);
      } else {
        rotateRight(node->parent);
      }
    }
    delete_case3(node);
  }

  void delete_case3(Node *node) {
    Node *s = node->sibling();

    if (node == NULL || node->parent == NULL || s == NULL || s->left == NULL
        || s->right == NULL) {
      return;
    }

    if ((node->parent->color == BLACK) && (s->color == BLACK)
        && (s->left->color == BLACK) && (s->right->color == BLACK)) {
      s->color = RED;
      delete_case1(node->parent);
    } else
      delete_case4(node);
  }

  void delete_case4(Node *node) {
    Node *s = node->sibling();

    if ((node->parent->color == RED) && (s->color == BLACK)
        && (s->left->color == BLACK) && (s->right->color == BLACK)) {
      s->color = RED;
      node->parent->color = BLACK;
    } else
      delete_case5(node);
  }

  void delete_case5(Node *node) {
    Node *s = node->sibling();

    if (s->color == BLACK) {
      if ((node == node->parent->left) && (s->right->color == BLACK)
          && (s->left->color == RED)) {
        s->color = RED;
        s->left->color = BLACK;
        rotateRight(s);
      } else if ((node == node->parent->right) && (s->left->color == BLACK)
          && (s->right->color == RED)) {
        s->color = RED;
        s->right->color = BLACK;
        rotateLeft(s);
      }
    }
    delete_case6(node);
  }

  void delete_case6(Node *node) {
    Node *s = node->sibling();

    s->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) {
      s->right->color = BLACK;
      rotateLeft(node->parent);
    } else {
      s->left->color = BLACK;
      rotateRight(node->parent);
    }
  }

  void deleteOneChild(Node *node) {
    Node *child = this->isLeaf(node->right) ? node->left : node->right;

    this->replaceNode(node, child);
    if (node->color == BLACK) {
      if (child->color == RED) {
        child->color = BLACK;
      } else {
        delete_case1(child);
      }
    }

    //std::cout << "deleting :" << node->value << std::endl;
    delete node;
    node = NULL;
  }

// walk through the tree and find the correct spot to insert the new node
  bool addNewNode(Node *node, int value) {
    //std::cout << "addNewNode(" << value << ")" << std::endl;

    if (node->value == value) {
      //std::cout << "Error:" << node->value << "==" << value << std::endl;
      return false;
    } else if (value < node->value) {
      //std::cout << value << " < " << node->value << std::endl;
      // go left
      if (node->left->isNil) {
        //std::cout << "Left one is a NilNode" << std::endl;
        // Create new Node
        Node* newNode = new Node(value);
        node->left = newNode;
        newNode->parent = node;
        this->createNilLeaves(newNode);
        //handle Color
        this->insert_case1(newNode);
        return true;
      } else {
        return this->addNewNode(node->left, value);
      }
    } else if (value > node->value) {
      //std::cout << value << " > " << node->value << std::endl;
      // go right
      if (node->right->isNil) {
        //std::cout << "Right one is a NilNode" << std::endl;
        // Create new Node
        Node* newNode = new Node(value);
        node->right = newNode;
        newNode->parent = node;
        this->createNilLeaves(newNode);
        //handle Color
        this->insert_case1(newNode);
        return true;
      } else {
        return this->addNewNode(node->right, value);
      }
    }
    return false;
  }

  void printNodeCounter() {
    std::cout << "Count of Nodes: " << this->nodeCounter << std::endl;
  }

  void padding(char ch, int n) {
    int i;
    for (i = 0; i < n; i++)
      putchar(ch);
  }

  void structure(Node *root, int level) {
    int i;

    if (root == NULL) {
      padding('\t', level);
      puts("~");
    } else {
      this->structure(root->right, level + 1);
      padding('\t', level);
      char color = (root->color) ? 'B' : 'R';
      std::cout << root->value << "(" << color << ")" << std::endl;
      this->structure(root->left, level + 1);
    }
  }

  void print() {
    std::cout << std::endl;
    std::cout << std::endl;
    this->structure(this->getRootPtr(), 0);
    std::cout << std::endl;
    std::cout << std::endl;
  }

}
;

#endif
