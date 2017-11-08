/*
 * Lexeme.h
 * Author: Wyatt Emery
 * Date: OCT 20, 2017
 *
 * COSC 4785, Homework4
 *
 */


#include<string>
#include<vector>


#define SPACE 10

#define ERR 666 
#define IGNORE 987


#ifndef LEXEME_H
#define LEXEME_H

extern std::vector<std::string> lines;
typedef struct lexeme Lexeme;

struct lexeme
{
  std::string value;
  int column;
  int line;
};

extern void setToken(char* yytext, int yylineno, int column);
#endif
