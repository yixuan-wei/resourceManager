#include "pch.h"
#include <list>
#include <string>
#include "nodes.hpp"
#include "Node.hpp"
#include <fstream>

int main(){
    //TODO 读取resource.txt，建立依赖的dict
    fstream resourceFile("resource.txt",std::ios::in);
    if(!resourceFile.is_open()){
        std::cout<<"Opening resource file failed"<<std::endl;
    }
    while(!resourceFile.eof()){
        std::string dependence;
        getline(resourceFile, dependence);
        printf("%s\n",dependence);
    }
    //TODO 建立Node网络，记录网络每一种（个？）资源的dict
    //TODO 实时检查输入
    //TODO 检查删除节点的功能
    //TODO 退出功能
    return 0;
}