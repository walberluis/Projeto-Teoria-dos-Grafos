#include <iostream>
#include <vector>
#include <limits>
#include <cstring>
#include <fstream>

using namespace std;

struct Argumentos
{
    bool needHelp;
    bool needOutFile;
    string outFileName;
    string inFileName;
};

const int INF = numeric_limits<int>::max();

void imprimirAjuda(Argumentos *argumento)
{
    std::string help = "Help:\n";
    help += "-h: mostra o help\n";
    help += "-o <arquivo>: redireciona a saida para o 'arquivo'\n";
    help += "-f <arquivo>: indica o 'arquivo' que contém o grafo de entrada\n";
    help += "\tA matriz Floyd-Warshall mostra \"-\" se não houver caminho.\n";

    if (argumento->needOutFile)
    {
        ofstream fout(argumento->outFileName);

        if (!fout)
        {
            cerr << "Could not open output file: " << argumento->outFileName << endl;
            return;
        }

        fout << help;
    }
    else
    {
        cout << help;
    }

    delete argumento;
}

Argumentos *readArguments(int argc, char *argv[])
{
    Argumentos *argumento = new Argumentos();

    argumento->needHelp = false;
    argumento->needOutFile = false;
    argumento->outFileName = "";
    argumento->inFileName = "";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            argumento->needHelp = true;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            argumento->needOutFile = true;
            argumento->outFileName = argv[i + 1];
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            argumento->inFileName = argv[i + 1];
        }
    }

    return argumento;
}

void floyd_warshall(vector<vector<int>> &graph, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0;
            }
        }
    }

    // Floyd-Warshall
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (graph[i][k] != INF && graph[k][j] != INF)
                {
                    if (graph[i][j] > graph[i][k] + graph[k][j])
                    {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
    }

    return;
}

int main(int argc, char **argv)
{
    Argumentos *argumento = readArguments(argc, argv);

    if (argumento->needHelp)
    {
        imprimirAjuda(argumento);
        return 1;
    }

    ifstream fin(argumento->inFileName);

    if (!fin)
    {
        cerr << "Could not open input file: " << argumento->inFileName << endl;
        delete argumento;
        return 1;
    }

    int qtdVertices, qtdArestas;
    fin >> qtdVertices >> qtdArestas;

    vector<vector<int>> graph(qtdVertices, vector<int>(qtdVertices, INF));

    for (int i = 0; i < qtdArestas; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        u--;
        v--;
        graph[u][v] = w;
    }

    fin.close();

    int n = graph.size();

    floyd_warshall(graph, n);

    string res = "";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (graph[i][j] == INF)
            {
                res += "-";
            }
            else
            {
                res += to_string(graph[i][j]);
            }

            if (j != n - 1)
                res += " ";
        }
        res += "\n";
    }

    if (argumento->needOutFile)
    {
        ofstream fout(argumento->outFileName);

        if (!fout)
        {
            cerr << "Could not open output file: " << argumento->outFileName << endl;
            delete argumento;
            return 1;
        }

        fout << res;
    }
    else
    {
        cout << res;
    }

    delete argumento;
    return 0;
}
