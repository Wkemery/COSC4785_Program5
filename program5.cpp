/*
 * program4.cpp
 * Author: Wyatt Emery
 * Date: OCT 20, 2017
 *
 * COSC 4785, Homework4
 *
 */

#include<iostream>
#include<FlexLexer.h>
#include<string>
#include<map>
#include<cstdlib>
#include"Lexeme.h"
#include"Node.h"
#include"program5_bison.h"
using namespace std;

yyFlexLexer scanner;
vector<Node*> forest;
int main()
{
  
  yyparse();
  if(forest.size() > 0) cout << endl << endl;
  SymTable* symbolTable = new SymTable(0, "Root");
  for(unsigned int i = 0; i < forest.size(); i++)
  {
//     if(forest[i] != 0) forest[i]->print(&cout);
    if(forest[i] != 0) forest[i]->buildTable(symbolTable);
  }
  
  for(unsigned int i = 0; i < forest.size(); i++)
  {
    //do typecheck here. 
    bool ret = true;
    if(forest[i] != 0) ret = forest[i]->typeCheck(0);
    if(!ret) cerr << "Error occured!" << endl;
  }
  
  symbolTable->print(&cout, 0);
  
  for(unsigned int i = 0; i < forest.size(); i++)
  {
    delete forest[i];
  }
  
  return 0;
  
}
