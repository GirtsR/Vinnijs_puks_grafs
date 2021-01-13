#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

// Ar NO_VERTEX apzīmēju neeksistējošu škautni grafā
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
    void printEdge() const {
        std::cout << a << "<->" << b << ": " << w << std::endl;
    }
};

void primAlg(const std::vector<std::vector<int>> &graph, std::vector<Edge> &prims_edges) {
    //  min_neighbor apzīmēs virsotnes kaimiņu, uz kuru pievienotās škautnes svars ir vismazākais
    int min_neighbor[graph.size()];
    // min_edge masīvs izmantojams mazākā svara šķautnes glabāšanai
    int min_edge[graph.size()];
    // true, ja i-tā virsotne jau ievietota minimum spanning tree
    bool visited[graph.size()];

    // Inicializēju sākotnējās vērtības (visas virsotnes neapskatītas un min_edge nav noteikts)
    for (int i = 0; i < graph.size(); i++) {
        visited[i] = false;
        min_edge[i] = INT_MAX;
    }

    // Kā pirmo apskatīšu 1. virsotni
    min_edge[0] = 0;
    // 1. virsotnei mazākais kaimiņš netiek meklēts
    min_neighbor[0] = -1;

    for (int count = 1; count <= graph.size(); count++) {
        // Izvēlamies u tādu, ka w(v, u) = min, kur u vēl nav iekļauts MST
        int min = INT_MAX;
        int u;
        for (int v = 0; v < graph.size(); v++) {
            if (!visited[v] && min_edge[v] < min) {
                min = min_edge[v];
                u = v;
            }
        }

        visited[u] = true;
        // Atjauninam min_edge vērtību tām virsotnēm, kas ir virsotnes u kaimiņš un vēl nav apskatīts
        for (int v = 0; v < graph.size(); v++) {
            if (graph[u][v] != NO_VERTEX && !visited[v] && graph[u][v] < min_edge[v]) {
                min_neighbor[v] = u;
                min_edge[v] = graph[u][v];
            }
        }
    }

    // MST šķautnes būs katras virsotnes šķautne ar tās min_neighbor
    for (int i = 1; i < graph.size(); i++) {
        if (min_edge[i] != INT_MAX) {
            Edge edge(min_neighbor[i] + 1, i + 1, graph[min_neighbor[i]][i]);
            prims_edges.emplace_back(edge);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
       std::cerr << "Programmai nav noradits cels uz ieejas datiem!" << std::endl;
       exit(-1);
    }
    std::ifstream infile(argv[1]);
    unsigned int n;
    infile >> n;

    // Grafs ir neorientēts, šķautnes matricā iet abos virzienos
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, NO_VERTEX));
    while (infile) {
        int a, b, w;
        infile >> a >> b >> w;
        graph[a - 1][b - 1] = w;
        graph[b - 1][a - 1] = w;
    }
    std::cout << "Sakotnejais grafs:" << std::endl;
    printGraph(graph);

    // Sākumā no grafa jāatmet visas negatīvās šķautnes, jāpieskaita gala rezultātam
    std::vector<Edge> result_edges;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i; j < graph[i].size(); j++) {
            if (graph[i][j] < 0) {
                // Šķautne ir negatīva -> pievienojam pie result_edges
                Edge negative_edge(i + 1, j + 1, graph[i][j]);
                result_edges.emplace_back(negative_edge);
                graph[i][j] = NO_VERTEX;
                graph[j][i] = NO_VERTEX;
            } else if (graph[i][j] != NO_VERTEX) {
                // Šķautne ir nenegatīva -> pārveidojam tās svarus par negatīviem
                graph[i][j] *= -1;
                graph[j][i] *= -1;
            }
        }
    }
    std::cout << "Negativas skautnes:" << std::endl;
    for (Edge &edge : result_edges) {
        edge.printEdge();
    }

    /**
     * Maximum spanning tree atrodam, izmantojot Prima algoritmu,
     * sākumā visu šķautņu vērtības pārveidojot par negatīvām un
     * atrisinot minimum spanning tree uz jaunā grafa.
     * Visas šķautnes, kas nepiederēs maximum spanning tree, veidos vajadzīgās šķautnes uzdevuma izpildei
     */
    std::cout << "Grafs ar invertetiem skautnu svariem:" << std::endl;
    printGraph(graph);
    std::vector<Edge> prims_edges;
    primAlg(graph, prims_edges);

    std::cout << "Prima algoritma atrastas skautnes:" << std::endl;
    // Izdzēšam no grafa prims_edges
    for (Edge &prims_edge : prims_edges) {
        prims_edge.printEdge();
        graph[prims_edge.a - 1][prims_edge.b - 1] = NO_VERTEX;
        graph[prims_edge.b - 1][prims_edge.a - 1] = NO_VERTEX;
    }
    std::cout << "Grafs bez maximum spanning tree skautnem:" << std::endl;
    printGraph(graph);
    // Atlikušās šķautnes būs vajadzīgās
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i; j < graph[i].size(); j++) {
            if (graph[i][j] != NO_VERTEX) {
                Edge edge(i + 1, j + 1, graph[i][j] * -1);
                result_edges.emplace_back(edge);
            }
        }
    }
    int sum = 0;
    std::cout << "Vinnijam vajadzigas skautnes:" << std::endl;
    for (Edge &edge : result_edges) {
        sum += edge.w;
        edge.printEdge();
    }
    std::cout << "Summa: " << sum << std::endl;

    std::ofstream outfile("output.txt");
    outfile << result_edges.size() << std::endl;
    outfile << sum << std::endl;
    for (Edge &edge : result_edges) {
        outfile << edge.a << " " << edge.b << std::endl;
    }
    std::cout << "Rezultati pieejami faila output.txt!" << std::endl;
    return 0;
}
