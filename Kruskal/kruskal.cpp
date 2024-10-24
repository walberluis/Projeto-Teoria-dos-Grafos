#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

struct Edge {
    int src, dest, weight;
};

void displayHelp() {
    cout << "Help" << endl;
    cout << "-h: mostra help" << endl;
    cout << "-o <arquivo>: redireciona a saída para o arquivo" << endl;
    cout << "-f <arquivo>: lê o grafo do arquivo" << endl;
    cout << "-s : mostra a solução" << endl;
}

void make_set(int V, vector<int> &parent, vector<int> &rank) {
    for (int i = 1; i <= V; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

int find(int x, vector<int> &parent) {
    if (x != parent[x]) {
        parent[x] = find(parent[x], parent);
    }
    return parent[x];
}

void union_sets(int x, int y, vector<int> &parent, vector<int> &rank) {
    x = find(x, parent);
    y = find(y, parent);
    if (rank[x] > rank[y]) {
        parent[y] = x;
    } else {
        parent[x] = y;
        if (rank[x] == rank[y]) {
            rank[y]++;
        }
    }
}

bool compareEdges(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

pair<int, vector<pair<int, int>>> kruskal(int V, vector<Edge> &edges) {
    sort(edges.begin(), edges.end(), compareEdges);

    vector<int> parent(V + 1);
    vector<int> rank(V + 1, 0);
    make_set(V, parent, rank);

    int totalWeight = 0;
    vector<pair<int, int>> mstEdges;

    for (const auto &edge : edges) {
        int u = find(edge.src, parent);
        int v = find(edge.dest, parent);

        if (u != v) {
            mstEdges.push_back({edge.src, edge.dest});
            union_sets(u, v, parent, rank);
            totalWeight += edge.weight;
        }
    }

    return {totalWeight, mstEdges};
}

int main(int argc, char *argv[]) {
    string input_file = "";
    string output_file = "";
    string show_output = "result";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            displayHelp();
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            output_file = argv[i + 1];
            show_output = "none";
        } else if (strcmp(argv[i], "-f") == 0) {
            input_file = argv[i + 1];
        } else if (strcmp(argv[i], "-s") == 0) {
            show_output = "vertices";
        }
    }

    if (input_file.empty()) {
        cout << "No input file specified. Use the -f parameter" << endl;
        return 1;
    }

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Could not open input file: " << input_file << endl;
        return 1;
    }

    int V, E;
    fin >> V >> E;
    vector<Edge> edges(E);

    for (int i = 0; i < E; i++) {
        fin >> edges[i].src >> edges[i].dest >> edges[i].weight;
    }
    fin.close();

    auto result = kruskal(V, edges);

    if (!output_file.empty()) {
        ofstream fout(output_file);
        if (!fout) {
            cerr << "Could not open output file: " << output_file << endl;
            return 1;
        }
        for (const auto &edge : result.second) {
            fout << "(" << edge.first << ", " << edge.second << ") ";
        }
        fout << endl;
        fout.close();
    }

    if (show_output == "result") {
        cout << result.first << endl;
    }

    if (show_output == "vertices") {
        for (const auto &edge : result.second) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }

    return 0;
}
