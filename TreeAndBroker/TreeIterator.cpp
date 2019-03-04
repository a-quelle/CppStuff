#include "TreeIterator.h"
#include "Tree.h"

TreeIterator::TreeIterator() {
}

TreeIterator::TreeIterator(const TreeIterator& orig) {
}

TreeIterator::~TreeIterator() {
}

Node& TreeIterator::operator *(){
    return *node;
}
TreeIterator& TreeIterator::operator ++(){
    if(node->right){
        node = node->right;
        while(node->left)
            node = node->left;
    }
    else if(!node->parent) node=nullptr;
    else if(node->parent && node == node->parent->left)
        node = node->parent;
    else{
        while(node -> parent && node == node->parent->right)
            node = node->parent;
        node = node->parent;
    }
    return *this;
}
bool TreeIterator::operator !=(TreeIterator& rhs){
    return this->node != rhs.node;
}