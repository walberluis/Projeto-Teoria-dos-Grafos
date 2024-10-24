#include <iostream> 
#include <vector> 
#include <queue> 
#include <fstream> 
#include <limits.h> 
#include <sstream> 
#include <cstring> 
 
using namespace std; 
 
const int INFINITO = 999999; 
 
struct Compare { 
    bool operator()(pair<int, int> const& a, pair<int, int> const& b) const { 
        return a.second > b.second; 
    } 
}; 
 
void dijkstra(vector<vector<pair<int, int>>>& grafo, int origem, vector<int>& dist, vector<int>& prev) { 
    int n = grafo.size(); 
    dist.assign(n, INFINITO); 
    prev.assign(n, -1); 
    dist[origem] = 0; 
 
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> H; 
    H.push({origem, 0}); 
 
    while (!H.empty()) { 
        int u = H.top().first; 
        H.pop(); 
 
        for (auto& aresta : grafo[u]) { 
            int v = aresta.first; 
            int peso = aresta.second; 
 
            if (dist[v] > dist[u] + peso) { 
                dist[v] = dist[u] + peso; 
                prev[v] = u; 
                H.push({v, dist[v]}); 
            } 
        } 
    } 
} 
 
void printHelp() { 
    cout << "Uso: ./dijkstra [opções]\n"; 
    cout << "Opções:\n"; 
    cout << "  -h               : mostra o help\n"; 
    cout << "  -o <arquivo>     : redireciona a saída para o 'arquivo'\n"; 
    cout << "  -f <arquivo>     : indica o 'arquivo' que contém o grafo de entrada\n"; 
    cout << "  -i <vértice>     : vértice inicial\n"; 
} 
 
int main(int argc, char* argv[]) { 
    string inputFile, outputFile; 
    int origem = 1; // -------------------------> mudei -1 para 1
 
    for (int i = 1; i < argc; i++) { 
        if (strcmp(argv[i], "-h") == 0) { 
            printHelp(); 
            return 0; 
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) { 
            outputFile = argv[++i]; 
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) { 
            inputFile = argv[++i]; 
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) { 
            origem = stoi(argv[++i]); 
        } 
    } 
 
    if (inputFile.empty()) { // -------------------------> tirei origem == -1
        cout << "Erro: arquivo de entrada e vértice inicial são obrigatórios.\n"; 
        printHelp(); 
        return 1; 
    } 
 
    ifstream infile(inputFile); 
    if (!infile) { 
        cout << "Erro ao abrir o arquivo de entrada.\n"; 
        return 1; 
    } 
 
    int n, m; 
    infile >> n >> m; 
 
    vector<vector<pair<int, int>>> grafo(n + 1); // -------------------------> mudei n para n + 1
    for (int i = 0; i < m; ++i) { 
        int u, v, p; 
        infile >> u >> v >> p; 
        grafo[u].push_back({v, p}); 
        grafo[v].push_back({u, p}); // Caso o grafo seja não direcionado 
    } 
    infile.close(); 
 
    vector<int> dist, prev; 
    dijkstra(grafo, origem, dist, prev); 
 
    ostream* out; 
    ofstream outfile; 
    if (!outputFile.empty()) { 
        outfile.open(outputFile); 
        if (!outfile) { 
            cout << "Erro ao abrir o arquivo de saída.\n"; 
            return 1; 
        } 
        out = &outfile; 
    } else { 
        out = &cout; 
    } 
 
    for (int i = 1; i <= n; ++i) { // -------------------------> mudei int i = 0 para int i = 1 e i < n para i <= n
        *out << i << ":" << (dist[i] == INFINITO ? -1 : dist[i]) << " "; 
    } 
    *out << endl; 
 
    if (outfile.is_open()) { 
        outfile.close(); 
    } 
 
    return 0; 
}