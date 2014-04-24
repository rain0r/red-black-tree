#ifndef C_BBTREE_H
#define C_BBTREE_H

class Tree {
private:
	enum Color {
		RED = 0, BLACK
	};

	struct Node {
		Node*parent, *left, *right;
		Color color;
		int value;
		bool isNil;

		explicit Node(int value) :
				parent(NULL), left(NULL), right(NULL), color(RED), value(value), isNil(
						false) {
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

	bool is_left_child(const Node *node) {
		if (node->parent == NULL)
			return false;
		else
			return node->parent->left == node;
	}

	static bool is_right_child(const Node *node) {
		if (node->parent == NULL)
			return false;
		else
			return node->parent->right == node;
	}

	Node* rotate_left(Node *n) {

	}

	Node* rotate_right(Node*n) {

	}

	Node *root;

	void createNilLeaf(Node *parent) {
		Node n = Node(-1);
		n.parent = parent;
		n.color = BLACK;
		n.isNil = true;
	}

public:
//	Tree() :
//			root(NULL) {
//	}

	bool is_empty() const {
		return !root;
	}

	bool insertValue(int value) {
		if (this->search(value) != NULL) {
			return false;
		}

		Node n = Node(value);
		n.isNil = false;
		n.right = NULL;
		n.left = NULL;

		if (this->is_empty()) {
			/**
			 * Insert the first node of the tree
			 * (root node)
			 */

			n.color = BLACK;
			this->root = &n;

			/**
			 * Create 2 NIL-Leaves
			 */
			for (int i = 0; i < 2; i++) {
				this->createNilLeaf(&n);
			}
		} else {
			/**
			 * The new Node is not the Root
			 */

			Node newNode = this->findInsertPlace(this->root, value);

		}

		return true;
	}

	Node* search(int value) {
		return NULL;
	}

	bool deleteValue(int value) {
		return false;
	}

	Node findInsertPlace(Node *n, int value) {
		if (value < n->value) {
			std::cout << value << " < " << n->value << std::endl;
			/**
			 * go left
			 */

			if (n->left == NULL) {
				/**
				 * Create new Node
				 */
				Node newNode = Node(value);
				n->left = &newNode;

				return newNode;
			} else {
				this->findInsertPlace(n->left, value);
			}
		} else if (value > n->value) {
			std::cout << value << " > " << n->value << std::endl;
			/**
			 * go right
			 */

			if (n->right == NULL) {
				/**
				 * Create new Node
				 */
				Node newNode = Node(value);
				n->right = &newNode;

				return newNode;
			} else {
				this->findInsertPlace(n->right, value);
			}

		}
	}
};

#endif
