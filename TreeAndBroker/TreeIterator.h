#ifndef TREEITERATOR_H
#define TREEITERATOR_H

class Node;

class TreeIterator {
    friend class Tree;
public:
    TreeIterator();
    TreeIterator(const TreeIterator& orig);
    ~TreeIterator();
    Node& operator *();
    TreeIterator& operator ++();
    bool operator !=(TreeIterator& rhs);
private:
Node* node = nullptr;
};

#endif /* TREEITERATOR_H */

