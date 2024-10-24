#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <stack>

using namespace std;

class Graph{
    private:
        int qtdVertex;
        int qtdEdges;
        vector<vector<int>> adj;
        vector<vector<int>> adjT;
        vector<bool> visited;
        stack<int> posOrdem;
        vector<vector<int>> componentesFortementeConexas;
    public:
        vector<vector<int>> getAdjList();
        void initVisited();
        void addEdge(int V1, int V2);
        Graph(int qtdVertex, int qtdEdges);
        void posOrdemDFS();
        void posOrdemDFSrec(int vertex);
        void kosarajuDFS(int node, vector<int> &component);
        void kosaraju();
        vector<vector<int>> getCFC();
};

Graph::Graph(int qtdVertex, int qtdEdges) : qtdVertex(qtdVertex), qtdEdges(qtdEdges), visited(qtdVertex), adj(qtdVertex), adjT(qtdVertex), posOrdem(), componentesFortementeConexas() {}

vector<vector<int>> Graph::getAdjList(){
    return adj;
}

void Graph::addEdge(int V1, int V2){
    adj[V1 - 1].push_back(V2 - 1);
    adjT[V2 - 1].push_back(V1 - 1);
}

void Graph::initVisited(){
    for(int i = 0; i < qtdVertex; i++){
        visited[i] = false;
    }
}

void Graph::posOrdemDFSrec(int vertex){
    visited[vertex] = true;

    for(int e : adjT[vertex]){
		if(!visited[e]){
			posOrdemDFSrec(e);
		}
	}

    posOrdem.push(vertex);

	return;
}

void Graph::posOrdemDFS(){
	initVisited();

	for(int i = 0; i < qtdVertex; i++){
		if(!visited[i]) Graph::posOrdemDFSrec(i);
    }

}

void Graph::kosarajuDFS(int node, vector<int> &component)
{
    visited[node] = true;
    component.push_back(node + 1);
    
    for (auto i : adj[node])
    {
        if (!visited[i])
        {
            kosarajuDFS(i, component);
        }
    }

    return;
}

void Graph::kosaraju(){
    posOrdemDFS();
    initVisited();

    vector<int> component;

    while(!posOrdem.empty()){
        int node = posOrdem.top();
        posOrdem.pop();

        if(!visited[node]){
            kosarajuDFS(node, component);
            componentesFortementeConexas.push_back(component);
            component.clear();
        }
    }
}

vector<vector<int>> Graph::getCFC(){
    return componentesFortementeConexas;
}

struct Arguments {
    int NeedOutFile;
    int HasInputFile;                      
    int NeedHelp;             
    string outFileName;       
    string inFileName;       
};

Arguments *lerArgumentos(int argc, char *argv[])
{
    Arguments *argumentos = new Arguments();

    argumentos->NeedOutFile = 0;
    argumentos->HasInputFile = 0;
    argumentos->NeedHelp = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            argumentos->NeedHelp = 1;
        }
        else if (strcmp(argv[i], "-o") == 0 && i < argc - 1)
        {
            argumentos->NeedOutFile = 1;
            argumentos->outFileName = argv[++i];
        }
        else if (strcmp(argv[i], "-f") == 0 && i < argc - 1)
        {
            argumentos->HasInputFile = 1;
            argumentos->inFileName = argv[++i];
        }
    }

    return argumentos;
}

void imprimirAjuda(Arguments *argumentos)
{
    cout << "Help:" << endl;
    cout << "-h: mostra o help" << endl;
    cout << "-o <arquivo>: redireciona a saida para o 'arquivo'" << endl;
    cout << "-f <arquivo>: indica o 'arquivo' que contém o grafo de entrada" << endl;

    delete argumentos;
    return;
}

int main(int argc, char *argv[])
{
    Arguments *argumentos = lerArgumentos(argc, argv);

    if(argumentos->NeedHelp){
        imprimirAjuda(argumentos);
        return 0;
    }
    
    ifstream fin(argumentos->inFileName);

    if(!fin)
    {
        cerr << "Could not open input file: " << argumentos->inFileName << endl;
        delete argumentos;
        return 1;
    }

    int qtdVertex, qtdEdges;
    fin >> qtdVertex >> qtdEdges;

    Graph graph(qtdVertex, qtdEdges);

    for (int i = 0; i < qtdEdges; i++)
    {
        int V1, V2;
        fin >> V1 >> V2;
        graph.addEdge(V1, V2);
    }

    graph.kosaraju();

    for (auto i : graph.getCFC())
    {
        if (argumentos->NeedOutFile)
        {
            ofstream fout(argumentos->outFileName, ios::app);
            for (auto j : i)
            {
                fout << j << " ";
            }
            fout << endl;
        }
        else
        {
            for (auto j : i)
            {
                cout << j << " ";
            }
            cout << endl;
        }
    }

    delete argumentos;

    return 0;
}