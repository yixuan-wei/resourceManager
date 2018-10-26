
#include <list>
#include <string.h>
#include <string>
#include "Node.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <shlobj.h>
#include <shellapi.h>

using namespace std;

int VisualizaGraph(list<Node*> *nodes, map<string,list<string>>*depends){
	ofstream dotFile("graphviz\\test.dot",ios::out);
	if (!dotFile.is_open()) {
		printf("Opening dot file failed\n");
		return 1;
	}
	dotFile<<"digraph edge_settings{\n";
	for(auto each =(*nodes).begin(); each!=(*nodes).end();each++){
		list<string> temp_list;
		if((*each)->GetUsable())  dotFile<<"    "<<(*each)->GetName()<<"[color=\"green\"]\n";
		else  dotFile<<"    "<<(*each)->GetName()<<"[color=\"red\"]\n";
		for(auto every=(*each)->GetDependencesBegin();every != (*each)->GetDependencesEnd();every++){
			dotFile<<"    "<<(*each)->GetName()<<"->"<<(*every)->GetName()<<"[style=\"\" color=\"green\"]\n";
			temp_list.push_back((*every)->GetName());
		}
		auto depend = (*depends).find((*each)->GetName());
		if(depend==(*depends).end()) continue;
		for(auto every = depend->second.begin();every!=depend->second.end();every++){
			if(find(temp_list.begin(),temp_list.end(),(*every))==temp_list.end()){
				dotFile<<"    "<<(*each)->GetName()<<"->"<<(*every)<<"[style=\"dotted\" color=\"red\"]\n";
			}
		}
	}
	dotFile<<"}";
	dotFile.close();
	string exe = ".\\graphviz\\dot.exe -Tpng -o graph.png graphviz\\test.dot";
	system(exe.data());
	system(".\\graph.png");
	return 0;
}

list<Node*>::iterator SearchFromNodes(list<Node*> *nodes, string& target) {
	list<Node*>::iterator each;
	for (each = (*nodes).begin(); each != (*nodes).end(); each++) {
		if (!strcmp((*each)->GetName().c_str(), target.c_str())) {
			return each;
		}
	}
	return each;
}

void DeleteFromNodes(list<Node*> *nodes, string& target) {
	for (auto each = (*nodes).begin(); each != (*nodes).end(); each++) {
		(*each)->DeleteDependence(target.c_str());
	}
	printf("Deleting %s finished\n", target.c_str());
}

bool CheckUsableInDependence(Node* &target, map<string, list<string>> *depends, list<string> *updated) {
	auto targetDepend = (*depends).find(target->GetName());
	if (find((*updated).begin(), (*updated).end(), target->GetName()) != (*updated).end()) {
		return target->GetUsable();
	}
	if (targetDepend == (*depends).end()) {
		(*updated).push_front(target->GetName());
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
			(*updated).push_front(target->GetName());
			return false;
		}
	}
	if (found) {
		target->SetUsable();
		(*updated).push_front(target->GetName());
		return true;
	}
}

void UpdateUsableNodes(list<Node*> *nodes, map<string, list<string>> *depends) {
	//TODO ��������Ϊѭ��
	//Using iteration to walk through the graph through one node recursively, and check from the bottom of graph
	list<string> updated;
	for (auto each = (*nodes).begin(); each != (*nodes).end(); each++) {
		string curName = (*each)->GetName();
		if (find(updated.begin(), updated.end(), curName) != updated.end())
			continue;
		CheckUsableInDependence(*each, depends, &updated);
	}
}

void PrintGraph(list<Node*> *nodes) {
	printf("--------------------\n");
	printf("Current Graph View:\n");
	for(list<Node*>::iterator var = (*nodes).begin();var!=(*nodes).end();var++)
	{
		printf("    %s is ", (*var)->GetName().c_str());
		// print whether usable or not
		if ((*var)->GetUsable()) printf("usable");
		else printf("not usable");
		printf(",links to ");
		int num = 0;
		for (list<Node*>::iterator depend = (*var)->GetDependencesBegin(); depend != (*var)->GetDependencesEnd(); depend++) {
			printf("%s, ", (*depend)->GetName().c_str());
			num++;
		}
		if (num == 0) printf("nothing,");
		printf("\n");
	}
	printf("--------------------\n");
}

void PrintDependences(map<string, list<string>> * depends) {
	printf("--------------------\n");
	printf("The Dependences are:\n");
	for (map<string, list<string>>::iterator each = (*depends).begin(); each != (*depends).end(); each++) {
		printf("%s depends on ",each->first.c_str());
		for (list<string>::iterator every = each->second.begin(); every != each->second.end(); every++)
			printf("%s, ", (*every).c_str());
		printf("\n");
	}
	printf("--------------------\n");
}

void PrintInstrHelp() {
	printf(">>>>Instructions include:\n");
	printf("    delete n               :delete a node named n from the graph\n");
	printf("    insert node n          :insert a node named n into the graph\n");
	printf("    insert dependence m n  :insert a dependence that m depends on n into the graph\n");
	printf("    q                      :quit the resource manager\n");
	printf("    load route_to_log      :load the stored graph and dependences from route_to_log\n");
	printf("    print                  :print the dependences for all resources, whether exists or not\n");
	printf("    visualize              :visualize the graph\n");
	printf("--------------------------\n");
}

void SplitString(string& s, list<string> *result) {
	string::size_type pos = s.find(" ");
	while (pos != string::npos) {
		(*result).push_back(s.substr(0, pos));
		s = s.substr(pos + 1);
		pos = s.find(" ");
	}
	(*result).push_back(s);
}

void ReplaceString(string& s) {
	//TODO 完成从头到尾检查替换\\&/
	const char from = '/';
	const char to = '\\';
	for (int i = 0; i < s.size(); i++) {
		if (s[i]==from) {
			s[i] = to;
		}
	}
}

list<Node*>::iterator FindCreatNode(list<Node*>*nodes, string name) {
	list<Node*>::iterator node = SearchFromNodes(nodes, name);
	if ((*nodes).empty() || node == (*nodes).end()) {
		Node* temp = new Node(name.c_str());
		(*nodes).push_front(temp);
		node = (*nodes).begin();
	}
	return node;
}

void CreatDependence(list<Node*>* nodes, string from, string to) {
	//test if node already exists & create node
	list<Node*>::iterator iterFrom = SearchFromNodes(nodes, from);
	list<Node*>::iterator iterTo = SearchFromNodes(nodes, to);
	//update dependence
	if(iterFrom!=(*nodes).end() && iterTo!=(*nodes).end())   (*iterFrom)->AddDependence(*iterTo);
	else if (iterFrom == (*nodes).end())    printf("ERROR: node %s does not exist\n", from.c_str());
	else if (iterTo == (*nodes).end())		 printf("ERROR: node %s does not exist\n", to.c_str());
	else {
		printf("ERROR: node %s does not exist\n", from.c_str());
		printf("ERROR: node %s does not exist\n", to.c_str());
	}
}

int InitFromResource(list<Node*> *nodes, map<string,list<string>> *depends) {
	printf("Read in the resource.txt\n");
	ifstream resourceFile(".\\resource.txt", ios::in);
	if (!resourceFile.is_open()) {
		printf("Opening resource file failed\n");
		return 1;
	}
	//read in and process the resource file
	while (!resourceFile.eof()) {
		//read in a line from txt
		string dependence;
		getline(resourceFile, dependence);
		if (dependence == "") break;
		// cut this line into separate contents
		string dependFrom, dependTo;
		list<string> temp_str;
		SplitString(dependence, &temp_str);
		dependFrom = temp_str.front();
		dependTo = temp_str.back();
		temp_str.clear();
		printf("%s depends on %s\n", dependFrom.c_str(), dependTo.c_str());
		//insert dependence into depends
		map<string, list<string>>::iterator depend = (*depends).find(dependFrom);
		if (depend != (*depends).end()) {
			if (find(depend->second.begin(), depend->second.end(), dependTo) != depend->second.end()) {
				printf("Dependence from %s to %s already existed\n", dependFrom.c_str(), dependTo.c_str());
			}
			else {
				depend->second.push_front(dependTo);
			}
		}
		else {
			list<string> temp_depend;
			temp_depend.push_front(dependTo);
			(*depends)[dependFrom] = temp_depend;
		}
		list<Node*>::iterator from = FindCreatNode(nodes, dependFrom);
		list<Node*>::iterator to = FindCreatNode(nodes, dependTo);
		(*from)->AddDependence(*to);
	}
	printf("--------Reading Finished---------\n");
	return 0;
}

int InitFromLog(list<Node*> *nodes, map<string, list<string>> *depends, string &route) {
	printf("Read in the log file\n");
	ReplaceString(route);
	ifstream resourceFile(route, ios::in);
	if (!resourceFile.is_open()) {
		printf("Opening log file failed\n");
		return 1;
	}
	//read in a line from log
	string line;
	getline(resourceFile, line);
	if (line == "") {
		printf("ERROR: Log format is wrong\n");
		return 1;
	}
	if(line=="Graph:"){
		//start nodes read in
		getline(resourceFile, line);
		while(!resourceFile.eof()&&line!="Dependences:"){
			list<string> temp_str;
			SplitString(line, &temp_str);
			if(temp_str.size()<2){
				printf("ERROR: LOG format is wrong, please check\n");
				return 1;
			}
			list<string>::iterator iter = temp_str.begin();
			string name = *iter;
			list<Node*>::iterator node = FindCreatNode(nodes,name);
			iter++;
			string usable = *iter;//TODO string转int
			if(usable=="1")  	(*node)->SetUsable();
			else 				(*node)->SetNotUsable();
			iter++;
			while(iter!=temp_str.end()){
				//遍历iter更新dependences
				list<Node*>::iterator to = FindCreatNode(nodes,*iter);
				(*node)->AddDependence(*to);
				iter++;
			}
			getline(resourceFile,line);
		}
		//start depends read in
		getline(resourceFile, line);
		while(!resourceFile.eof()){
			list<string> temp_str;
			SplitString(line, &temp_str);
			if(temp_str.size()<2){
				printf("ERROR: LOG format is wrong, please check\n");
				return 1;
			}
			//insert into depends
			list<string>::iterator iter = temp_str.begin();
			string name = *iter;
			list<string> temp_depend;
			iter++;
			while(iter!=temp_str.end()){
				temp_depend.push_front(*iter);
				iter++;
			}
			(*depends)[name] = temp_depend;
			getline(resourceFile,line);
		}
	}
	if(!resourceFile.eof()) {
		printf("ERROR: error when loading in LOG file\n");
		return 1;
	}
	printf("--------Loading Log Finished---------\n");
	return 0;
}

void DeleteNode(list<Node*>*nodes, map<string, list<string>> *depends, string &name) {
	printf("Going to delete a node\n");
	list<Node*>::iterator target = SearchFromNodes(nodes, name);
	if (target == (*nodes).end()) {
		printf("ERROR: no such node exists in the graph\n");
	}
	else {
		string targetName = (*target)->GetName();
		DeleteFromNodes(nodes, targetName);
		delete (*target);
		target = (*nodes).erase(target);
		//update whether all the nodes are usable
		UpdateUsableNodes(nodes, depends);
	}
}

void ClearManager(list<Node*> *nodes, map<string, list<string>>* depends) {
	// 保存所有node和依赖关系，邻接表
	ofstream outfile("ResourceManagerLog", ios::out);
	outfile << "Graph:" << endl;
	for (list<Node*>::iterator each = (*nodes).begin(); each != (*nodes).end(); each++) {
		outfile << (*each)->GetName() << " " << (*each)->GetUsable();
		for (list<Node*>::iterator depend = (*each)->GetDependencesBegin(); depend != (*each)->GetDependencesEnd(); depend++) {
			outfile << " " << (*depend)->GetName();
		}
		outfile << endl;
	}
	outfile << "Dependences:" << endl;
	for (map<string, list<string>>::iterator each = (*depends).begin(); each != (*depends).end(); each++) {
		outfile << each->first;
		for (list<string>::iterator depend = each->second.begin(); depend != each->second.end(); depend++) {
			outfile << " " << *depend;
		}
		outfile << endl;
	}
	outfile.close();
	//删除所有内存（nodes,和里面每一项的所有依赖）
	for (list<Node*>::iterator node = (*nodes).begin(); node != (*nodes).end();) {
		delete(*node);
		node = (*nodes).erase(node);
	}
	(*nodes).clear();
	(*depends).clear();
}

int main(){
	list<Node*> nodes; //store all existing nodes
	map<string, list<string>> depends; //store all the dependences from original txt
	//read in the resource.txt and initialize nodes & depends
	if (InitFromResource(&nodes, &depends)) {
		printf("Resource Manager going to abort\n");
		return 1;
	}
	PrintInstrHelp();
	//TODO examine the input and execute corresponding function all the time
	while (true) {
		PrintGraph(&nodes);
		printf("Input your next operation\n");
		string instruction;
		getline(cin,instruction);
		list<string> temp_instr;
		SplitString(instruction, &temp_instr);
		instruction = temp_instr.front();
		//delete a node from graph
		if (!strcmp(instruction.c_str(),"delete")) {
			if (temp_instr.size() == 2) {
				DeleteNode(&nodes, &depends, temp_instr.back());
			}
			else {
				printf("ERROR: delete format wrong, please check!\n");
			}
			continue;
		}
		//insert a node or a dependence into graph
		else if (!strcmp(instruction.c_str(), "insert")) {
			temp_instr.pop_front();
			string sub_instr = temp_instr.front();
			if (!strcmp(sub_instr.c_str(),"node") && temp_instr.size()==2) {
				printf("Going to insert a node\n");
				FindCreatNode(&nodes, temp_instr.back());
			}
			else if (!strcmp(sub_instr.c_str(), "dependence") && temp_instr.size()==3) {
				temp_instr.pop_front();
				printf("Going to insert a dependence\n");
				CreatDependence(&nodes, temp_instr.front(), temp_instr.back());
				UpdateUsableNodes(&nodes, &depends);
			}
			else {
				printf("ERROR: insert instruction input failed, please check format!\n");
				printf("Going to print help\n");
			}
			continue;
		}
		else if (!strcmp(instruction.c_str(), "q")) {
			printf("Ready to quit the manager\n");
			ClearManager(&nodes, &depends);
			return 0;
		}
		else if (!strcmp(instruction.c_str(), "load")) {
			if (temp_instr.size() == 2) {
				printf("Ready to load in log file\n");
				ClearManager(&nodes, &depends);
				if (InitFromLog(&nodes, &depends, temp_instr.back())) { 
					printf("Initialization failed, use the older version\n"); 
				}
			}
			else {
				printf("ERROR: load format wrong, please check!\n");
			}
			continue;
		}
		else if (!strcmp(instruction.c_str(), "print")) {
			PrintDependences(&depends);
			continue;
		}
		else if(!strcmp(instruction.c_str(),"visualize")){
			VisualizaGraph(&nodes,&depends);
			continue;
		}
		else{
			// 检查并尝试进行节点删除，否则打印帮助
			if (temp_instr.size() == 1) {
				printf("Going to delete this node? y/n\n");
				char in = 'n';
				cin>>in;
				if (in == 'y') DeleteNode(&nodes, &depends, temp_instr.front());
				else if (in != 'n') printf("ERROR: wrong input\n");
			}
			else {
				printf("ERROR: wrong input format, please check!\n");
			}
			printf("Going to print help\n");
			PrintInstrHelp();
		}
	}
    return 0;
}