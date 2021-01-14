#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Node
{
    int key; // Node value
    int degree; // Shows number of children of this node
    Node *parent; // Pointer to parent node
    Node *child; // Pointer to child node
    Node *left; // Pointer to left brother
    Node *right; // Pointer to right brother
    char mark; // Mark that shows if node has lost child node from the moment that node has become the node of some different node
    char C; // Flag for assisting with the find node function
};

typedef Node *NodePointer;

NodePointer createNode(int value) {
    NodePointer x = new Node;
    x->key = value;
    return x;
}

class FIBHeap {
private:
    int numberOfNodes; // Number of nodes
    NodePointer H;

    NodePointer MakeFIBHeap() {
        this->numberOfNodes = 0;
//        return nullptr;
        // I think we don't need this at all
         NodePointer min_node_pointer = nullptr;
         return min_node_pointer;
    }
public:
    FIBHeap() {
        this->H = MakeFIBHeap();
    }

    NodePointer getH() {
        return this->H;
    }

    int nodeNumber() {
        return this->numberOfNodes;
    }

    void fibHeapLink(NodePointer H1, NodePointer y, NodePointer z) {
        (y->left)->right = y->right;
        (y->right)->left = y->left;
        if (z->right == z)
            H1 = z;
        y->left = y;
        y->right = y;
        y->parent = z;
        if (z->child == nullptr)
            z->child = y;
        y->right = z->child;
        y->left = (z->child)->left;
        ((z->child)->left)->right = y;
        (z->child)->left = y;
        if (y->key < (z->child)->key)
            z->child = y;
        z->degree++;
    }

    // I think we can change this a bit to:
    NodePointer FIBHeapInsert(NodePointer H, NodePointer x) {
        x->degree = 0;
        x->parent = nullptr;
        x->child = nullptr;
        x->left = x;
        x->right = x;
        x->mark = 'F';
        x->C = 'N';
        if (H != nullptr) {
            (H->left)->right = x;
            x->right = H;
            x->left = H->left;
            H->left = x;
            if (x->key < H->key)
                H = x;
        }
        else H = x;
        this->numberOfNodes++;
        return H;
    }

    NodePointer FIBHeapUnion(NodePointer H1, NodePointer H2) {
        if(H1 == nullptr && H2 == nullptr)
            return nullptr;
        NodePointer temp;
        // We essentially create a new Fibonacci heap that is a mix between the two heaps and essentially a new root list
        // These are the minimum nodes, or better said the roots
        NodePointer H = H1;
        if(H == nullptr || (H2 != nullptr && (H2->key < H1->key)))
            H = H2;
        else {
            (H->left)->right = H2;
            (H2->left)->right = H;
            temp = H->left;
            H->left = H2->left;
            H2->left = temp;
        }

        this->numberOfNodes = getNumberOfNodesFromRootNode(H1) + getNumberOfNodesFromRootNode(H2);
        return H;
    }

    int getNumberOfNodesFromRootNode(NodePointer H_min) {
        if(H_min == nullptr)
            return 0;
        NodePointer copy = H_min;
        int numberOf = 0;
        do {
            copy = copy->right;
            numberOf++;
        } while (copy != H_min && copy->right != nullptr);
        return numberOf;
    }

    NodePointer FIBHeapExtractMin(NodePointer H_min) {
        NodePointer z = H_min;
        if(z != nullptr){
            auto x = z->child;

            if (x != nullptr)
            {
                NodePointer np;
                NodePointer ptr = x;
                do
                {
                    np = x->right;
                    (H_min->left)->right = x;
                    x->right = H_min;
                    x->left = H_min->left;
                    H_min->left = x;
                    if (x->key < H_min->key) H_min = x;
                    x->parent = nullptr;
                    x = np;
                }
                while (np != ptr);
            }

            H_min->left->right = H_min->right;
            H_min->right->left = H_min->left;

            if(z == z->left && z->child == nullptr) H_min = nullptr;
            else {
                H_min = z->right;
                Consolidate(H_min);
            }
            this->numberOfNodes--;
        }
        return z;


//        // H_min is the heap we are extracting from
//        NodePointer p, tempPtr, z = H_min;
//        p = z;
//        tempPtr = z;
//        if (z == nullptr)
//            return z;
//        NodePointer x, tempPtr2;
//        x = nullptr;
//        if (z->child != nullptr)
//            x = z->child;
//        if (x != nullptr){
//            tempPtr = x;
//            do {
//                tempPtr2 = x->right;
//                (H_min->left)->right = x;
//                x->right = H_min;
//                x->left = H_min->left;
//                H_min->left = x;
//                if (x->key < H_min->key)
//                    H_min = x;
//                x->parent = nullptr;
//                x = tempPtr2;
//            } while (tempPtr2 != tempPtr);
//        }
//        (z->left)->right = z->right;
//        (z->right)->left = z->left;
////        H_min = z->right;
//        if (z == z->right && z->child == nullptr)
//            H = nullptr;
//        else {
//            H_min = z->right;
//            Consolidate(H_min);
//        }
//        this->H = this->H - 1;
//        return p;
    }

    void Consolidate(NodePointer H_min) {
        int d, i;
        float f = (log(this->numberOfNodes)) / (log(2));
        int D = f;
        NodePointer A[D];
        for (i = 0; i <= D; i++) A[i] = nullptr;
        NodePointer x = H_min, y, temp;
        NodePointer temp2 = x;
        do {
            temp2 = temp2->right;
            d = x->degree;
            while (A[d] != nullptr) {
                y = A[d];
                if (x->key > y->key) {
                    temp = x;
                    x = y;
                    y = temp;
                }

                if (y == H_min) H_min = x;
                fibHeapLink(H_min, y, x);
                if (x->right == x)
                    H_min = x;
                    A[d] = nullptr;
                d++;
            }
            A[d] = x;
            x = x->right;
        } while (x != H_min);

        this->H = nullptr;

        for (int j = 0; j <= D; j++) {
            if (A[j] != nullptr) {
                A[j]->left = A[j];
                A[j]->right =A[j];
                if (this->H != nullptr) {
                    (this->H->left)->right = A[j];
                    A[j]->right = this->H;
                    A[j]->left = this->H->left;
                    this->H->left = A[j];
                    if (A[j]->key < this->H->key) this->H = A[j];
                } else this->H = A[j];
                if(this->H == nullptr)
                    this->H = A[j];
                else if (A[j]->key < this->H->key)
                    this->H = A[j];
            }
        }
    }

    int FIBHeapDecreaseKey(NodePointer H1, int x, int k) {
        NodePointer y;
        if (H1 == nullptr) {
            cout << "Heap empty" << endl;
            return 0;
        }

        // We find the node with the key
        NodePointer tempPtr = FindNode(H1, x);

        if (tempPtr == nullptr) {
            cout << "Node not found in the Heap" << endl;
            return 1;
        }

        if (tempPtr->key < k) {
            cout << "New key is greater than current key" << endl;
            return 0;
        }

        tempPtr->key = k;
        y = tempPtr->parent;

        if (y != nullptr && tempPtr->key < y->key) {
            Cut(H1, tempPtr, y);
            Cascading_cut(H1, y);
        }

        if (tempPtr->key < H->key)
            this->H = tempPtr;
        return 0;
    }


    void Display(NodePointer H) {
        NodePointer p = H;
        if (p == nullptr) {
            cout << "The Heap is Empty" << endl;
            return;
        }

        cout << "The root Nodes of Heap are: " << endl;

        do {
            cout << p->key;
            p = p->right;
            if (p != H)
                cout << "-->";
        } while (p != H && p->right != nullptr);

        cout << endl;
    }

    NodePointer FindNode(NodePointer H, int k) {
        NodePointer x = H;
        x->C = 'Y';
        NodePointer p = nullptr;

        if (x->key == k) {
            p = x;
            x->C = 'N';
            return p;
        }

        if (p == nullptr) {
            if (x->child != nullptr )
                p = FindNode(x->child, k);
            if ((x->right)->C != 'Y' )
                p = FindNode(x->right, k);
        }

        x->C = 'N';

        return p;
    }

    int FIBHeapDelete(NodePointer H1, int k) {
        NodePointer tempPtr = nullptr;
        int rez = FIBHeapDecreaseKey(H1, k, -5000);

        if (!rez)
            tempPtr = FIBHeapExtractMin(this->H);
        if (tempPtr != nullptr)
            cout << "Key Deleted" << endl;
        else
            cout << "Key not Deleted" << endl;
        return 0;
    }

    void Cut(NodePointer H1, NodePointer x, NodePointer y) {
        if (x == x->right)
            y->child = nullptr;
        (x->left)->right = x->right;
        (x->right)->left = x->left;
        if (x == y->child)
            y->child = x->right;
        y->degree = y->degree - 1;
        x->right = x;
        x->left = x;
        (H1->left)->right = x;
        x->right = H1;
        x->left = H1->left;
        H1->left = x;
        x->parent = nullptr;
        x->mark = 'F';
    }

    void Cascading_cut(NodePointer H1, NodePointer y) {
        NodePointer z = y->parent;
        if (z != nullptr)
            if (y->mark == 'F')
                y->mark = 'T';
            else {
                Cut(H1, y, z);
                Cascading_cut(H1, z);
            }
    }
};

int main()
{
    FIBHeap initial;
    FIBHeap initial2;
    NodePointer H, H2;
    H = initial.getH();
    H2 = initial2.getH();
    cout << "Welcome to the Fibonacci heap test cases!" << endl;
    cout << "Next we will have a bunch of called test cases!" << endl;
    // TEST CASES:

    // 1. INSERT TEST CASES
    // 5 nodes inputted
    // Situation when we create a root list with just x
    H = initial.FIBHeapInsert(H, createNode(1));
    // Situation when we just add new nodes to heap
    H = initial.FIBHeapInsert(H, createNode(2));
    H = initial.FIBHeapInsert(H, createNode(23));
    H = initial.FIBHeapInsert(H, createNode(15));
    // Situation when the root node changes
    H = initial.FIBHeapInsert(H, createNode(-15));

    // 2. UNION TEST CASES
    // Additional heap that is created
    H2 = initial2.FIBHeapInsert(H2, createNode(1));
    H2 = initial2.FIBHeapInsert(H2, createNode(2));
    H2 = initial2.FIBHeapInsert(H2, createNode(23));
    H2 = initial2.FIBHeapInsert(H2, createNode(15));
    // Union operation
    // Special operation when we call it one after the other
    H = initial.FIBHeapUnion(nullptr, H2);
    H = initial.FIBHeapUnion(nullptr, nullptr);
    H = initial.FIBHeapUnion(H, H2);
    H2 = initial2.FIBHeapUnion(H, H2);

    // Testing next
    H = initial.FIBHeapInsert(H, createNode(2));
    H = initial.FIBHeapInsert(H, createNode(23));
    H = initial.FIBHeapInsert(H, createNode(15));

    H = initial.FIBHeapExtractMin(H);
    initial.Display(H);
    initial2.Display(H2);

    return 0;
}