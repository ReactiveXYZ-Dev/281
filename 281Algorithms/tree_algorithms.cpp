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
#include <cassert>

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
    
    // root node
    Node* root = nullptr;
    
    // get rid of compiler warning
    AVLTree(){}
    
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
        Node* n = this->_insert_impl(this->root, val);
        ++ this->size;
        this->root = n;
        return n;
    }
    
    // delete an node
    Node* remove(int val) {
        Node* n = this->_remove_impl(this->root, val);
        -- this->size;
        this->root = n;
        return n;
    }
    
    // clear the contents of this tree
    void clear() {
        this->destroy(this->root);
        this->root = nullptr;
        this->size = 0;
    }
    
    // get number of elements
    size_t num_elements() const {
        return this->size;
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
    // size
    size_t size = 0;
    
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
            // @NOTICE: equal, disallow or use other strategies
            return node;
        }
        
        // update height of the node
        node = this->update_height(node);
        // check balance
        return this->check_balance(node);
    }
    
    Node* _remove_impl(Node* node, int val) {
        
        if (!node) return node;
        
        // search through left/right subtree
        if (val < node->val) {
            node->left = this->_remove_impl(node->left, val);
        }
        if (val > node->val) {
            node->right = this->_remove_impl(node->right, val);
        }
        // equal, found the node
        if (val == node->val) {
            // if node has no children
            if (!node->left && !node->right) {
                delete node;
                node = nullptr;
            }
            // two children
            else if (node->left && node->right) {
                // find minimum node in the right subtree
                // or the in-order successor
                Node* min = node->right;
                while (min->left != nullptr) {
                    min = min->left;
                }
                // copy over that value
                node->val = min->val;
                // now remove the one with the same value in the subtree
                node->right = this->_remove_impl(node->right, min->val);
            }
            // one children
            else {
                Node* child;
                if (node->left == nullptr) {
                    child = node->right;
                    node->right = nullptr;
                } else {
                    child = node->left;
                    node->left = nullptr;
                }
                // copy over child value
                node->val = child->val;
                // destroy child
                delete child;
                child = nullptr;
            }
        }
        
        // if root got deleted
        if (!node) return node;
        
        // update node weight for AVL
        node = this->update_height(node);
        
        // check balance and adjust
        return this->check_balance(node);
    }
    
    // check balance of a node and adjust
    Node* check_balance(Node* node) {
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
    
    // get balance of node
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
        if (!root) return;
        if (!root->left && !root->right) {
            // leaf
            delete root;
            root = nullptr;
            return;
        }
        // destroy subtrees
        this->destroy(root->left);
        this->destroy(root->right);
    }
};

void test_insertion() {
    AVLTree tree;
    ostringstream oss;

    // Set 1
    int set1[] = { 10, 20, 30, 40, 50, 25 };
    
    for (int val : set1) {
        tree.insert(val);
    }
    
    tree.pre_order(tree.root, oss);
    assert(oss.str() == "30 20 10 25 40 50 ");
    
    oss.str("");
    oss.clear();
    tree.clear();
    
    // Set 2
    int set2[] = { 9, 5, 10, 0, 6, 11, -1, 1, 2 };
    
    for (int val : set2) {
        tree.insert(val);
    }
    
    tree.pre_order(tree.root, oss);
    assert(oss.str() == "9 1 0 -1 5 2 6 10 11 ");
    
    cout << "Test insertion passed!" << endl;
}

void test_deletion() {
    AVLTree tree;
    ostringstream oss;
    
    int set2[] = { 9, 5, 10, 0, 6, 11, -1, 1, 2 };
    
    for (int val : set2) {
        tree.insert(val);
    }
    
    tree.remove(10);
    
    tree.pre_order(tree.root, oss);
    assert(oss.str() == "1 0 -1 9 5 2 6 11 ");
    
    cout << "Test deletion passed!" << endl;
}

int main() {
    
    test_insertion();
    test_deletion();
    
    return 0;
}
