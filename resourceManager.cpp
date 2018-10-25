#include "pch.h"
#include <list>
#include <string>
//#include "nodes.hpp"
#include "Node.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

auto SearchFromNodes(std::list<Node*> &nodes,std::string& target) {
	std::list<Node*>::iterator each;
	for (each = nodes.begin(); each != nodes.end(); each++) {
		if (!strcmp((*each)->GetName().c_str(), target.c_str())) {
			return each;
		}
	}
	return each;
}

void DeleteFromNodes(std::list<Node*> &nodes, std::string& target) {
	for (auto each = nodes.begin(); each != nodes.end(); each++) {
		(*each)->DeleteDependence(target.c_str());
	}
	printf("Deleting %s finished\n", target.c_str());
}

bool CheckUsableInDependence(Node* &target, std::map<std::string, std::list<std::string>> &depends, std::list<std::string> &updated) {
	auto targetDepend = depends.find(target->GetName());
	if (std::find(updated.begin(), updated.end(), target->GetName()) != updated.end()) {
		return target->GetUsable();
	}
	if (targetDepend == depends.end()) {
		updated.push_front(target->GetName());
		return true;
	}
	bool found = false;
	for (auto each = targetDepend->second.begin(); each != targetDepend->second.end(); each++) {
		found = false;
		auto every = target->GetDependencesBegin();
		for (; every != target->GetDependencesEnd(); every++) {
			if (!strcmp((*every)->GetName().c_str(), (*each).c_str())) {
				found = CheckUsableInDependence(*every, depends, updated);
				break;
			}
		}
		if (!found) {
			target->SetNotUsable();
			updated.push_front(target->GetName());
			return false;
		}
	}
	if (found) {
		target->SetUsable();
		updated.push_front(target->GetName());
		return true;
	}
}

void UpdateUsableNodes(std::list<Node*> &nodes, std::map<std::string, std::list<std::string>> &depends) {
	//TODO 将迭代改为循环
	//Using iteration to walk through the graph through one node recursively, and check from the bottom of graph
	std::list<std::string> updated;
	for (auto each = nodes.begin(); each != nodes.end(); each++) {
		std::string curName = (*each)->GetName();
		if (std::find(updated.begin(), updated.end(), curName) != updated.end()) 
			continue;
		CheckUsableInDependence(*each, depends, updated);
	}
}

void PrintGraph(std::list<Node*> &nodes) {
	printf("--------------------\n");
	printf("Current Graph View:\n");
	for(std::list<Node*>::iterator var = nodes.begin();var!=nodes.end();var++)
	{
		printf("    %s is ", (*var)->GetName().c_str());
		// print whether usable or not
		if ((*var)->GetUsable()) printf("usable");
		else printf("not usable");
		printf(",depends on ");
		int num = 0;
		for (std::list<Node*>::iterator depend = (*var)->GetDependencesBegin(); depend != (*var)->GetDependencesEnd(); depend++) {
			printf("%s, ", (*depend)->GetName().c_str());
			num++;
		}
		if (num == 0) printf("nothing,");
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
	//建立所有依赖的map
	std::list<Node*> nodes; //store all existing nodes
	std::map<std::string, std::list<std::string>> depends; //store all the dependences from original txt

	printf("Read in the resource.txt\n");
    std::ifstream resourceFile(".\\resource.txt",std::ios::in);
    if(!resourceFile.is_open()){
        printf("Opening resource file failed\n");
		return 1;
    }
	//read in and process the resource file
    while(!resourceFile.eof()){
		//read in a line from txt
        std::string dependence;
        getline(resourceFile, dependence);
		if (dependence == "") break;
		// cut this line into separate contents
		std::string dependFrom, dependTo;
		std::list<std::string> temp_str;
		SplitString(dependence, temp_str);
		dependFrom = temp_str.front();
		dependTo = temp_str.back();
		temp_str.clear();
		printf( "%s depends on %s\n", dependFrom.c_str(),dependTo.c_str());
		//insert dependence into depends
		auto depend = depends.find(dependFrom);
		if (depend != depends.end()) {
			if (std::find(depend->second.begin(),depend->second.end(),dependTo) != depend->second.end()) {
				printf("Dependence from %s to %s already existed\n", dependFrom.c_str(), dependTo.c_str());
			}
			else {
				depend->second.push_front(dependTo);
			}
		}
		else {
			std::list<std::string> temp_depend;
			temp_depend.push_front(dependTo);
			depends[dependFrom] = temp_depend;
		}
		//test if node already exists & create node
		//TODO 统一成检测并添加的函数模块
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
		//update dependence
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
				std::string targetName = (*target)->GetName();
				DeleteFromNodes(nodes, targetName);
				delete (*target);
				nodes.remove(*target);
				//update whether all the nodes are usable
				UpdateUsableNodes(nodes, depends);
				continue;
			}
		}
		//insert a node or a dependence into graph
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