#include <iostream>
#include <vector>

struct Node {
    int x;
    Node* l;
    Node* r;
};

Node* add(Node* root, int x) {
    if (!root) return new Node{x, nullptr, nullptr};
    if (x < root->x) root->l = add(root->l, x);
    else root->r = add(root->r, x);
    return root;
}

void find(Node* root, int x, int &c) {
    if (!root) return;
    if (x < root->x) {
        c = root->x;
        find(root->l, x, c);
    }
    else {
        find(root->r, x, c);
    }
}

int main() {
    std::ios_base::sync_with_stdio(0), std::cin.tie(0);
    Node* root = nullptr;
    int x, c;
    char y;
    while (true) {
        if (!(std::cin >> y)) break;
        std::cin >> x;
        if (y == '+') {
            root = add(root, x);
        }
        else {
            c = 0;
            find(root, x, c);
            std::cout << c << "\n";
        }
    }
    return 0;
}