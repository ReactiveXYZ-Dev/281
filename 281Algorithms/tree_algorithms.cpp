//
//  tree_algorithms.cpp
//  281Algorithms
//
//  Created by Jackie Chung on 11/12/17.
//  Copyright © 2017 Reactive XYZ. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

// A simple ADT for an AVL tree
class AVLTree {
public:
    // Tree node
    struct Node {
        Node(int val) {
            this->val = val;
            this->height = 1; // default to 1 as a leaf node
        }
        int val;
        int height;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    
    // init a value at root
    explicit AVLTree(int val) {
        this->root = new Node(val);
    }
    
    // find a node with value
    Node* find(int val) const {
        return this->_find_impl(this->root, val);
    }
    
    // insert a new node
    Node* insert(int val) {
        return this->_insert_impl(this->root, val);
    }
    
    // delete an node
    Node* remove(int val) {
        return this->_remove_impl(this->root, val);
    }
    
    // get height
    int height(Node* root) const {
        if (!root) {
            return 0;
        }
        return root->height;
    }
    
    // update node's height
    Node* update_height(Node* node) {
        node->height = max(this->height(node->left), this->height(node->right)) + 1;
        return node;
    }
    
    // traversals
    void pre_order(Node* root, ostream& oss) const {
        if (root == nullptr) {
            return;
        }
        oss << root->val << ' ';
        this->pre_order(root->left, oss);
        this->pre_order(root->right, oss);
    }
    
    void in_order(Node* root, ostream& oss) const {
        if (root == nullptr) {
            return;
        }
        this->in_order(root->left, oss);
        oss << root->val << ' ';
        this->in_order(root->right, oss);
    }
    
    void post_order(Node* root, ostream& oss) const {
        if (root == nullptr) {
            return;
        }
        this->post_order(root->left, oss);
        this->post_order(root->right, oss);
        oss << root->val << ' ';
    }
    
    void level_order(Node* root, ostream& oss) const {
        // init a queue for BFS
        queue<Node*> q;
        // visit root
        q.push(this->root);
        while (!q.empty()) {
            // get first node
            Node* current = q.front();
            q.pop();
            // print current
            oss << current->val << ' ';
            // get its children and push into queue
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }
    
    // destructor
    ~AVLTree() {
        // destroy all nodes
        this->destroy(this->root);
    }
    
private:
    // root node
    Node* root = nullptr;
    
    // tree operation implementations
    Node* _find_impl(Node* root, int val) const {
        if (!root) return nullptr;
        if (root->val == val) return root;
        
        Node* l = this->_find_impl(root->left, val);
        Node* r = this->_find_impl(root->right, val);
        
        if (l) return l;
        if (r) return r;
        
        return nullptr;
    }
    
    Node* _insert_impl(Node* node, int val) {
        // if no value at root
        if (!node) return new Node(val);
        
        // find path to insert based on value
        if (val < node->val) {
            node->left = this->_insert_impl(node->left, val);
        } else if (val > node->val) {
            node->right = this->_insert_impl(node->right, val);
        } else {
            // equal, default to insert right
            node->right = this->_insert_impl(node->right, val);
        }
        
        // update height of the node
        node = this->update_height(node);
        // check balance
        int balance = this->balance(node);
        
        // left left
        if (balance > 1 && this->balance(node->left) >= 0) {
            return this->right_rotate(node);
        }
        
        // right right
        if (balance < -1 && this->balance(node->right) <= 0) {
            return this->left_rotate(node);
        }
        
        // left right
        if (balance > 1 && this->balance(node->left) < 0) {
            node->left = this->left_rotate(node->left);
            return this->right_rotate(node);
        }
        
        // right left
        if (balance < -1 && this->balance(node->right) > 0) {
            node->right = this->right_rotate(node->right);
            return this->left_rotate(node);
        }
        
        return node;
    }
    
    Node* _remove_impl(Node* node, int val) {
        // TODO
        return nullptr;
    }
    
    // check balance of node
    int balance(Node* node) const {
        if (!node) {
            return 0;
        }
        return this->height(node->left) - this->height(node->right);
    }
    
    // rotation operations for AVL tree
    Node* left_rotate(Node* node) {
        Node* r = node->right;
        Node* l = r->left;
        // rotate
        r->left = node;
        node->right = l;
        // update tree height
        this->update_height(node);
        this->update_height(r);
        
        return r; // new root
    }
    
    Node* right_rotate(Node* node) {
        Node* l = node->left;
        Node* r = l->right;
        // rotate
        l->right = node;
        node->left = r;
        // update tree height
        this->update_height(node);
        this->update_height(l);
        
        return l; // new root
    }
    
    // destroy the tree
    void destroy(Node* root) {
        if (!root->left && !root->right) {
            // leaf
            delete root;
            root = nullptr;
            return;
        }
        // destroy subtrees
        destroy(root->left);
        destroy(root->right);
    }
};


int main() {
    
    return 0;
}
