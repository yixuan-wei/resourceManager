#include <list>
#include <string>

class Node{
    private:
        std::string name=nullptr;
        std::list<Node*> dependences;
    public:
        Node(const char* string,const char* dependence){
            SetName(string);
            UpdateDependence(dependence);
            nodes.push_back(this);
        }
        ~Node(){
            
            dependences.clear();
        }
        std::string GetName(){return name;}
        std::list<Node*> GetDependences(){return dependences;}
        void SetName(const char* newName){name.assign(newName);}
        //void InitDependences(std::list<Node*> newDependences){dependences = newDependences;}
        void UpdateDependence(const char* dependence){
            Node* newDependence=nullptr;
            for(std::list<Node*>::iterator dependIter = dependences.begin(); dependIter!=dependences.end();dependIter++){
                if(!strcmp((*dependIter)->GetName().c_str(),dependence)){
                    newDependence = *dependIter;
                    printf("WARNING: the dependence from %s to %s already exists\n");
                }
            }
            dependences.push_back(newDependence);
        }
        void DeleteDependence(char* target){
            //TODO 对dependences遍历，检查target
            for(std::list<Node*>::iterator dependIter = dependences.begin(); dependIter!=dependences.end();dependIter++){
                if((*dependIter)->GetName()==target){

                }
            }
        }
};

std::list<Node*> nodes;// to store all existing nodes

std::list<Node*>::iterator SearchFromNodes(std::string target){
    std::list<Node*>::iterator each;
    for(each=nodes.begin();each!=nodes.end();each++){
        if(!strcmp((*each)->GetName().c_str(),target.c_str())){
            return each;
        }
    }
    return each;
}

int DeleteFromNodes(std::string target)
{
    std::list<Node*>::iterator t = SearchFromNodes(target);
    if(t!=nodes.end()){
        nodes.erase(t);
        return 0;
    }
    return 1;
}

int main(){
    //TODO 读取resources.txt，建立依赖的dict
    //TODO 建立Node网络，记录网络每一种（个？）资源的dict
    //TODO 实时检查输入
    //TODO 检查删除节点的功能
    //TODO 退出功能
    return 0;
}