#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <fstream>
#include <climits>

struct Argumentos {
    bool needHelp;
    bool needOutFile;
    std::string outFileName;
    std::string inFileName;
    bool needSolution;
    int startVertex;
};

struct Resultado {
    int sum; 
    std::vector<std::pair<int, int>> result;
};

Argumentos *readArguments(int argc, char *argv[]){
    Argumentos *argumento = new Argumentos();
    
    argumento->needHelp = false;
    argumento->needOutFile = false;
    argumento->outFileName = "";
    argumento->inFileName = "";
    argumento->needSolution = false;
    argumento->startVertex = 0;
    
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0){
            argumento->needHelp = true;
        }else if(strcmp(argv[i], "-o") == 0){
            argumento->needOutFile = true;
            argumento->outFileName = argv[i+1];
        }else if(strcmp(argv[i], "-f") == 0){
            argumento->inFileName = argv[i+1];
        }else if(strcmp(argv[i], "-s") == 0){
            argumento->needSolution = true;
        }else if(strcmp(argv[i], "-i") == 0){
            argumento->startVertex = atoi(argv[i+1]);
            argumento->startVertex--;
        }
    }

    return argumento;
}

void imprimirAjuda(Argumentos *argumento){
    std::string help = "Help:\n";
    help += "-h: mostra o help\n";
    help += "-o <arquivo>: redireciona a saida para o 'arquivo'\n";
    help += "-f <arquivo>: indica o 'arquivo' que contém o grafo de entrada\n";
    help += "-s: mostra a solução\n";
    help += "-i: vértice inicial\n";
    
    if (argumento->needOutFile){
        std::ofstream fout(argumento->outFileName);

        if (!fout){
            std::cerr << "Could not open output file: " << argumento->outFileName << std::endl;
            return;
        }

        fout << help;


    } else {
        std::cout << help;
    }

    delete argumento;
}

class Graph{
    private:
        int qtdVertices;
        int qtdArestas;
        std::vector<std::vector<std::pair<int, int>>> adj;
        std::priority_queue<std::pair<int,int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> fila;
    public:
        Graph(int qtdVertices, int qtdArestas, int vertexInicial) : qtdVertices(qtdVertices), qtdArestas(qtdArestas){
            adj.resize(qtdVertices);
            fila.push(std::make_pair(0, vertexInicial));
        }


        void adicionaAresta(int vertice1, int vertice2, int peso){
            adj[vertice1 - 1].push_back(std::make_pair(vertice2 - 1, peso));
            adj[vertice2 - 1].push_back(std::make_pair(vertice1 - 1, peso));
        }

        Resultado *prim(int inicial){
            Resultado *resultado = new Resultado();

            int soma = 0;
            std::vector<int> visitados(qtdVertices, 0);
            std::vector<int> pai(qtdVertices, -1);
            std::vector<int> dist(qtdVertices, INT_MAX);
            dist[inicial] = 0;

            while (!fila.empty())
            {
                int vertex1 = fila.top().second;
                fila.pop();

                if (visitados[vertex1]) continue;

                visitados[vertex1] = 1;

                for (auto i : adj[vertex1])
                {
                    int vertex2 = i.first;
                    int pesoAresta = i.second;

                    if (!visitados[vertex2] && pesoAresta < dist[vertex2])
                    {
                        dist[vertex2] = pesoAresta;
                        pai[vertex2] = vertex1;
                        fila.push(std::make_pair(dist[vertex2], vertex2));
                    }
                }
            }

            for (int i = 0; i < qtdVertices; i++)
            {
                if (pai[i] != -1)
                {
                    resultado->result.push_back(std::make_pair(i + 1, pai[i] + 1));
                    soma += dist[i];
                }
            }

            resultado->sum = soma;

            return resultado;
        }
};

int main(int argc, char *argv[])
{
    Argumentos *argumento = readArguments(argc, argv);

    if (argumento->needHelp){
        imprimirAjuda(argumento);
        return 1;
    }

    std::ifstream fin(argumento->inFileName);

    if (!fin){
        std::cerr << "Could not open input file: " << argumento->inFileName << std::endl;
        delete argumento;
        return 0;
    }

    int qtdVertices, qtdArestas;
    fin >> qtdVertices >> qtdArestas;

    Graph *grafo = new Graph(qtdVertices, qtdArestas, argumento->startVertex);

    for (int i = 0; i < qtdArestas; i++){
        int vertice1, vertice2, peso;
        fin >> vertice1 >> vertice2 >> peso;
        grafo->adicionaAresta(vertice1, vertice2, peso);
    }

    Resultado *resultado = grafo->prim(argumento->startVertex);

    if (argumento->needSolution){
        if (argumento->needOutFile){
            std::ofstream fout(argumento->outFileName);

            if (!fout){
                std::cerr << "Could not open output file: " << argumento->outFileName << std::endl;
                return 1;
            }

            for (auto i : resultado->result){
                fout << "(" << i.second << "," << i.first << ") ";
            }
        } else {
            for (auto i : resultado->result){
                std::cout << "(" << i.second << "," << i.first << ") ";
            }
        }
    } else {
        if (argumento->needOutFile){
            std::ofstream fout(argumento->outFileName);

            if (!fout){
                std::cerr << "Could not open output file: " << argumento->outFileName << std::endl;
                return 1;
            }

            fout << resultado->sum << std::endl;
        } else {
            std::cout << resultado->sum << std::endl;
        }
    }

    delete argumento;
    delete resultado;
    delete grafo;

    return 0;
}