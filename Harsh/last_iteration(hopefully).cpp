#include <iostream>
#include <string>
#include <fstream>
#include <chrono> 
#include <climits>

using namespace std;

const int MAX_BUILDINGS = 1000;

class Room {
public:
    string name;
    int floor;
    int capacity;
    Room* next; 

    Room(string n, int f, int c, Room* nxt = nullptr) {
        name = n;
        floor = f;
        capacity = c;
        next = nxt;
    }
};

class PathNode {
public:
    string name; 
    int distance;
    PathNode* next; 

    PathNode(string n, int d, PathNode* nxt = nullptr) {
        name = n;
        distance = d;
        next = nxt;
    }
};

class BuildingNode {
public:
    string name;
    PathNode* adjList;      
    Room* rooms;            
    BuildingNode* next;     

    BuildingNode(string n) {
        name = n;
        adjList = nullptr;
        rooms = nullptr;
        next = nullptr;
    }
};

class Graph {
public:
    BuildingNode *head;

    Graph(){
        head = nullptr;
    }
};

class MyQueue {
public:
    int arr[MAX_BUILDINGS];
    int front = 0;
    int rear = 0;

    void push(int index) {
        if(rear < MAX_BUILDINGS){
            arr[rear++] = index;
        }
    }
    int pop() {
        if(!isEmpty()){
            return arr[front++];
        }
        return -1;
    }
    bool isEmpty() {
        return front == rear;
    }
};

BuildingNode* findBuilding(Graph &campus, string name) {
    BuildingNode* temp = campus.head;
    while(temp != nullptr) {
        if(temp->name == name){
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

int listToArray(Graph &campus, BuildingNode* arr[]) {
    int count = 0;
    BuildingNode* temp = campus.head;
    while(temp != nullptr && count < MAX_BUILDINGS) {
        arr[count++] = temp;
        temp = temp->next;
    }
    return count;
}

int getIndexFromArray(BuildingNode* arr[], int n, string name) {
    for(int i=0; i<n; i++) {
        if(arr[i]->name == name){
            return i;
        }
    }
    return -1;
}

void addRoom(Graph &campus, string buildingName, string roomName, int floor, int capacity){
    BuildingNode* building = findBuilding(campus, buildingName);
    if(building == nullptr){
        cout << "Building not found." << endl;
        return;
    }
    Room* temp = building->rooms;
    while(temp != nullptr){
        if(temp->name == roomName){
            cout << "Room already exists." << endl;
            return;
        }
        temp = temp->next;
    }
    
    Room* newRoom = new Room(roomName, floor, capacity, building->rooms);
    building->rooms = newRoom;
}

void addBuilding(Graph &campus, string name) {
    if (findBuilding(campus, name) != nullptr) {
        cout << "Building already exists." << endl;
        return;
    }
    
    BuildingNode* newNode = new BuildingNode(name);
    
    if (campus.head == nullptr) {
        campus.head = newNode;
        return;
    }
    
    BuildingNode* temp = campus.head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void addPath(Graph &campus, string from, string to, int dist) {
    BuildingNode* b1 = findBuilding(campus, from);
    BuildingNode* b2 = findBuilding(campus, to);

    if (b1 == nullptr || b2 == nullptr) {
        return;
    }

    PathNode* newNode1 = new PathNode(to, dist, b1->adjList);
    b1->adjList = newNode1;

    PathNode* newNode2 = new PathNode(from, dist, b2->adjList);
    b2->adjList = newNode2;
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

void deletePath(Graph &campus, string from, string to) {
    BuildingNode* b1 = findBuilding(campus, from);
    BuildingNode* b2 = findBuilding(campus, to);
    
    if (b1 != nullptr && b2 != nullptr) {
        removeEdge(b1, to);
        removeEdge(b2, from);
        cout << "Path deleted." << endl;
    } else {
        cout << "Buildings not found." << endl;
    }
}

void deleteBuilding(Graph &campus, string name) {
    BuildingNode* curr = campus.head;
    BuildingNode* prev = nullptr;

    while(curr != nullptr && curr->name != name) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        cout << "Building not found." << endl;
        return;
    }

    BuildingNode* temp = campus.head;
    while(temp != nullptr) {
        if(temp != curr) {
            removeEdge(temp, name);
        }
        temp = temp->next;
    }

    if (prev == nullptr) {
        campus.head = curr->next;
    } else {
        prev->next = curr->next;
    }

    delete curr;
    cout << "Building deleted." << endl;
}

void displayGraph(Graph &campus) {
    BuildingNode* temp = campus.head;
    while (temp != nullptr) {
        cout << temp->name << " -> ";
        PathNode* p = temp->adjList;
        while (p != nullptr) {
            cout << p->name << "(" << p->distance << ") ";
            p = p->next;
        }
        cout << " | Rooms: ";
        Room* r = temp->rooms;
        while (r != nullptr) {
            cout << r->name << " ";
            r = r->next;
        }
        cout << endl;
        temp = temp->next;
    }
}

int findMinDistance(int dist[], bool visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < n; v++){
        if (visited[v] == false && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void findShortestPath(Graph &campus, string start, string end, bool quiet = false) {
    BuildingNode* map[MAX_BUILDINGS];
    int n = listToArray(campus, map);

    int startIdx = getIndexFromArray(map, n, start);
    int endIdx = getIndexFromArray(map, n, end);

    if (startIdx == -1 || endIdx == -1) {
        if (!quiet){
            cout << "Buildings not found." << endl;
        }
        return;
    }

    int dist[MAX_BUILDINGS];
    bool visited[MAX_BUILDINGS];
    int parent[MAX_BUILDINGS];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[startIdx] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = findMinDistance(dist, visited, n);
        if (u == -1){
            break;
        }
        visited[u] = true;

        PathNode* curr = map[u]->adjList;
        while (curr != nullptr) {
            int v = getIndexFromArray(map, n, curr->name);
            if (v != -1 && !visited[v] && dist[u] != INT_MAX && dist[u] + curr->distance < dist[v]) {
                dist[v] = dist[u] + curr->distance;
                parent[v] = u;
            }
            curr = curr->next;
        }
    }

    if (!quiet) {
        if (dist[endIdx] == INT_MAX){
            cout << "No path exists." << endl;
        }
        else {
            cout << "Shortest Path Distance: " << dist[endIdx] << endl;
        }
    }
}

void runInsertionSort(BuildingNode* arr[], int n) {
    for (int i = 1; i < n; i++) {
        BuildingNode* key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j]->name > key->name) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void runShellSort(BuildingNode* arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            BuildingNode* temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap]->name > temp->name; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void merge(BuildingNode* arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    BuildingNode* L[500]; 
    BuildingNode* R[500];

    for (int i = 0; i < n1; i++){
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++){
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (L[i]->name <= R[j]->name){
            arr[k++] = L[i++];
        }
        else{
            arr[k++] = R[j++];
        }
    }
    while (i < n1){
        arr[k++] = L[i++];
    }
    while (j < n2){
        arr[k++] = R[j++];
    }
}

void runMergeSort(BuildingNode* arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        runMergeSort(arr, left, mid);
        runMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void runInsertionSortRooms(Room* arr[], int n) {
    for (int i = 1; i < n; i++) {
        Room* key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j]->name > key->name) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void runShellSortRooms(Room* arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            Room* temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap]->name > temp->name; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void mergeRooms(Room* arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Room* L[500]; 
    Room* R[500];

    for (int i = 0; i < n1; i++){
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++){
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (L[i]->name <= R[j]->name){
            arr[k++] = L[i++];
        }
        else{
            arr[k++] = R[j++];
        }
    }
    while (i < n1){
        arr[k++] = L[i++];
    }
    while (j < n2){
        arr[k++] = R[j++];
    }
}

void runMergeSortRooms(Room* arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        runMergeSortRooms(arr, left, mid);
        runMergeSortRooms(arr, mid + 1, right);
        mergeRooms(arr, left, mid, right);
    }
}

int runBinarySearch(BuildingNode* arr[], int n, string key) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid]->name == key){
            return mid;
        }
        if (arr[mid]->name < key){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    return -1;
}

int runBinarySearchRoom(Room* arr[], int n, string key) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid]->name == key){
            return mid;
        }
        if (arr[mid]->name < key){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    return -1;
}

void dfsRecursive(int u, bool visited[], BuildingNode* map[], int n, bool quiet) {
    visited[u] = true;
    if(!quiet){
        cout << map[u]->name << " ";
    }
    PathNode* curr = map[u]->adjList;
    while (curr != nullptr) {
        int v = getIndexFromArray(map, n, curr->name);
        if (v != -1 && !visited[v]){
            dfsRecursive(v, visited, map, n, quiet);
        }
        curr = curr->next;
    }
}

void runDFS(int startIdx, BuildingNode* map[], int n, bool quiet) {
    bool visited[MAX_BUILDINGS];
    for(int i=0; i<n; i++){
        visited[i] = false;
    }
    if(!quiet){
        cout << "DFS: ";
    }
    dfsRecursive(startIdx, visited, map, n, quiet);
    if(!quiet){
        cout << endl;
    }
}

void runBFS(int startIdx, BuildingNode* map[], int n, bool quiet) {
    bool visited[MAX_BUILDINGS];
    for(int i=0; i<n; i++){
        visited[i] = false;
    }
    MyQueue q;
    
    visited[startIdx] = true;
    q.push(startIdx);
    
    if(!quiet){
        cout << "BFS: ";
    }
    while (!q.isEmpty()) {
        int u = q.pop();
        if(!quiet){
            cout << map[u]->name << " ";
        }
        
        PathNode* curr = map[u]->adjList;
        while (curr != nullptr) {
            int v = getIndexFromArray(map, n, curr->name);
            if (v != -1 && !visited[v]) {
                visited[v] = true;
                q.push(v);
            }
            curr = curr->next;
        }
    }
    if(!quiet){
        cout << endl;
    }
}

void runBenchmarks(Graph &campus) {
    BuildingNode* temp[MAX_BUILDINGS];
    int n = listToArray(campus, temp);
    
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }

    cout << endl << "--- 1. SORTING BENCHMARK (Buildings) ---" << endl;
    BuildingNode* sortTemp[MAX_BUILDINGS];

    for(int i=0;i<n;i++){
        sortTemp[i] = temp[i];
    }
    auto t1 = chrono::high_resolution_clock::now();
    runInsertionSort(sortTemp, n);
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Insertion Sort: " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    for(int i=0;i<n;i++){
        sortTemp[i] = temp[i];
    }
    t1 = chrono::high_resolution_clock::now();
    runShellSort(sortTemp, n);
    t2 = chrono::high_resolution_clock::now();
    cout << "Shell Sort:     " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    for(int i=0;i<n;i++){
        sortTemp[i] = temp[i];
    }
    t1 = chrono::high_resolution_clock::now();
    runMergeSort(sortTemp, 0, n-1);
    t2 = chrono::high_resolution_clock::now();
    cout << "Merge Sort:     " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    for(int i=0;i<n;i++){
        temp[i] = sortTemp[i];
    }

    cout << endl << "--- 2. SEARCHING BENCHMARK (Buildings) ---" << endl;
    string target = temp[n-1]->name; 
    int iters = 1000;

    t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<iters; k++){
        findBuilding(campus, target);
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "Linear Search (x1k): " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<iters; k++){
        runBinarySearch(temp, n, target);
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "Binary Search (x1k): " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    cout << endl << "--- 3. TRAVERSAL BENCHMARK ---" << endl;
    int startIdx = 0;
    iters = 100;
    
    t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<iters; k++){
        runDFS(startIdx, temp, n, true);
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "DFS (x100): " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;

    t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<iters; k++){
        runBFS(startIdx, temp, n, true);
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "BFS (x100): " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;
    
    cout << endl << "--- 4. PATH ALGORITHM BENCHMARK ---" << endl;
    string endNode = temp[n-1]->name;
    string startNode = temp[0]->name;
    iters = 100;

    t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<iters; k++){
        findShortestPath(campus, startNode, endNode, true);
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "Dijkstra (x100):     " << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << " us" << endl;
}

void searchRoomLinearDemo(Graph &campus) {
    string bName;
    string rName;
    cout << "Enter Building Name: ";
    cin >> bName;
    
    BuildingNode* b = findBuilding(campus, bName);
    if (b == nullptr) {
        cout << "Building not found." << endl;
        return;
    }
    
    cout << "Enter Room Name to Search: ";
    cin >> rName;
    
    Room* temp = b->rooms;
    while(temp != nullptr) {
        if(temp->name == rName) {
            cout << "Room Found: " << temp->name << " [Floor: " << temp->floor << "]" << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Room not found." << endl;
}

void searchRoomDemo(Graph &campus, int sortType) {
    string bName;
    string rName;
    cout << "Enter Building Name: ";
    cin >> bName;
    
    BuildingNode* b = findBuilding(campus, bName);
    if (b == nullptr) {
        cout << "Building not found." << endl;
        return;
    }
    
    Room* rArr[MAX_BUILDINGS]; 
    int rCount = 0;
    Room* curr = b->rooms;
    while(curr != nullptr && rCount < MAX_BUILDINGS) {
        rArr[rCount++] = curr;
        curr = curr->next;
    }
    
    if (rCount == 0) {
        cout << "No rooms." << endl;
        return;
    }
    
    cout << "Sorting " << rCount << " rooms..." << endl;
    if (sortType == 1){
        runInsertionSortRooms(rArr, rCount);
    }
    else if (sortType == 2){
        runShellSortRooms(rArr, rCount);
    }
    else if (sortType == 3){
        runMergeSortRooms(rArr, 0, rCount-1);
    }
         
    cout << "Enter Room Name to Search: ";
    cin >> rName;
    
    int result = runBinarySearchRoom(rArr, rCount, rName);
    if (result != -1) {
        cout << "Room Found: " << rArr[result]->name << " [Floor: " << rArr[result]->floor << "]" << endl;
    } else {
        cout << "Room not found." << endl;
    }
}

void searchBuildingLinearDemo(Graph &campus) {
    string bName;
    cout << "Enter Building Name: ";
    cin >> bName;
    
    BuildingNode* b = findBuilding(campus, bName);
    if (b != nullptr) {
        cout << "Building Found: " << b->name << endl;
    } else {
        cout << "Building not found." << endl;
    }
}

void searchBuildingDemo(Graph &campus, int sortType) {
    if (campus.head == nullptr) {
        cout << "No buildings loaded." << endl;
        return;
    }

    BuildingNode* temp[MAX_BUILDINGS];
    int n = listToArray(campus, temp);
    
    cout << "Sorting " << n << " buildings..." << endl;
    if (sortType == 1){
        runInsertionSort(temp, n);
    }
    else if (sortType == 2){
        runShellSort(temp, n);
    }
    else if (sortType == 3){
        runMergeSort(temp, 0, n-1);
    }
    
    string bName;
    cout << "Enter Building Name to Search: ";
    cin >> bName;
    
    int result = runBinarySearch(temp, n, bName);
    if (result != -1) {
        cout << "Building Found: " << temp[result]->name << endl;
    } else {
        cout << "Building not found." << endl;
    }
}

void safeToFile(Graph &campus, string fileName) {
    ofstream file(fileName);
    if(!file){
        return;
    }

    BuildingNode* temp = campus.head;
    while(temp != nullptr) {
        file << temp->name << endl;
        Room* r = temp->rooms;
        while(r != nullptr) {
            file << "ROOM " << r->name << " " << r->floor << " " << r->capacity << endl;
            r = r->next;
        }
        temp = temp->next;
    }
    file << "END_BUILDINGS" << endl;

    temp = campus.head;
    while(temp != nullptr) {
        PathNode* curr = temp->adjList;
        while(curr != nullptr) {
            file << temp->name << " " << curr->name << " " << curr->distance << endl;
            curr = curr->next;
        }
        temp = temp->next;
    }
    file << "END_PATHS" << endl;
    file.close();
    cout << "Saved." << endl;
}

void loadFromFile(Graph &campus, string fileName) {
    ifstream file(fileName);
    if (!file) {
        cout << "Failed to open." << endl;
        return;
    }

    campus.head = nullptr;

    string line;
    BuildingNode* last = nullptr;

    while (getline(file, line)) {
        if (line == "END_BUILDINGS"){
            break;
        }
        if (line.find("ROOM ") == 0) {
            string temp = line.substr(5);
            int sp1 = temp.find(' ');
            string rName = temp.substr(0, sp1);
            string rest = temp.substr(sp1 + 1);
            int sp2 = rest.find(' ');
            int fl = stoi(rest.substr(0, sp2));
            int cap = stoi(rest.substr(sp2 + 1));
            if(last){
                addRoom(campus, last->name, rName, fl, cap);
            }
        } else {
            addBuilding(campus, line);
            last = findBuilding(campus, line);
        }
    }
    
    while (getline(file, line)) {
        if (line == "END_PATHS"){
            break;
        }
        string from = "";
        string to = "";
        string dist = "";
        int i = 0;
        while(i < line.length() && line[i] != ' '){
            from += line[i++];
        }
        i++;
        while(i < line.length() && line[i] != ' '){
            to += line[i++];
        }
        i++;
        while(i < line.length()){
            dist += line[i++];
        }
        if(from!="" && to!="" && dist!=""){
            addPath(campus, from, to, stoi(dist));
        }
    }
    file.close();
    cout << "Loaded." << endl;
}

int main() {
    Graph campus;
    int choice;
    string s1;
    string s2;
    string f;
    int d;

    while (1) {
        cout << endl << "===== CAMPUS SYSTEM (LINKED LIST + BINARY SEARCH) =====" << endl;
        cout << "1. Add Building" << endl;
        cout << "2. Add Room" << endl;
        cout << "3. Add Path" << endl;
        cout << "4. Delete Building" << endl;
        cout << "5. Delete Path" << endl;
        cout << "6. Display Graph" << endl;
        cout << "7. Save to File" << endl;
        cout << "8. Load from File" << endl;
        cout << "9. Find Shortest Path (Dijkstra)" << endl;
        cout << "10. DFS Reachability" << endl;
        cout << "11. BFS Reachability" << endl;
        cout << "12. Search Room (Linear)" << endl;
        cout << "13. Search Room (Binary + Insertion)" << endl;
        cout << "14. Search Room (Binary + Shell)" << endl;
        cout << "15. Search Room (Binary + Merge)" << endl;
        cout << "16. Search Building (Linear)" << endl;
        cout << "17. Search Building (Binary + Insertion)" << endl;
        cout << "18. Search Building (Binary + Shell)" << endl;
        cout << "19. Search Building (Binary + Merge)" << endl;
        cout << "20. RUN BENCHMARKS" << endl;
        cout << "0. Exit" << endl << "Choice: ";
        cin >> choice;

        if (choice == 0){
            break;
        }
        else if (choice == 1) {
            cout<<"Name: ";
            cin>>s1;
            addBuilding(campus, s1);
        }
        else if (choice == 2) {
            cout<<"Bld Name: ";
            cin>>s1;
            cout<<"Rm Name: ";
            cin>>s2;
            cout<<"Floor: ";
            cin>>d;
            addRoom(campus, s1, s2, d, 0);
        }
        else if (choice == 3) {
            cout<<"From: ";
            cin>>s1;
            cout<<"To: ";
            cin>>s2;
            cout<<"Dist: ";
            cin>>d;
            addPath(campus, s1, s2, d);
        }
        else if (choice == 4) {
            cout<<"Name: ";
            cin>>s1;
            deleteBuilding(campus, s1);
        }
        else if (choice == 5) {
            cout<<"From: ";
            cin>>s1;
            cout<<"To: ";
            cin>>s2;
            deletePath(campus, s1, s2);
        }
        else if (choice == 6) {
            displayGraph(campus);
        }
        else if (choice == 7) {
            cout<<"File: ";
            cin>>f;
            safeToFile(campus, f);
        }
        else if (choice == 8) {
            cout<<"File: ";
            cin>>f;
            loadFromFile(campus, f);
        }
        else if (choice == 9) {
            cout<<"Start: ";
            cin>>s1;
            cout<<"End: ";
            cin>>s2;
            findShortestPath(campus, s1, s2);
        }
        else if (choice == 10) {
            cout<<"Start: ";
            cin>>s1; 
            BuildingNode* map[MAX_BUILDINGS];
            int n = listToArray(campus, map);
            int idx = getIndexFromArray(map, n, s1);
            if(idx!=-1){
                runDFS(idx, map, n, false);
            }
        }
        else if (choice == 11) { 
            cout<<"Start: ";
            cin>>s1; 
            BuildingNode* map[MAX_BUILDINGS];
            int n = listToArray(campus, map);
            int idx = getIndexFromArray(map, n, s1);
            if(idx!=-1){
                runBFS(idx, map, n, false);
            }
        }
        else if (choice == 12) {
            searchRoomLinearDemo(campus);
        }
        else if (choice == 13) {
            searchRoomDemo(campus, 1);
        }
        else if (choice == 14) {
            searchRoomDemo(campus, 2);
        }
        else if (choice == 15) {
            searchRoomDemo(campus, 3);
        }
        else if (choice == 16) {
            searchBuildingLinearDemo(campus);
        }
        else if (choice == 17) {
            searchBuildingDemo(campus, 1);
        }
        else if (choice == 18) {
            searchBuildingDemo(campus, 2);
        }
        else if (choice == 19) {
            searchBuildingDemo(campus, 3);
        }
        else if (choice == 20) {
            runBenchmarks(campus);
        }
        else {
            cout << "Invalid." << endl;
        }
    }
    return 0;
}
