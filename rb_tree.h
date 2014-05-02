#ifndef C_BBTREE_H
#define C_BBTREE_H

class Tree {
 private:
  enum Color {
    RED = 0,
    BLACK
  };

  struct Node {
    Node *parent, *left, *right;
    Color color;
    int value;
    bool isNil;

    explicit Node(int value)
        : parent(NULL),
          left(NULL),
          right(NULL),
          color(RED),
          value(value),
          isNil(false) {
    }

    Node* grandparent() const {
      return parent->parent;
    }

    Node* uncle() const {
      if (parent == grandparent()->left)
        return grandparent()->right;
      else
        return grandparent()->left;
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

  Node* rotateLeft(Node *n) {

  }

  Node* rotateRight(Node *n) {

  }

  Node *rootPtr = NULL;

  void createNilLeaf(Node *parent) {
    Node n = Node(-1);
    n.parent = parent;
    n.color = BLACK;
    n.isNil = true;
  }

 public:
  Tree()
//      : rootPtr(NULL)
 {
    std::cout << "Tree()" << std::endl;
  }

  Node* getRoot() {
    return this->rootPtr;
  }

  bool isEmpty() const {
//    std::cout << "isEmpty() returns: " << !rootPtr << std::endl;
    return !rootPtr;
  }

  bool insertValue(int value) {
    std::cout << "insertValue(" << value << ")" << std::endl;

    // check if the value to insert already exists
    if (this->search(value) != NULL) {
      return false;
    }

    if (this->rootPtr != NULL) {
      std::cout << "Value of root Node: " << this->rootPtr->value << std::endl;
    }

    Node newNode = Node(value);

    if (this->rootPtr != NULL) {
      std::cout << "Value of root Node: " << this->rootPtr->value << std::endl;
    }

    if (this->isEmpty()) {
      // Insert the first node of the tree
      // (root node)
      std::cout << "Adding root Node" << std::endl;
      newNode.color = BLACK;
      this->rootPtr = &newNode;

      // Create 2 NIL-Leaves
      for (int i = 0; i < 2; i++) {
        this->createNilLeaf(&newNode);
      }
    } else {
      // The new Node is not the Root
      if (this->addNewNode(this->rootPtr, value)) {
        std::cout << "Added new Node with value: " << value << std::endl;
      } else {
        std::cout << "Error adding new Node with value: " << value << std::endl;
      }
    }

    // this->postorder(this->rootPtr, 2);
    return true;
  }

  Node* search(int value) {
//    std::cout << "search(" << value << ")" << std::endl;
    return NULL;
  }

  bool deleteValue(int value) {
    std::cout << "deleteValue(" << value << ")" << std::endl;
    return false;
  }

  // walk through the tree and find the correct spot to insert the new node
  bool addNewNode(Node *node, int value) {
    std::cout << "addNewNode(" << value << ")" << std::endl;

    if (node->value == value) {
      return false;
    } else if (value < node->value) {
      std::cout << value << " < " << node->value << std::endl;
      // go left
      if (node->left == NULL) {
        // Create new Node
        Node newNode = Node(value);
        node->left = &newNode;
        return true;
      } else {
        this->addNewNode(node->left, value);
      }
    } else if (value > node->value) {
      std::cout << value << " > " << node->value << std::endl;

      // go right
      if (node->right == NULL) {
        // Create new Node
        Node newNode = Node(value);
        node->right = &newNode;
        return true;
      } else {
        this->addNewNode(node->right, value);
      }
    }
    return false;
  }

  void postorder(Node* p, int indent = 0) {
    //std::cout << "postorder()" << std::endl;

    if (p != NULL) {
      if (p->left)
        postorder(p->left, indent + 4);
      if (p->right)
        postorder(p->right, indent + 4);
      if (indent) {
        std::cout << std::setw(indent) << ' ';
      }
      std::cout << p->value << std::endl;
    }
  }

  int countNodes(Node *root) {
    if (root == NULL)
      return 0;
    else {
      int count = 1;
      count += countNodes(root->left);
      count += countNodes(root->right);
      return count;
    }
  }

};

#endif
