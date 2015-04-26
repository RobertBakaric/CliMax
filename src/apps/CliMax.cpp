/*
 * CliMax.cpp
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
#include <fstream>
#include <vector>
#include <boost/program_options.hpp>

#include <Graph.hpp>
#include <Clique.hpp>

namespace po = boost::program_options;
using namespace std;



void PrintAcknowledgements(){

cout <<"***************************************************************************************"<<"\n";
cout <<"                                        CliMax                                         "<<"\n";
cout <<"                                          by                                           "<<"\n";
cout <<"                                    Robert Bakaric                                     "<<"\n\n";
cout <<"CONTACT:                                                                               "<<"\n";
cout <<" Code written and maintained by Robert Bakaric,                                        "<<"\n";
cout <<" email: rbakaric@irb.hr , bakaric@evolbio.mpg.de                                       "<<"\n\n";
cout <<"ACKNOWLEDGEMENT:                                                                       "<<"\n";
cout <<"     Tomita, E.,Tanaka, A. and  Takahashi, H. (2006) The worst-case time complexity    "<<"\n";
cout <<"     for generating all maximal cliques and computational experiments. Theor. Comput.  "<<"\n";
cout <<"     Sci. 363:28-42                                                                    "<<"\n";
cout <<"                                                                                       "<<"\n";
cout <<"LICENSE:                                                                               "<<"\n";
cout <<" The program is distributed under the GNU General Public License. You should have      "<<"\n";
cout <<" received a copy of the licence together  with this software. If not, see              "<<"\n";
cout <<" http://www.gnu.org/licenses/                                                          "<<"\n";
cout <<"***************************************************************************************"<<"\n\n\n";
}


template <typename INT, typename CHARA>
po::variables_map SetOptions(INT& argc, CHARA& argv){

    try {
        string version = "1.0";
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("version,v", "print version information")
            ("min_clique_size,c", po::value< int >()->default_value(2), "Minimum clique size")
            ("graph-file,i", po::value< string >(), "Adjecency list (tab seperated -> child <tab> parent)")
        ;

        po::positional_options_description p;
        p.add("input-file,i", -1);
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);
    
        if (vm.count("help")) {
            cout << "Usage: ./program [options]\n\n";
            PrintAcknowledgements();
            cout << desc;
            exit(0);
        }else if (vm.count("version")) {
            cout << "Program version:  " << version << "\n";
            exit(0);
        }
        if (!vm.count("graph-file")){
            cout << "Adjecensy list not provided \n";
            exit(0);
        }
        return vm;    
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        exit(0);
    }    
}


template <typename Tint>
void ReadGraph(const string & file, vector<Tint>& parent, vector<Tint>& child){
  
  fstream fs;
  fs.open (file.c_str(), ios::in);
  if ( !fs.is_open())
    throw runtime_error ("Cannot open file: " + file );


  int i = 0; 
  while( !fs.eof()) {
    if(child.size() == i){
       child.resize((child.size()+1)*2);
       parent.resize((parent.size()+1)*2);
    }
    fs >> child[i];
    fs >> parent[i++] ;
  }  
 
  parent.resize(i-1);
  child.resize(i-1);
  fs.close();
}


int main (int argc, char** argv){
   
   
/* Get cmd Options */  
   po::variables_map arg;
   arg = SetOptions(argc,argv);

   string file =  arg["graph-file"].as<string>();
   int cutoff =  arg["min_clique_size"].as<int>();

   vector<int> parent;
   vector<int> child;
   
   try{
      
/* Load Graph */  
      ReadGraph(file, parent, child);

/* Make CliMax object */
      CliMax<int> clique(parent, child, cutoff);
  
/* Compute Maximal Cliques */  
      clique.ComputeMaxCliques();
      
/* Print cliques */
      vector<vector<int>> mc = clique.GetMaxCliques();
      cout << "MaxCliqSize\t" << "(Vertices)\n";
      for(int i = 0; i< mc.size(); i++){
         cout << mc[i].size() << "\t(" << mc[i][0];
         for(int j = 1; j< mc[i].size(); j++)
            cout << "," << mc[i][j] ;
         cout << ")\n";
      }
    
   }catch(runtime_error& e){
      cerr << e.what() << "\n";
   }
   
   return 0;
}
