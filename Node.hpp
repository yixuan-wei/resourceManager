#include <list>
#include <string>
#include "nodes.hpp"

class Node{
    private:
        std::string name=nullptr;
        std::list<Node*> dependences;
    public:
        Node(const char* string,const char* dependence);
        ~Node();
        std::string GetName(){return name;}
        std::list<Node*> GetDependences(){return dependences;}
        void SetName(const char* newName){name.assign(newName);}
        
        std::list<Node*>::iterator SearchDependences(const char* dependence);
        void DeleteDependence(char* target);
};

Node::Node(const char* string,const char* dependence){
            SetName(string);
            //UpdateDependence(dependence);
            nodes.push_back(this);
}

Node::~Node(){
    DeleteFromNodes(this->GetName());
    dependences.clear();
}

std::list<Node*>::iterator Node::SearchDependences(const char* dependence){
    std::list<Node*>::iterator dependIter;
    for(dependIter = this->dependences.begin(); dependIter!=dependences.end();dependIter++){
        if(!strcmp((*dependIter)->GetName().c_str(),dependence)){
            return dependIter;
        }
    }
    return dependIter;
}

void Node::DeleteDependence(char* target){
    //TODO 对dependences遍历，检查target
    for(std::list<Node*>::iterator dependIter = dependences.begin(); dependIter!=dependences.end();dependIter++){
        if((*dependIter)->GetName()==target){

        }
    }
}