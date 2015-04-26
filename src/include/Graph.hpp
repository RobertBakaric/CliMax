/*
 * GraphMan.hpp
 * 
 * Copyright 2015 Robert Bakaric <rbakaric@irb.hr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <vector>
#include <unordered_map>

using namespace std;


/* Graph */

template <typename Tint>
class Graph{

  protected:
/* Data Container */
  unordered_map<Tint, vector<Tint>> GraphMap;
  unordered_map<Tint,Tint>          GraphMapRev;
  unordered_map<Tint,Tint>          Traversed;
  vector<Tint>                      TraversalPath;
  vector<Tint>                      Distance;
  Tint Max;

/* Functions */ 
  void MakeGraph(const vector<Tint>& parent, const vector<Tint>& child);
  
  public:

/* Constructor */
   Graph(const vector<Tint>& parent, const vector<Tint>& child);
/* Destructor */
  ~Graph();
/* Explicite Constructor */
   void make(const vector<Tint>& parent, const vector<Tint>& child);
/* Explicite destructor */
   void destroy();
  
/* Functions */
   void AddVertex(const Tint vertex, const Tint location); /* Note. to avoid cycles use CreateVertex() for vertex variable */
   void RemoveVertex(const Tint vertex);
   void JoinVertex(const Tint vertexA,const Tint vertexB);
   void RelocateVertex(const Tint vertex,const Tint parentvertex);
   void EraseVertex(const Tint vertex);
   void DepthFirstTrav(const Tint vertex, Tint depth, Tint Flag);
   /* BreadthFirstTrav() : NOT FINISHED YET */
   Tint CreateVertex();
   vector<Tint>& GetDepthVec();
   vector<Tint>& GetVerticesVec();
   void Clean();
   void DmpGraph(vector<Tint>& parent, vector<Tint>& child);
};



/* Constructors */

template <typename Tint>
Graph<Tint>::Graph(const vector<Tint>& parent, const vector<Tint>& child){
  Max = 0;
  MakeGraph(parent, child);
}

template <typename Tint>
void Graph<Tint>::make(const vector<Tint>& parent, const vector<Tint>& child){
  Max = 0;
  MakeGraph(parent, child);
}


template <typename Tint>
void Graph<Tint>::MakeGraph(const vector<Tint>& parent, const vector<Tint>& child){
  for (Tint i = 0; i< parent.size(); i++){
    
    if (child[i] > Max) Max = child[i];
    if (parent[i] > Max) Max = parent[i];
    
    GraphMap[parent[i]].push_back(child[i]);
    GraphMapRev[child[i]] = parent[i];
  }
}


/* Destructors */

template <typename Tint>
Graph<Tint>::~Graph(){
  GraphMap.clear();
  GraphMapRev.clear();
  Traversed.clear();
  vector<Tint>().swap(TraversalPath);
  vector<Tint>().swap(Distance);

}

template <typename Tint>
void Graph<Tint>::destroy(){
  GraphMap.clear();
  GraphMapRev.clear();
  Traversed.clear();
  vector<Tint>().swap(TraversalPath);
  vector<Tint>().swap(Distance);

}



/* Functions */

template <typename Tint>
Tint Graph<Tint>::CreateVertex(){
   return ++Max;
}


template <typename Tint>
void Graph<Tint>::AddVertex(const Tint vertex, const Tint location){
   GraphMap[location].push_back(vertex);
   GraphMapRev[vertex] = location;
}


template <typename Tint>
void Graph<Tint>::RemoveVertex(const Tint vertex){
   Tint parent = GraphMapRev[vertex];
   GraphMap[parent].erase( remove( begin(GraphMap[parent]), end(GraphMap[parent]), vertex ), end(GraphMap[parent]) );
   RelocateVertex(vertex,0);
}

template <typename Tint>
void Graph<Tint>::EraseVertex(const Tint vertex){
   Tint parent = GraphMapRev[vertex];
   for(Tint i = 0; i< GraphMap[vertex].size(); i++){
     RelocateVertex(GraphMap[vertex][i],parent);
   }
   GraphMap[parent].erase( remove( begin(GraphMap[parent]), end(GraphMap[parent]), vertex ), end(GraphMap[parent]) );
}


template <typename Tint>
void Graph<Tint>::RelocateVertex(const Tint vertex, const Tint newvertexparent){
   GraphMapRev[vertex] = newvertexparent;
   GraphMap[newvertexparent].push_back(vertex);
}


template <typename Tint>
void Graph<Tint>::JoinVertex(const Tint vertexchild, const Tint vertexparent){
   AddVertex(vertexchild,vertexparent);
}


template <typename Tint>
void Graph<Tint>::DepthFirstTrav(const Tint vertex, Tint depth, Tint Flag){
  for(Tint i = 0;i< GraphMap[vertex].size();i++){ 
    if(Traversed.find(GraphMap[vertex][i]) != Traversed.end()){
      cerr << "Cycle detected! Edge:" << vertex << " -- " << GraphMap[vertex][i] << endl;
      continue;
    }
    if(Flag == 0)
      depth--;
    Flag = 0;
    Traversed[GraphMap[vertex][i]] = 1;
    Distance.push_back(++depth);
    TraversalPath.push_back(GraphMap[vertex][i]);
    if(GraphMap.find(GraphMap[vertex][i]) != GraphMap.end()){
      DepthFirstTrav(GraphMap[vertex][i], depth, 1);
     // Distance.push_back(depth);
     // TraversalPath.push_back(GraphMap[vertex][i]);
    }
  }
}


template <typename Tint>
void Graph<Tint>::Clean(){
  vector<Tint>().swap(Distance);
  vector<Tint>().swap(TraversalPath);
  Traversed.clear();
}

template <typename Tint>
vector<Tint>& Graph<Tint>::GetDepthVec(){
  return Distance;
}

template <typename Tint>
vector<Tint>& Graph<Tint>::GetVerticesVec(){
  return TraversalPath;
}


/* FM: Bad implementation */
template <typename Tint>
void Graph<Tint>::DmpGraph(vector<Tint>& parent, vector<Tint>& child){
  Tint i = 0;
  for ( auto it = GraphMap.begin(); it != GraphMap.end(); ++it ){ // extreamly inefficient 
    for(Tint j = 0; j< GraphMap[it->first].size();j++){
      if(parent.size() == i){
        parent.resize((parent.size()+1) * 2);
        child.resize((child.size()+1) * 2);
      }
      parent[i] = it->first;
      child[i++]  = GraphMap[it->first][j];
    }
  }
  parent.resize(i);
  child.resize(i);
}


