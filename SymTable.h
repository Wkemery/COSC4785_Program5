/*
 * SymTable.h
 * Author: Wyatt Emery
 * Date: NOV 2017
 *
 * COSC 4785, Homework5
 *
 */

#include<unordered_map>
#include<iostream>
#include<vector>
using namespace std;

class Type;

extern vector<Type*> extraTypes;

string nameMangle(string funcName, vector<string>* funcParamTypes);
string nameUnMangle(string mangledName);

class Type
{
private:
  string _lval;
  string _rval;
  vector<string>* _parameters;
  string _classType;
  string _value;
public:
  Type(string lval = "", string rval = "", vector<string>* parameters = 0, string classType = "", bool deleteME = false);
  ~Type();
  
  /*Accessor Functions*/
  string getlval(void) const;
  string getrval(void) const;
  vector<string>* getParams(void) const;
  string getClassType(void) const;
  void print(ostream* out) const;
  
  /*Modifier Functions*/
  void setInit(string val);
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
  /* parent is the parent of this symbol table,
   * value is the name associated with this table.
   */
  ~SymTable();
  
  /*Modifier Functions*/
  int addChild(SymTable* child);
  /* Insert child as a part of the children of this table. child should have a 
   * value defined for it
   * Return 0 for success, -1 for failure due to duplicate entry
   */
  int insert(string identifier, Type* type);
  /* Insert and entry into the entries for this symbol table, identifier is 
   * the key. type is the Type
   * Return 0 for success, -1 for failure due to duplicate entry
   */
  
  /*Accessor Functions*/
  Type* lookup(string identifier) const;
  /* look in the current symbol table and all the way up the root for identifier 
   * Return the Type of identfier, 0, if it DNE */
  Type* lookup(string className, string identifier, int linenum) const;
  /* Look in the symbol of className for the given identifier. linenum is for 
   * better error reporting and optional. 
   * Return the Type of identfier, 0 if it DNE*/
  Type* getClassType(void) const;
  /* Look up the symbol tables until you find the class that the calling 
   * symbol table is a part of. 
   * Return the Type of the class found, 0, if it DNE */
  bool classLookup(string identifier) const;
  /* Return true if the identifier is a class type in the symbol table tree,
   * false otherwise*/
  string getValue(void) const;
  /* return the private variable _value, which is the identfier associated with 
   * the symbol table, a random number for blocks.
   * This introduces a problem, that I am aware of. 
   * It is possible to get a name conflict if the same random number is chosen 
   * twice. However, the probability is incredibly small. and rerunning the 
   * program will "solve" that.
   */
  string findFunc(void) const;
  /* Similar to classLookup. 
   * look up the symbol tables until you find the function that the calling
   * symbol table is a child of.
   * Return the name of the function.
   * Should never return an error value, because all Statements MUST be in a 
   * function, syntactically
   */
  
  SymTable* getParent(void) const;
  /* Return the parent of this symbol Table. 
   */
  SymTable* lookupChild(string className) const;
  /* lookup up className in the current symbol table as a child
   * return the pointer to that child, 0, if it DNE
   */
  const SymTable* getRoot(void) const;
  /* look up the tree until you find the root.
   * Return a pointer to the root tree.
   */
  
  void print(ostream* out, int level);
  /* Print the symbol table, all it's entries and all its children
   */
};