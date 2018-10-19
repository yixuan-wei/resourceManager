#include "pch.h"
#include <list>
#include <string>
//#include "nodes.hpp"
#include "Node.hpp"
#include <fstream>
#include <iostream>

std::list<Node*>::iterator SearchFromNodes(std::list<Node*> &nodes,std::string& target) {
	std::list<Node*>::iterator each;
	for (each = nodes.begin(); each != nodes.end(); each++) {
		if (!strcmp((*each)->GetName().c_str(), target.c_str())) {
			return each;
		}
	}
	return each;
}

void SplitString(std::string& s, std::string& result1, std::string& result2) {
	std::string::size_type pos = s.find(" ");
	result1 = s.substr(0, pos);
	result2 = s.substr(pos+1);
}

int main(){
    //读取resource.txt
	//TODO 建立依赖的dict
	std::list<Node*> nodes;
    std::ifstream resourceFile(".\\resource.txt",std::ios::in);
    if(!resourceFile.is_open()){
        std::cout<<"Opening resource file failed"<<std::endl;
		return 1;
    }
    while(!resourceFile.eof()){
        std::string dependence;
        getline(resourceFile, dependence);
		if (dependence == "") break;
		std::cout << dependence << std::endl;
		std::string dependFrom, dependTo;
		SplitString(dependence, dependFrom, dependTo);
		std::cout << dependFrom << " depends on " << dependTo << std::endl;
		//test if node already exists & create node
		std::list<Node*>::iterator searchFrom = SearchFromNodes(nodes, dependFrom);
		if (nodes.empty() || searchFrom == nodes.end()) {
			Node* temp = new Node(dependFrom.c_str());
			nodes.push_front(temp);
			searchFrom = nodes.begin();
		}

		std::list<Node*>::iterator searchTo = SearchFromNodes(nodes, dependTo);
		if (searchTo == nodes.end()) {
			Node* temp = new Node(dependTo.c_str());
			nodes.push_front(temp);
			searchTo = nodes.begin();
		}
		//TODO update dependence
		//(*searchFrom)->
    }

    //TODO 建立Node网络，记录网络每一种（个？）资源的dict
    //TODO 实时检查输入
    //TODO 检查删除节点的功能
    //TODO 退出功能
    return 0;
}