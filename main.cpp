#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template <typename type>

struct Node {
    int key; // Node value
    int degree; // Shows number of children of this node
    Node<type>* parent; // Pointer to parent node
    Node<type>* child; // Pointer to child node
    Node<type>* left; // Pointer to left brother
    Node<type>* right; // Pointer to right brother
    bool mark; // Mark that shows if node has lost child node from the moment that node has become the node of some different node
    bool checked; // Flag for assisting with the find node function

    Node(type value){
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        child = nullptr;
        degree = 0;
        key = value;
        mark = false;
        checked = false;
    }
};

template <typename type>

class FIBHeap {
private:
    int n;
    Node<type> *min;

    // Help function used for various operations
    Node<type>* FindNode(Node<type> *y, int k) {
        Node<type>* x = y;
        x->checked = true;
        Node<type> *pointer = nullptr;
        if (x->key == k){
            pointer = x;
            x->checked = false;
            return pointer;
        }

        if (x->child != nullptr) {
            auto z = FindNode(x->child, k);
            if(z) pointer = z;
        }

        if (!x->right->checked) {
            auto z = FindNode(x->right, k);
            if (z) pointer = z;
        }

        x->checked = false;
        return pointer;
    }

public:
    FIBHeap() {
        n = 0;
        min = nullptr;
    }

    void FIBHeapInsert(Node<type>* x){
        x->degree = 0;
        x->parent = nullptr;
        x->mark = false;

        if (min == nullptr) {
            x->left = x;
            x->right = x;
            min = x;
        } else {
            min->left->right = x;
            x->right = min;
            x->left = min->left;
            min->left = x;
            if(x->key < min->key) min = x;
        }

        n++;
    }

    FIBHeap FIBHeapUnion(FIBHeap h1, FIBHeap h2){
        FIBHeap h3 = FIBHeap();
        h3.min = h1.min;

        if(h2.min != nullptr && h3.min != nullptr) {
            (h3.min)->left->right = h2.min;
            (h2.min)->left->right = h3.min;
            Node<type> *np = (h3.min)->left;
            (h3.min)->left = (h2.min)->left;
            (h2.min)->left = np;
        }

        if(h1.min == nullptr || (h2.min != nullptr && h2.min->key < h1.min->key))
            h3.min = h2.min;

        h3.n = h1.n + h2.n;
        return h3;
    }


    Node<type>* FIBHeapExtractMin(){
        Node<type>* z = this->min;
        if(z != nullptr){
            auto x = z->child;

            if (x != nullptr)
            {
                Node<type> *np;
                Node<type> *ptr = x;
                do
                {
                    np = x->right;
                    (min->left)->right = x;
                    x->right = min;
                    x->left = min->left;
                    min->left = x;
                    if (x->key < min->key) min = x;
                    x->parent = nullptr;
                    x = np;
                }
                while (np != ptr);
            }

            min->left->right = min->right;
            min->right->left = min->left;

            if(z == z->left && z->child == nullptr) min = nullptr;
            else {
                min = z->right;
                Consolidate();
            }
            n--;
        }
        return z;
    }


    void Consolidate() {
        int D = log2(this->n) + 1;
        std::vector<Node<type>*> A(D, nullptr);
        Node<type>* x = this->min;
        do {
            int d = x->degree;
            while (A[d] != nullptr){
                Node<type>* y = A[d];
                if(x->key > y->key){
                    auto b = x;
                    x = y;
                    y = b;
                }
                if (y == this->min)
                    this->min = x;
                gHeapLink(y, x);
                if (x->right == x)
                    this->min = x;
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            x = x->right;
        } while (x != this->min);

        this->min = nullptr;
        for (int j = 0; j < D; j++){
            if (A[j] != nullptr){
                A[j]->left = A[j];
                A[j]->right =A[j];
                if (this->min != nullptr){
                    (this->min->left)->right = A[j];
                    A[j]->right = this->min;
                    A[j]->left = this->min->left;
                    this->min->left = A[j];
                    if (A[j]->key < this->min->key) this->min = A[j];
                }
                else
                    this->min = A[j];
                if(this->min == nullptr)
                    this->min = A[j];
                else if(A[j]->key < this->min->key)
                    this->min = A[j];
            }
        }
    }

    void gHeapLink(Node<type>* y, Node<type>* z){
        (y->left)->right = y->right;
        (y->right)->left = y->left;
        if (z->right == z) this->min = z;

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

    void FIBHeapDecreaseKey(type x, type k)
    {
        if (this->min == nullptr){
            std::cout << "Empty heap. " << std::endl;
            return;
        }
        Node<type>* nodeThatWeFound = FindNode(this->min, x);
        // nullptr, null, etc... defaults to false
        if (!nodeThatWeFound) {
            std::cout << "That node is not in heap. "<< std::endl;
            return;
        }
        if (nodeThatWeFound->key < k) {
            std::cout << "Input key bigger than current key. " <<std::endl;
            return;
        }

        nodeThatWeFound->key = k;
        Node<type>* y = nodeThatWeFound->parent;

        if (y != nullptr && nodeThatWeFound->key < y->key){
            Cut(nodeThatWeFound, y);
            Cascading_cut(y);
        }

        if (nodeThatWeFound->key < this->min->key)
            this->min = nodeThatWeFound;

    }

    void Cut(Node<type>* x, Node<type>* y)
    {
        if (x == x->right) y->child = nullptr;
        x->left->right = x->right;
        x->right->left = x->left;

        if (x == y->child) y->child = x->right;
        y->degree--;

        x->right = x; x->left = x;

        min->left->right = x;
        x->right = min;
        x->left = min->left;
        min->left = x;

        x->parent = nullptr;
        x->mark = false;
    }

    void Cascading_cut(Node<type>* y)
    {
        Node<type>* z = y->parent;
        if (z != nullptr){
            if (!y->mark) y->mark = true;
            else {
                Cut(y, z);
                Cascading_cut(z);
            }
        }
    }

    void FIBHeapDelete(type k) {
        Node<type>* np;

        // -5000 replaces -infinity from book
        FIBHeapDecreaseKey(k, -5000);
        np = FIBHeapExtractMin();

        if (np != nullptr)
            cout << "Key deleted!" << endl;
        else
            cout << "Key not deleted." << endl;
    }

};

int main() {
    // Input test
    // newly initialized empty heap
    FIBHeap<int> testHeap = FIBHeap<int>();
    auto test_node1 = new Node<int>(5);
    auto test_node2 = new Node<int>(22);
    auto test_node3 = new Node<int>(24);
    auto test_node4 = new Node<int>(23);

    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);
    testHeap.FIBHeapInsert(test_node3);
    testHeap.FIBHeapInsert(test_node4);

    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;
    delete test_node3;
    test_node3 = nullptr;
    delete test_node4;
    test_node4 = nullptr;

    // Union test with both heaps full
    testHeap = FIBHeap<int>();
    test_node1 = new Node<int>(5);
    test_node2 = new Node<int>(22);
    test_node3 = new Node<int>(24);
    test_node4 = new Node<int>(23);
    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);
    testHeap.FIBHeapInsert(test_node3);
    testHeap.FIBHeapInsert(test_node4);

    FIBHeap<int> testHeap2 = FIBHeap<int>();
    testHeap2.FIBHeapInsert(test_node1);
    testHeap2.FIBHeapInsert(test_node2);
    testHeap2.FIBHeapInsert(test_node3);
    testHeap2.FIBHeapInsert(test_node4);

    FIBHeap<int> testHeap3 = testHeap.FIBHeapUnion(testHeap, testHeap2);
    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;
    delete test_node3;
    test_node3 = nullptr;
    delete test_node4;
    test_node4 = nullptr;

    // Union test when not both are full
    testHeap = FIBHeap<int>();
    test_node1 = new Node<int>(25);
    test_node2 = new Node<int>(12);
    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);

    testHeap2 = FIBHeap<int>();

    testHeap3 = testHeap.FIBHeapUnion(testHeap, testHeap2);

    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;

    testHeap = FIBHeap<int>();
    testHeap2 = FIBHeap<int>();
    testHeap3 = testHeap.FIBHeapUnion(testHeap, testHeap2);

    // Test delete
    testHeap = FIBHeap<int>();
    test_node1 = new Node<int>(5);
    test_node2 = new Node<int>(22);
    test_node3 = new Node<int>(24);
    test_node4 = new Node<int>(23);
    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);
    testHeap.FIBHeapInsert(test_node3);
    testHeap.FIBHeapInsert(test_node4);

    testHeap.FIBHeapDelete(5);

    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;
    delete test_node3;
    test_node3 = nullptr;
    delete test_node4;
    test_node4 = nullptr;

    // Test extract minimum
    testHeap = FIBHeap<int>();
    test_node1 = new Node<int>(5);
    test_node2 = new Node<int>(22);
    test_node3 = new Node<int>(24);
    test_node4 = new Node<int>(23);
    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);
    testHeap.FIBHeapInsert(test_node3);
    testHeap.FIBHeapInsert(test_node4);

    // Deleting in a row
    Node<int> *node = testHeap.FIBHeapExtractMin();
    cout << endl << "Extracted key: " << node->key << "!" << endl;
    node = testHeap.FIBHeapExtractMin();
    cout << endl << "Extracted key: " << node->key << "!" << endl;

    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;
    delete test_node3;
    test_node3 = nullptr;
    delete test_node4;
    test_node4 = nullptr;

    // Test decrease key
    testHeap = FIBHeap<int>();
    testHeap.FIBHeapDecreaseKey(232, 1);

    test_node1 = new Node<int>(5);
    test_node2 = new Node<int>(22);
    test_node3 = new Node<int>(24);
    test_node4 = new Node<int>(23);
    testHeap.FIBHeapInsert(test_node1);
    testHeap.FIBHeapInsert(test_node2);
    testHeap.FIBHeapInsert(test_node3);
    testHeap.FIBHeapInsert(test_node4);

    // Heap not empty but key not present
    testHeap.FIBHeapDecreaseKey(232, 1);

    // Larger key
    testHeap.FIBHeapDecreaseKey(5, 1232323);

    // Normal case
    testHeap.FIBHeapDecreaseKey(5, 1);

    delete test_node1;
    test_node1 = nullptr;
    delete test_node2;
    test_node2 = nullptr;
    delete test_node3;
    test_node3 = nullptr;
    delete test_node4;
    test_node4 = nullptr;

    return 0;
}