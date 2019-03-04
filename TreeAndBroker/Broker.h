
#ifndef BROKER_H
#define BROKER_H
#include <vector>
#include "Tree.h"
#include <string>
#include <map>

struct Broker {
    Broker();
    Broker(const Broker& orig);
    virtual ~Broker();
    Tree& makeTree(const std::string& name);
    Tree& getTree(const std::string& name);
        
    std::map<std::string,int> treeMap;
private:
    std::vector<Tree> trees;
};

#endif /* BROKER_H */

