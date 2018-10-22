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

void PrintGraph(std::list<Node*> &nodes) {
	printf("Current Graph View:\n");
	for(std::list<Node*>::iterator var = nodes.begin();var!=nodes.end();var++)
	{
		std::cout << (*var)->GetName() << " depends on ";
		int num = 0;
		for (std::list<Node*>::iterator depend = (*var)->GetDependencesBegin(); depend != (*var)->GetDependencesEnd(); depend++) {
			std::cout << (*depend)->GetName() << ", ";
			num++;
		}
		if (num == 0) printf("nothing,");
		//TODO print whether usable or not
		printf("\n");
	}
	printf("--------------------\n");
}

void PrintInstrHelp() {
	printf(">>>>Instructions include:\n");
	printf("	delete n:               delete a node named n from the graph\n");
	printf("	insert node n:          insert a node named n into the graph\n");
	printf("	insert dependence m n:  insert a dependence that m depends on n into the graph\n");
	printf("	q:                      quit the resource manager\n");
	printf("--------------------------\n");
}

void SplitString(std::string& s, std::list<std::string> &result) {
	std::string::size_type pos = s.find(" ");
	while (pos != std::string::npos) {
		result.push_back(s.substr(0, pos));
		s = s.substr(pos + 1);
		pos = s.find(" ");
	}
	result.push_back(s);
}

int main(){
    //读取resource.txt
	//TODO 建立所有依赖的dict
	std::list<Node*> nodes;
	printf("Read in the resource.txt\n");
    std::ifstream resourceFile(".\\resource.txt",std::ios::in);
    if(!resourceFile.is_open()){
        std::cout<<"Opening resource file failed"<<std::endl;
		return 1;
    }
	//read in and process the resource file
    while(!resourceFile.eof()){
        std::string dependence;
        getline(resourceFile, dependence);
		if (dependence == "") break;
		//std::cout << dependence << std::endl;
		std::string dependFrom, dependTo;
		std::list<std::string> temp_str;
		SplitString(dependence, temp_str);
		dependFrom = temp_str.front();
		dependTo = temp_str.back();
		temp_str.clear();
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
		(*searchFrom)->AddDependence(*searchTo);
    }
	printf("--------Reading Finished---------\n");
	PrintInstrHelp();
	//TODO examine the input and execute corresponding function all the time
	while (true) {
		PrintGraph(nodes);
		printf("Input your next operation\n");
		std::string instruction;
		std::getline(std::cin,instruction);
		std::list<std::string> temp_instr;
		SplitString(instruction, temp_instr);
		instruction = temp_instr.front();
		//delete a node from graph
		if (!strcmp(instruction.c_str(),"delete")) {
			printf("Going to delete a node\n");
			std::list<Node*>::iterator target = SearchFromNodes(nodes, temp_instr.back());
			if (target == nodes.end()) {
				printf("ERROR: no such node exists in the graph\n");
				continue;
			}
			else {
				nodes.erase(target);
				delete *target;
				continue;
			}
		}
		else if (!strcmp(instruction.c_str(), "insert")) {
			temp_instr.pop_front();
			std::string sub_instr = temp_instr.front();
			if (sub_instr.c_str() == "node") {
				printf("Going to insert a node\n");
			}
			else if (sub_instr.c_str() == "dependence") {
				printf("Going to insert a dependence\n");
			}
			else {
				printf("insert instruction input failed, check format!\n");
				printf("Going to print help\n");
			}
		}
		else if (!strcmp(instruction.c_str(), "q")) {
			printf("Ready to quit the manager\n");
			//TODO 删除所有内存（nodes,和里面每一项的所有依赖）
			//TODO 保存所有node和依赖关系
		}
		else{
			//TODO 检查并尝试进行节点删除，否则打印帮助
			printf("Going to print help\n");
		}
	}
    return 0;
}