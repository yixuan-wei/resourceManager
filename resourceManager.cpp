#include "pch.h"
#include <list>
#include <string>
//#include "nodes.hpp"
#include "Node.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <fstream>

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
	printf("    load route_to_log:      load the stored graph and dependences from route_to_log\n");
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

std::string ReplaceString(std::string& s, const char *&from,const char *&to) {
	//TODO 完成从头到尾检查替换\\&/
	for (int i = 0; i < s.size(); i++) {
		if (!strcmp(s.substr(i,strlen(from)).c_str(),from)) {
			s.replace(i, strlen(from), to);
			i += strlen(to) - 1;
		}
	}
}

int InitFromResource(std::list<Node*> &nodes, std::map<std::string,std::list<std::string>> &depends) {
	printf("Read in the resource.txt\n");
	std::ifstream resourceFile(".\\resource.txt", std::ios::in);
	if (!resourceFile.is_open()) {
		printf("Opening resource file failed\n");
		return 1;
	}
	//read in and process the resource file
	while (!resourceFile.eof()) {
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
		printf("%s depends on %s\n", dependFrom.c_str(), dependTo.c_str());
		//insert dependence into depends
		auto depend = depends.find(dependFrom);
		if (depend != depends.end()) {
			if (std::find(depend->second.begin(), depend->second.end(), dependTo) != depend->second.end()) {
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
		CreatDependence(nodes, dependFrom, dependTo);
	}
	printf("--------Reading Finished---------\n");
	return 0;
}

int InitFromLog(std::list<Node*> &nodes, std::map<std::string, std::list<std::string>> &depends, std::string route) {
	printf("Read in the log file\n");
	std::ifstream resourceFile(ReplaceString(route,"/","\\"), std::ios::in);
	if (!resourceFile.is_open()) {
		printf("Opening log file failed\n");
		return 1;
	}
	//TODO?? 不一定需要，LOG改成先记录dependences再记录graph
	//read in and process the resource file
	while (!resourceFile.eof()) {
		//read in a line from log
		std::string dependence;
		getline(resourceFile, dependence);
		if (dependence == "") break;
		if(dependence=="Graph:"){
			//TODO start nodes read in
			while(!resourceFile.eof()||dependence=="Dependences:"){
				std::list<std::string> temp_str;
				SplitString(dependence, temp_str);
				if(temp_str.size()<2){
					printf("ERROR: LOG format is wrong, please check\n");
					return 1;
				}
				auto iter = temp_str.begin();
				std::string name = *iter;
				auto node = FindCreatNode(nodes,name);
				iter++;
				std::string usable = *iter;//TODO string转int
				if(usable=="1")  	(*node)->SetUsable();
				else 				(*node)->SetNotUsable();
				iter++;
				while(iter!=temp_str.end()){
					//TODO 遍历iter更新dependences
					auto to = FindCreatNode(nodes,*iter);
					(*node)->AddDependence(*to);
				}
				temp_str.end();
				getline(resourceFile,dependence);
			}
			//TODO start depends read in
			//根据读入，新建node，
			while(!resourceFile.eof()){
				std::list<std::string> temp_str;
				SplitString(dependence, temp_str);
				if(temp_str.size()<2){
					printf("ERROR: LOG format is wrong, please check\n");
					return 1;
				}
				//TODO update depends
				auto iter = temp_str.begin();
				std::string name = *iter;
				auto depend = depends.find(*iter);
				iter++;
				while(iter!=temp_str.end()){
					if (depend != depends.end()) {
						if (std::find(depend->second.begin(), depend->second.end(), *iter) != depend->second.end()) {
							printf("Dependence from %s to %s already existed\n", name.c_str(), *iter.c_str());
						}
						else {
							depend->second.push_front(*iter);
						}
					}
					else {
						std::list<std::string> temp_depend;
						temp_depend.push_front(*iter);
						depends[name] = temp_depend;
					}
					iter++;
				}
				getline(resourceFile,dependence);
			}
		}
	}
	if(resourceFile.eof()) {
		printf("ERROR: error when loading in LOG file\n");
		return 1;
	}
	printf("--------Reading Finished---------\n");
	return 0;
}

auto FindCreatNode(std::list<Node*>&nodes,std::string name) {
	std::list<Node*>::iterator node = SearchFromNodes(nodes, name);
	if (nodes.empty() || node == nodes.end()) {
		Node* temp = new Node(name.c_str());
		nodes.push_front(temp);
		node = nodes.begin();
	}
	return node;
}

void CreatDependence(std::list<Node*>&nodes, std::string from, std::string to) {
	//test if node already exists & create node
	auto iterFrom = FindCreatNode(nodes, from);
	auto iterTo = FindFindCreatNode(nodes, to);
	//update dependence
	(*iterFrom)->AddDependence(*iterTo);
}

void DeleteNode(std::list<Node*>&nodes, std::map<std::string, std::list<std::string>> &depends, std::string name) {
	printf("Going to delete a node\n");
	std::list<Node*>::iterator target = SearchFromNodes(nodes, name);
	if (target == nodes.end()) {
		printf("ERROR: no such node exists in the graph\n");
	}
	else {
		std::string targetName = (*target)->GetName();
		DeleteFromNodes(nodes, targetName);
		delete (*target);
		target = nodes.erase(target);
		//update whether all the nodes are usable
		UpdateUsableNodes(nodes, depends);
	}
}

void ClearManager(std::list<Node*> &nodes, std::map<std::string, std::list<std::string>>& depends) {
	// 保存所有node和依赖关系，邻接表
	std::ofstream outfile("ResourceManagerLog", std::ios::out);
	outfile << "Graph:" << std::endl;
	for (auto each = nodes.begin(); each != nodes.end(); each++) {
		outfile << (*each)->GetName() << " " << (*each)->GetUsable();
		for (auto depend = (*each)->GetDependencesBegin(); depend != (*each)->GetDependencesEnd(); depend++) {
			outfile << " " << (*depend)->GetName();
		}
		outfile << std::endl;
	}
	outfile << "Dependences:" << std::endl;
	for (auto each = depends.begin(); each != depends.end(); each++) {
		outfile << each->first << " ";
		for (auto depend = each->second.begin(); depend != each->second.end(); depend++) {
			outfile << " " << *depend;
		}
		outfile << std::endl;
	}
	outfile.close();
	//删除所有内存（nodes,和里面每一项的所有依赖）
	for (auto node = nodes.begin(); node != nodes.end();) {
		delete(*node);
		node = nodes.erase(node);
	}
	nodes.clear();
	depends.clear();
}

int main(){
	std::list<Node*> nodes; //store all existing nodes
	std::map<std::string, std::list<std::string>> depends; //store all the dependences from original txt
	//read in the resource.txt and initialize nodes & depends
	if (InitFromResource(nodes, depends)) {
		printf("Resource Manager going to abort\n");
		return 1;
	}
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
			if (temp_instr.size() == 2) {
				DeleteNode(nodes, depends, temp_instr.back());
			}
			else {
				printf("ERROR: delete format wrong, please check!\n");
			}
			continue;
		}
		//insert a node or a dependence into graph
		else if (!strcmp(instruction.c_str(), "insert")) {
			temp_instr.pop_front();
			std::string sub_instr = temp_instr.front();
			if (!strcmp(sub_instr.c_str(),"node") && temp_instr.size()==2) {
				printf("Going to insert a node\n");
				FindFindCreatNode(nodes, temp_instr.back());
			}
			else if (!strcmp(sub_instr.c_str(), "dependence") && temp_instr.size()==3) {
				temp_instr.pop_front();
				printf("Going to insert a dependence\n");
				CreatDependence(nodes, temp_instr.front(), temp_instr.back());
				UpdateUsableNodes(nodes, depends);
			}
			else {
				printf("ERROR: insert instruction input failed, please check format!\n");
				printf("Going to print help\n");
			}
			continue;
		}
		else if (!strcmp(instruction.c_str(), "q")) {
			printf("Ready to quit the manager\n");
			ClearManager(nodes, depends);
			return 0;
		}
		else if (!strcmp(instruction.c_str(), "load")) {
			if (temp_instr.size() == 2) {
				printf("Ready to load in log file\n");
				ClearManager(nodes, depends);
				if (InitFromLog(nodes, depends, temp_instr.back())) { 
					printf("Initialization failed, use the older version\n"); 
				}
			}
			else {
				printf("ERROR: load format wrong, please check!\n");
			}
			continue;
		}
		else{
			// 检查并尝试进行节点删除，否则打印帮助
			if (temp_instr.size() == 1) {
				printf("Going to delete this node? y/n\n");
				char in = 'n';
				scanf_s("%c", &in);
				if (in == 'y') DeleteNode(nodes, depends, temp_instr.front());
				else if (in != 'n') printf("ERROR: wrong input\n");
			}
			else {
				printf("ERROR: wrong input format, please check!\n");
			}
			printf("Going to print help\n");
		}
	}
    return 0;
}