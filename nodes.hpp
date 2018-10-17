#include <list>
//#include "Node.hpp"
template<typename T>
std::list<T> nodes;// to store all existing nodes

template<typename T1, typename T2>
T1 SearchFromNodes(std::string target){
    std::list<T2>::iterator each;
    for(each=nodes.begin();each!=nodes.end();each++){
        if(!strcmp((*each)->GetName().c_str(),target.c_str())){
            return each;
        }
    }
    return each;
}

template<typename T>
int DeleteFromNodes(std::string target)
{
    std::list<T>::iterator t = SearchFromNodes(target);
    if(t!=nodes.end()){
        nodes.erase(t);
        return 0;
    }
    return 1;
}