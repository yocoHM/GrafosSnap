//
//  main.cpp
//  XcodeTest

#include "Snap.h"
#include <iostream>

using namespace TSnap;

int main(int argc, char* argv[]) {
  
  TPt<TNodeEDatNet<TInt, TFlt>>  G = TNodeEDatNet<TInt, TFlt>::New();
  
  printf("Creando el grafo:\n");
  for (int n = 1; n < 15; n++) {
    G->AddNode(n);
  }
  
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
    void DiscoverNode(int NId){
      printf("%i\n",NId);
    }
    void FinishNode(const int& NId) { }
    void ExamineEdge(const int& NId1, const int& NId2) { }
    void TreeEdge(const int& NId1, const int& NId2) { }
    void BackEdge(const int& NId1, const int& NId2) { }
    void FwdEdge(const int& NId1, const int& NId2) { }
  };
  
  std::cout << "---DFS---" << std::endl;
  myVis vis(G->GetNodes());
  TCnCom::GetDfsVisitor(G, vis);
  
  std::cout << "---BFS---" << std::endl;
  
  PNGraph GBFS = TSnap::GetBfsTree(G, 1, true, true);
  
  for (TNGraph::TNodeI NI = GBFS->BegNI(); NI < GBFS->EndNI(); NI++)
  {
    printf("%d\n", NI.GetId());
  }
  
  return 0;
}
