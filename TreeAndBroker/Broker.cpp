#include "Broker.h"
#include <map>
#include <utility>

using namespace std;

Broker::Broker() {
}

Broker::Broker(const Broker& orig) {
}

Broker::~Broker() {
}

Tree& Broker::makeTree(const std::string& name){
    trees.emplace_back(Tree(name));
    treeMap[name]=trees.size()-1;
    return trees.back();
}

Tree& Broker::getTree(const std::string& name){
    return trees[treeMap[name]];
}
