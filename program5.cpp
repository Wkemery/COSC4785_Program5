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
vector<string> lines;
int main()
{
  
  lines.push_back("");
  yyparse();
  if(forest.size() > 0) cout << endl << endl;

  for(unsigned int i = 0; i < forest.size(); i++)
  {
    if(forest[i] != 0) forest[i]->print(&cout);
    delete forest[i];
  }
  
  
  return 0;
  
}
