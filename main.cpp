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
        Node<type>* z = min;
        if(z != nullptr){
            auto x = z->child;

            // ubacivanje djece
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
            n = n - 1;
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

void testInsert() {
    FIBHeap<int> fg = FIBHeap<int>();
    // insert u praznu gomilu
    auto novi = new Node<int>(5);
    fg.FIBHeapInsert(novi);

    // insert u nepraznu gomilu
    auto novi2 = new Node<int>(2);
    fg.FIBHeapInsert(novi2);
    delete novi;
    novi = nullptr;
    delete novi2;
    novi2 = nullptr;
}

void testUnion2Pune() {
    FIBHeap<int> fg1 = FIBHeap<int>();
    auto novi1 = new Node<int>(5);
    auto novi2 = new Node<int>(2);
    auto novi3 = new Node<int>(6);
    auto novi4 = new Node<int>(7);
    fg1.FIBHeapInsert(novi1);
    fg1.FIBHeapInsert(novi2);

    FIBHeap<int> fg2 = FIBHeap<int>();
    fg2.FIBHeapInsert(novi3);
    fg2.FIBHeapInsert(novi3);

    FIBHeap<int> fg3 = fg1.FIBHeapUnion(fg1, fg2);
    delete novi1;
    delete novi2;
    delete novi3;
    delete novi4;
}

void testUnion1Prazna() {
    FIBHeap<int> fg1 = FIBHeap<int>();
    auto novi1 = new Node<int>(5);
    auto novi2 = new Node<int>(2);
    fg1.FIBHeapInsert(novi1);
    fg1.FIBHeapInsert(novi2);

    FIBHeap<int> fg2 = FIBHeap<int>();

    FIBHeap<int> fg3 = fg1.FIBHeapUnion(fg1, fg2);
    delete novi1;
    delete novi2;
}

void testUnion2Prazne() {
    FIBHeap<int> fg1 = FIBHeap<int>();
    FIBHeap<int> fg2 = FIBHeap<int>();
    FIBHeap<int> fg3 = fg1.FIBHeapUnion(fg1, fg2);
}

void testExtractMin() {
    auto novi1 = new Node<int>(1);
    auto novi2 = new Node<int>(2);
    auto novi3 = new Node<int>(3);
    auto novi4 = new Node<int>(4);
    auto novi5 = new Node<int>(5);
    FIBHeap<int> fg = FIBHeap<int>();
    fg.FIBHeapInsert(novi1);
    fg.FIBHeapInsert(novi2);
    fg.FIBHeapInsert(novi3);
    fg.FIBHeapInsert(novi4);
    fg.FIBHeapInsert(novi5);

    // extract min kad se nema djece
    Node<int>* b = fg.FIBHeapExtractMin();

    // extract min kad se ima djece
    Node<int>* c = fg.FIBHeapExtractMin();

    delete novi1;
    delete novi2;
    delete novi3;
    delete novi4;
    delete novi5;
}

void testDecreaseKey() {
    auto novi0 = new Node<int>(11);
    auto novi1 = new Node<int>(232);
    auto novi2 = new Node<int>(23);
    auto novi3 = new Node<int>(24);
    auto novi4 = new Node<int>(36);
    auto novi5 = new Node<int>(37);
    auto novi6 = new Node<int>(9);

    FIBHeap<int> fg = FIBHeap<int>();

    // kada je gomila prazna
    fg.FIBHeapDecreaseKey(5123, 2);

    fg.FIBHeapInsert(novi0);
    fg.FIBHeapInsert(novi1);
    fg.FIBHeapInsert(novi2);
    fg.FIBHeapInsert(novi3);
    fg.FIBHeapInsert(novi4);
    fg.FIBHeapInsert(novi5);
    fg.FIBHeapInsert(novi6);

    // kada nema kljuca
    fg.FIBHeapDecreaseKey(5123, 2);
    // kada je kljuc veci od trazenog
    fg.FIBHeapDecreaseKey(2, 232323);

    // slucaj kada treba smanjiti key
    Node<int>* a = fg.FIBHeapExtractMin();
//    std::cout << "Izbaceni: " << a->key << std::endl; // izbacuje se kec, zatim se decrease-a 9 na 1
    fg.FIBHeapDecreaseKey(4, 1);
    fg.FIBHeapDecreaseKey(9, 0);

    delete novi0;
    delete novi1;
    delete novi2;
    delete novi3;
    delete novi4;
    delete novi5;
    delete novi6;
}

void testDeleteKey(){
    auto novi0 = new Node<int>(1);
    auto novi1 = new Node<int>(2);
    auto novi2 = new Node<int>(3);
    auto novi3 = new Node<int>(4);
    auto novi4 = new Node<int>(6);
    auto novi5 = new Node<int>(7);
    auto novi6 = new Node<int>(9);

    FIBHeap<int> fg = FIBHeap<int>();

    fg.FIBHeapInsert(novi0);
    fg.FIBHeapInsert(novi1);
    fg.FIBHeapInsert(novi2);
    fg.FIBHeapInsert(novi3);
    fg.FIBHeapInsert(novi4);
    fg.FIBHeapInsert(novi5);
    fg.FIBHeapInsert(novi6);

    Node<int>* a = fg.FIBHeapExtractMin();
    fg.FIBHeapDelete(4);

    delete novi0;
    delete novi1;
    delete novi2;
    delete novi3;
    delete novi4;
    delete novi5;
    delete novi6;
}

int main() {
    testInsert();
    testUnion2Pune();
    testUnion1Prazna();
    testUnion2Prazne();
    testUnion2Pune();
    testExtractMin();
    testDecreaseKey();
    testDeleteKey();

    return 0;
}