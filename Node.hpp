#include <list>
#include <string>

//TODO this or not???
class Node{
    private:
        std::string name;
        std::list<Node*> dependences;
    public:
        Node(const char* string);
        ~Node();
        std::string GetName(){return name;}
        std::list<Node*> GetDependences(){return dependences;}
        void SetName(const char* newName){this->name.assign(newName);}
		void AddDependence(Node* target);
        std::list<Node*>::iterator SearchDependences(const char* dependence);
        void DeleteDependence(char* target);
};

Node::Node(const char* string){
            SetName(string);
            //UpdateDependence(dependence);
            //nodes.push_back(this);
}

Node::~Node(){
    //DeleteFromNodes(this->GetName());
    this->dependences.clear();
}

void Node::AddDependence(Node* target) {
	//TODO 检查是否已经存在映射
	std::list<Node*>::iterator test = this->SearchDependences((*target).name.c_str());
	this->dependences.push_front(target);
}

std::list<Node*>::iterator Node::SearchDependences(const char* dependence){
    std::list<Node*>::iterator dependIter;
    for(dependIter = this->dependences.begin(); dependIter!=this->dependences.end();dependIter++){
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