#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <tuple>
#include <thread>

// DroneOperation Class Definitions
class DroneOperation {
public:
    virtual void execute() = 0;    //Defining execute method which can be used in different class for different Operations
    virtual ~DroneOperation() {}   //Deconstructor for Dronoperation after constructor initialization
};

class Takeoff : public DroneOperation {       //Takeoff class which inherits DronOperation class
public:
    void execute() override {                 //overiding execute method of original DronOperation class to function for Takeoff 
        std::cout << "Drone is taking off." << std::endl;
    }
};

class Survey : public DroneOperation {        //Survey class which inherits DronOperation class
public:
    void execute() override {                 //overiding execute method of original DronOperation class to function for Survey
        std::cout << "Drone is surveying the path." << std::endl;
    }
};

class ReturnToHome : public DroneOperation {   //ReturnHome class which inherits DronOperation class
public:
    void execute() override {                  //overiding execute method of original DronOperation class to function for ReturnToHome
        std::cout << "Drone is returning to home." << std::endl;
    }
};

class Land : public DroneOperation {           //Land class which inherits DronOperation class
public:
    void execute() override {                  //overiding execute method of original DronOperation class to function for Land
        std::cout << "Drone is landing." << std::endl;
    }
};

class Failure : public DroneOperation {        //Failure class which inherits DronOperation class
public:
    void execute() override {                  //overiding execute method of original DronOperation class to function for Failure
        std::cerr << "Drone has encountered a failure." << std::endl;
    }
};

class MissionPlanning {                        // MissionPlanning Class
public:
    MissionPlanning(const std::string& filename);     // Defining MissionPlanning Constructor
    void addEdge(int u, int v, int weight);           // addEdge method to add edge to the graph
    std::vector<int> dijkstra(int src, int dest);     //dijkstra algorithm to find shortest path between start noe and end node
    void printPath(const std::vector<int>& path);     //at last printing the shortest path

private:
    std::vector<std::vector<std::pair<int, int>>> adj;
    int V; // Number of vertices

    struct Node {
        int vertex;
        int distance;
        bool operator>(const Node& other) const {
            return distance > other.distance;
        }
    };
};

MissionPlanning::MissionPlanning(const std::string& filename) {      //MissionPlanning Constructor which takes filename as input and process the data of nodes present in csv file
    std::ifstream file(filename);                                    //Taking input from file
    std::string line;

    if (!file.is_open()) {                                          //Checking if file is open or not
        throw std::runtime_error("Unable to open file");            //If file is open somewhere then we cant access it here how will throw error that unable to open file
    }

    std::vector<std::tuple<int, int, int>> edges;                   //vector of tuple to easily modify values of nodes and weight

    while (std::getline(file, line)) {                              //Using while loop to take input from csv file ans storing node no and respective weights
        std::stringstream ss(line);                                 //Using StringStream to take any input
        int u, v, weight;
        char comma;

        ss >> u >> comma >> v >> comma >> weight;
        edges.emplace_back(u, v, weight);                           //puhing nodes and their distance in the edges vector
    }

    V = 0;                                                          //Number of vertices, will be updated
    for (const auto& [u, v, weight] : edges) {                      
        if (u > V) V = u;
        if (v > V) V = v;
    }
    V++;                                                            //Since vertices are 0-indexed

    adj.resize(V);

    for (const auto& [u, v, weight] : edges) {                      //Adding edges to graph
        addEdge(u, v, weight);
    }
}

void MissionPlanning::addEdge(int u, int v, int weight) {           //addEdge Method of MissionPlanning class to add an Edge 
    adj[u].emplace_back(v, weight);
    adj[v].emplace_back(u, weight);                                 // Undirected graph
}

std::vector<int> MissionPlanning::dijkstra(int src, int dest) {     // dijkstra algorithm Method of  MissionPlanning Class which takes input as source and destination
    std::vector<int> dist(V, std::numeric_limits<int>::max());
    std::vector<int> parent(V, -1);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;       //Queue to push nodes

    dist[src] = 0;                                                              //Initially distance from source 0 to  is 0
    pq.push({src, 0});

    while (!pq.empty()) {                                                       //While Queue isEmpty or nodes a re still present in the queue
        int u = pq.top().vertex;                                                //storing queue top value in u
        pq.pop();                                                               //popping queue topvalue

        for (const auto& [v, weight] : adj[u]) {                                //for each value of node v , weight(distance) from adj veector which containd node and distance values
            if (dist[u] + weight < dist[v]) {                                   //Check if a shorter path to v exists via u   
                dist[v] = dist[u] + weight;                                     //if it exists then update the distance to v
                parent[v] = u;                                                  //and set parent of v to u
                pq.push({v, dist[v]});                                          //pust node v and diatance of node v to queue
            }
        }
    }

    std::vector<int> path;                                                      //initialize shortest path
    for (int at = dest; at != -1; at = parent[at]) {                            //push shortest path in the path vector
        path.push_back(at);
    }
    // std::reverse(path.begin(), path.end());                                     //reverse the vector order as path is pushed as stack

    printPath(path);                                                            //print the path
    return path;                                                                //return the path
}               

void MissionPlanning::printPath(const std::vector<int>& path) {                 //printpath method of MissionPlanning class which takes path as input
    std::cout << "Shortest path: ";
    // for (int node : path) {                                                  //iterate over path
    //     std::cout << node << " ";
    // }
    // std::cout << std::endl;
    for(int i=path.size()-1;i>=0;i--)
    {
        std::cout<<path[i];
        if(i!=0)
        {
            std::cout<<"->";
        }
    }
    std::cout << std::endl;
}

void executeOperation(DroneOperation* operation) {                                // Function to be run by threads
    operation->execute();
}

int main() {
    try {
        // Create DroneOperation objects
        DroneOperation* takeoff = new Takeoff();                                  //Drone object of Takeoff class
        DroneOperation* survey = new Survey();                                    //Drone object of Survey class
        DroneOperation* returnHome = new ReturnToHome();                          //Drone object of ReturnToHome class
        DroneOperation* land = new Land();                                        //Drone object of Land class
        DroneOperation* failure = new Failure();

        
        std::thread takeoffThread(executeOperation, takeoff);                     //Execute takeoff operation in a separate thread
        takeoffThread.join();                                                     //Ensure takeoff completes before proceeding

        MissionPlanning mission("graph.csv");                                     //Initialize MissionPlanning with CSV data

        int src, dest;
        std::cout << "Enter starting position: ";                                 
        std::cin >> src;                                                          //Enter Start postion node
        std::cout << "Enter ending position: ";
        std::cin >> dest;                                                         //Enter end postion node

        if (src<0 || dest<0) {
            std::cerr << "Error: Positions must be non-negative integers." << std::endl;    //input should be non negative
            failure->execute();
            return 1;
        }

        if (src >100 || dest> 100) {
            std::cerr << "Error: Entered Nodes are notd present in the graph." << std::endl;    //input should be non negative
            failure->execute();
            return 1;
        }

        std::vector<int> path;
        std::thread missionThread([&]() {                                         //Running mission planning in a separate thread
            path = mission.dijkstra(src, dest);
        });
        missionThread.join();                                                     //Ensuring mission planning completes before proceeding to other functions

        std::thread surveyThread(executeOperation, survey);                       //Executing survey operation in separate threads
        std::thread returnHomeThread(executeOperation, returnHome);               //Executing return home operation in separate threads
        std::thread landThread(executeOperation, land);                           //Executing land operation in separate threads

        surveyThread.join();
        returnHomeThread.join();                                                  //joining all the threads
        landThread.join();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
