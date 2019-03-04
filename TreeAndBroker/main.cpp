#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "Tree.h"
#include "Broker.h"
using namespace std;

int main(int argc, char** argv) {
      
    Broker broker;
        
    broker.makeTree("parental tree").makeRoot("Anton Quelle");
    broker.getTree("parental tree").getRoot().appendLeft("Ger Quelle").appendLeft("Dirk Quelle").appendRight("IteratorTest");
    broker.getTree("parental tree").getRoot().getLeft().appendRight("Ans Quelle");
    broker.getTree("parental tree").getRoot().appendRight("Simone Quelle");
    broker.getTree("parental tree").getRoot().getRight().appendLeft("Opa Gijzel").appendLeft("Zijn opa");
    broker.getTree("parental tree").getRoot().getRight().getLeft().appendRight("Zijn oma");
    broker.getTree("parental tree").getRoot().getRight().appendRight("Oma Gijzel").appendLeft("Haar opa");
    broker.getTree("parental tree").getRoot().getRight().getRight().appendRight("Haar oma");
    
    broker.makeTree("Second Tree").makeRoot("Root").appendLeft("left");
    broker.getTree("Second Tree").getRoot().appendRight("right").appendRight("rightRight");
    
    
    cout << broker.getTree("parental tree") << endl;
    cout << broker.getTree("Second Tree") << endl;
    
    for_each(broker.getTree("parental tree").begin(),broker.getTree("parental tree").end(),
            [](Node& node){ cout << node.getName()<< endl;});
    
    for_each(broker.getTree("Second Tree").begin(),broker.getTree("Second Tree").end(),
            [](Node& node){ cout << node.getName()<< endl;});
            
    return 0;
}

