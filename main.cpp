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

class FIBHeap {
private:
    int numberOfNodes; // Number of nodes
    NodePointer H;

    NodePointer MakeFIBHeap() {
        this->numberOfNodes = 0;
        return nullptr;
        // I think we don't need this at all
        // NodePointer min_node_pointer = nullptr;
        // return min_node_pointer;
    }
public:
    FIBHeap() {
        this->H = MakeFIBHeap();
    }

    int fibHeapLink(NodePointer H1, NodePointer y, NodePointer z) {
        y->left->right = y->right;
        y->right->left = y->left;
        if (z->right == z)
            H1 = z;
        y->left = y;
        y->right = y;
        y->parent = z;
        if (z->child == nullptr)
            z->child = y;
        y->right = z->child;
        y->left = z->child->left;
        z->child->left->right = y;
        z->child->left = y;
        if (y->key < z->child->key)
            z->child = y;
        z->degree++;
    }

    NodePointer Create_Node(int value) {
        NodePointer x = new Node;
        x->key = value;
        return x;
    }

    // I think we can change this a bit to:
    NodePointer FIBHeapInsert(NodePointer x) {
        x->degree = 0;
        x->parent = nullptr;
        x->child = nullptr;
        x->mark = 'F';
        if(this->H == nullptr)
            this->H = x;
        else {
            // This represents adding the element to the list of roots and becomes the left sibling of the root
            H->left->right = x;
            x->right = H;
            x->left = H->left;
            H->left = x;
            if(x->key < this->H->key) this->H = x;
        }
        this->numberOfNodes++;
    }
//    NodePointer FIBHeapInsert(NodePointer H, NodePointer x) {
//        x->degree = 0;
//        x->parent = nullptr;
//        x->child = nullptr;
//        x->left = x;
//        x->right = x;
//        x->mark = 'F';
//        x->C = 'N';
//        if (H != nullptr) {
//            H->left->right = x;
//            x->right = H;
//            x->left = H->left;
//            H->left = x;
//            if (x->key < H->key)
//                H = x;
//        }
//        else
//        {
//            H = x;
//        }
//        numberOfNodes = numberOfNodes + 1;
//        return H;
//    }

    NodePointer FIBHeapUnion(NodePointer H1, NodePointer H2) {
        NodePointer temp;
        NodePointer H = MakeFIBHeap();
        // We essentially create a new Fibonacci heap that is a mix between the two heaps and essentially a new root list
        // These are the minimum nodes, or better said the roots
        H = H1;
        H->left->right = H2;
        H2->left->right = H;
        temp = H->left;
        H->left = H2->left;
        H2->left = temp;
        return H;
    }

    NodePointer FIBHeapExtractMin(NodePointer H_min) {
        // H1 is the heap we are extracting from
        NodePointer p, x = nullptr, temp2, temp, z = H_min;
        p = z;
        temp2 = z;
        if (z == nullptr) return z;

        // We set the initial value of x and we don't need this
        // x = nullptr;
        // We initialize the first child because we need to go through all the children x of z, with a check of nullptr
        if (z->child != nullptr) x = z->child;

        if(x != nullptr) {
            temp2 = x;
            do {
                temp = x->right;
                H_min->left->right = x;
                x->right = H_min;
                x->left = H_min->left;
                H_min->left = x;
                if (x->key < H_min->key)
                    H_min = x;
                x->parent = nullptr;
                x = temp;
            } while (temp != temp2);
        }

        z->left->right = z->right;
        z->right->left = z->left;
        H_min = z->right;
        if (z == z->right && z->child == nullptr)
            this->H = nullptr;
        else {
            H_min = z->right;
            Consolidate(H_min);
        }

        this->numberOfNodes--;
        return p;
    }

    int Consolidate(NodePointer H_min) {
        int d, i;
        float f = (log(this->numberOfNodes)) / (log(2));
        int D = f;
        NodePointer A[D];
        for (i = 0; i <= D; i++) A[i] = nullptr;
        NodePointer x = H_min;
        NodePointer y, temp;
        NodePointer pt = x;
        do {
            pt = pt->right;
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
                if (x->right == x) H_min = x;
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

    int Display(NodePointer H) {
        NodePointer p = H;
        if (p == NULL)
        {
            cout<<"The Heap is Empty"<<endl;
            return 0;
        }
        cout<<"The root Nodes of Heap are: "<<endl;
        do
        {
            cout<<p->key;
            p = p->right;
            if (p != H)
            {
                cout<<"-->";
            }
        }
        while (p != H && p->right != NULL);
        cout<<endl;
    }

    NodePointer Find(NodePointer H, int k) {
        NodePointer x = H;
        x->C = 'Y';
        NodePointer p = NULL;
        if (x->key == k)
        {
            p = x;
            x->C = 'N';
            return p;
        }
        if (p == NULL)
        {
            if (x->child != NULL )
                p = Find(x->child, k);
            if ((x->right)->C != 'Y' )
                p = Find(x->right, k);
        }
        x->C = 'N';
        return p;
    }

    int FIBHeapDecreaseKey(NodePointer H1, int x, int k) {
        NodePointer y;
        if (H1 == NULL)
        {
            cout<<"The Heap is Empty"<<endl;
            return 0;
        }
        NodePointer ptr = Find(H1, x);
        if (ptr == NULL)
        {
            cout<<"Node not found in the Heap"<<endl;
            return 1;
        }
        if (ptr->key < k)
        {
            cout<<"Entered key greater than current key"<<endl;
            return 0;
        }
        ptr->key = k;
        y = ptr->parent;
        if (y != NULL && ptr->key < y->key)
        {
            Cut(H1, ptr, y);
            Cascading_cut(H1, y);
        }
        if (ptr->key < H->key)
            H = ptr;
        return 0;
    }
    int FIBHeapDelete(NodePointer H1, int k) {
        NodePointer np = NULL;
        int t;
        t = FIBHeapDecreaseKey(H1, k, -5000);
        if (!t)
            np = FIBHeapExtractMin(H);
        if (np != NULL)
            cout<<"Key Deleted"<<endl;
        else
            cout<<"Key not Deleted"<<endl;
        return 0;
    }

    int Cut(NodePointer H1, NodePointer x, NodePointer y) {
        if (x == x->right)
            y->child = NULL;
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
        x->parent = NULL;
        x->mark = 'F';
    }
    int Cascading_cut(NodePointer H1, NodePointer y) {
        NodePointer z = y->parent;
        if (z != NULL)
        {
            if (y->mark == 'F')
            {
                y->mark = 'T';
            }
            else
            {
                Cut(H1, y, z);
                Cascading_cut(H1, z);
            }
        }
    }
};

int main()
{
    cout << "Welcome to the Fibonacci heap test cases!" << endl;
    // Test cases

    return 0;
}