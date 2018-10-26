#include <list>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;
class Node{
    private:
        string name;
        list<Node*> dependences;
		bool usable;
    public:
        Node(const char* string);
        //~Node();
        string GetName(){return this->name;}
        list<Node*>::iterator GetDependencesBegin(){ return this->dependences.begin();}
		list<Node*>::iterator GetDependencesEnd() { return this->dependences.end(); }
		bool GetUsable() { return this->usable; }
		void SetNotUsable() { this->usable = false; }
		void SetUsable() { this->usable = true; }
        void SetName(const char* newName){this->name.assign(newName);}
		void AddDependence(Node* target);
        list<Node*>::iterator SearchDependences(const char* dependence);
        void DeleteDependence(const char* target);
};

Node::Node(const char* string){
    this->SetName(string);
	this->usable = true;
}

void Node::AddDependence(Node* target) {
	//Test if already exists a dependence before insert this one
	list<Node*>::iterator test = this->SearchDependences((*target).name.c_str());
	if (test == this->dependences.end()) {
		this->dependences.push_front(target);
	}
	else {
		printf("Already exists dependence from %s to %s, insertion cancelled\n", this->name.c_str(), target->name.c_str());
	}
}

list<Node*>::iterator Node::SearchDependences(const char* dependence){
    list<Node*>::iterator dependIter;
    for(dependIter = this->dependences.begin(); dependIter!=this->dependences.end();dependIter++){
        if(!strcmp((*dependIter)->GetName().c_str(),dependence)){
            return dependIter;
        }
    }
    return dependIter;
}

void Node::DeleteDependence(const char* target){
	list<Node*>::iterator test = this->SearchDependences(target);
	if (test != this->dependences.end()) {
		this->dependences.remove(*test);
		printf("Deleted dependence from %s to %s\n", this->name.c_str(), target);
		this->usable = false;
	}
}