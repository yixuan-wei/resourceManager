#include <list>
#include <string>

//TODO this or not???
class Node{
    private:
        std::string name;
        std::list<Node*> dependences;
		bool usable;
    public:
        Node(const char* string);
        //~Node();
        std::string GetName(){return this->name;}
        std::list<Node*>::iterator GetDependencesBegin(){ return this->dependences.begin();}
		std::list<Node*>::iterator GetDependencesEnd() { return this->dependences.end(); }
        void SetName(const char* newName){this->name.assign(newName);}
		void AddDependence(Node* target);
        std::list<Node*>::iterator SearchDependences(const char* dependence);
        void DeleteDependence(const char* target);
};

Node::Node(const char* string){
            SetName(string);
			this->usable = true;
            //UpdateDependence(dependence);
            //nodes.push_back(this);
}

void Node::AddDependence(Node* target) {
	//Test if already exists a dependence before insert this one
	std::list<Node*>::iterator test = this->SearchDependences((*target).name.c_str());
	if (test == this->dependences.end()) {
		this->dependences.push_front(target);
	}
	else {
		printf("Already exists dependence from %s to %s, insertion cancelled\n", this->name.c_str(), target->name.c_str());
	}
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

void Node::DeleteDependence(const char* target){
    //TODO 对dependences遍历，检查target
	std::list<Node*>::iterator test = this->SearchDependences(target);
	if (test != this->dependences.end()) {
		this->dependences.remove(*test);
	}
	else {
		printf("%s does not depend on %s\n", this->name.c_str(), target);
	}
}