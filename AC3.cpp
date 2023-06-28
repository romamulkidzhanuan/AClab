#include <iostream>

template <typename T>
class AANode {
public:
    T key;
    int level;
    AANode<T>* left;
    AANode<T>* right;

    AANode(T value) {
        key = value;
        level = 1;
        left = nullptr;
        right = nullptr;
    }
};

template <typename T>
class AATree {
private:
    AANode<T>* root;

    AANode<T>* skew(AANode<T>* node) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->left != nullptr && node->left->level == node->level) {
            AANode<T>* leftChild = node->left;
            node->left = leftChild->right;
            leftChild->right = node;
            return leftChild;
        }

        return node;
    }

    AANode<T>* split(AANode<T>* node) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->right != nullptr && node->right->right != nullptr && node->right->right->level == node->level) {
            AANode<T>* rightChild = node->right;
            node->right = rightChild->left;
            rightChild->left = node;
            rightChild->level++;
            return rightChild;
        }

        return node;
    }

    AANode<T>* insert(AANode<T>* node, T value) {
        if (node == nullptr) {
            return new AANode<T>(value);
        }

        if (value < node->key) {
            node->left = insert(node->left, value);
        } else if (value > node->key) {
            node->right = insert(node->right, value);
        } else {
            return node;  // Duplicate keys not allowed
        }

        node = skew(node);
        node = split(node);

        return node;
    }

    AANode<T>* remove(AANode<T>* node, T value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->key) {
            node->left = remove(node->left, value);
        } else if (value > node->key) {
            node->right = remove(node->right, value);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            } else if (node->left == nullptr) {
                AANode<T>* temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                AANode<T>* temp = node;
                node = node->left;
                delete temp;
            } else {
                AANode<T>* successor = node->right;
                while (successor->left != nullptr) {
                    successor = successor->left;
                }
                node->key = successor->key;
                node->right = remove(node->right, successor->key);
            }
        }

        node = skew(node);
        node = split(node);

        return node;
    }

    void fixLevel(AANode<T>* node) {
        int level = std::min(node->left != nullptr ? node->left->level : 0, node->right != nullptr ? node->right->level : 0) + 1;
        if (level < node->level) {
            node->level = level;
            if (node->right != nullptr && level < node->right->level) {
                node->right->level = level;
            }
        }
    }

    void printInOrder(AANode<T>* node) {
        if (node == nullptr) {
            return;
        }

        printInOrder(node->left);
        std::cout << node->key << " ";
        printInOrder(node->right);
    }

public:
    AATree() {
        root = nullptr;
    }

    void insert(T value) {
        root = insert(root, value);
    }

    void remove(T value) {
        root = remove(root, value);
    }

    void print() {
        printInOrder(root);
        std::cout << std::endl;
    }
};

int main() {
    AATree<int> aaTree;
    aaTree.insert(10);
    aaTree.insert(5);
    aaTree.insert(15);
    aaTree.insert(3);
    aaTree.insert(7);
    aaTree.insert(12);
    aaTree.insert(17);
    aaTree.insert(1);
    aaTree.insert(4);
    aaTree.insert(6);
    aaTree.insert(9);
    aaTree.insert(11);
    aaTree.insert(14);
    aaTree.insert(16);
    aaTree.insert(19);

    std::cout << "Initial tree: ";
    aaTree.print();

    aaTree.remove(3);
    aaTree.remove(9);
    aaTree.remove(17);

    std::cout << "Tree after removals: ";
    aaTree.print();

    return 0;
}

