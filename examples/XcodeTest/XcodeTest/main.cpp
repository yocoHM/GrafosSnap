//
//  main.cpp
//  XcodeTest

#include "Snap.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <queue>
//#include "distanciaId.h"

using namespace TSnap;

void dijkstra(TPt<TNodeEDatNet<TInt, TFlt> >  graph, const int &SrcNId);

void floydWarshall(TPt<TNodeEDatNet<TInt, TFlt> >  graph, int &vertices);

void prim(TPt<TNodeEDatNet<TInt, TFlt>> Grafo, int source);

bool sortFunction(int a, int b,std::vector<float> distancias);

void kruskal(TPt<TNodeEDatNet<TInt, TFlt> > g, int vertices);

int encontrarSet(int x, int padre[]);

void setUnion(int u, int v, int padre[]);

class distanciaId
{
private:
public:
  double distancia;
  int id;
  int padre;
  distanciaId() { distancia = -1; id = 0; }
  void setDistancia(double _distancia) { distancia = _distancia; }
  void setId(int _id) { id = _id; }
  void setPadre(int _padre) { padre = _padre; }
  double getDistancia() { return distancia; }
  int getId() { return id; }
  int getPadre() { return padre; }
};

class sorter {
  std::vector<float> type_;
public:
  sorter(std::vector<float> type) : type_(type) {}
  bool operator()(int o1, int o2) const
  {
    return sortFunction(o1, o2, type_ );
  }
};

struct {
  bool operator()(distanciaId* a, distanciaId* b)
  {
    return a->getDistancia() < b->getDistancia();
  }
} comparaDistancia;
struct {
  bool operator()(distanciaId* a, distanciaId* b)
  {
    return a->getId() < b->getId();
  }
} comparaId;

int main(int argc, char* argv[]) {
  
  //declaracion del grafo
  TPt<TNodeEDatNet<TInt, TFlt>>  G = TNodeEDatNet<TInt, TFlt>::New();
  
  //creacion de los vertices
  for (int n = 1; n < 15; n++) {
    G->AddNode(n);
  }
  
  //conexion de los vertices con aristas con peso
  G->AddEdge(1,4,8);
  G->AddEdge(1,3,8);
  G->AddEdge(2,5,7);
  G->AddEdge(3,2,7);
  G->AddEdge(3,10,4);
  G->AddEdge(3,5,8);
  G->AddEdge(4,7,3);
  G->AddEdge(4,8,2);
  G->AddEdge(4,5,1);
  G->AddEdge(5,6,9);
  G->AddEdge(6,13,4);
  G->AddEdge(7,4,6);
  G->AddEdge(8,7,3);
  G->AddEdge(8,9,3);
  G->AddEdge(9,10,2);
  G->AddEdge(9,12,4);
  G->AddEdge(10,6,6);
  G->AddEdge(10,3,10);
  G->AddEdge(11,12,6);
  G->AddEdge(12,9,2);
  G->AddEdge(12,11,8);
  G->AddEdge(12,14,9);
  G->AddEdge(13,14,6);
  G->AddEdge(14,13,2);
  
  class myVis {
  public:
    myVis() { }
    myVis(const int& Nodes) { }
    void DiscoverNode(int NId){ } //printf("%i\n",NId);
    void FinishNode(const int& NId) { }
    void ExamineEdge(const int& NId1, const int& NId2) { }
    void TreeEdge(const int& NId1, const int& NId2) { }
    void BackEdge(const int& NId1, const int& NId2) { }
    void FwdEdge(const int& NId1, const int& NId2) { }
  };
  
  //DFS
  std::cout << "---DFS---" << std::endl;
  myVis vis(G->GetNodes());
  auto begin = std::chrono::high_resolution_clock::now();
  TCnCom::GetDfsVisitor(G, vis);
  auto end = std::chrono::high_resolution_clock::now();
  auto dfs = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  std::cout << "Tiempo de ejecución de DFS: " << dfs.count() << " us" << std::endl;
  
  //BFS
  std::cout << "---BFS---" << std::endl;
  begin = std::chrono::high_resolution_clock::now();
  PNGraph GBFS = TSnap::GetBfsTree(G, 1, true, true);
  end = std::chrono::high_resolution_clock::now();
  auto bfs = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  std::cout << "Tiempo de ejecución de BFS: " << bfs.count() << " us" << std::endl;
  
  //  for (TNGraph::TNodeI NI = GBFS->BegNI(); NI < GBFS->EndNI(); NI++) {
  //    printf("%d\n", NI.GetId());
  //  }
  
  //Dijkstra
  std::cout << "---Dijkstra---" << std::endl;
  dijkstra(G, 1);
  
  
  //Floyd Warshall
  std::cout << "---Floyd Warshall---" << std::endl;
  int numNodos = G->GetNodes();
  floydWarshall(G, numNodos);
  
  //Prim
  std::cout << "-------Prim------" << std::endl;
  prim(G, 1);
  
  //Kruskal
  std::cout << "-------Kruskal------" << std::endl;
  kruskal(G, numNodos);
  
  return 0;
  
}

void floydWarshall(TPt<TNodeEDatNet<TInt, TFlt> >  G, int &vertices) {
  
  int infinito = 1000000;
  std::vector<int> fila;
  std::vector< std::vector<int> > mat;
  
  //inicializacion de la matriz
  for (int i = 0; i < vertices; i++) {
    
    for (int j = 0; j < vertices; j++) {
      fila.push_back(-2);
    }//cierre del segundo fors
    
    mat.push_back(fila);
  }//cierre del primer for
  
  //matriz inicial
  for (int i = 0; i < vertices; i++) {
    
    for (int j = 0; j < vertices; j++) {
      
      if (i == j) {
        mat[i][j] = 0;
      }//cierre del if
      else if (G->IsEdge(i+1, j+1)) {
        mat[i][j] = G->GetEDat(i+1,j+1);
      }//cierre del else if
      else {
        mat[i][j] = infinito;
      }//cierre del else
      
    }//cierre del segundo for
    
  }//cierre del primer for
  
  //empieza a contar el tiempo de ejecución
  auto begin = std::chrono::high_resolution_clock::now();
  //implementacion de Floyd Warshall
  for(int k = 0; k < vertices; ++k){
    
    for(int i = 0; i < vertices; ++i){
      
      for(int j = 0; j < vertices; ++j) {
        
        int temp = mat[i][k] + mat[k][j];
        if ( temp < mat[i][j] ){
          mat[i][j] = temp;
        }//cierre del if
        
      }//cierre del tercer for
      
    }//cierre del segundo for
    
  }//cierre del primer for
  auto end = std::chrono::high_resolution_clock::now();
  auto floyd = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  
  //  //impresion de la matriz
  //  for (int i = 0; i < vertices; i++) {
  //    for (int j = 0; j < vertices; j++) {
  //
  //      if (mat[i][j] == infinito) {
  //        std::cout << "Del nodo " << i+1 << " al nodo " << j+1 << " : inf" << std::endl;
  //      }//cierre del if
  //      else {
  //        std::cout << "Del nodo " << i+1 << " al nodo " << j+1 << " : " << mat[i][j] << std::endl;
  //      }//cierre del else
  //
  //    }//cierre del segundo for
  //    std::cout << std::endl;
  //  }//cierre del primer for
  
  //impresion del tiempo de ejecucion
  std::cout << "Tiempo de ejecución de Floyd-Warshall: " << floyd.count() << " us" << std::endl;
  
}//cierre de floyWarshall

void dijkstra(TPt<TNodeEDatNet<TInt, TFlt> >  graph, const int &v){
  
  int size = graph->GetNodes();
  std::deque<int> noVisitados;
  std::vector<int> visitados;
  
  //empieza a contar el tiempo de ejecución
  auto begin = std::chrono::high_resolution_clock::now();
  std::vector<float> distances (size,INFINITY);
  std::vector<int> parents (size,-1);
  
  for(int i=0; i<size; i++){
    noVisitados.push_back(i+1);
  }//cierre del for
  
  distances[v-1] = 0;
  
  while(!noVisitados.empty()){
    
    std::sort(noVisitados.begin(), noVisitados.end(), sorter(distances));
    int u = noVisitados[0];
    int sourceNode = u;
    
    noVisitados.pop_front();
    visitados.push_back(u);
    
    TNodeEDatNet<TInt, TFlt>::TNodeI NI = graph->GetNI(sourceNode);
    
    for (int e = 0; e < NI.GetOutDeg(); e++){
      
      int destNode = NI.GetOutNId(e);
      TNodeEDatNet<TInt, TFlt>::TEdgeI EI = graph->GetEI(sourceNode,destNode);
      float edgeWeight = (float)EI.GetDat();
      
      if(distances[destNode-1] > distances[sourceNode-1] + edgeWeight){
        distances[destNode-1] = distances[sourceNode-1] + edgeWeight;
        parents[destNode-1] = sourceNode;
      }//cierre del if
      
    }//cierre del for
    
  }//cierre del while
  auto end = std::chrono::high_resolution_clock::now();
  auto dijkstra = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  
  //  int node = 1;
  //  for (int i = 0 ; i< parents.size(); i++){
  //    std::cout << parents[node-1] << " -> " << node << " distance:  " << distances[node-1] << std::endl;
  //    node++;
  //  }//cierre del for
  
  //impresion del tiempo de ejecucion
  std::cout << "Tiempo de ejecución de Dijkstra: " <<dijkstra.count() << " us" << std::endl;
  
}//cierre de dijkstra

void prim(TPt<TNodeEDatNet<TInt, TFlt>> Grafo, int source)
{
  double inf = 1000000;
  distanciaId* u;
  std::vector<distanciaId*> delta(Grafo->GetNodes());
  std::vector<int> Q(Grafo->GetNodes());
  std::vector<distanciaId*> S;
  
  auto begin = std::chrono::high_resolution_clock::now();
  for (TNodeEDatNet< TInt, TFlt >::TNodeI NI = Grafo->BegNI(); NI < Grafo->EndNI(); NI++)
  {
    distanciaId* temp = new distanciaId();
    temp->setDistancia(inf);
    temp->setId(NI.GetId());
    delta[NI.GetId() - 1] = temp;
  }
  delta[source-1]->setDistancia(0);
  while (!delta.empty())
  {
    std::sort(delta.begin(), delta.end(), comparaDistancia);
    u = delta[0];
    for (auto i : delta)
    {
      if (Grafo->IsEdge(u->getId(), i->getId()))
      {
        if (i->getDistancia() > Grafo->GetEDat(u->getId(), i->getId()))
        {
          i->setPadre(u->getId());
          i->setDistancia(Grafo->GetEDat(u->getId(), i->getId()));
        }
      }
    }
    S.push_back(delta[0]);
    delta.erase(delta.begin());
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto prim = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  std::cout << "Tiempo de ejecución de Prim: " << prim.count() << " us" << std::endl;
  
  //  for (auto i : S) {
  //    std::cout << i->getPadre() << "--->" << i->getId() << std::endl;
  //  }
}

void kruskal(TPt<TNodeEDatNet<TInt, TFlt> > G, int vertices) {
  
  int padre[vertices];
  std::vector<std::pair<int, TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI> > aristas;
  std::vector<TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI> tree;
  
  auto begin = std::chrono::high_resolution_clock::now();
  for (TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
    int id = NI.GetId();
    padre[id-1] = id;
  }
  
  for (TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++)
    aristas.push_back(std::make_pair(ei.GetDat(), ei));
  
  std::sort(aristas.begin(), aristas.end());
  
  for (unsigned int i = 0; i < aristas.size(); ++i) {
    int u = aristas[i].second.GetSrcNId();
    int v = aristas[i].second.GetDstNId();
    if (encontrarSet(u, padre) != encontrarSet(v, padre)) {
      tree.push_back(aristas[i].second);
      setUnion(u, v, padre);
    }
  }
  
  while (!tree.empty()) {
//    std::cout << tree.front().GetSrcNId() << " --> " << tree.front().GetDstNId();
//    std::cout << " peso: " << tree.front().GetDat() << std::endl;
    tree.erase(tree.begin());
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto kruskal = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
  std::cout << "Tiempo de ejecución de Kruskal: " << kruskal.count() << " us" << std::endl;

  
}

int encontrarSet(int x, int padre[]) {
  if (padre[x-1] != x)
    padre[x-1] = encontrarSet(padre[x-1], padre);
  return padre[x-1];
}

void setUnion(int u, int v, int padre[]) {
  u = encontrarSet(u, padre);
  v = encontrarSet(v, padre);
  padre[u-1] = v;
}

//void printVector(std::vector<int> vector){
//  
//  printf("[ ");
//  
//  for (int i = 0; i < vector.size(); i++){
//    printf("%d ",vector[i]);
//  }
//  
//  printf("] \n");
//  
//}//cierre de printVector

bool sortFunction(int a, int b, std::vector<float> distancias){
  return distancias[a-1] < distancias[b-1];
}//cierre de sortFunction

