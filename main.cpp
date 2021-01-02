#include <iostream>

using namespace std;

struct Node {
    int key;
    Node *parent;
    Node *left;
    Node *right;
    char color; // 0 is black, 1 is red
};

typedef Node *NodePointer;

class RBStablo {
private:
    NodePointer root;
    NodePointer T_nill;

    void inOrderHelper(NodePointer node) {
        if (node != T_nill) {
            inOrderHelper(node->left);
            cout << node->key << " ";
            inOrderHelper(node->right);
        }
    }

    // fix the red-black tree
    void RBInsertFixUp(NodePointer z){
        NodePointer temp;
        while (z->parent->color == 1) {
            if (z->parent == z->parent->parent->right) {
                temp = z->parent->parent->left;
                if (temp->color == 1) {
                    temp->color = 0; // Case 1
                    z->parent->color = 0; // Case 1
                    z->parent->parent->color = 1; // Case 1
                    z = z->parent->parent; // Case 1
                } else {
                    if (z == z->parent->left) {
                        z = z->parent; // Case 2
                        rightRotate(z); // Case 2
                    }
                    z->parent->color = 0; // Case 3
                    z->parent->parent->color = 1; // Case 3
                    leftRotate(z->parent->parent); // Case 3
                }
            } else {
                temp = z->parent->parent->right;

                if (temp->color == 1) {
                    temp->color = 0; // Else case 1
                    z->parent->color = 0; // Else case 1
                    z->parent->parent->color = 1; // Else case 1
                    z = z->parent->parent; // Else case 1
                } else {
                    if (z == z->parent->right) {
                        z = z->parent; // Else case 2
                        leftRotate(z); // Else case 2
                    }
                    z->parent->color = 0; // Else case 3
                    z->parent->parent->color = 1; // Else case 3
                    rightRotate(z->parent->parent); // Else case 3
                }
            }
            if (z == root) break;
        }
        root->color = 0;
    }

    // Fixing the RBStablo after delete
    void RBDeleteFixup(NodePointer x) {
        NodePointer w;
        while (x != this->root && x->color == 0) {
            if (x == x->parent->left) {
                w = x->parent->right;
                if (w->color == 1) {
                    w->color = 0; // Case 1
                    x->parent->color = 1; // Case 1
                    leftRotate(x->parent); // Case 1
                    w = x->parent->right; // Case 1
                }

                if (w->left->color == 0 && w->right->color == 0) {
                    w->color = 1; // Case 2
                    x = x->parent; // Case 2
                } else {
                    if (w->right->color == 0) {
                        w->left->color = 0;// Case 3
                        w->color = 1; // Case 3
                        rightRotate(w); // Case 3
                        w = x->parent->right; // Case 3
                    }

                    w->color = x->parent->color; // Case 4
                    x->parent->color = 0; // Case 4
                    w->right->color = 0; // Case 4
                    leftRotate(x->parent); // Case 4
                    x = this->root; // Case 4
                }
            } else {
                w = x->parent->left;
                if (w->color == 1) {
                    w->color = 0; // Else case 1
                    x->parent->color = 1; // Else case 1
                    rightRotate(x->parent); // Else case 1
                    w = x->parent->left; // Else case 1
                }

                if (w->right->color == 0 && w->left->color == 0) {
                    w->color = 1; // Else case 2
                    x = x->parent; // Else case 2
                } else {
                    if (w->left->color == 0) {
                        w->right->color = 0; // Else case 3
                        w->color = 1; // Else case 3
                        leftRotate(w); // Else case 3
                        w = x->parent->left; // Else case 3
                    }

                    w->color = x->parent->color; // Else case 4
                    x->parent->color = 0; // Else case 4
                    w->left->color = 0; // Else case 4
                    rightRotate(x->parent); // Else case 4
                    x = this->root; // Else case 4
                }
            }
        }
        x->color = 0;
    }
public:
    RBStablo() {
        this->T_nill = new Node;
        this->T_nill->color = 0;
        this->T_nill->left = nullptr;
        this->T_nill->right = nullptr;
        this->root = T_nill;
    }

    void inorder() {
        inOrderHelper(this->root);
    }

    // Rotating left
    void leftRotate(NodePointer x) {
        NodePointer y = x->right;
        x->right = y->left;
        if (y->left != T_nill)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // Rotating right
    void rightRotate(NodePointer x) {
        NodePointer y = x->left;
        x->left = y->right;
        if (y->right != T_nill)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // Insert node
    void RBInsert(int key) {
        auto node = new Node;
        node->parent = nullptr;
        node->key = key;
        node->left = T_nill;
        node->right = T_nill;
        node->color = 1; // node must be red

        NodePointer y = nullptr;
        NodePointer x = this->root;

        while (x != T_nill) {
            y = x;
            if (node->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (node->key < y->key)
            y->left = node;
        else
            y->right = node;

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) return;

        RBInsertFixUp(node);
    }

    // Transplant
    void RBTransplant(NodePointer u, NodePointer v) {
        if (u->parent == nullptr)
            this->root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }

    // Delete node
    void RBDelete(int key) {
        NodePointer x, y, z = T_nill;
        NodePointer root_copy = this->root;

        // Because through our program we send in our key of the node we still need to first check if the node is present in the tree at all
        // Technically we can also send a whole node, to first initialize that node but it is pattern breaking
        while (root_copy != T_nill) {
            if (root_copy->key == key)
                z = root_copy;

            if (root_copy->key <= key)
                root_copy = root_copy->right;
            else
                root_copy = root_copy->left;
        }

        if (z == T_nill) {
            cout << "The key isn't present, nothing to delete!" << endl;
            return;
        }

        y = z;
        char y_original_color = y->color;
        if (z->left == T_nill) {
            x = z->right;
            RBTransplant(z, z->right);
        } else if (z->right == T_nill) {
            x = z->left;
            RBTransplant(z, z->left);
        } else {
            y = tree_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                RBTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            RBTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (y_original_color == 0) RBDeleteFixup(x);
    }

    // find the node with the tree_minimum key
    NodePointer tree_minimum(NodePointer node) {
        while (node->left != T_nill) node = node->left;
        return node;
    }
};

int main() {
    cout << "Welcome to the RED-BLACK TREE interface menu!" << endl;
    cout << "You will be given different commands that you can choose and perform different actions on the tree!" << endl;

    // Menu options, tree and node for insertion and deletion
    int menu_option = 0, node_number = 0;
    string menu_options[4] = {"Node insertion!", "Inorder output!", "Delete node", "EXIT"};
    RBStablo red_black_tree;

    cout << "If you want the tree to have an initial structure of nodes [6, 11, 10, 2, 9, 7, 5, 13, 22, 27, 36, 12, 31] type in 1 or type in anything else to omit starting input!" << endl;
    cout << "Input: ";
    cin >> menu_option;
    if(menu_option == 1) {
        // Starting tree input
        red_black_tree.RBInsert(6);
        red_black_tree.RBInsert(11);
        red_black_tree.RBInsert(10);
        red_black_tree.RBInsert(2);
        red_black_tree.RBInsert(9);
        red_black_tree.RBInsert(7);
        red_black_tree.RBInsert(5);
        red_black_tree.RBInsert(13);
        red_black_tree.RBInsert(22);
        red_black_tree.RBInsert(27);
        red_black_tree.RBInsert(36);
        red_black_tree.RBInsert(12);
        red_black_tree.RBInsert(31);
    }

    // Possible only if you put in the initial nodes
    if(menu_option == 1) {
        cout << "If you want to have a standard deletion of nodes [5, 27, 36, 12, 11] type in 1 or type in anything else to omit starting deletion!" << endl;
        cout << "Input: ";
        menu_option = 0;
        cin >> menu_option;
        if(menu_option == 1) {
            // Starting tree input
            red_black_tree.RBDelete(5);
            red_black_tree.RBDelete(27);
            red_black_tree.RBDelete(36);
            red_black_tree.RBDelete(12);
            red_black_tree.RBDelete(11);
        }
    }

    // Menu selection
    do {
        cout << endl << "Menu:" << endl;
        for(int i = 0; i < 4; ++i)
            cout << i+1 << ". " << menu_options[i] << endl;
        cin >> menu_option;
        switch(menu_option) {
            case 1:
                cout << "You have chosen node insertion!" << endl;
                cout << "Type in a key to insert" << endl;
                cin >> node_number;
                cout << "You have chosen " << node_number << "!" << endl;
                red_black_tree.RBInsert(node_number);
                break;
            case 2:
                cout << "You have chosen inorder print out!" << endl;
                red_black_tree.inorder();
                break;
            case 3:
                cout << "You have chosen deleting a node!" << endl;
                cout << "Type in a key to delete" << endl;
                cin >> node_number;
                cout << "You have chosen " << node_number << "!" << endl;
                red_black_tree.RBDelete(node_number);
                break;
            case 4:
                cout << "Hope you had fun!" << endl;
                break;
            default:
                cout << "That command is not present! Please type in different command!" << endl;
        }
    } while(menu_option != 4);

    return 0;
}
