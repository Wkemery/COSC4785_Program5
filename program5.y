/*
 * program4.y
 * Author: Wyatt Emery
 * Date: NOV 3, 2017
 * COSC 4785, Homework4
 */

%{


#include <iostream>
#include <FlexLexer.h>
#include "Node.h"
#include "Lexeme.h"

using namespace std;

extern vector<Node*> forest;
extern yyFlexLexer scanner;

#define yylex() scanner.yylex()

// need the function prototype for the parser.

void yyerror(const char *);

%}

%union {
  Node* ttype;
  Lexeme* token;

}

/* I eliminated the use of %empty in this file. It generally causes too many 
 * problems and I don't want to deal with them. Instead, I just expand the 
 * higher level production to not include whatever could have been empty.
 */

/* vardecr, constructordecr, methoddecr, and statementr no longer build a 
 * left recursive tree as it is defined in the grammar. Now there is a singal 
 * RNode which encompasses all four of these grammar items. All the children
 * are stored in a single vector. This eliminated a lot of the need for
 * complicated error checking since statements are no longer children of
 * statements etc... However, when printed out, if there are errors, the 
 * number of children associated with an RNode includes error nodes which 
 * will not be printed out. 
 */



%token<token> CLASS THIS IF ELSE WHILE RETURN PRINT READ VOID NEW NULLKEYWORD 

%token<token> INT ASSIGNOP DOTOP COMMA LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE

%token<token>PLUS MINUS NOT

%token<token>DOUBEQ NOTEQ LESSEQ GREATEQ LESS GREAT

%token<token>TIMES DIVIDE MOD DOUBAND DOUBBAR

%token<token>IDENTIFIER SEMICO NUM 

%type<ttype> expression name multibracks unaryop relationop productop 
%type<ttype> sumop arglist brackexpression newexpression classdec vardec statement
%type<ttype> conditionalstatement block statementr classbody vardecr param
%type<ttype> type methoddec constructordec paramlist constructordecr methoddecr


%destructor {delete($$);} CLASS THIS IF ELSE WHILE RETURN PRINT READ VOID NEW 
%destructor {delete($$);} NULLKEYWORD INT ASSIGNOP DOTOP COMMA LPAREN RPAREN 
%destructor {delete($$);} LBRACK RBRACK LBRACE RBRACE PLUS MINUS NOT DOUBEQ 
%destructor {delete($$);} NOTEQ LESSEQ GREATEQ LESS GREAT TIMES DIVIDE MOD 
%destructor {delete($$);} DOUBAND DOUBBAR IDENTIFIER SEMICO NUM

%destructor {delete($$);} expression name multibracks unaryop 
%destructor {delete($$);} relationop productop sumop arglist brackexpression 
%destructor {delete($$);} newexpression classdec conditionalstatement statement
%destructor {delete($$);} block statementr classbody type vardecr param
%destructor {delete($$);} methoddec constructordec paramlist constructordecr
%destructor {delete($$);} methoddecr

%precedence IFEL
%precedence ELSE
%precedence NAME

%left DOUBEQ NOTEQ LESSEQ GREATEQ LESS GREAT RE
%left PLUS MINUS DOUBBAR BIN
%left TIMES DIVIDE MOD DOUBAND PRO
%precedence NEG
%precedence LBRACK


%% 
input:  %empty
        | input classdec { 
            if(!$2->getErr()) forest.push_back($2);
            else delete $2;
        }
        | error{
          cerr << ": Unknown Around " << yylval.token->line << ":" 
          << yylval.token->column <<endl << endl; 
          yyclearin;
          yyerrok;
        }
;

classdec: CLASS IDENTIFIER classbody {
                $$ = new ClassDec($2->value, $3);
                $$->setLineNumber($1->line);
                delete $1; delete $2;
                }
;

classbody:  LBRACE RBRACE {
                  $$ = new ClassBody(CLASSBODYEMPTY);
                  $$->setLineNumber($1->line);
                  delete $1; delete $2;
                  }
            | LBRACE vardecr RBRACE{
                  $$ = new ClassBody($2, CLASSBODYVAR);
                  $$->setLineNumber($1->line);
                  delete $1; delete $3;
                  }
            | LBRACE vardecr constructordecr RBRACE{
                  $$ = new ClassBody($2, $3, CLASSBODYVARCON);
                  $$->setLineNumber($1->line);
                  delete $1; delete $4;
                  }
            | LBRACE vardecr constructordecr methoddecr RBRACE{
                  $$ = new ClassBody($2, $3, $4, CLASSBODYVARCONMET);
                  $$->setLineNumber($1->line);
                  delete $1; delete $5;
                  }
            | LBRACE constructordecr methoddecr RBRACE{
                  $$ = new ClassBody($2, $3, CLASSBODYCONMET);
                  $$->setLineNumber($1->line);
                  delete $1; delete $4;
                  }
            | LBRACE constructordecr RBRACE{
                  $$ = new ClassBody($2, CLASSBODYCON);
                  $$->setLineNumber($1->line);
                  delete $1; delete $3;
            }
            | LBRACE constructordecr error{
                  $$ = new ErrNode();
                  cerr << "Missing right brace around " << yylval.token->line
                  << ":" << yylval.token->column << endl << endl;
                  yyerrok;
                  delete $1; delete $2;
            }
            | LBRACE methoddecr RBRACE{
                  $$ = new ClassBody($2, CLASSBODYMET);
                  $$->setLineNumber($1->line);
                  delete $1; delete $3;
                  }
            | LBRACE vardecr methoddecr RBRACE{
                  $$ = new ClassBody($2, $3, CLASSBODYVARMET);
                  $$->setLineNumber($1->line);
                  delete $1; delete $4;
            }
;

vardecr: vardec { 
              $$ = new RNode(RECVARDEC);
              $$->setLineNumber($1->getLineNumber());
              ((RNode*)$$)->add($1);
}
          | vardecr vardec {
                ((RNode*)$1)->add($2);
                $$ = $1;
                }
          | vardecr error {
                  $$ = new ErrNode();  
                  cerr << ": Unexpected token after variable declaration around " 
                  << yylval.token->line
                  << ":" << yylval.token->column << endl << endl;
                  yyclearin;
                  yyerrok;
                  delete $1;
                }
;

constructordecr: constructordec { 
                        $$ = new RNode(RECCONDEC);
                        $$->setLineNumber($1->getLineNumber());
                        ((RNode*)$$)->add($1);
                        }
                  | constructordecr constructordec {
                        ((RNode*)$1)->add($2);
                        $$ = $1;
                        }
                  | constructordecr error {
                        $$ = new ErrNode();  
                        cerr << ": Unexpected token after constructor declaration around " 
                        << yylval.token->line
                        << ":" << yylval.token->column << endl << endl;
                        yyclearin;
                        yyerrok;
                        delete $1;
                        }
;

methoddecr: methoddec { 
                  $$ = new RNode(RECMETDEC);
                  $$->setLineNumber($1->getLineNumber());
                  ((RNode*)$$)->add($1);
                  }
            | methoddecr methoddec {
                  ((RNode*)$1)->add($2);
                  $$ = $1;
                  }
            | methoddecr error {
                  $$ = new ErrNode();  
                  cerr << ": Unexpected token after method declaration around " 
                  << yylval.token->line
                  << ":" << yylval.token->column << endl << endl;
                  yyclearin;
                  yyerrok;
                  delete $1;
                  }
;

statement: name ASSIGNOP expression SEMICO {
                $$ = new Statement($1, $3, STMNTNAMEEXP);
                $$->setLineNumber($1->getLineNumber());
                delete $2; delete $4;
                }
            | name LPAREN arglist RPAREN SEMICO {
                  $$ = new Statement($1, $3, STMNTNAMEARGL);
                  $$->setLineNumber($1->getLineNumber());
                  delete $2; delete $4; delete $5;
                  }
            | name LPAREN RPAREN SEMICO {
                    $$ = new Statement($1,STMNTNAMEEMPTY);
                    $$->setLineNumber($1->getLineNumber());
                    delete $2; delete $3, delete $4;
                  }
            | PRINT LPAREN arglist RPAREN SEMICO {
                  $$ = new Statement($3, STMNTPRNTARGL);
                  $$->setLineNumber($1->line);
                  delete $1; delete $2; delete $4; delete $5;
                  }
            | WHILE LPAREN expression RPAREN statement {
                  $$ = new Statement($3, $5, STMNTWHILE);
                  $$->setLineNumber($1->line);
                  delete $1; delete $2; delete $4;
                  }
            | RETURN expression SEMICO{
                  $$ = new Statement($2, SMTNTRETURN);
                  $$->setLineNumber($1->line);
                  delete $1; delete $3;
                  }
            | RETURN SEMICO {
                  $$ = new Statement(SMTNTRETURN);
                  $$->setLineNumber($1->line);
                  delete $1; delete $2;
                  }
            | SEMICO {
                  $$ = new Statement(STMNTSEMI);
                  $$->setLineNumber($1->line);
                  delete $1;
                  }
            | conditionalstatement {
                  $$ = new Statement($1, STMNTCOND);
                  $$->setLineNumber($1->getLineNumber());
                  }
            | block {
                  $$ = new Statement($1, STMNTBLOCK);
                  $$->setLineNumber($1->getLineNumber());
                  }
            | name ASSIGNOP expression error {
                  $$ = new ErrNode();
                  cerr << "Expected semicolon after expression around " 
                  << yylval.token->line
                  << ":" << yylval.token->column << endl << endl;
                  yyerrok;
                  delete $1; delete $2; delete $3;
                }
            | name ASSIGNOP error {
                  $$ = new ErrNode();
                  cerr << "Expected semicolon before " << yylval.token->line
                  << ":" << yylval.token->column << endl << endl;
                  yyerrok;
                  delete $1; delete $2;
                }
;

block:  LBRACE RBRACE{
              $$ = new Block(BLOCKEMPTY);
              $$->setLineNumber($1->line);
              delete $1; delete $2;
              }
        | LBRACE vardecr RBRACE {
              $$ = new Block($2, BLOCKVARDEC);
              $$->setLineNumber($1->line);
              delete $1; delete $3;
              }
        | LBRACE vardecr statementr RBRACE{
              $$ = new Block($2, $3, BLOCKVARSTMNT);
              $$->setLineNumber($1->line);
              delete $1; delete $4;
              }
        | LBRACE statementr RBRACE {
              $$ = new Block($2, BLOCKSTMNT);
              $$->setLineNumber($1->line);
              delete $1; delete $3;
              }
        | LBRACE error {
            $$ = new ErrNode();
            cerr << "Expected right bracket at " << $1->line
            << ":" << $1->column << endl << endl;
            yyerrok;
            delete $1;
          
        }
;

statementr: statement { 
                  $$ = new RNode(RECSTMNT);
                  $$->setLineNumber($1->getLineNumber());
                  ((RNode*)$$)->add($1);
                  }
            | statementr statement {
                  ((RNode*)$1)->add($2);
                  $$ = $1;
                  }
            | statementr error {
                  $$ = new ErrNode();
                  cerr << "Incomplete Statement around " << yylval.token->line << ":" 
                  << yylval.token->column << endl << endl;
                  yyerrok;
                  yyclearin;
                  delete $1;
                  }
;

conditionalstatement: IF LPAREN expression RPAREN statement %prec IFEL{
                            $$ = new CondStatement($3, $5, CONDSTMNT);
                            $$->setLineNumber($1->line);
                            delete $1; delete $2; delete $4;
                            }                      
                      | IF LPAREN expression RPAREN statement ELSE statement {
                            $$ = new CondStatement($3, $5, $7, CONDSTMNTELSE);
                            $$->setLineNumber($1->line);
                            delete $1; delete $2; delete $4; delete $6;
                            }
                      | IF LPAREN expression error statement %prec IFEL{
                            $$ = new ErrNode();
                            cerr << "Expected right parenthesis after"
                            << " expression around " << $2->line << ":" 
                            << $2->column << endl << endl;
                            yyerrok;
                            delete $1; delete $2; delete $3; delete $5;
                            }
                      | IF LPAREN expression RPAREN error %prec IFEL{
                            $$ = new ErrNode();
                            cerr << "Expected statement after"
                            << " conditional statement around " << $4->line << ":" 
                            << $4->column << endl << endl;
                            yyerrok;
                            delete $1; delete $2; delete $3; delete $4;
                            }

;
constructordec: IDENTIFIER LPAREN paramlist RPAREN block {
                      $$ = new ConstructorDec($1->value, $3, $5, CONSTDEC);
                      $$->setLineNumber($1->line);
                      delete $1; delete $2; delete $4;
                      }
                | IDENTIFIER LPAREN RPAREN block {
                      $$ = new ConstructorDec($1->value, $4, CONSTDECEMPTY);
                      $$->setLineNumber($1->line);
                      delete $1; delete $2; delete $3;
                      }
                | IDENTIFIER LPAREN paramlist RPAREN error {
                      $$ = new ErrNode();
                      cerr << "Expected block after " << $4->line << ":" 
                      << $4->column << endl << endl;
                      yyerrok;
                      delete $1; delete $2; delete $3; delete $4;
                      }
                | IDENTIFIER LPAREN RPAREN error {
                      $$ = new ErrNode();
                      cerr << "Expected block after " << $3->line << ":" 
                      << $3->column << endl << endl;
                      yyerrok;
                      delete $1; delete $2; delete $3;
                      }
;

methoddec: type IDENTIFIER LPAREN paramlist RPAREN block {
                $$ = new MethodDec($1, $2->value, $4, $6, METHODDECTYPE);
                $$->setLineNumber($1->getLineNumber());
                delete $2; delete $3; delete $5;
                }
          | type IDENTIFIER LPAREN RPAREN block {
                $$ = new MethodDec($1, $2->value, $5, METHODDECTYPEEMPTY);
                $$->setLineNumber($1->getLineNumber());
                delete $2; delete $3; delete $4;
                }
          | VOID IDENTIFIER LPAREN paramlist RPAREN block {
                $$ = new MethodDec($2->value, $4, $6, METHODDECVOID);
                $$->setLineNumber($1->line);
                delete $1; delete $2; delete $3; delete $5;
            
                }
          | VOID IDENTIFIER LPAREN RPAREN block {
                $$ = new MethodDec($2->value, $5, METHODDECVOIDEMPTY);
                $$->setLineNumber($1->line);
                delete $1; delete $2; delete $3; delete $4;
                }
          | type IDENTIFIER LPAREN paramlist RPAREN error {
                $$ = new ErrNode();
                cerr << "Expected block after " << $5->line << ":" 
                << $5->column << endl << endl;
                yyerrok;
                delete $1; delete $2; delete $3; delete $4; delete $5;
                }
          | type IDENTIFIER LPAREN RPAREN error {
                $$ = new ErrNode();
                cerr << "Expected block after " << $4->line << ":" 
                << $4->column << endl << endl;
                yyerrok;
                delete $1; delete $2; delete $3; delete $4;
                }
          | VOID IDENTIFIER LPAREN paramlist RPAREN error {
                $$ = new ErrNode();
                cerr << "Expected block after " << $5->line << ":" 
                << $5->column << endl << endl;
                yyerrok;
                delete $1; delete $2; delete $3; delete $4; delete $5;
                  
                }
          | VOID IDENTIFIER LPAREN RPAREN error {
                $$ = new ErrNode();
                cerr << "Expected block after " << $4->line << ":" 
                << $4->column << endl << endl;
                yyerrok;
                delete $1; delete $2; delete $3; delete $4;
                }
                

;

paramlist: param { 
                  $$ = new RNode(RECPARAM);
                  $$->setLineNumber($1->getLineNumber());
                  ((RNode*)$$)->add($1);
                  }                 
            | paramlist COMMA param {
                  ((RNode*)$1)->add($3);
                  $$ = $1;
                  delete $2;
                  }
;

param: type IDENTIFIER {
            $$ = new Param($1, $2->value);
            $$->setLineNumber($1->getLineNumber());
            delete $2;
            }
;
vardec: type IDENTIFIER SEMICO {
              $$ = new VarDec($1, $2->value);
              $$->setLineNumber($1->getLineNumber());
              delete $2; delete $3;
              }
        | type IDENTIFIER error {
              $$ = new ErrNode();
              cerr << "Expected Semicolon At "<< $2->line 
              << ":" << $2->column+$2->value.length() << endl << endl;
              yyerrok;
              delete $1; delete $2;
              }
        | type error SEMICO {
              $$ = new ErrNode();
              cerr << "Expected Identifier before "<< $3->line 
              << ":" << $3->column << endl << endl;
              yyerrok;
              delete $1; delete $3;
              }
;
expression: NUM { 
                  $$ = new Expression($1->value, EXPNUM);
                  $$->setLineNumber($1->line);
                  delete $1;
                  }
            | NULLKEYWORD { 
                  $$ = new Expression("null", EXPNULL);
                  $$->setLineNumber($1->line);
                  delete $1;
                  }
            | READ LPAREN RPAREN { 
                  $$ = new Expression("read", EXPREAD); 
                  $$->setLineNumber($1->line);
                  delete $1; delete $2; delete $3;
                  }
            | READ LPAREN error  {
                  $$ = new ErrNode();
                  cerr << "Expected Right Parenthesis At "<< $2->line << ":" 
                  << $2->column + 1  <<endl << endl;
                  yyerrok;
                  delete $1; delete $2;
                  }
            | unaryop expression %prec NEG { 
                  $$ = new Expression($1, $2, EXPUNARY); 
                  $$->setLineNumber($1->getLineNumber());
                  }
            | unaryop error %prec NEG { 
                  $$ = new ErrNode();
                  cerr << "Expected expression at " << yylval.token->line 
                  << ":" << yylval.token->column << endl << endl;
                  delete $1;
                  yyclearin;
                  yyerrok;
                  }
            | expression relationop expression %prec RE{ 
                  $$ = new Expression($1, $2, $3, EXPRELATION);
                  $$->setLineNumber($1->getLineNumber());
                  }
            | expression relationop error %prec RE{ 
                  $$ = new ErrNode();
                  cerr << "Expected expression at " << yylval.token->line 
                  << ":" << yylval.token->column << endl << endl;
                  yyclearin;
                  yyerrok;
                  delete $1; delete $2; 
                  
                  }
            | expression productop expression %prec PRO{
                    $$ = new Expression($1, $2, $3, EXPPRODUCT);
                    $$->setLineNumber($1->getLineNumber());
                  }
            | expression productop error %prec RE{ 
                  $$ = new ErrNode();
                  cerr << "Expected expression at " << yylval.token->line 
                  << ":" << yylval.token->column << endl << endl;
                  yyerrok;
                  yyclearin;
                  delete $1; delete $2;
                  }
            | expression sumop expression %prec BIN {
                  $$ = new Expression($1, $2, $3, EXPSUMOP); 
                  $$->setLineNumber($1->getLineNumber());
                  }
            | expression sumop error %prec RE{ 
                  $$ = new ErrNode();
                  cerr << "Expected expression at " << yylval.token->line 
                  << ":" << yylval.token->column << endl << endl;
                  yyerrok;
                  yyclearin;
                  delete $1; delete $2;
                  }
            | LPAREN expression RPAREN { 
                  $$ = new Expression($2, EXPPAREN);
                  $$->setLineNumber($1->line);
                  delete $1; delete $3;
            }
            | LPAREN expression error { 
                  $$ = new ErrNode();
                  cerr << "Expected Right Parenthesis at "
                  << yylval.token->line << ":" << yylval.token->column 
                  <<endl << endl;
                  delete $1; delete $2;
                  yyerrok;
                  }
            | newexpression { 
                  $$ = new Expression($1, EXPNEW); 
                  $$->setLineNumber($1->getLineNumber());
                  }
            | name %prec NAME{
                  $$ = new Expression($1, EXPNAME);
                  $$->setLineNumber($1->getLineNumber());
                  }
            | name LPAREN arglist RPAREN {
                  $$ = new Expression($1, $3, EXPNAMEARG);
                  $$->setLineNumber($1->getLineNumber());
                  delete $2; delete $4;
                  }
            | name LPAREN RPAREN {
                    $$ = new Expression($1, EXPNAMEEMPTY);
                    $$->setLineNumber($1->getLineNumber());
                    delete $2; delete $3;
                  }
            | name LPAREN error RPAREN {
                  $$ = new ErrNode();
                  cerr << "Expected ArgList before ')' at "
                  << $4->line << ":" << $4->column <<endl << endl;
                  yyclearin;
                  yyerrok;
                  delete $1; delete $2; delete $4;
                  }
            | name LPAREN arglist error {
                  $$ = new ErrNode();
                  cerr << "Expected Right Parenthesis At "
                  << yylval.token->line << ":" << yylval.token->column 
                  <<endl << endl;
                  yyerrok;
                  delete $1; delete $2; delete $3;
                  }            
;
name: THIS { 
            $$ = new Name("this", NAMETHIS);
            $$->setLineNumber($1->line);
            delete $1;
            }
      | IDENTIFIER %prec NAME { 
            $$ = new Name($1->value, NAMEID);
            $$->setLineNumber($1->line);
            delete $1;
            }
      | name DOTOP IDENTIFIER { 
            $$ = new Name($1, $3->value, NAMEDOTID);
            $$->setLineNumber($1->getLineNumber());
            delete $2; delete $3;
            }
      | name DOTOP error{
            $$ = new ErrNode();
            cerr << "Expected Identifier After '.' at " << $2->line << ":" 
            << $2->column + 1<< endl << endl;
            yyclearin;
            yyerrok;
            delete $1; delete $2;
            }
      | error DOTOP IDENTIFIER {
            $$ = new ErrNode();
            cerr << "Expected name before '.' at " << $2->line << ":" 
            << $2->column << endl << endl;
            yyclearin;
            yyerrok;
            delete $2; delete $3;
            }
      | name LBRACK expression RBRACK { 
            $$ = new Name($1, $3, NAMEEXP);
            $$->setLineNumber($1->getLineNumber());
            delete $2; delete $4;
            }
      | IDENTIFIER LBRACK expression RBRACK{
            $$ = new Name($3, $1->value, NAMEIDEXP);
            $$->setLineNumber($1->line);
            delete $1; delete $2; delete $4;
            }
      | IDENTIFIER LBRACK expression error{
            $$ = new ErrNode();
            cerr << "Expected Right Bracket At " << yylval.token->line << ":" 
            << yylval.token->column << endl << endl;
            yyerrok;
            delete $1; delete $2; delete $3;
            }
      | name LBRACK expression error { 
            $$ = new ErrNode();
            cerr<< "Expected Right Bracket At " << yylval.token->line << ":"
            << yylval.token->column << endl << endl;
            yyerrok;
            delete $1; delete $2; delete $3;
            }
;
newexpression: NEW IDENTIFIER LPAREN arglist RPAREN {
                    $$ = new NewExpression($2->value, $4, NEWEXPARG);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2; delete $3; delete $5;
                    }
              | NEW IDENTIFIER LPAREN RPAREN { 
                    $$ = new NewExpression($2->value, NEWEXPPAREN);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2; delete $3; delete $4;
                    }
              | NEW IDENTIFIER LPAREN error { 
                    $$ = new ErrNode();
                    cerr << "Expected Right Parenthesis After '(' at " 
                    << $3->line << ":" << $3->column << endl << endl;
                    yyerrok;
                    delete $1; delete $2; delete $3;
                    }
              | NEW IDENTIFIER LPAREN arglist error {
                    $$ = new ErrNode();
                    cerr << "Expected Right Parenthesis At " << yylval.token->line 
                    << ":" << yylval.token->column << endl << endl;
                    yyerrok;
                    delete $1; delete $2; delete $3; delete $4;
                    }
/*              | NEW IDENTIFIER {
                    $$ = new NewExpression($2->value, NEWEXPEMPTY);
                    delete $1; delete $2;
                    }*/
              | NEW IDENTIFIER brackexpression {
                    $$ = new NewExpression($2->value, $3, NEWEXPBRACK);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2;
              }
              | NEW IDENTIFIER multibracks {
                $$ = new NewExpression($2->value, $3, NEWEXPMULTI);
                $$->setLineNumber($1->line);
                delete $1; delete $2;
              }
              | NEW IDENTIFIER brackexpression multibracks {
                    $$ = new NewExpression($2->value, $3, $4, NEWEXPBRACKMULTI);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2;
              }
/*              | NEW INT {
                $$ = new NewExpression(NEWEXPEMPTY);
                delete $1; delete $2;
              }*/
              | NEW INT brackexpression {
                    $$ =  new NewExpression($3,NEWEXPBRACK);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2;
              }
              | NEW INT brackexpression multibracks {
                    $$ =  new NewExpression($3, $4, NEWEXPBRACKMULTI);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2;
              }
              | NEW INT multibracks {
                    $$ = new NewExpression($3, NEWEXPMULTI);
                    $$->setLineNumber($1->line);
                    delete $1; delete $2;
              }
              | NEW error{ 
                    $$ = new ErrNode();
                    cerr << "After 'new' at " << $1->line << ":" 
                    << $1->column + 3 << endl << endl;
                    yyerrok;
                    delete $1;
              }
                  
;
brackexpression: LBRACK expression RBRACK { 
                        $$ = new RNode(RECBRACKEXP);
                        $$->setLineNumber($1->line);
                        ((RNode*)$$)->add($2);
                        delete $1; delete $3;
                        }                 
                  | brackexpression LBRACK expression RBRACK {
                        ((RNode*)$1)->add($3);
                        $$ = $1;
                        delete $2; delete $4;
                        }
                  | LBRACK expression error {
                        $$ = new ErrNode();
                        cerr << "Expected right Bracket at " 
                        << yylval.token->line << ":" << yylval.token->column 
                        << endl << endl;
                        yyerrok;
                        delete $1; delete $2;
                        }
                  | brackexpression LBRACK expression error {
                        $$ = new ErrNode();
                        cerr << "Expected right Bracket at " 
                        << yylval.token->line << ":" << yylval.token->column 
                        << endl << endl;
                        yyerrok;
                        delete $1; delete $2; delete $3;
                        }
;

arglist: expression { 
                  $$ = new RNode(RECARG);
                  $$->setLineNumber($1->getLineNumber());
                  ((RNode*)$$)->add($1);
                  }                 
         | arglist COMMA expression {
                  ((RNode*)$1)->add($3);
                  $$ = $1;
                  delete $2;
                  }
;
unaryop:  PLUS {$$ = new UnaryOp("+"); $$->setLineNumber($1->line); delete $1;}
| MINUS {$$ = new UnaryOp("-"); $$->setLineNumber($1->line); delete $1;}
| NOT {$$ = new UnaryOp("!"); $$->setLineNumber($1->line); delete $1;}

;

relationop: DOUBEQ {$$ = new RelationOp("=="); delete $1;}
| NOTEQ {$$ = new RelationOp("!="); delete $1;}
| LESSEQ {$$ = new RelationOp("<="); delete $1;}
| GREATEQ {$$ = new RelationOp(">="); delete $1;}
| LESS {$$ = new RelationOp("<"); delete $1;}
| GREAT {$$ = new RelationOp(">"); delete $1;}
;

productop:  TIMES {$$ = new ProductOp("*"); delete $1;}
| DIVIDE {$$ = new ProductOp("/"); delete $1;}
| MOD {$$ = new ProductOp("%"); delete $1;}
| DOUBAND {$$ = new ProductOp("&&"); delete $1;}
;
sumop:  MINUS {$$ = new SumOp("-"); delete $1;}
| PLUS {$$ = new SumOp("+"); delete $1;}
| DOUBBAR {$$ = new SumOp("||"); delete $1;}
;

type: INT {
            $$ = new TypeNode("int", TYPE);
            $$->setLineNumber($1->line);
            delete $1;
            }
      | IDENTIFIER {
            $$ = new TypeNode($1->value, TYPE);
            $$->setLineNumber($1->line);
            delete $1;
            }
      | INT multibracks {
            $$ = new TypeNode("int", $2, TYPEBRACKS);
            $$->setLineNumber($1->line);
            delete $1;
            }
      | IDENTIFIER multibracks {
            $$ = new TypeNode($1->value, $2, TYPEBRACKS);
            $$->setLineNumber($1->line);
            delete $1;
            }
;

multibracks: LBRACK RBRACK { 
                  $$ = new Multibracks();
                  ((Multibracks*)$$)->add();
                  $$->setLineNumber($1->line);
                  delete $1; delete $2;
                  }                 
            | multibracks LBRACK RBRACK {
                  ((Multibracks*)$1)->add();
                  $$ = $1;
                  delete $2; delete $3;
                  }              
            | LBRACK error {
                    $$ = new ErrNode();
                    cerr << "Expected Right Bracket at " << yylval.token->line 
                    << ":" << yylval.token->column <<endl << endl;
                    yyerrok;
                    delete $1;
                    }
              | multibracks LBRACK error {
                    $$ = new ErrNode();
                    cerr << "Expected Right Bracket at " << yylval.token->line 
                    << ":" << yylval.token->column <<endl << endl;
                    yyerrok;
                    delete $1; delete $2;
                    }
;

%%
