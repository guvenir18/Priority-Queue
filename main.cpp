/*
    İlke Anıl Güvenir
    150180042
    Note: Please compile with c++11
*/


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std;


// Our taxi struct to hold coordinates.
struct Taxi {
    double longtitude;
    double latitude;
    double distance;
    Taxi();
    Taxi(double, double);
    void calculateDistance(double, double);
};

// Taxi constructor
Taxi::Taxi(double longtitude, double latitude) {
    this->longtitude = longtitude;
    this->latitude = latitude;
}

// Default taxi constructor
Taxi::Taxi() {
    longtitude = 0.0;
    latitude = 0.0;
    distance = 0.0;
}

// Calculates euclidian distance of target distance and taxi distance using math.h functions
void Taxi::calculateDistance(double target_longtitude, double target_latitude) {
    distance = sqrt(pow(target_longtitude - longtitude, 2) + pow(target_latitude - latitude, 2));
}


// Priority Queue
struct PriorityQueue {
    Taxi* head;
    int size;
    int capacity;
    double main_longtitude;
    double main_latitude;
    PriorityQueue(int);
    ~PriorityQueue();
    void addTaxi(Taxi);
    Taxi callTaxi();
    void updateTaxi();
    void shiftUp(int);
    void shiftDown(int);
};

// Binary heap function to relocate a node downwards (When we remove minimum node and replace it with last node).
void PriorityQueue::shiftDown(int index) {
    int maxIndex = index; // Current node we are comparing
    int left = 2 * index + 1; // Left child node.
    // Check if we are at the end of tree and if our left child node is smaller then current node.
    if (left <= size && head[left].distance < head[maxIndex].distance) { 
        maxIndex = left; 
    } 
    int right = 2 * index + 2; // Right child node.
    // Check if we are at the end of tree and if our right child node is smaller then current node.
    if (right <= size && head[right].distance < head[maxIndex].distance) { 
        maxIndex = right; 
    }
    // If function founds a smaller node, then swap it and call recursively itself to check remaining nodes. Otherwise do nothing.
    if (index != maxIndex) {
        Taxi temp = head[index];
        head[index] = head[maxIndex];
        head[maxIndex] = temp; 
        shiftDown(maxIndex); 
    } 
}

// Binary heap function to relocate a node upwards. (when we add a new node)
void PriorityQueue::shiftUp(int index) {
    // Check if we are at the start of heap and if parent of node is larger than our node. Then swap and continue 
    while(index > 0 && head[(index - 1) / 2].distance > head[index].distance) {
        Taxi temp = head[index];
        head[index] = head[(index - 1) / 2];
        head[(index - 1) / 2] = temp;
        index = (index - 1) / 2;
    }
}

// Constructor for priority queue. 
PriorityQueue::PriorityQueue(int capacity) {
    this->capacity = capacity;
    head = new Taxi[capacity];
    size = 0;
    main_longtitude = 33.40819;
    main_latitude = 39.19001;
}

// Destructor for priority queue
PriorityQueue::~PriorityQueue() {
    delete[] head;
}

// Function to add new taxi into heap.
void PriorityQueue::addTaxi(Taxi newTaxi) {
    head[size] = newTaxi; // Add new taxi to end of array.
    shiftUp(size); // Shift up new node
    size++; // Increase size.
}

// Function to call a taxi from heap (Remove minimum)
Taxi PriorityQueue::callTaxi() {
    Taxi called = head[0];
    head[0] = head[size-1];
    size--;
    shiftDown(0);
    return called;
}

// Function to update a taxi's distance
void PriorityQueue::updateTaxi() {
    int i;
    if (size == 0) { // If there is no taxi, just skip operation.
        return;
    } else if (size == 1) {
        i = 0;
    } else {
        i = rand() % size; // Choose randomly a taxi.
    }
    double old_distance = head[i].distance;
    head[i].distance -= 0.01;
    shiftUp(i); // Relocate updated taxi.
}

int main(int argc, char* argv[])
{
    PriorityQueue taxiq = PriorityQueue(1000000); // Create a binary heap with 1M capacity.
    fstream file;
    srand(time(NULL));
    file.open("locations.txt");
    string dummy;
    getline(file, dummy);
    int m = stoi(argv[1]); // Number of operations
    int c_operation = 0; // Total operation counter
    int c_update = 0; // Update operation counter
    int c_add = 0; // Add operation counter
    double p = p = stod(argv[2]); // Probability.
    auto start = chrono::high_resolution_clock::now();
    while(c_operation < m) {
        int event = rand();
        // here if we multiply our maximum number we can get from rand() with our p then call a random number with rand()
        // and check if it is smaller or larger then our multiply, we can simulate probability.
        if (event <= RAND_MAX * p) { // p case
            taxiq.updateTaxi();
            c_update++;
        } else {   // 1 - p case.
            double longtitude, latitude;
            file >> longtitude >> latitude; // Read values from file
            Taxi newtaxi = Taxi(longtitude, latitude);
            newtaxi.calculateDistance(taxiq.main_longtitude, taxiq.main_latitude); // Pass coordinates of hotel into function
            taxiq.addTaxi(newtaxi);
            c_add++;
        }
        c_operation++;
        if (c_operation % 100 == 0) { // Whenever operation count reaches 100, call a taxi
            Taxi calledtaxi = taxiq.callTaxi();
            cout << "Distance of called taxi: " << calledtaxi.distance << endl;
        }
    }
    auto stop = chrono::high_resolution_clock::now(); 
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	auto total_duration = duration.count();
	cout << "Program took: " << total_duration/1000 << " milliseconds\n";
    cout << "Number of taxi additions: " << c_add << " Total number of distance updates: " << c_update << endl;
    return 0;
}