#include <fstream>
#include <iostream>
#include <vector>

#define NO_VERTEX 101

void printGraph(const std::vector<std::vector<int>> &graph) {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << std::setw(3) << i + 1 << ")";
        for (int j = 0; j < graph[i].size(); j++) {
            std::cout << std::setw(5) << graph[i][j];
        }
        std::cout << std::endl;
    }
}

struct Edge {
    int a;
    int b;
    int w;
    Edge(int a, int b, int w) {
        this->a = a;
        this->b = b;
        this->w = w;
    }
    void printEdge() {
        std::cout << a << " " << b << " "
                  << " " << w << std::endl;
    }
};

void primAlg(const std::vector<std::vector<int>> &graph, std::vector<Edge> &prims_edges) {
    bool selected[graph.size()];

    int no_edge = 0;

    // Sāk ar pirmo virsotni
    selected[0] = true;

    int x, y;
    while (no_edge < graph.size() - 1) {
        int min = INT_MAX;
        for (int i = 0; i < graph.size(); i++) {
            if (selected[i]) {
                for (int j = 0; j < graph.size(); j++) {
                    if (!selected[j] && graph[i][j] != NO_VERTEX) {
                        if (graph[i][j] < min) {
                            min = graph[i][j];
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }
        Edge min_edge(x + 1, y + 1, graph[x][y]);
        prims_edges.push_back(min_edge);
        selected[y] = true;
        no_edge++;
    }
}

int main() {
    // TODO - read as arg
    std::ifstream infile("/Users/girtsrudziss/studijas/Atru_Algoritmu_Konstruesana_un_Analize/Progr_darbs/sample_data/sample1.txt");
    unsigned int n;
    infile >> n;
    // Grafs ir neorientēts, sķautnes matricā iet abos virzienos
    // Neeksistējošu šķautni nevar apzīmēt ar 0, tāpēc izmantoju NO_VERTEX (101 vērtību)
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, NO_VERTEX));
    printGraph(graph);
    while (infile) {
        int a, b, w;
        infile >> a >> b >> w;
        std::cout << a << " " << b << " " << w << std::endl;
        graph[a - 1][b - 1] = w;
        graph[b - 1][a - 1] = w;
    }
    printGraph(graph);

    // Sākumā no grafa jāatmet visas negatīvās šķautnes, jāpieskaita gala rezultātam
    // Pēc tam aprēķinam minimum-weight-feedback-edge-set (vai arī maximum weight spanning tree)
    std::vector<Edge> result_edges;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i; j < graph[i].size(); j++) {
            if (graph[i][j] < 0) {
                Edge negative_edge(i + 1, j + 1, graph[i][j]);
                result_edges.emplace_back(negative_edge);
                graph[i][j] = NO_VERTEX;
                graph[j][i] = NO_VERTEX;
            }
        }
    }
    for (Edge &edge : result_edges) {
        edge.printEdge();
    }
    /**
     * Maximum spanning tree atrodam, izmantojot Prima algoritmu,
     * sākumā visu šķautņu vērtības pārveidojot par negatīvām un
     * atrisinot minimum spanning tree uz jaunā grafa.
     * Visas šķautnes, kas nepiederēs maximum spanning tree, veidos vajadzīgās šķautnes uzdevuma izpildei
     */
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            if (graph[i][j] != NO_VERTEX) {
                Edge edge(i + 1, j + 1, graph[i][j]);
                graph[i][j] *= -1;
            }
        }
    }
    printGraph(graph);
    std::vector<Edge> prims_edges;
    primAlg(graph, prims_edges);

    // Izdzēšam no grafa prims_edges
    for (Edge &prims_edge : prims_edges) {
        prims_edge.printEdge();
        graph[prims_edge.a - 1][prims_edge.b - 1] = NO_VERTEX;
        graph[prims_edge.b - 1][prims_edge.a - 1] = NO_VERTEX;
    }
    printGraph(graph);
    // Atlikušās virsotnes būs vajadzīgās
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i; j < graph[i].size(); j++) {
            if (graph[i][j] != NO_VERTEX) {
                Edge edge(i + 1, j + 1, graph[i][j] * -1);
                result_edges.emplace_back(edge);
            }
        }
    }
    int sum = 0;
    for (Edge &edge : result_edges) {
        sum += edge.w;
        edge.printEdge();
    }
    std::cout << "Summa: " << sum << std::endl;

    return 0;
}
