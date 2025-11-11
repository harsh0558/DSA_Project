#include <iostream>
using namespace std;

class PathNode{
public:
    string name;
    int distance;
    PathNode *next;

    PathNode(string n, int d, PathNode* nxt = nullptr){
        name = n;
        distance = d;
        next = nxt;
    }

};

class BuildingNode{
public:
    string name;
    PathNode *adjList;
    BuildingNode *next;

    BuildingNode(string n) {
        name = n;
        adjList = nullptr;
        next = nullptr;
    }
};

class Graph{
public:
    BuildingNode *head;

    Graph(){
        head = nullptr;
    }
};

BuildingNode* findBuilding(Graph &campus, string name){
    BuildingNode* temp = campus.head;
    while(temp != nullptr){
        if(temp->name == name){
            return temp;
        }else{
            temp = temp->next;
        }
    }    
    return nullptr;   
}

void addPath(Graph &campus, string from, string to, int dist){
    BuildingNode *b1 = findBuilding(campus,from);
    BuildingNode *b2 = findBuilding(campus,to);

    if(b1 == nullptr && b2 != nullptr){
        cout<< from<<" buildng not found"<<endl;
        return;
    }else if(b1 != nullptr && b2 == nullptr){
        cout<< to<<" buildng not found"<<endl;
        return;
    }else if(b1 == nullptr && b2 == nullptr){
        cout<<"buildings not found"<<endl;
        return;
    }

    PathNode *newPath1 = new PathNode(to, dist, b1->adjList);
    b1->adjList = newPath1;

    PathNode *newPath2 = new PathNode(from, dist, b2->adjList);
    b2->adjList = newPath2;
}

void addBuilding(Graph &campus, string name){
    BuildingNode* newBuilding = new BuildingNode(name);

    if(campus.head == nullptr){
        campus.head = newBuilding;
        return;
    }
    BuildingNode* temp = campus.head;
    while(temp->next != nullptr){
        temp = temp->next;
        
    }
    temp->next = newBuilding;
}

void displayGraph(Graph &campus){
    BuildingNode *temp = campus.head;
    while(temp != nullptr){
        cout<< temp->name<< " -> ";
        PathNode *adj = temp->adjList;
        while(adj != nullptr){
            cout<<adj->name<<" ("<<adj->distance<<") ";
            adj = adj->next;
        }
        cout<<endl;
        temp = temp->next;
    }
    
}

void removeEdge(BuildingNode *building, string name){
    PathNode *curr = building->adjList;
    PathNode *prev = nullptr;

    while(curr !=nullptr && curr->name != name){
        prev = curr;
        curr = curr->next;
    }
    if(curr == nullptr){
        return;
    }

    if(prev == nullptr){
        building->adjList = curr->next;
    }else{
        prev->next = curr->next;
    }

    delete curr;

}

void deletePath(Graph &campus, string from, string to){
    BuildingNode *b1 = findBuilding(campus, from);
    BuildingNode *b2 = findBuilding(campus, to);

    if(b1 == nullptr && b2 != nullptr){
        cout<< from<<" buildng not found"<<endl;
        return;
    }else if(b1 != nullptr && b2 == nullptr){
        cout<< to<<" buildng not found"<<endl;
        return;
    }else if(b1 == nullptr && b2 == nullptr){
        cout<<"buildings not found"<<endl;
        return;
    }

    removeEdge(b1,to);
    removeEdge(b2,from);

    cout<<"path between "<<from<<" and "<<to<<" deleted"<<endl;

}

void deleteBuilding(Graph &campus, string name){
    BuildingNode *curr = campus.head;
    BuildingNode *prev = nullptr;

    while(curr != nullptr && curr->name != name){
        prev = curr;
        curr = curr->next;
    }

    if(curr == nullptr){
        cout<<"building not found"<<endl;
        return;
    }

    BuildingNode *temp = campus.head;
    while(temp != nullptr){
        if(temp->name != name){
            PathNode *pathCurr = temp->adjList;
            PathNode *pathPrev = nullptr;

            while(pathCurr != nullptr){
                if(pathCurr->name == name){
                    if(pathPrev == nullptr){
                        temp->adjList = pathCurr->next;
                    }else{
                        pathPrev->next = pathCurr->next;
                    }
                    delete pathCurr;
                    break;
                }
                pathPrev = pathCurr;
                pathCurr = pathCurr->next;
            }
        }
        temp = temp->next;
    }
    
    PathNode *adj = curr->adjList;
    while(adj != nullptr){
        PathNode *toDlt = adj;
        adj = adj->next;
        delete toDlt;
    }

    if(prev == nullptr){
        campus.head = curr->next;
    }else{
        prev->next = curr->next;
    }
    delete curr;
}

int findMinDistance(int dist[], bool visited[], int size){
    int min = INT_MAX;
    int minIdx = -1;
    for(int i=0;i<size;i++){
        if(!visited[i] && dist[i] <min){
            min = dist[i];
            minIdx = i;
        }
    }
    return minIdx;
}


void findShortestPath(Graph &campus, string start, string end){
    int count =0;
    BuildingNode* temp = campus.head;
    while(temp != nullptr){
        count++;
        temp = temp->next;
    }

    if(count == 0){
        cout<<" No buildings added"<<endl;
        return;
    }

    string *names = new string[count];
    temp = campus.head;
    for(int i=0;i<count;i++){
        names[i] = temp->name;
        temp = temp->next;
    }

    int *dist = new int[count];
    bool *visited = new bool[count];
    int *parent = new int[count];

    for(int i=0;i<count;i++){
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }

    int startIndex = -1;
    int endIndex = -1;
    for(int i=0;i<count;i++){
        if(names[i] == start){
            startIndex = i;
            
        }
        if(names[i] == end){
            endIndex = i;
        }
    }

    if(startIndex == -1 || endIndex == -1){
        cout<<"Either of the building not found"<<endl;
        return;
    }

    dist[startIndex] = 0;
    for(int step = 0; step<count-1;step++){
        int u = findMinDistance(dist,visited,count);
        if(u==-1){
            break;
        }
        visited[u] = true;

        BuildingNode* curr = campus.head;
        for(int i=0;i<u;i++){
            curr = curr->next;
        }

        PathNode *adj = curr->adjList;
        while(adj !=nullptr){
            int v = -1;
            for(int i=0;i<count;i++){
                if(names[i] == adj->name){
                    v =i;
                    break;
                }
            }
            if(v != -1 && !visited[v] && dist[u] + adj->distance <dist[v]){
                dist[v] = dist[u] + adj->distance;
                parent[v] = u;
            }
            adj = adj->next;
        }
    }

    if(dist[endIndex] == INT_MAX){
        cout<<"No path exists"<<endl;
    }else{
        cout<<"Shortest path from "<<start<<" to "<<end<<endl;
        cout<<"Distance: "<<dist[endIndex]<<endl;
        cout<<"Path: ";

        string path[50];
        int pathLen = 0;
        int curr = endIndex;
        while(curr != -1){
            path[pathLen++] = names[curr];
            curr = parent[curr];
        }

        for(int i=pathLen -1; i>=0;i--){
            cout<<path[i];
            if(i>0){
                cout<<" -> ";
            }
        }
        cout<<endl;
    }
}

int main() {
    Graph campus;

    addBuilding(campus, "CS_Block");
    addBuilding(campus, "Library");
    addBuilding(campus, "Admin");
    addBuilding(campus, "Cafeteria");

    addPath(campus, "CS_Block", "Library", 5);
    addPath(campus, "CS_Block", "Admin", 10);
    addPath(campus, "Library", "Admin", 7);
    addPath(campus, "Library", "Cafeteria", 3);

    displayGraph(campus);

    cout << "\nFinding shortest path from CS_Block to Cafeteria...\n";
    findShortestPath(campus, "CS_Block", "Cafeteria");

    return 0;
}
