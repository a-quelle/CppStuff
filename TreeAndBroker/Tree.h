#ifndef TREE_H
#define TREE_H
#include <string>
#include <iostream>
#include "TreeIterator.h"

class Node;

class Tree{
    
    friend class Node;
    friend class Broker;

    Node* root = nullptr;
    std::string name;
    
    Tree(const std::string& name);
     
    
public:
    ~Tree() noexcept;
    const std::string& getName() const;
    Node& getRoot() const;
    Node& makeRoot(const std::string name);
    Tree(const Tree&) = delete;
    Tree(Tree&& old) noexcept;
    friend std::ostream& operator << (std::ostream& oss, const Tree& tree);
    TreeIterator end() const;
    TreeIterator begin() const;
        
};

class Node {
    friend class Tree;
    friend class TreeIterator;
    
    Node *parent = nullptr, *left = nullptr, *right = nullptr;
    std::string content;
    
    Node(const std::string& content);
public:
    ~Node();
    Node& appendLeft(const std::string& content);
    Node& appendRight(const std::string& content);
    Node& getLeft() const;
    Node& getRight() const;
    const std::string& getName() const;
    Node& getParent() const;
    friend std::ostream& operator << (std::ostream& oss, const Node& node);
        
};

#endif /* TREE_H */

