# resourceManager
This is a resource manager tool for programming portfolio of SMU Guildhall 2018, 
specific realization requirements could refer to "requirement.png". <br>
**Requirements**: C++17, built by g++ (GCC version 6.3.0), using the graphviz open-source tool for visualization of directed tool, only suitable for windows

### File Explanation:
- **resourceManager.cpp**: Responsible for main activity of resource manager, including commands and exact realization for each 
command
- **Node.hpp**: The Node class, including definition of all functions and parameters.
- **Node.hpp.gch**: temp file to boost building for g++ of file Node.hpp
- **ResourceManagerLog**: log file to store the manager status the last time it quits, read according to command
- **resource.txt**: stores the dependence relationship, read when launching the manager, 
would initialize the dependence relationship from start, unable to change relationship unless quit and refresh resource.txt
- **resourceManager.exe**: the successful version built from g++
- **test.png**: the graph visualization image
- **requirement.png**: exact requirements from the SMU Guildhall on resource manager

### Commands supported:
- **delete n**               :delete a node named n from the graph
- **insert node n**          :insert a node named n into the graph
- **insert dependence m n**  :insert a dependence that m depends on n into the graph
- **q**                      :quit the resource manager
- **load route_to_log**      :load the stored graph and dependences from route_to_log
- **print**                  :print the dependences for all resources, whether exists or not
- **visualize**              :visualize the graph
