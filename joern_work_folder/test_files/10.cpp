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

Node* add(Node* root, int x, int k) {
    if (!root) return new Node{x, 1, nullptr, nullptr};
    if (k <= size(root->l)) {
        root->l = add(root->l, x, k);
    }
    else {
        root->r = add(root->r, x, k - (size(root->l) + 1));
    }
    root->sz = size(root->l) + size(root->r) + 1;
    return root;
}

void find(Node* root) {
    if (!root) return;
    find(root->l);
    std::cout << root->x << " ";
    find(root->r);
}

int main() {
    std::ios_base::sync_with_stdio(0), std::cin.tie(0);
    Node* root = nullptr;
    int i, x;
    while (true) {
        if (!(std::cin >> i)) break;
        std::cin >> x;
        root = add(root, x, i);
    }
    find(root);
    return 0;
}