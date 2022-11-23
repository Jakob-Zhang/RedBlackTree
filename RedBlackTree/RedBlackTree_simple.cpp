#include <iostream>
#include <deque>
#include <string>
#include <vector>


using Color = bool;
using KEY_TYPE = int;
using VALUE_TYPE = int;

static const Color red = false;
static const Color black = true;

class RedBlackTreeNode {
public:
    Color color;
    RedBlackTreeNode* left;
    RedBlackTreeNode* right;
    RedBlackTreeNode* parent;

    KEY_TYPE key;
    VALUE_TYPE val;

    RedBlackTreeNode(Color color_) : color(color_), parent(nullptr), left(nullptr), right(nullptr), key(INT_MIN) {}
    RedBlackTreeNode(Color color_, KEY_TYPE key_, RedBlackTreeNode* nil) :color(color_), parent(nil), left(nullptr), right(nullptr), key(key_) {}
};

class RedBlackTree {
    RedBlackTreeNode* root;
    RedBlackTreeNode* nil;
public:
    RedBlackTree() {
        nil = new RedBlackTreeNode(black);
        root = nil;
    }
    void rotateL(RedBlackTreeNode* left_node);

    void rotateR(RedBlackTreeNode* right_node);

    void insertNode(KEY_TYPE key);

    void fixInsert(RedBlackTreeNode* node);

    RedBlackTreeNode* searchNode(KEY_TYPE key);

    RedBlackTreeNode* successor(RedBlackTreeNode* node);

    void deleteNode(KEY_TYPE key);

    void fixDelete(RedBlackTreeNode* node);

};

void RedBlackTree::rotateL(RedBlackTreeNode* left_node) {
    RedBlackTreeNode* x = left_node->right;
    left_node->right = x->left;
    if (x->left != nil) {
        x->left->parent = left_node;
        //left_node->right->parent = left_node;  两个等价
    }
    x->parent = left_node->parent;
    if (left_node == root) {
        root = x;
    }
    else if (left_node == left_node->parent->left) {
        left_node->parent->left = x;
    }
    else {
        left_node->parent->right = x;
    }
    left_node->parent = x;
    x->left = left_node;
}

void RedBlackTree::rotateR(RedBlackTreeNode* right_node) {
    RedBlackTreeNode* x = right_node->left;
    right_node->left = x->right;
    if (x->right != nil) {
        x->right->parent = right_node;
        //right_node->left->parent = right_node;
    }
    x->parent = right_node->parent;
    if (right_node == root) {
        root = x;
    }
    else if (right_node == right_node->parent->right) {
        right_node->parent->right = x;
    }
    else {
        right_node->parent->left = x;
    }
    right_node->parent = x;
    x->right = right_node;
}
void RedBlackTree::insertNode(KEY_TYPE key) {
    RedBlackTreeNode* prev = nil;
    RedBlackTreeNode* cur = root;
    while (cur != nil) {
        prev = cur;
        if (key > cur->key) {
            cur = cur->right;
        }
        else if (key < cur->key) {
            cur = cur->left;
        }
        else{
            return;
        }
    }
    RedBlackTreeNode* new_node = new RedBlackTreeNode(red, key, nil);
    if (prev == nil) {
        root = new_node;
    }
    else if (key < prev->key) {
        prev->left = new_node;
    }
    else {
        prev->right = new_node;
    }
    fixInsert(new_node);
    //print();
}

void RedBlackTree::fixInsert(RedBlackTreeNode* new_node) {
    while (new_node->parent->color == red) {
        if (new_node->parent = new_node->parent->parent->left) {
            RedBlackTreeNode* uncle = new_node->parent->parent->right;
            if (uncle->color == red) {
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                new_node = new_node->parent->parent;
            }
            else {
                if (new_node == new_node->parent->right) {
                    new_node = new_node->parent;
                    rotateL(new_node);
                }
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                rotateR(new_node->parent->parent);
            }
        }
        else {
            RedBlackTreeNode* uncle = new_node->parent->parent->left;
            if (uncle->color == red) {
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                new_node = new_node->parent->parent;
            }
            else {
                if (new_node == new_node->parent->left) {
                    new_node = new_node->parent;
                    rotateR(new_node);
                }
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                rotateL(new_node->parent->parent);
            }
        }
    }
    root->color = black;
}

RedBlackTreeNode* RedBlackTree::searchNode(KEY_TYPE key) {
    RedBlackTreeNode* cur = root;
    while (cur != nil) {
        if (key < cur->key) {
            cur = cur->left;
        }
        else if (key > cur->key) {
            cur = cur->right;
        }
        else {
            return cur;
        }
    }
    return cur;
}

RedBlackTreeNode* RedBlackTree::successor(RedBlackTreeNode* node) {
    if (node->right != nil) {
        RedBlackTreeNode* res = node->right;
        while(res->left != nil) {
            res = res->left;
        }
        return res;
    }
    else {
        while (node != root && node != node->parent->left) {
            node = node->parent;
        }
        return node->parent;
    }
}

void RedBlackTree::deleteNode(KEY_TYPE key) {
    RedBlackTreeNode* key_node = searchNode(key);
    RedBlackTreeNode* delete_node;
    RedBlackTreeNode* delete_node_son;
    if (key_node->left != nil && key_node->right != nil) {
        delete_node = successor(key_node);
        //是右子树的原因是successor返回的是key_node的中序后驱，没有比它更小的，根据AVL定义，没有左子树;
        delete_node_son = delete_node->right;
    }
    else {
        delete_node = key_node;
        if (key_node->left != nil) {
            delete_node_son = key_node->left;
        }
        else {
            delete_node_son = key_node->right;
        }
    }

    delete_node_son->parent = delete_node->parent;
    if (delete_node == root) {
        root = delete_node_son;
    }
    else if (delete_node == delete_node->parent->left) {
        delete_node->parent->left = delete_node_son;
    }
    else {
        delete_node->parent->right = delete_node_son;
    }

    key_node->key = delete_node->key;
    key_node->val = delete_node->val;

    if (delete_node->color == black) {
        fixDelete(delete_node_son);
    }
    delete delete_node;
    //print();
}
int main() {
 
    return 0;
}