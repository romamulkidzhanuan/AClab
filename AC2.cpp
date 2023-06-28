#include <iostream>
using namespace std;

struct Rational {
    int numerator;
    int denominator;

    Rational(int num, int den) : numerator(num), denominator(den) {}
};

struct Node {
    Rational key;
    Node* parent;
    Node* left;
    Node* right;
    bool isBlack;
    int size; // кількість вузлів у піддереві

    Node(Rational k) : key(k), parent(nullptr), left(nullptr), right(nullptr), isBlack(false), size(1) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* nil; // спеціальний нульовий вузол

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void removeFixup(Node* x);
    Node* minimum(Node* x);
    Node* maximum(Node* x);
    Node* select(Node* x, int k);
    int rank(Node* x);
    void transplant(Node* u, Node* v);
    void printInOrder(Node* x);
    void updateSize(Node* x);

public:
    RedBlackTree();
    void insert(Rational key);
    void remove(Rational key);
    void printInOrder();
    int findRank(Rational key);
    Rational findKey(int rank);
};

RedBlackTree::RedBlackTree() {
    nil = new Node(Rational(0, 1));
    nil->isBlack = true;
    root = nil;
}

void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;

    // Оновлення розміру піддерев
    updateSize(x);
    updateSize(y);
}

void RedBlackTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;

    if (y->right != nil) {
        y->right->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;

    // Оновлення розміру піддерев
    updateSize(x);
    updateSize(y);
}

void RedBlackTree::insertFixup(Node* z) {
    while (z->parent->isBlack == false) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            if (y->isBlack == false) {
                z->parent->isBlack = true;
                y->isBlack = true;
                z->parent->parent->isBlack = false;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->isBlack = true;
                z->parent->parent->isBlack = false;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;

            if (y->isBlack == false) {
                z->parent->isBlack = true;
                y->isBlack = true;
                z->parent->parent->isBlack = false;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->isBlack = true;
                z->parent->parent->isBlack = false;
                leftRotate(z->parent->parent);
            }
        }
    }

    root->isBlack = true;
}

void RedBlackTree::insert(Rational key) {
    Node* z = new Node(key);
    Node* y = nil;
    Node* x = root;

    while (x != nil) {
        y = x;

        if (z->key.numerator * y->key.denominator < y->key.numerator * z->key.denominator) {
            x = x->left;
        } else {
            x = x->right;
        }

        y->size++;
    }

    z->parent = y;

    if (y == nil) {
        root = z;
    } else if (z->key.numerator * y->key.denominator < y->key.numerator * z->key.denominator) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = nil;
    z->right = nil;
    z->isBlack = false;
    z->size = 1;

    insertFixup(z);
}

void RedBlackTree::removeFixup(Node* x) {
    while (x != root && x->isBlack == true) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;

            if (w->isBlack == false) {
                w->isBlack = true;
                x->parent->isBlack = false;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->isBlack == true && w->right->isBlack == true) {
                w->isBlack = false;
                x = x->parent;
            } else {
                if (w->right->isBlack == true) {
                    w->left->isBlack = true;
                    w->isBlack = false;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = true;
                w->right->isBlack = true;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;

            if (w->isBlack == false) {
                w->isBlack = true;
                x->parent->isBlack = false;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->isBlack == true && w->left->isBlack == true) {
                w->isBlack = false;
                x = x->parent;
            } else {
                if (w->left->isBlack == true) {
                    w->right->isBlack = true;
                    w->isBlack = false;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = true;
                w->left->isBlack = true;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->isBlack = true;
}

void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void RedBlackTree::remove(Rational key) {
    Node* z = root;

    while (z != nil) {
        if (key.numerator * z->key.denominator == z->key.numerator * key.denominator) {
            break;
        } else if (key.numerator * z->key.denominator < z->key.numerator * key.denominator) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == nil) {
        return;
    }

    Node* y = z;
    Node* x;
    bool yOriginalColor = y->isBlack;

    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->isBlack;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->isBlack = z->isBlack;
    }

    delete z;

    if (yOriginalColor == true) {
        removeFixup(x);
    }
}

Node* RedBlackTree::minimum(Node* x) {
    while (x->left != nil) {
        x = x->left;
    }

    return x;
}

Node* RedBlackTree::maximum(Node* x) {
    while (x->right != nil) {
        x = x->right;
    }

    return x;
}

Node* RedBlackTree::select(Node* x, int k) {
    int r = rank(x);

    if (k == r) {
        return x;
    } else if (k < r) {
        return select(x->left, k);
    } else {
        return select(x->right, k - r);
    }
}

int RedBlackTree::rank(Node* x) {
    int r = x->left->size + 1;
    Node* y = x;

    while (y != root) {
        if (y == y->parent->right) {
            r += y->parent->left->size + 1;
        }

        y = y->parent;
    }

    return r;
}

void RedBlackTree::updateSize(Node* x) {
    x->size = x->left->size + x->right->size + 1;
}

void RedBlackTree::printInOrder(Node* x) {
    if (x != nil) {
        printInOrder(x->left);
        cout << x->key.numerator << "/" << x->key.denominator << " ";
        printInOrder(x->right);
    }
}

void RedBlackTree::printInOrder() {
    printInOrder(root);
}

int RedBlackTree::findRank(Rational key) {
    Node* x = root;
    int rank = 0;

    while (x != nil) {
        if (key.numerator * x->key.denominator == x->key.numerator * key.denominator) {
            rank += x->left->size + 1;
            break;
        } else if (key.numerator * x->key.denominator < x->key.numerator * key.denominator) {
            x = x->left;
        } else {
            rank += x->left->size + 1;
            x = x->right;
        }
    }

    return rank;
}

Rational RedBlackTree::findKey(int rank) {
    Node* x = select(root, rank - 1);
    return x->key;
}

int main() {
    RedBlackTree tree;
    tree.insert(Rational(1, 2));
    tree.insert(Rational(3, 4));
    tree.insert(Rational(2, 3));
    tree.insert(Rational(5, 6));

    cout << "In-order traversal: ";
    tree.printInOrder();
    cout << endl;

    Rational key = tree.findKey(2);
    cout << "Key with rank 2: " << key.numerator << "/" << key.denominator << endl;

    int rank = tree.findRank(Rational(3, 4));
    cout << "Rank of key 3/4: " << rank << endl;

    tree.remove(Rational(2, 3));
    cout << "In-order traversal after removing 2/3: ";
    tree.printInOrder();
    cout << endl;

    return 0;
}