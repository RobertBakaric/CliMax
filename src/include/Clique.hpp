/*
 * Clique.hpp
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

#include <iostream>
#include <vector>
#include <set>

using namespace std;


template <typename Tint>
class CliMax : protected Graph<Tint>{
   
/* Data structures */
   Tint MinClique;
   vector <vector <Tint>> MaxCliques;
   
/* Functions */
 
   /* Optimization: 
    * this would go much faster if vectors instead os sets were used 
    */
   void Expand(set<Tint>& Memb, set<Tint>& Subg, set<Tint>& Cand); 
   
   public: 

/* Constructor */
   CliMax(vector<Tint>& parent, vector <Tint>& child, Tint Min);
   
/* Destructor */
   ~CliMax();
   
/* Functions */

   void ComputeMaxCliques();
   vector <vector<Tint>>& GetMaxCliques();

/* Not finished: */   
   //ComputeMaxClique();
   
};




/* Constructors */
template <typename Tint>
CliMax<Tint>::CliMax(vector<Tint>& parent, vector <Tint>& child, Tint Min): Graph<Tint>(parent,child){
   MinClique = Min;   
}

/* Destructors */
template <typename Tint>
CliMax<Tint>::~CliMax(){
   vector <vector <Tint>>().swap( MaxCliques );
}

/* Functions */
template <typename Tint>
vector <vector<Tint>>& CliMax<Tint>::GetMaxCliques(){
   
   return MaxCliques;
   
}

/*  Implementation od CLIQUES by  Tomita et al. (2006) */
template <typename Tint>
void CliMax<Tint>::ComputeMaxCliques(){
   
   set<Tint> Memb;   
   set<Tint> Subg;
   set<Tint> Cand;

   for ( auto it = this->GraphMap.begin(); it != this->GraphMap.end(); ++it ){ 
      if (it->second.size() < MinClique-1) 
         continue;
      Subg.insert(it->first);
      Cand.insert(it->first);
         
   }
   Expand(Memb, Subg, Cand); 
}

template <typename Tint>
void CliMax<Tint>::Expand(set<Tint>&Memb, set<Tint>&Subg, set<Tint>&Cand){
   
   if (!Subg.size()){
     if (Memb.size() >= MinClique){
        vector<Tint> vec(Memb.begin(), Memb.end());
        MaxCliques.push_back(vec);
      }
     return;
   }
   
    Tint u = -1;
    Tint max = 0;
   
    /* a vertex 'u' that maximizes intersection */
    
     for(auto it = Subg.begin(); it != Subg.end(); it++){  // this needs further optimization 
        Tint intersec = 0;
        vector<Tint> & Children = this->GraphMap.find(*it)->second;
        if (Children.size() <= max) 
           continue;
        for(Tint i = 0; i< Children.size(); i++){
           if (Cand.find(Children[i]) != Cand.end()) 
              intersec++;
        }
        if (u == -1 || intersec > max){
            max = intersec;
            u = *it;
        }   	 
     }
     vector<Tint> & Children =  this->GraphMap.find(u)->second;
     set<Tint> tmp;
     
     for(Tint i = 0; i< Children.size(); i++) // I could do better here
       tmp.insert(Children[i]);
     
    /* compute EXT_u */
    set<Tint> Ext_u;
    for (auto it = Cand.begin(); it != Cand.end(); it++){
       if (tmp.find(*it) != tmp.end()) 
          continue;
       Ext_u.insert(*it);
    }
        
    while(Ext_u.size()>0){
       auto it = Ext_u.begin();
       vector<Tint> & Children  = this->GraphMap.find(*it)->second;
       set<Tint> vert;  
       for(Tint i = 0; i< Children.size(); i++)
          vert.insert(Children[i]);
          
       Memb.insert(*it);
       set<Tint> Subg_new;
       set<Tint> Cand_new;
       
       set_intersection(Subg.begin(), Subg.end(), vert.begin(), vert.end(), insert_iterator<set<Tint> >(Subg_new, Subg_new.begin()));
       set_intersection(Cand.begin(), Cand.end(), vert.begin(), vert.end(), insert_iterator<set<Tint> >(Cand_new, Cand_new.begin()));
       
       Expand(Memb, Subg_new, Cand_new);
       Memb.erase(*it);
       Cand.erase(*it);
       Ext_u.erase(it);
   }
}


/*  Left for implementation:
 * 
 *     Maximum Clique 
 * 
 *     + Though this can be derived from maximal ones, the algorithm is 
 *       quite inefficient iff the maximum one is required.
 * */

