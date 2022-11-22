﻿#include <iostream>
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
int main() {
 
    return 0;
}