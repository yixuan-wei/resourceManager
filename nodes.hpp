#include <list>
#include <Node.hpp>

std::list<Node*> nodes;// to store all existing nodes

std::list<Node*>::iterator SearchFromNodes(std::string target){
    std::list<Node*>::iterator each;
    for(each=nodes.begin();each!=nodes.end();each++){
        if(!strcmp((*each)->GetName().c_str(),target.c_str())){
            return each;
        }
    }
    return each;
}

int DeleteFromNodes(std::string target)
{
    std::list<Node*>::iterator t = SearchFromNodes(target);
    if(t!=nodes.end()){
        nodes.erase(t);
        return 0;
    }
    return 1;
}