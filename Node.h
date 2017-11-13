/*
 * Node.h
 * Author: Wyatt Emery
 * Date: NOV 3, 2017
 *
 * COSC 4785, Homework3
 *
 */

#include<vector>
#include<string>
#include<stack>
#include<unordered_map>
using namespace std;
#ifndef NODE_H
#define NODE_H

/* These are all for finding out the kind of node. It helps with printing*/
#define PLAIN 1000
#define EXPNUM 1001 /*Expression ->  */
#define EXPNULL 1002 /*Expression ->  */
#define EXPREAD 1003 /*Expression ->  */
#define EXPUNARY 1004 /*Expression ->  */
#define EXPRELATION 1005 /*Expression -> */
#define EXPPRODUCT 1006 /*Expression -> */
#define EXPSUMOP 1007 /* Expression ->  */
#define EXPPAREN 1008 /* Expression -> */
#define EXPNEW 1009 /* Expression -> */
#define EXPNAME 1010 /* Expression -> */
#define EXPNAMEARG 1011 /* Expression -> */
#define EXPNAMEEMPTY 1012

#define NAMETHIS 2001 /* Name -> */
#define NAMEID 2002 /* Name -> */
#define NAMEDOTID 2003 /* Name -> */
#define NAMEEXP 2004 /* Name -> */
#define NAMEIDEXP 2005 /* Name -> */

#define NEWEXPARG 3001 /* NewExpression -> */
#define NEWEXPBRACK 3002 /* NewExpression -> */
#define NEWEXPBRACKMULTI 3003 /* NewExpression -> */
#define NEWEXPMULTI 3004 /* NewExpression -> */
#define NEWEXPEMPTY 3005 /* NewExpression -> */
#define NEWEXPPAREN 3006

#define STMNTNAMEEXP 4001
#define STMNTNAMEARGL 4002
#define STMNTPRNTARGL 4003
#define STMNTWHILE 4004
#define SMTNTRETURN 4005
#define STMNTCOND 4006
#define STMNTBLOCK 4007
#define STMNTVARDEC 4008
#define STMNTSEMI 4010
#define STMNTNAMEEMPTY 4011

#define CONDSTMNT 5001
#define CONDSTMNTELSE 5002

#define BLOCKVARDEC 6001
#define BLOCKSTMNT 6002
#define BLOCKVARSTMNT 6003
#define BLOCKEMPTY 6004

#define RECSTMNT 7002
#define RECCONDEC 7004
#define RECVARDEC 7005
#define RECMETDEC 7006
#define RECPARAM 7008
#define RECARG 7009
#define RECBRACKEXP 7010

#define CLASSBODYEMPTY 8001
#define CLASSBODYVAR 8002
#define CLASSBODYVARCON 8003
#define CLASSBODYVARCONMET 8004
#define CLASSBODYCONMET 8005
#define CLASSBODYMET 8006
#define CLASSBODYCON 8007
#define CLASSBODYVARMET 8008

#define CONSTDEC 9001
#define CONSTDECEMPTY 9002

#define TYPEBRACKS 10001
#define TYPE 10002

#define METHODDECVOID 11001
#define METHODDECTYPE 11002
#define METHODDECTYPEEMPTY 11003
#define METHODDECVOIDEMPTY 11004

#define PARAMLDONE 12001
#define PARAMLIST 12002

// string nameMangle(string funcName, vector<string>* funcParamTypes);

class Type
{
private:
  string _lval;
  string _rval;
  vector<string>* _parameters;
  string _classType;
public:
  Type(string lval, string rval, vector<string>* parameters, string classType);
  string getlval(void) const;
  string getrval(void) const;
  vector<string>* getParams(void);
  string getClassType(void) const;
  void print(ostream* out);
};

class SymTable
{
private:
  SymTable* _parent;
  string _value;
  unordered_map<string, Type*> _entries;
  unordered_map<string, SymTable*> _children;
public:
  SymTable(SymTable* parent, string value);
  ~SymTable();
  int addChild(SymTable*);
  Type* lookup(string identifier) const;
  Type* lookup(string className, string identifier);
  bool classLookup(string identifier) const;
  int insert(string identifier, Type* type);
  string getValue(void) const;
  Type* getClassType() const;
  const SymTable* getRoot() const;
  SymTable* lookupChild(string className) const;
  string findFunc() const;
  void print(ostream* out, int level);
};

class Node
{
protected:
  Node(string value, string name, int kind);
  vector<Node*> _subNodes;
  const string _value;
  const string _nodeName;
  const int _kind;
  bool _err;
  string _type;
public:
  virtual ~Node();
  virtual void print(ostream *out) = 0;
  string getNodeName(void) const;
  int getNodeKind(void) const;
  virtual string getValue(void) const;
  virtual void buildTable(SymTable* table);
  virtual Type* getTypeCheck(SymTable* table);
  virtual bool typeCheck(SymTable* table);
  void setErr();
  bool getErr();
};

class ClassDec : public Node
{
public:
  ClassDec(string value, Node* node1);
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};

class ClassBody : public Node
{
public:
  ClassBody(int kind);
  ClassBody(Node* node1, int kind);
  ClassBody(Node* node1, Node* node2, int kind);
  ClassBody(Node* node1, Node* node2, Node* node3, int kind);  
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};



class Statement : public Node
{
private:
  SymTable* _myTable;
public:
  Statement(int kind);
  Statement(Node* node1, Node* node2, int kind);
  Statement(Node* node1, int kind);
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};

class Block : public Node
{
public:
  Block(int kind);
  Block(Node* node1, int kind);  
  Block(Node* node1, Node* node2, int kind); 
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};

class RNode : public Node
{
public:
  RNode(int kind);
  void add(Node* child);
  vector<string>* getParamTypes() const; //return the types of the paramlist
  vector<string>* getParamNames() const; // return the identifiers of the paramlist
  void buildTable(SymTable* table);
  void print(ostream* out);
  Type* getTypeCheck(SymTable* table);
  bool typeCheck(SymTable* table);
};

class CondStatement : public Node 
{
public:
  CondStatement(Node* node1, Node* node2, int kind);
  CondStatement(Node* node1, Node* node2, Node* node3, int kind);
  void buildTable(SymTable* table);
  Type* getTypeCheck(SymTable*);  
  void print(ostream* out);
};

class Name : public Node
{
public:
  Name(string value, int kind);
  Name(Node* name, string value, int kind);
  Name(Node* name, Node* expression, int kind);
  void print(ostream* out);
  Type* getTypeCheck(SymTable* table, string mangledName);
  string getFuncName();
};

class Expression : public Node
{
public:
  Expression(Node* next, int kind);
  Expression(string value, int kind);
  Expression(Node* unaryop, Node* expression, int kind);
  Expression(Node* expression1, Node* op, Node* expression2, int kind);
  Type* getTypeCheck(SymTable* table);
  void print(ostream* out);
};

class Param : public Node
{
public:
  Param(Node* node1, string value);
  void print(ostream* out);
  string getType() const;
  string getParamName() const;
};

class NewExpression : public Node
{
public:
  NewExpression(int kind);
  NewExpression(string value, int kind);
  NewExpression(Node* node1, int kind);
  NewExpression(Node* node1, Node* node2, int kind);
  NewExpression(string simpletype, Node* arglist, int kind);
  NewExpression(string simpletype, Node* type2 , Node* brackexp, int kind);
  Type* getTypeCheck(SymTable*);
  void print(ostream* out);
};

class ConstructorDec : public Node
{
public:
  ConstructorDec(string value, Node* node1, Node* node2, int kind);
  ConstructorDec(string value, Node* node1, int kind);
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};

class MethodDec : public Node
{
public:
  MethodDec(Node* node1, string value, Node* node2, Node* node3, int kind);
  MethodDec(Node* node1, string value, Node* node2, int kind);
  MethodDec(string value, Node* node1, Node* node2, int kind);
  MethodDec(string value, Node* node1, int kind);
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
private:
  string _resultType;
};

class VarDec: public Node
{
public:
  VarDec(Node* node1, string value);
  void buildTable(SymTable* table);
  bool typeCheck(SymTable* table);
  void print(ostream* out);
};

class TypeNode: public Node
{
public:
  TypeNode(string value, int kind);
  TypeNode(string value, Node* node1, int kind);
  void print(ostream* out);
  string getType();
};

class Multibracks: public Node
{
public:
  Multibracks();
  void add();
  void print(ostream* out);
  string getType(void);
private:
  int _count;
};

class UnaryOp : public Node
{
public:
  UnaryOp(string value);
  void print(ostream* out);
};

class RelationOp : public Node
{
public:
  RelationOp(string value);
  void print(ostream* out);
};

class ProductOp : public Node
{
public:
  ProductOp(string value);
  void print(ostream* out);
};

class SumOp : public Node
{
public:
  SumOp(string value);
  void print(ostream* out);
};

class ErrNode : public Node
{
public:
  ErrNode();
  void print(ostream* out);
};
#endif
