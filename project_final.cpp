#include <iostream>
#include <string>
#include <fstream>
using namespace std;


class Room {
public:
    string name;
    int floor;
    int capacity;
    Room* next;

    Room(string n, int f, int c, Room* nxt = nullptr){
        name = n;
        floor = f;
        capacity = c;
        next = nxt;
    }
};



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
    Room *rooms;
    BuildingNode *next;

    BuildingNode(string n) {
        name = n;
        adjList = nullptr;
        rooms = nullptr;
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


Room* findRoom(BuildingNode* building, string roomName){
    Room* temp = building->rooms;
    while(temp != nullptr){
        if(temp->name == roomName){
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}


void addRoom(BuildingNode* building, string roomName, int floor, int capacity){
    if(findRoom(building, roomName) != nullptr){
        cout << "Room already exists in this building.\n";
        return;
    }
    Room* newRoom = new Room(roomName, floor, capacity, building->rooms);
    building->rooms = newRoom;
    cout << "Room added successfully.\n";
}


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
        cout<<from<<" buildng not found"<<endl;
        return;
    }else if(b1 != nullptr && b2 == nullptr){
        cout<<to<<" buildng not found"<<endl;
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
        cout<<" | Rooms: ";
        Room* r = temp->rooms;
        while(r != nullptr){
            cout<<r->name<<"(F"<<r->floor<<",C"<<r->capacity<<") ";
            r = r->next;
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

void reachableDFSHelper(BuildingNode *building, bool visited[], string names[], int count){
    int index =-1;
    for(int i=0; i<count;i++){
        if(names[i] == building->name){
            index = i;
            break;
        }
    }

    if(index == -1 || visited[index]){
        return;
    }

    visited[index] = true;
    cout<<building->name<<" ";
    PathNode *adj = building->adjList;
    while(adj != nullptr){
        BuildingNode *nextBuilding = building->next;
        BuildingNode *temp = building;
        temp = building->next;
        BuildingNode *b = nullptr;
        BuildingNode *t = building;
        t = building;
        b = nullptr;

        BuildingNode *curr = building;
        while(curr != nullptr){
            if(curr->name == adj->name){
                b = curr;
                break;
            }
            curr = curr->next;
        }
        if(b != nullptr){
            reachableDFSHelper(b, visited, names, count);
        }
        adj = adj->next;
    }
}

void reachableDFS(Graph &campus, string start){
    int count =0;
    BuildingNode *temp = campus.head;
    while(temp != nullptr){
        count++;
        temp = temp->next;
    }

    if(count == 0){
        return;
    }

    string *names = new string[count];
    temp = campus.head;
    for(int i=0; i<count; i++){
        names[i] = temp->name;
        temp = temp->next;
    }

    bool *visited = new bool[count];
    for(int i=0;i<count; i++){
        visited[i] = false;
    }

    BuildingNode *startNode = campus.head;
    while(startNode != nullptr && startNode->name != start){
        startNode = startNode->next;
    }

    if(startNode == nullptr){
        cout<<"Building not found"<<endl;
        return;
    }

    cout<<"Reachable buildings from "<<start<<" (DFS): ";
    reachableDFSHelper(startNode, visited, names, count);
    cout <<endl;

}

void safeToFile(Graph &campus, string fileName){
    ofstream file(fileName);
    if(!file){
        cout << "Failed to open file " << fileName << endl;
        return;
    }

    BuildingNode *temp = campus.head;

    while(temp != nullptr){
        file << temp->name << endl;
        Room* r = temp->rooms;
        while(r != nullptr){
            file << "ROOM " << r->name << " " << r->floor << " " << r->capacity << endl;
            r = r->next;
        }
        temp = temp->next;
    }
    file << "END_BUILDINGS" << endl;

    temp = campus.head;
    while(temp != nullptr){
        PathNode* pathPtr = temp->adjList;
        while(pathPtr != nullptr){
            file << temp->name << " " << pathPtr->name << " " << pathPtr->distance << endl;
            pathPtr = pathPtr->next;
        }
        temp = temp->next;
    }
    file << "END_PATHS" << endl;

    file.close();
    cout<<"Data saved to "<< fileName <<" file successfully.\n";
}

void loadFromFile(Graph &campus, string fileName){
    ifstream file(fileName);
    if(!file){
        cout<<"Failed to open file "<< fileName << endl;
        return;
    }

    campus.head = nullptr;
    string line;
    BuildingNode* lastBuilding = nullptr;

    while(getline(file, line)){
        if(line == "END_BUILDINGS") break;
        if(line.rfind("ROOM", 0) == 0){
            int space1 = line.find(' ', 5);
            int space2 = line.find(' ', space1 + 1);
            string roomName = line.substr(5, space1 - 5);
            int floor = stoi(line.substr(space1 + 1, space2 - space1 - 1));
            int capacity = stoi(line.substr(space2 + 1));

            if(lastBuilding != nullptr){
                addRoom(lastBuilding, roomName, floor, capacity);
            }
        }else{
            addBuilding(campus, line);
            lastBuilding = findBuilding(campus, line);
        }
    }

    while(getline(file, line)){
        if(line == "END_PATHS") break;
        string from="", to="", distStr="";
        int i=0;
        while(i<line.length() && line[i] != ' '){
            from += line[i]; i++;
        }
        i++;
        while(i<line.length() && line[i] != ' '){
            to += line[i]; i++;
        }
        i++;
        while(i<line.length()){
            distStr += line[i]; i++;
        }
        int distance = stoi(distStr);
        addPath(campus, from, to, distance);
    }

    file.close();
    cout<<"Data loaded from "<< fileName <<" file successfully.\n";
}


void shortestPathBetweenRooms(Graph &campus, string room1, string room2){
    BuildingNode *b1 = campus.head;
    BuildingNode *b2 = campus.head;
    BuildingNode *building1 = nullptr;
    BuildingNode *building2 = nullptr;

    while(b1 != nullptr){
        if(findRoom(b1, room1) != nullptr){
            building1 = b1;
            break;
        }
        b1 = b1->next;
    }
    while(b2 != nullptr){
        if(findRoom(b2, room2) != nullptr){
            building2 = b2;
            break;
        }
        b2 = b2->next;
    }

    if(building1 == nullptr || building2 == nullptr){
        cout<<"One or both rooms not found in any building.\n";
        return;
    }

    cout << "Finding shortest path between buildings of rooms " << room1 << " and " << room2 << "...\n";
    findShortestPath(campus, building1->name, building2->name);
}


int main() {
    Graph campus;
    int choice;
    string from, to, name, filename;
    int dist, floor, capacity;
    string roomName1, roomName2;

    while(1) {
        cout<<endl;
        cout<<"===== Campus Navigation System =====" << endl;
        cout<<"1. Add Building" << endl;
        cout<<"2. Add Path" << endl;
        cout<<"3. Delete Building" << endl;
        cout<<"4. Delete Path" << endl;
        cout<<"5. Display Graph" << endl;
        cout<<"6. Find Shortest Path" << endl;
        cout<<"7. Show Reachable Buildings" << endl;
        cout<<"8. Save to File" << endl;
        cout<<"9. Load from File" << endl;
        cout<<"10. Add Room to Building" << endl;
        cout<<"11. Find Shortest Path Between Rooms" << endl;
        cout<<"0. Exit" << endl;
        cout<<"Enter choice: ";
        cin >> choice;

        if(choice == 0) {
            cout << "Exiting program..." << endl;
            break;
        }else if(choice == 1) {
            cout << "Enter building name: ";
            cin >> name;
            addBuilding(campus, name);
        } else if(choice == 2) {
            cout<< "Enter first building name: ";
            cin>> from;
            cout<< "Enter second building name: ";
            cin>> to;
            cout<< "Enter distance between them: ";
            cin>> dist;
            addPath(campus, from, to, dist);
        }else if(choice == 3) {
            cout<< "Enter building name to delete: ";
            cin>> name;
            deleteBuilding(campus, name);
        }else if(choice == 4) {
            cout<< "Enter first building name: ";
            cin>> from;
            cout<< "Enter second building name: ";
            cin>> to;
            deletePath(campus, from, to);
        }else if(choice == 5) {
            displayGraph(campus);
        }else if(choice == 6) {
            cout<< "Enter starting building: ";
            cin>> from;
            cout<< "Enter destination building: ";
            cin>> to;
            findShortestPath(campus, from, to);
        }else if(choice == 7) {
            cout<< "Enter starting building for DFS: ";
            cin>> from;
            reachableDFS(campus, from);
        }else if(choice == 8) {
            cout << "Enter filename to save data: ";
            cin>> filename;
            safeToFile(campus, filename);
        }else if(choice == 9) {
            cout<< "Enter filename to load data: ";
            cin>> filename;
            loadFromFile(campus, filename);
        }else if(choice == 10){
            cout << "Enter building name to add room: ";
            cin >> name;
            BuildingNode* b = findBuilding(campus, name);
            if(b == nullptr){
                cout << "Building not found.\n";
            }else{
                cout << "Enter room name: ";
                cin >> roomName1;
                cout << "Enter floor number: ";
                cin >> floor;
                cout << "Enter capacity: ";
                cin >> capacity;
                addRoom(b, roomName1, floor, capacity);
            }
        }else if(choice == 11){
            cout << "Enter first room name: ";
            cin >> roomName1;
            cout << "Enter second room name: ";
            cin >> roomName2;
            shortestPathBetweenRooms(campus, roomName1, roomName2);
        }else{
            cout<< "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}