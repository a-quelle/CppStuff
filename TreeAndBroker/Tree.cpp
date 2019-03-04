#include "Tree.h"
#include <string>
#include <iostream>

using namespace std;

Node::Node(const string& content): content(content){
}

Node::~Node(){
    if(left)
        delete left;
    if(right)
        delete right;
}

Node& Node::appendLeft(const string& content){
    if(left)
        delete left;
    left = new Node(content);
    left->parent = this;
    return *left;
    
}

Node& Node::appendRight(const string& content){
    if(right)
        delete right;
    right = new Node(content);
    right->parent = this;
    return *right;
}

Node& Node::getLeft() const{
    if(!left)
        cout << "Warning, returning reference to nullptr!" << endl;
    return *left;
}

Node& Node::getRight() const{
    if(!right)
        cout << "Warning, returning reference to nullptr!" << endl;
    return *right;
}

Node& Node::getParent() const{
    if(!parent)
        cout << "Warning, returning reference to nullptr!" << endl;
    return *parent;
}

std::ostream& operator << (std::ostream& oss, const Node& node){
    oss << node.content << endl;
    if(node.left)
        oss << *node.left;
    if(node.right)
        oss << *node.right;
    return oss;
}

const std::string& Node::getName() const{
    return content;
}

Tree::Tree(const std::string& name):name(name){
};

Tree::~Tree() noexcept{
    if(root)
        delete root;
}

Node& Tree::makeRoot(const string name){
    if(root){
        delete root;
    }
    root = new Node(name);
    return *root;
}

const string& Tree::getName() const{
    return name; 
}


Tree::Tree(Tree&& old) noexcept{
    name = old.name;
    root = old.root;
    old.root = nullptr;
}


Node& Tree::getRoot() const{
    return *root;
}

ostream& operator << (std::ostream& oss, const Tree& tree) {
        oss << tree.name << endl;
        if(tree.root)
            oss << *tree.root;
    }
TreeIterator Tree::end() const{
    return TreeIterator();
}

TreeIterator Tree::begin() const{
    TreeIterator iterator;
    iterator.node = root;
    while(iterator.node->left)
        iterator.node = iterator.node->left;
    return iterator;
}
