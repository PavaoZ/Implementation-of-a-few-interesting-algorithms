#include <iostream>

using namespace std;

enum Color {RED, BLACK};

template <typename type>
struct Node {
    int data;
    Color color;
    Node *left, *right, *parent;

    Node(type data) {
        this->data = data;
        left =  nullptr;
        right = nullptr;
        parent = nullptr;
        this->color = RED;
    }
    ~Node(){
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }
};

template <typename type>

class RBStablo {
    Node<type> *root;

    void RBInsertFixUp(Node<type>* &root, Node<type> *z) {
        while(z != root && z->color != BLACK && z->parent->color == RED) {
            if(z->parent == z->parent->parent->left) {
                Node<type> *y = z->parent->parent->right;
                if(y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if(z == z->parent->right){
                        z = z->parent;
                        leftRotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(root, z->parent->parent);
                }
            }
            else {
                Node<type> *y = z->parent->parent->left;
                if(y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if(z == z->parent->left){
                        z = z->parent;
                        rightRotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(root, z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void inOrderRun(Node<type>* &root) {
        if (root == nullptr) return;
        inOrderRun(root->left);
        std::cout << root->data << " ";
        inOrderRun(root->right);
    }

    void RBTransplant(Node<type>* &root, Node<type>* u, Node<type>* v) {
        if(u != nullptr && u->parent == nullptr)
            root = v;
        else if(u != nullptr && u->parent != nullptr && u == u->parent->left)
            u->parent->left = v;
        else if(u != nullptr && u->parent != nullptr)
            u->parent->right = v;

        if(v != nullptr && u != nullptr)
            v->parent = u->parent;

    }

    void RBDeleteFixup(Node<type> * &x) {
        while(x != nullptr && x != this->root && x->color == BLACK) {
            if(x->parent != nullptr && x == x->parent->left) {
                Node<type>* w = x->parent->right;
                if(w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(this->root, x->parent);
                    w = x->parent->right;
                }

                if(w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(this->root, w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(this->root, x->parent);
                    x = this->root;
                }
            } else {
                Node<type>* w = x->parent->left;
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(this->root, x->parent);
                    w = x->parent->left;
                }

                if(w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(this->root, w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(this->root, x->parent);
                    x = this->root;
                }
            }
        }
    }

    Node<type>* tree_minimum(Node<type>* poc) {
        while (poc->left != nullptr)
            poc = poc->left;
        return poc;
    }

    void inOrderHelper(Node<type>* node) {
        if (node != nullptr) {
            inOrderHelper(node->left);
            cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }

public:
    RBStablo() {
        this->root = nullptr;
    }

    ~RBStablo() {
        delete this->root;
        this->root = nullptr;
    }
    void leftRotate(Node<type> *&root, Node<type> *x) {
        Node<type> *y = x->right;

        x->right = y->left;

        if (y->left != nullptr) y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node<type> *&root, Node<type> *y) {
        Node<type> *x = y->left;

        y->left = x->right;

        if (y->left != nullptr) y->left->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr) root = x;

        else if (y == y->parent->left) y->parent->left = x;

        else y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void RBInsert(type value) {
        Node<type> *novi = new Node<type>(value);
        Node<type> *y = nullptr;
        Node<type> *x = root;

        while(x != nullptr) {
            y = x;
            if(novi->data < x->data) x = x->left;
            else x = x->right;
        }

        novi->parent = y;

        if(y == nullptr) root = novi;
        else if (novi->data < y->data) y->left = novi;
        else y->right = novi;

        novi->left = nullptr;
        novi->right = nullptr;
        novi->color = RED;

        RBInsertFixUp(this->root, novi);
    }

    void inorder() {
        inOrderHelper(this->root);
    }

    void RBDelete(type value) {
        findNode(this->root, value);
    }

    void rbDelete(Node<type>* z) {
        Node<type>* x;
        Node<type>* y = z;
        Color orig_col = y->color;
        if(z->left == nullptr) {
            x = z->right;
            RBTransplant(this->root, z, z->right);
        } else if(z->right == nullptr) {
            x = z->left;
            RBTransplant(this->root, z, z->left);
        } else {
            y = tree_minimum(z->right);
            orig_col = y->color;
            x = y->right;
            if(y->parent == z && x != nullptr) x->parent = y;
            else {
                RBTransplant(this->root, y, y->right);
                y->right = z->right;
                if(y->right != nullptr) y->right->parent = y;
            }
            RBTransplant(this->root, z, y);
            y->left  = z->left;
            if(y->left != nullptr) y->left->parent = y;
            y->color = z->color;
        }
        if(orig_col == BLACK) RBDeleteFixup(x);

        z->right = nullptr;
        z->left = nullptr;
        delete z;
    }

    void findNode(Node<type> * cvor, type value) {
        if(cvor == nullptr) return;

        if(cvor->data == value) {
            rbDelete(cvor);
            return;
        }
        if(value < cvor->data) findNode(cvor->left, value);
        else findNode(cvor->right, value);
    }
};


int main() {
    RBStablo<int> stablo;

    cout << "Welcome to the RED-BLACK TREE interface menu!" << endl;
    cout << "You will be given different commands that you can choose and perform different actions on the tree!" << endl;

    // Menu options, tree and node for insertion and deletion
    int menu_option, node_number = 0;
    string menu_options[4] = {"Node insertion!", "Inorder output!", "Delete node", "EXIT"};
    RBStablo<int> red_black_tree;

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
        int menu_option2;
        cin.clear();
        cin.ignore();
        cin >> menu_option2;
        if(menu_option2 == 1) {
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
        cin.clear();
        cin.ignore();
        cin >> menu_option;
        switch(menu_option) {
            case 1:
                cout << "You have chosen node insertion!" << endl;
                cout << "Type in a key to insert" << endl;
                cin.clear();
                cin.ignore();
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
                cin.clear();
                cin.ignore();
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
