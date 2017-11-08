/*
 * Lexeme.cpp
 * Author: Wyatt Emery
 * Date: OCT 20, 2017
 *
 * COSC 4785, Homework4
 *
 */

#include<iostream>
using namespace std;
#include"Lexeme.h"
#include"Node.h"
#include"program5_bison.h"
void setToken(char* yytext, int lineno, int column)
{
  
  if(yytext != NULL) yylval.token->value = yytext;
  else yylval.token->value = "";
//   cerr << "yylval.token->value = " << yylval.token->value << endl;;
  
  yylval.token->line = lineno;
  yylval.token->column = column;
}