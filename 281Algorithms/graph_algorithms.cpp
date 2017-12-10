#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <limits>
#include <queue>

using namespace std;

// Simple ADT of a graph
class Graph {
public:
    // ADT for walking through adjacent vertices of a vertex in the graph
    class Adj_Iterator {
    public:
        
        // copy ctor
        Adj_Iterator(const Adj_Iterator& rhs) {
            this->is_adj_list = rhs.is_adj_list;
            this->adj_list_ptr = rhs.adj_list_ptr;
            this->it_list = rhs.it_list;
            
            this->adj_matrix_ptr = rhs.adj_matrix_ptr;
            this->it_matrix = rhs.it_matrix;
            
        }
        
        // inject iterator of current vertext
        Adj_Iterator(vector<int>::const_iterator it_matrix,
                     const vector<int>* adj_matrix_ptr) {
            this->it_matrix = it_matrix;
            this->adj_matrix_ptr = adj_matrix_ptr;
            this->is_adj_list = false;
            // since it's matrix, proceed to first valid vertex
            while (this->it_matrix != this->adj_matrix_ptr->end() &&
                   *(this->it_matrix) == -1) {
                ++ this->it_matrix;
            }
        }
        
        Adj_Iterator(vector<pair<int, int>>::const_iterator it_list,
                     const vector<pair<int, int>>* adj_list_ptr) {
            this->it_list = it_list;
            this->adj_list_ptr = adj_list_ptr;
            this->is_adj_list = true;
        }
        
        // assignment operator
        Adj_Iterator& operator=(const Adj_Iterator& rhs) {
            if (*this == rhs) {
                return *this;
            }
            this->is_adj_list = rhs.is_adj_list;
            this->adj_list_ptr = rhs.adj_list_ptr;
            this->it_list = rhs.it_list;
            
            this->adj_matrix_ptr = rhs.adj_matrix_ptr;
            this->it_matrix = rhs.it_matrix;
            return *this;
        }
        
        // increment by 1
        Adj_Iterator& operator++() {
            if (this->is_adj_list) {
                this->it_list ++;
            } else {
                this->it_matrix ++;
                
                while (this->it_matrix != this->adj_matrix_ptr->end() &&
                       *(this->it_matrix) == -1) {
                    this->it_matrix ++;
                }
            }
            return *this;
        }
        
        // check equality
        bool operator==(const Adj_Iterator& rhs) const {
            if (this->is_adj_list != rhs.is_adj_list) return false;
            if (this->is_adj_list) {
                return this->it_list == rhs.it_list;
            }
            return this->it_matrix == rhs.it_matrix;
        }
        
        bool operator!=(const Adj_Iterator& rhs) const {
            return !(*this == rhs);
        }
        
        // return copy of the adjacent vertext id
        int operator*() const {
            if (this->is_adj_list) {
                return this->it_list->first;
            }
            return static_cast<int>(this->it_matrix - this->adj_matrix_ptr->begin());
        };
        
    private:
        // make Graph a friend
        friend class Graph;
        // flag check which iterator to use
        bool is_adj_list;
        // lower level vector iterator & their srcs
        const vector<int>* adj_matrix_ptr;
        const vector<pair<int, int>>* adj_list_ptr;
        vector<int>::const_iterator it_matrix;
        vector<pair<int, int>>::const_iterator it_list;
        
    };
    
    // Mode types
    enum Mode { ADJ_MATRIX, ADJ_LIST, BOTH };
    
public:
    
    // set num vertices
    // REQUIRED
    void set_num_vertices(int num) {
        this->num_vertices = num;
    }
    
    // get num vertices
    int get_num_vertices() const {
        return this->num_vertices;
    }
    
    const vector<vector<int>>& get_edges() const {
        return this->edges;
    }
    
    // set whether the graph is directed or not
    void set_directed(bool directed) {
        this->is_directed = directed;
    }
    
    // adj_matrix or adj_list or both
    void set_graph_representation(Mode mode) {
        assert(this->num_vertices != -1);
        
        this->mode = mode;
        if (this->mode == ADJ_LIST || this->mode == BOTH) {
            this->adj_list.resize(this->num_vertices);
        }
        if (this->mode == ADJ_MATRIX || this->mode == BOTH) {
            // init with V by V matrix
            this->adj_matrix.resize(this->num_vertices, vector<int>(this->num_vertices, -1));
        }
    }
    
    // add edge with optional weight
    void add_edge(int from, int to, int weight = 1) {
        assert(this->num_vertices != -1);
        
        if (this->mode == ADJ_LIST || this->mode == BOTH) {
            this->adj_list[from].push_back(pair<int, int>(to, weight));
            if (!this->is_directed) {
                // not directed, add the edge in reverse relationship
                this->adj_list[to].push_back(pair<int, int>(from, weight));
            }
        }
        if (this->mode == ADJ_MATRIX || this->mode == BOTH) {
            this->adj_matrix[from][to] = weight;
            if (!this->is_directed) {
                // not directed, same thing
                this->adj_matrix[to][from] = weight;
            }
        }
        
        this->edges.push_back(vector<int>({from, to, weight}));
    }
    
    // fetch weight/distance between two vertices
    // return -1 if not counnected
    int weight_between(int from, int to) const {
        assert(this->num_vertices != -1);
        
        // fast retrieval if in ADJ_MATRIX or BOTH mode
        if (this->mode == ADJ_MATRIX || this->mode == BOTH) {
            return this->adj_matrix[from][to];
        }
        
        // otherwise in ADJ_LIST, do a linear traversal
        for (auto neighbour : this->adj_list[from]) {
            if (neighbour.first == to) {
                return neighbour.second;
            }
        }
        
        return -1;
        
    }
    
    // walk through adj vertices
    Adj_Iterator adj_begin(int current_vertex) const {
        // fast walk through if use list or both mode
        if (this->mode == ADJ_LIST || this->mode == BOTH) {
            return Adj_Iterator(this->adj_list[current_vertex].begin(), &(this->adj_list[current_vertex]));
        }
        // fallback to matrix
        return Adj_Iterator(this->adj_matrix[current_vertex].begin(), &(this->adj_matrix[current_vertex]));
    }
    
    Adj_Iterator adj_end(int current_vertex) const {
        // fast walk through if use list or both mode
        if (this->mode == ADJ_LIST || this->mode == BOTH) {
            return Adj_Iterator(this->adj_list[current_vertex].end(), &(this->adj_list[current_vertex]));
        }
        // fallback to matrix
        return Adj_Iterator(this->adj_matrix[current_vertex].end(), &(this->adj_matrix[current_vertex]));
    }
    
private:
    // mode for representation
    Mode mode;
    // num vertices
    int num_vertices = -1;
    // directed
    bool is_directed = false;
    // for adj matrix
    vector<vector<int>> adj_matrix;
    // for adj list, <vertexId, weight> pair
    vector<vector<pair<int, int>>> adj_list;
    // edges as a dedicated vector <from, to, weight>
    vector<vector<int>> edges;
    
};

// comparator when using PQ mode
struct Pair_PQ_Comp {
    
    // return true for lower priority
    bool operator()(const pair<int, int>& lhs,
                    const pair<int, int>& rhs) const {
        
        return lhs.second > rhs.second;
    }
    
};

// MST algorithms
class MST_Prims {
public:
    // mode to find shortest
    enum Mode { LINEAR, PQ };
public:
    
    // either linear or PQ
    void set_mode(Mode mode) {
        this->mode = mode;
    }
    
    // build the MST
    void build(const Graph& graph) {
        // resize data structure based on graph size
        this->resize(graph);
        // perform algorithm based on mode
        if (this->mode == LINEAR) {
            this->build_linear(graph);
        } else if (this->mode == PQ) {
            this->build_pq(graph);
        }
    }
    
    // get final weight
    long long get_weight() const {
        return this->weight;
    }
    
    // print generated path
    void print_path() const {
        for (auto i : this->path) {
            cout << i << ' ';
        }
    }
    
private:
    // mode
    Mode mode = LINEAR;
    // prims data structures
    vector<bool> visited;
    vector<int> prevs;
    // --- dists vector for linear search
    vector<int> dists;
    // --- dists pq with <vertex, weight>
    priority_queue<pair<int, int>, vector<pair<int, int>>, Pair_PQ_Comp> pq;
    // path
    vector<int> path;
    // final weight
    long long weight = 0;
    
    // resize the graph
    void resize(const Graph& graph) {
        size_t size = graph.get_num_vertices();
        this->visited.resize(size, false);
        this->dists.resize(size, numeric_limits<int>::max());
        this->prevs.resize(size, -1); // -1 meaning no previous value
    }
    
    void build_linear(const Graph& graph) {
        // visit 0th item
        int current = 0;
        this->dists[current] = 0;
        // visit V times
        for (unsigned i = 0; i < graph.get_num_vertices(); ++ i) {
            // find unvisited vertex with shortest tentative distance
            int current_min = numeric_limits<int>::max();
            for (unsigned j = 0; j < graph.get_num_vertices(); ++ j) {
                if (!this->visited[j] && this->dists[j] < current_min) {
                    current_min = this->dists[j];
                    current = j;
                }
            }
            // mark the new current as visited
            this->visited[current] = true;
            // add to weight
            this->weight += current_min;
            // add to path
            this->path.push_back(current);
            
            // visit all adjacent vertices and update dist
            auto it = graph.adj_begin(current);
            auto it_end = graph.adj_end(current);
            
            while (it != it_end) {
                int next = *it;
                int dist = graph.weight_between(current, next);
                if (!this->visited[next] && dist < this->dists[next]) {
                    this->dists[next] = dist;
                    this->prevs[next] = current;
                }
                
                ++ it;
            }
        }
    }
    
    void build_pq(const Graph& graph) {
        
        // visit 0th item
        int current = 0;
        this->dists[current] = 0;
        this->pq.push(make_pair(current, 0));
        
        while (!pq.empty()) {
            // get vertex with smallest tentative distance
            auto pair = this->pq.top();
            current = pair.first;
            this->pq.pop();
            // mark the new current as visited
            if (!this->visited[current]) {
                this->visited[current] = true;
                // add weight
                this->weight += pair.second;
                // save to path
                this->path.push_back(current);
            }
            // loop through adjancent vertices and update dists
            auto it = graph.adj_begin(current);
            auto it_end = graph.adj_end(current);
            
            while (it != it_end) {
                int next = *it;
                int dist = graph.weight_between(current, next);
                if (!this->visited[next] && dist < this->dists[next]) {
                    this->dists[next] = dist;
                    this->prevs[next] = current;
                    // push into priority queue
                    this->pq.push(make_pair(next, dist));
                }
                ++ it;
            }
        }
        
    }
};

class MST_Kruskals {
private:
    // Union-find (Disjoint set) data structure
    class DisjointSet {
    public:
        // init with number of items to merge
        DisjointSet(int n) {
            this->n = n;
            // resize reps and ranks
            this->reps.resize(n);
            this->ranks.resize(n);
            // initially, mark each vertex the representative of itself
            for (unsigned i = 0; i < n; ++ i) {
                this->reps[i] = i;
                this->ranks[i] = 0;
            }
        }
        
        // find the representative of a vertex
        // with path compression
        int find(int vertex) {
            if (vertex != this->reps[vertex]) {
                this->reps[vertex] = this->find(this->reps[vertex]);
            }
            return this->reps[vertex];
        }
        
        // merge two disjoint sets
        void merge(int v1, int v2) {
            // find representative for both
            int v1_rep = this->find(v1);
            int v2_rep = this->find(v2);
            // compare rank and merge
            if (this->ranks[v1_rep] > this->ranks[v2_rep]) {
                this->reps[v2_rep] = v1_rep;
            } else if (this->ranks[v1_rep] < this->ranks[v2_rep]) {
                this->reps[v1_rep] = v2_rep;
            } else {
                // equal, pick a random one
                // in this case, v2_rep is made the new one
                this->reps[v1_rep] = v2_rep;
                // thus, increase v2_rep's rank
                this->ranks[v2_rep] ++;
            }
        }
    private:
        // number of items to merge
        int n;
        // representatives vector
        vector<int> reps;
        // rank vector
        vector<int> ranks;
    };
public:
    
    // build the MST
    void build(const Graph& graph) {
        // sort edges
        auto edges = graph.get_edges();
        sort(edges.begin(), edges.end(), [](
                                            const vector<int>& a,
                                            const vector<int>& b
                                            ) {
            return a[2] < b[2];
        });
        
        // create disjoint set
        DisjointSet ds(graph.get_num_vertices());
        
        // iterate through all edges
        for (unsigned i = 0; i < edges.size(); ++ i) {
            
            int v1 = edges[i][0];
            int v2 = edges[i][1];
            
            // find v1 and v2's representative
            int set1 = ds.find(v1);
            int set2 = ds.find(v2);
            
            // compare set leader
            if (set1 != set2) {
                // not equal, in different set, no cycle!
                this->path.push_back(make_pair(v1, v2));
                // save weight
                this->weight += edges[i][2];
                // merge the two sets
                ds.merge(set1, set2);
            }
        }
    }
    
    // get final weight
    long long get_weight() const {
        return this->weight;
    }
    
    // print generated path
    void print_path() const {
        for (auto i : this->path) {
            cout << i.first << '-' << i.second << ' ';
        }
    }
    
private:
    // total weight
    long long weight = 0;
    // path to take, comprised of vector of edges
    vector<pair<int, int>> path;
    
};

// Shortest path algorithms
class Shortest_Dijkstra {
public:
    // method to find shortest vertex to visit
    enum Mode { LINEAR, PQ };
public:
    
    // set mode for solving the problem
    void set_mode(Mode mode) {
        this->mode = mode;
    }
    
    // find shortest path between two vertices
    long long find_shortest_path(const Graph& graph, int from, int to) {
        this->resize(graph);
        this->start = from;
        this->dest = to;
        // build path based on mode
        if (this->mode == LINEAR) {
            this->build_linear(graph);
        } else if (this->mode == PQ) {
            this->build_pq(graph);
        }
        return this->dists[from];
    }
    
    // print generated path
    void print_path() const {
        int current = this->start;
        while (this->prevs[current] != -1) {
            cout << current << '-';
            current = this->prevs[current];
        }
        cout << current;
    }
    
private:
    // mode
    Mode mode = LINEAR;
    // save destination and start
    int start, dest;
    // data structure needed
    vector<bool> visited;
    vector<int> dists;
    vector<int> prevs;
    // --- PQ needed for PQ mode
    priority_queue<pair<int, int>, vector<pair<int, int>>, Pair_PQ_Comp> pq;
    
    // resize data containers
    void resize(const Graph& graph) {
        int num_vertices = graph.get_num_vertices();
        this->visited.resize(num_vertices, false);
        this->dists.resize(num_vertices, numeric_limits<int>::max());
        this->prevs.resize(num_vertices, -1); // for no prevs
    }
    
    // build path using linear search
    void build_linear(const Graph& graph) {
        // visit destination
        int current = this->dest;
        this->dists[current] = 0;
        // loop V times
        for (unsigned i = 0; i < graph.get_num_vertices(); ++ i) {
            // find one with shortest tentative distance
            int current_min = numeric_limits<int>::max();
            for (unsigned i = 0; i < graph.get_num_vertices(); ++ i) {
                if (!this->visited[i] && this->dists[i] < current_min) {
                    current = i;
                    current_min = this->dists[i];
                }
            }
            // mark as visited
            this->visited[current] = true;
            // backchecking all adjacent edges
            // if prev_dist + new_dist < current, update
            auto it = graph.adj_begin(current);
            auto it_end = graph.adj_end(current);
            while (it != it_end) {
                int prev = *it;
                int dist = graph.weight_between(prev, current);
                if (!this->visited[prev] && this->dists[current] + dist < this->dists[prev]) {
                    this->dists[prev] = this->dists[current] + dist;
                    // update previous
                    this->prevs[prev] = current;
                }
                
                ++ it;
            }
        }
    }
    
    // build using priority queue
    void build_pq(const Graph& graph) {
        // visit destination
        int current = this->dest;
        this->dists[current] = 0;
        this->pq.push(make_pair(current, 0));
        
        while (!this->pq.empty()) {
            // get the vertex with shortest tentative distance
            auto pair = this->pq.top();
            current = pair.first;
            this->pq.pop();
            // mark as visited
            if (!this->visited[current]) {
                this->visited[current] = true;
            }
            // backchecking all adjancent edges
            // if prev_dist + new_dist < current, update
            auto it = graph.adj_begin(current);
            auto it_end = graph.adj_end(current);
            while (it != it_end) {
                int prev = *it;
                int dist = graph.weight_between(prev, current);
                if (!this->visited[prev] && this->dists[current] + dist < this->dists[prev]) {
                    this->dists[prev] = this->dists[current] + dist;
                    // update previous
                    this->prevs[prev] = current;
                    // push into priority queue
                    this->pq.push(make_pair(prev, this->dists[prev]));
                }
                
                ++ it;
            }
        }
    }
};

// BFS, DFS searches
class Search {
    
    
};


// tests
void test_graph();
void test_MST();
void test_dijkstra();

// test the graph algorithms
int main() {
    
    test_graph();
    test_MST();
    test_dijkstra();
    
    return 0;
}

void test_graph() {
    
    // test adj list mode
    Graph graph1;
    
    graph1.set_num_vertices(5);
    graph1.set_graph_representation(Graph::Mode::ADJ_LIST);
    
    graph1.add_edge(0, 1);
    graph1.add_edge(0, 2);
    graph1.add_edge(0, 3);
    graph1.add_edge(0, 4);
    
    int i = 1;
    auto it_adj = graph1.adj_begin(0);
    auto it_adj_end = graph1.adj_end(0);
    while (it_adj != it_adj_end) {
        assert(*it_adj == i);
        ++ it_adj;
        ++ i;
    }
    
    // test adj matrix mode
    Graph graph2;
    
    graph2.set_num_vertices(5);
    graph2.set_graph_representation(Graph::Mode::ADJ_MATRIX);
    
    graph2.add_edge(0, 1);
    graph2.add_edge(0, 2);
    graph2.add_edge(0, 3);
    graph2.add_edge(0, 4);
    
    i = 1;
    it_adj = graph2.adj_begin(0);
    it_adj_end = graph2.adj_end(0);
    while (it_adj != it_adj_end) {
        assert(*it_adj == i);
        ++ it_adj;
        ++ i;
    }
    
    cout << "Test graph passed!" << endl;;
    
}

void test_MST() {
    
    Graph g1;
    g1.set_num_vertices(6); // a,b,c,d,e,f
    g1.set_graph_representation(Graph::Mode::BOTH);
    
    g1.add_edge(0, 1, 13);
    g1.add_edge(0, 3, 1);
    g1.add_edge(0, 2, 8);
    g1.add_edge(1, 2, 15);
    g1.add_edge(2, 3, 5);
    g1.add_edge(2, 4, 3);
    g1.add_edge(3, 4, 4);
    g1.add_edge(3, 5, 5);
    g1.add_edge(4, 5, 2);
    
    // prims - linear search
    MST_Prims prims_linear;
    prims_linear.set_mode(MST_Prims::Mode::LINEAR);
    prims_linear.build(g1);
    
    assert(prims_linear.get_weight() == 23);
    prims_linear.print_path();
    
    cout << endl;
    
    // prims - PQ
    MST_Prims prims_pq;
    prims_pq.set_mode(MST_Prims::Mode::PQ);
    prims_pq.build(g1);
    
    assert(prims_pq.get_weight() == 23);
    prims_pq.print_path();
    
    cout << endl;
    
    // kruskals
    MST_Kruskals kruskals;
    kruskals.build(g1);
    
    assert(kruskals.get_weight() == 23);
    kruskals.print_path();
    
    cout << endl;
    
    cout << "Test MST passed!" << endl;
}

void test_dijkstra() {
    
    Graph g1;
    g1.set_num_vertices(6);
    g1.set_graph_representation(Graph::Mode::BOTH);
    
    g1.add_edge(0, 1, 3);
    g1.add_edge(0, 2, 1);
    g1.add_edge(0, 3, 5);
    g1.add_edge(1, 2, 5);
    g1.add_edge(2, 3, 2);
    g1.add_edge(2, 4, 4);
    g1.add_edge(3, 5, 5);
    g1.add_edge(4, 5, 1);
    
    // Dijkstra, linear search
    Shortest_Dijkstra d_linear;
    d_linear.set_mode(Shortest_Dijkstra::Mode::LINEAR);
    assert(d_linear.find_shortest_path(g1, 5, 1) == 9);
    d_linear.print_path();
    
    cout << endl;
    
    // Dijkstra, PQ
    Shortest_Dijkstra d_pq;
    d_pq.set_mode(Shortest_Dijkstra::Mode::LINEAR);
    assert(d_pq.find_shortest_path(g1, 5, 1) == 9);
    d_pq.print_path();
    
    cout << endl;
    
    cout << "Test Dijkstra passed!" << endl;
}
