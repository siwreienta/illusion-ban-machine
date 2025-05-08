#include <iostream>
#include <vector>

struct Node {
    int x;
    int sz;
    Node* l;
    Node* r;
};

int size(Node* v) {
    return (!v) ? 0 : v->sz;
}

Node* add(Node* root, int x) {
    if (!root) return new Node{x, 1, nullptr, nullptr};
    if (x < root->x) {
        root->l = add(root->l, x);
    }
    else if (x > root->x) {
        root->r = add(root->r, x);
    }
    root->sz = size(root->l) + size(root->r) + 1;
    return root;
}

int find(Node* root, int k) {
    if (size(root->l) + 1 == k) return root->x;
    if (size(root->l) >= k) return find(root->l, k);
    return find(root->r, k - (size(root->l) + 1)); 
}

int main() {
    std::ios_base::sync_with_stdio(0), std::cin.tie(0);
    Node* root = nullptr;
    int x;
    char y;
    while (true) {
        if (!(std::cin >> y)) break;
        std::cin >> x;
        if (y == '+') {
            root = add(root, x);
        }
        else {
            std::cout << find(root, x) << "\n";
        }
    }
    return 0;
}