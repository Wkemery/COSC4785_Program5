##############################
# Makefile
# Author: Wyatt Emery
# Date: NOV 21, 2017
#
# COSC 4785, Homework 5
# 
##############################

CXX=g++
CXXFLAGS=-ggdb -Wall -std=c++11
FLEX=flex++
FLEXFLAGS=--warn
YACC=bison
YACCFLAGS=--report=state -W -d -v

.PHONY: clean tar

program5: program5.cpp program5_lex.cpp program5_bison.c program5_bison.h \
	Lexeme.cpp Node.cpp Lexeme.h Node.h SymTable.h SymTable.cpp
	${CXX} ${CXXFLAGS} program5.cpp program5_lex.cpp Lexeme.cpp program5_bison.c \
	Node.cpp SymTable.cpp -o program5

program5_lex.cpp: program5.lpp
	${FLEX} ${FLEXFLAGS} program5.lpp

program5_bison.c: program5.y
	${YACC} ${YACCFLAGS} -o program5_bison.c program5.y


tar: program5.cpp program5.lpp Lexeme.cpp Lexeme.h Makefile
	tar -cf wemery_HW5.tar program5.cpp program5.lpp Lexeme.cpp Lexeme.h \
	program5.y Node.h Node.cpp SymTable.h SymTable.cpp program5_readme.txt \
	program5.pdf Makefile

clean: 
	/bin/rm -f *.o core.* *.output program5 program5_lex.cpp wemery_HW5.tar \
    program5_bison.c program5_bison.h

