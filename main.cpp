//Bahattin Taşcı 31076

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Queue {
private:

    vector<int> array;
    int front, rear;

public:
    Queue() {
        front = 0;
        rear = 0;
    }

    int return_front() {
        return array[front];
    }

    bool isEmpty() const {
        return front == rear;
    }

    void enqueue(int value) {
        array.push_back(value);
        rear++;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return -1;
        }
        int dequeuedValue = array[front];
        front++;
        return dequeuedValue;
    }

    int peek() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return -1;
        }
        return array[front];
    }
};


class Graph {
private:
    int ver_size;
    vector<vector<int>> adjacenyList;
    vector<int> matched_node;  // Store for the matching pairs
    vector<bool> visited_node;  // for in finding augmenting paths

public:
    Graph(int v) {
        ver_size = v;
        adjacenyList.resize(v);
        matched_node.resize(v, -1);  // Initialize all vertices as unmatched
    }

    void edgeCreating(int u, int v) {
        if (u >= 1 && v >= 1 && u < ver_size &&  v < ver_size) {
            adjacenyList[u].push_back(v);
            adjacenyList[v].push_back(u);
        }
        else {
            if (u != 0 && v != 0) {
                cout << "Invalid Edge: " << u << " - " << v << endl;
            }

        }
    }


    // Find an augmenting path using DFS
    bool depthFirstSearch(int u, vector<bool>& visited) {
        for (int v : adjacenyList[u]) {
            if (!visited[v]) {
                visited[v] = true;

                // If v is not matched or recursively if the vertex it is matched to has an augmenting path
                if (matched_node[v] == -1 || depthFirstSearch(matched_node[v], visited)) {
                    matched_node[v] = u;
                    matched_node[u] = v;
                    return true;
                }
            }
        }
        return false;
    }

    // In the bipartite graph finding maximum matching
    int matchMax() {
        int maxMatch = 0;
        vector<int> greenNode,redNode;

        // Check if the graph is bipartite and after that get the two color sets
        if (!bipartite_checking(&redNode, &greenNode)) {

            return -1;
        }

        // In red set try to find augmenting path for each unmatched vertex
        for (int u : redNode) {
            visited_node.assign(ver_size, false);
            if (matched_node[u] == -1 && depthFirstSearch(u, visited_node)) {
                maxMatch++;
            }
        }

        cout << "Maximum matching size: " << maxMatch << endl;
        return maxMatch;
    }

    bool bipartite_checking(vector<int>* red = NULL, vector<int>* green = NULL) {
        // -1: uncolored
        // 0: red
        // 1: green
        vector<int> colors_of_nodes(ver_size, -1);
        bool bipartite_mini_check = true;
        vector<int> redNodes, greenNodes;

        for (int start = 1; start < ver_size; ++start) {
            if (colors_of_nodes[start] == -1) {
                Queue queue;
                colors_of_nodes[start] = 0;
                redNodes.push_back(start);
                queue.enqueue(start);

                while (!queue.isEmpty() && bipartite_mini_check) {
                    int u = queue.return_front();
                    queue.dequeue();

                    for (int v : adjacenyList[u]) {
                        if (colors_of_nodes[v] == -1) {
                            colors_of_nodes[v] = 1 - colors_of_nodes[u];
                            if (colors_of_nodes[v] == 0) {
                                redNodes.push_back(v);
                            }
                            else {
                                greenNodes.push_back(v);
                            }
                            queue.enqueue(v);
                        }
                        else if (colors_of_nodes[v] == colors_of_nodes[u]) {
                            bipartite_mini_check = false;
                            v = v+u;
                        }
                    }
                }
            }
        }


        if (bipartite_mini_check) {
            cout << "The graph is bipartite." << endl;

            if (green != NULL) {
                *green = greenNodes;
            }

            if (red != NULL) {
                *red = redNodes;
            }


        }
        else {
            cout << "The graph is not bipartite." << endl;
        }

        return bipartite_mini_check;
    }
};

int main() {
    bool check = true;
    while (check) {
        string graphname;
        cout << "Enter the graph name (e.g., 'graph1' or 'exit' to terminate): ";
        cin >> graphname;

        if (graphname == "exit") {
            check = false;
        }
        else {
            graphname += ".txt";
            ifstream filename(graphname);
            if (!filename.is_open()) {
                cout << "Error opening file!" << endl;
                continue;
            }

            int vertic_number, edge_num, x, y;
            filename >> vertic_number >> edge_num;

            Graph graph(vertic_number + 1);

            while (filename >> x >> y) {
                graph.edgeCreating(x, y);
            }

            graph.matchMax();
            filename.close();
        }
    }

    return 0;
}