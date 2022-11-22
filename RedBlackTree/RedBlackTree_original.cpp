

#include <iostream>
#include <deque>
#include <string>

#define RED true;
#define BLACK false;

using namespace std;

class RedBlackTreeNode {
public:
    int val;
    RedBlackTreeNode* left;
    RedBlackTreeNode* right;
    bool color;
    RedBlackTreeNode* parent;

    RedBlackTreeNode() = default;
    RedBlackTreeNode(int val, bool color);
    RedBlackTreeNode(RedBlackTreeNode* h, int val, bool color);

};

class RedBlackTree {

public:
    RedBlackTreeNode* root = nullptr;
    RedBlackTreeNode* nil = nullptr;
    RedBlackTree() = default;
    RedBlackTree(int val) {
        this->root = new RedBlackTreeNode(val, false);
    }

    void show(RedBlackTreeNode* h) {
        if (h->val == NULL) return;
        show(h->left);
        cout << h->val << endl;
        show(h->right);
        return;
    }
    int compare(int l, int r) {
        if (l < r) return -1;
        if (l > r) return 1;
        if (l == r) return 0;
    }
    bool isRed(RedBlackTreeNode* h);
    RedBlackTreeNode* rotateLeft(RedBlackTreeNode* h);
    RedBlackTreeNode* rotateRight(RedBlackTreeNode* h);
    void flipColors(RedBlackTreeNode* h);
    void put(int val);

    void fixInsert(RedBlackTreeNode* new_node);

    void print();
    void printMiddle(RedBlackTreeNode* node);

    RedBlackTreeNode* put_in_l(RedBlackTreeNode* h, RedBlackTreeNode* parent, int val);
    RedBlackTreeNode* put_in_r(RedBlackTreeNode* h, RedBlackTreeNode* parent, int val);
};


void RedBlackTree::print() {
    deque<RedBlackTreeNode*> dqueue;//-使用deque实现队列
    dqueue.push_back(root);
    while (!dqueue.empty()) {
        int size = (int)dqueue.size();
        for (int i = 0; i < size; ++i) {
            RedBlackTreeNode* temp = dqueue.front();
            dqueue.pop_front();
            if (temp->left != nullptr) {
                dqueue.push_back(temp->left);
            }
            if (temp->right != nullptr) {
                dqueue.push_back(temp->right);
            }
            std::string color = temp->color ? "b: " : "r: ";
            std::string keystr = temp == nullptr ? "nullptr" : std::to_string(temp->val);
            std::cout << color << keystr << "  ";
        }
        std::cout << std::endl;
    }
}

//-打印中序遍历
void RedBlackTree::printMiddle(RedBlackTreeNode* node) {
    if (node == nullptr) {
        return;
    }
    printMiddle(node->left);
    std::string color = node->color ? "b:" : "r:";
    std::cout << color << std::to_string(node->val) << "  ";
    printMiddle(node->right);
}




RedBlackTreeNode::RedBlackTreeNode(int val, bool color) {
    this->val = val;
    this->left = nullptr;
    this->right = nullptr;
    this->color = color;
    this->parent = nullptr;

}

RedBlackTreeNode::RedBlackTreeNode(RedBlackTreeNode* h, int val, bool color) {
    this->val = val;
    this->left = nullptr;
    this->right = nullptr;
    this->color = color;
    this->parent = h;
}

bool RedBlackTree::isRed(RedBlackTreeNode* x) {
    if (x == nullptr) return false;
    return x->color == RED;

}

RedBlackTreeNode* RedBlackTree::rotateLeft(RedBlackTreeNode* h) {
    if (isRed(h->right)) {
        // x->left to h->left
        RedBlackTreeNode* x = h->right;
        if (x->left)
            h->right->parent = h;
        h->right = x->left;

        if (h == root)
            root = x;

        //h to x->left
        x->left = h;
        x->parent = h->parent;
        h->parent = x;

        //colors
        x->color = h->color;
        h->color = RED;
        return x;
    }
    return h;
}

RedBlackTreeNode* RedBlackTree::rotateRight(RedBlackTreeNode* h) {
    if (isRed(h->left)) {
        // x->right to h->right
        RedBlackTreeNode* x = h->left;
        if (x->right)
            h->left->parent = h;
        h->left = x->right;

        if (h == root)
            root = x;

        //h to x->right
        x->right = h;
        x->parent = h->parent;
        h->parent = x;

        //colors
        x->color = h->color;
        h->color = RED;
        return x;
    }
    return h;
}

void RedBlackTree::flipColors(RedBlackTreeNode* h) {
    if (!isRed(h) && isRed(h->left) && isRed(h->right)) {
        h->color = RED;
        h->left->color = BLACK;
        h->right->color = BLACK;
    }
}

RedBlackTreeNode* RedBlackTree::put_in_l(RedBlackTreeNode* h, RedBlackTreeNode* parent, int val) {
    if (h == nullptr) return new RedBlackTreeNode(parent, val, true);
    int cmp = compare(h->val, val);
    if (cmp > 0) h->left = put_in_l(h->left, h, val);
    else if (cmp < 0) h->right = put_in_r(h->right, h, val);
    else if (cmp == 0) return h;

    if (!isRed(h->left) && isRed(h->right))  h = rotateLeft(h);
    if (isRed(h->left) && isRed(h->left->left))h = rotateRight(h);
    if (isRed(h->right) && isRed(h->left))  flipColors(h);

    return h;
}

RedBlackTreeNode* RedBlackTree::put_in_r(RedBlackTreeNode* h, RedBlackTreeNode* parent, int val) {
    if (h == nullptr) return new RedBlackTreeNode(parent, val, true);
    int cmp = compare(h->val, val);
    if (cmp > 0) h->left = put_in_l(h->left, h, val);
    else if (cmp < 0) h->right = put_in_r(h->right, h, val);
    else if (cmp == 0) return h;

    if (!isRed(h->right) && isRed(h->left))  h = rotateRight(h);
    if (isRed(h->right) && isRed(h->right->right)) h = rotateLeft(h);
    if (isRed(h->right) && isRed(h->left)) flipColors(h);
    return h;
}

void RedBlackTree::fixInsert(RedBlackTreeNode* new_node) {
    while (new_node->parent == nullptr || new_node->parent->color) {
        if (new_node->parent == new_node->parent->parent->left) {
            RedBlackTreeNode* uncle = new_node->parent->parent->right;
            if (uncle->color) {//-如果伯父是红色，那么将父亲和伯父同时变黑，不会破坏左右平衡
                uncle->color = BLACK;
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;//-将祖父变红，才能实现下一轮回溯修复
                new_node = new_node->parent->parent;
            }
            else {//-如果伯父是黑色
                //-判断new_node是不是右孩子，如果是右孩子转换成左孩子
                if (new_node == new_node->parent->right) {
                    new_node = new_node->parent;
                    rotateLeft(new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rotateRight(new_node);
            }
        }
        else {
            RedBlackTreeNode* uncle = new_node->parent->parent->left;
            if (uncle->color) {//-如果伯父是红色
                uncle->color = BLACK;
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            }
            else {//-如果伯父是黑色
                if (new_node == new_node->parent->left) {
                    new_node = new_node->parent;
                    rotateRight(new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rotateLeft(new_node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}


void RedBlackTree::put(int val) {
    if (root == nullptr)
        root = new RedBlackTreeNode(val, false);
    int cmp = compare(root->val, val);
    if (cmp > 0) root->left = put_in_l(root->left, root, val);
    else if (cmp < 0) root->right = put_in_r(root->right, root, val);
    else if (cmp == 0) return;

    if (!isRed(root->left) && isRed(root->right))  root = rotateLeft(root);
    if (isRed(root->left) && isRed(root->left->left))root = rotateRight(root);
    if (isRed(root->right) && isRed(root->left)) {
        flipColors(root);
        root->color = BLACK;
    }
}

int main()
{
    std::cout << "Hello World!\n";
    RedBlackTree a;
    //cout << a.root << endl;
    a.put(8);
    a.put(7);
    a.put(9);
    a.put(9);
    cout << a.root->val << endl;
    cout << a.root->color << endl;

    cout << a.root->left->val << endl;
    cout << a.root->left->color << endl;

    cout << a.root->right->val << endl;
    cout << a.root->right->color << endl;

    cout << a.root->right->right->val << endl;
    cout << a.root->right->right->color << endl;


}

