#include <iostream>

using namespace std;

struct Node {
    int key; // holds the key
    Node *parent; // pointer to the parent
    Node *left; // pointer to left child
    Node *right; // pointer to right child
    // moze ili int ili char
    char color; // 1 -> Red, 0 -> Black
};

typedef Node *NodePtr;

class RBStablo {
private:
    NodePtr root;
    NodePtr T_nill;

    // initializes the nodes with appropriate values
    // all the pointers are set to point to the null pointer
    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->key = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }

    void preOrderHelper(NodePtr node) {
        if (node != T_nill) {
            cout<<node->key<<" ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(NodePtr node) {
        if (node != T_nill) {
            inOrderHelper(node->left);
            cout<<node->key<<" ";
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(NodePtr node) {
        if (node != T_nill) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout<<node->key<<" ";
        }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
        if (node == T_nill || key == node->key) {
            return node;
        }

        if (key < node->key) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // fix the red-black tree
    void fixInsert(NodePtr k){
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // uncle
                if (u->color == 1) {
                    // case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        // case 3.2.2
                        k = k->parent;
                        rightRotate(k);
                    }
                    // case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right; // uncle

                if (u->color == 1) {
                    // mirror case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        // mirror case 3.2.2
                        k = k->parent;
                        leftRotate(k);
                    }
                    // mirror case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    void printHelper(NodePtr root, string indent, bool last) {
        // print the tree structure on the screen
        if (root != T_nill) {
            cout<<indent;
            if (last) {
                cout<<"R----";
                indent += "     ";
            } else {
                cout<<"L----";
                indent += "|    ";
            }

            string sColor = root->color?"RED":"BLACK";
            cout<<root->key<<"("<<sColor<<")"<<endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
        // cout<<root->left->key<<endl;
    }

public:
    RBStablo() {
        T_nill = new Node;
        T_nill->color = 0;
        T_nill->left = nullptr;
        T_nill->right = nullptr;
        root = T_nill;
    }

    // Pre-Order traversal
    // Node->Left Subtree->Right Subtree
    void preorder() {
        preOrderHelper(this->root);
    }

    // In-Order traversal
    // Left Subtree -> Node -> Right Subtree
    void inorder() {
        inOrderHelper(this->root);
    }

    // Post-Order traversal
    // Left Subtree -> Right Subtree -> Node
    void postorder() {
        postOrderHelper(this->root);
    }

    // search the tree for the key k
    // and return the corresponding node
    NodePtr searchTree(int k) {
        return searchTreeHelper(this->root, k);
    }

    // find the node with the minimum key
    NodePtr minimum(NodePtr node) {
        while (node->left != T_nill) {
            node = node->left;
        }
        return node;
    }

    // find the node with the maximum key
    NodePtr maximum(NodePtr node) {
        while (node->right != T_nill) {
            node = node->right;
        }
        return node;
    }

    // find the successor of a given node
    NodePtr successor(NodePtr x) {
        // if the right subtree is not null,
        // the successor is the leftmost node in the
        // right subtree
        if (x->right != T_nill) {
            return minimum(x->right);
        }

        // else it is the lowest ancestor of x whose
        // left child is also an ancestor of x.
        NodePtr y = x->parent;
        while (y != T_nill && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    // find the predecessor of a given node
    NodePtr predecessor(NodePtr x) {
        // if the left subtree is not null,
        // the predecessor is the rightmost node in the
        // left subtree
        if (x->left != T_nill) {
            return maximum(x->left);
        }

        NodePtr y = x->parent;
        while (y != T_nill && x == y->left) {
            x = y;
            y = y->parent;
        }

        return y;
    }

    // rotating left
    void leftRotate(NodePtr x) {
        NodePtr y = x->right; // set y
        x->right = y->left; // turn y's subtree into x's right subtree
        if (y->left != T_nill)
            y->left->parent = x;
        y->parent = x->parent; // link x's parent to y
        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x; // put x on y's left
        x->parent = y;
    }

    // rotating right
    void rightRotate(NodePtr x) {
        NodePtr y = x->left; // set y
        x->left = y->right; // turn y's subtree into x's left subtree
        if (y->right != T_nill)
            y->right->parent = x;
        y->parent = x->parent; // link x's parent to y
        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x; // put x on y's right
        x->parent = y;
    }

    // insert the key to the tree in its appropriate position
    // and fix the tree
    void insert(int key) {
        // Ordinary Binary Search Insertion
        NodePtr node = new Node;
        node->parent = nullptr;
        node->key = key;
        node->left = T_nill;
        node->right = T_nill;
        node->color = 1; // new node must be red

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != T_nill) {
            y = x;
            if (node->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        // y is parent of x
        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }

        // if new node is a root node, simply return
        if (node->parent == nullptr){
            node->color = 0;
            return;
        }

        // if the grandparent is null, simply return
        if (node->parent->parent == nullptr) {
            return;
        }

        // Fix the tree
        fixInsert(node);
    }

    NodePtr getRoot(){
        return this->root;
    }

    // delete the node from the tree
//    void deleteNode(int key) {
//        deleteNodeHelper(this->root, key);
//    }

    // print the tree structure on the screen
    void prettyPrint() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }
};

int main() {
    cout << "Welcome to the RED-BLACK TREE interface menu!" << endl;
    cout << "You will be given different commands that you can choose and perform different actions on the tree!" << endl;
    // Menu options and tree
    int menu_option = 0;
    string menu_options[3] = {"Node insertion!", "INORDER output!", "EXIT"};
    RBStablo red_black_tree;

    cout << "If you want the tree to have an initial structure of elements [6, 11, 10, 2, 9, 7, 5, 13, 22, 27, 36, 12, 31] type in 1 or anything else to omit starting input!" << endl;
    cout << "Input: ";
    cin >> menu_option;
    if(menu_option == 1) {
        // Starting tree input
        red_black_tree.insert(6);
        red_black_tree.insert(11);
        red_black_tree.insert(10);
        red_black_tree.insert(2);
        red_black_tree.insert(9);
        red_black_tree.insert(7);
        red_black_tree.insert(5);
        red_black_tree.insert(13);
        red_black_tree.insert(22);
        red_black_tree.insert(27);
        red_black_tree.insert(36);
        red_black_tree.insert(12);
        red_black_tree.insert(31);
    }

    // Menu selection
    do {
        cout << "Menu:" << endl;
        for(int i = 0; i < 3; ++i)
            cout << i+1 << ". " << menu_options[i] << endl;
        cin >> menu_option;
        switch(menu_option) {
            case 1:
                cout << "You have chosen node insertion!" << endl;
                int node_number;
                cout << "Type in a key to insert" << endl;
                cin >> node_number;
                cout << "You have chosen " << node_number << "!" << endl;
                red_black_tree.insert(node_number);
                break;
            case 2:
                cout << "You have chosen inorder print out!" << endl;
                break;
            case 3:
                cout << "Hope you had fun!" << endl;
                break;
            default:
                cout << "That command is not present! Please type in different command!" << endl;
        }
    } while(menu_option != 0);
//    RBStablo bst;
//    bst.deleteNode(25);
//    bst.prettyPrint();
    return 0;
}
