/*
 * Node.cpp
 * Author: Wyatt Emery
 * Date: NOV 3, 2017
 *
 * COSC 4785, Homework3
 *
 */

#include<iostream>
using namespace std;
#include"Node.h"
#include<cstdlib>

bool PDebug = false; //prints leaves on true
bool EasyReading = false; /* on true, adds extra new lines for easy reading of 
output*/
Type numLiteralType("", "int", 0, "");
Type nullType("", "NULL", 0, "");

string nameMangle(string funcName, vector<string>* funcParamTypes)
{
  funcName.insert(0, to_string(funcName.size())); //prepend size of funcname
  funcName.insert(0, "_Z"); //prepend _Z
  if(funcParamTypes == 0) return funcName;
  for(unsigned int i = 0; i < funcParamTypes->size(); i++)
  {
    funcName.append(to_string(funcParamTypes->at(i).size()));
    funcName.append(funcParamTypes->at(i));
  }
  return funcName;
}

string nameUnMangle(string mangledName)
{
  if(mangledName.substr(0, 2) != "_Z") return mangledName;
  mangledName.erase(0, 2); //erase _Z
  int pos = mangledName.find_first_not_of("01231456789");
  //function name starts at pos
  int funclength = stoi(mangledName.substr(0, pos));
  
  return mangledName.substr(pos, funclength);
}
/******************************************************************************/

Type::Type(string lval = "", string rval = "", vector<string>* parameters = 0, string classType = "")
:_lval(lval), _rval(rval), _parameters(parameters), _classType(classType)
{}

string Type::getlval(void) const{return _lval; }

string Type::getrval(void) const {return _rval;}

vector<string>* Type::getParams(void) { return _parameters;}

string Type::getClassType(void) const {return _classType; }

void Type::print(ostream* out) 
{ 
  if(_classType != "")
    *out << "Class Type";
  else
  {
    if(_rval == "" && _lval == "")
    {
      *out << "Const ";
    }
    else
    {
      if(_lval == "") *out << _rval << " ";
      else *out << _lval << " ";
    }

    if(_parameters != 0 && _parameters->size() > 0) 
    {
      *out << "<- ";
      for(unsigned int i = 0; i < _parameters->size(); i++)
      {
        *out << _parameters->at(i) << " ";
      }
    }
  }
}

/******************************************************************************/

SymTable::SymTable(SymTable* parent, string value):_parent(parent), _value(value)
{}

SymTable::~SymTable()
{
  for(auto it = _entries.begin(); it != _entries.end(); it++)
  {
    delete it->second;
  }
  for(auto it = _children.begin(); it!= _children.end(); it ++)
  {
    delete it->second;
  }
}

int SymTable::addChild(SymTable* child)
{
  auto it = _children.find(child->getValue());
  if(it != _children.end()) return -1;
  _children.insert(pair<string, SymTable*>(child->getValue(), child));
  return 0;
}

Type* SymTable::lookup(string identifier) const
{
  auto it = _entries.find(identifier);
  if((it == _entries.end()) && (_parent != 0)) return _parent->lookup(identifier);
  if(it == _entries.end()) return 0;
  else return it->second;
}

Type* SymTable::lookup(string className, string identifier)
{
  //lookup the id in the class symtable of className
  
  if(!this->classLookup(className))
  {
    //TODO: error
    return 0;
  }
  
  //the class does exist
  const SymTable* rootTable = this->getRoot();
  const SymTable* classTable = rootTable->lookupChild(className);
  if(classTable == 0)
  {
    //TODO: error
    return 0;
  }
  
  Type* idType = classTable->lookup(identifier);
  if(idType == 0)
  {
    //TODO: error
    return 0;
  }
  
  if(idType->getClassType() != "")
  {
    //TODO: error
    return 0;
  }
  
  return idType;
  
}

const SymTable* SymTable::getRoot() const
{
  if(_parent == 0)
    return this;
  else return _parent->getRoot();
}

SymTable* SymTable::lookupChild(string className) const
{
  auto it = _children.find(className);
  if(it == _children.end())
  {
    //TODO: error
    return 0;
  }
  return it->second;
}


bool SymTable::classLookup(string identifier) const
{
  
  if(_parent == 0)
  {
    if(this->lookup(identifier) != 0) return true;
  }
  else
  {
    return _parent->classLookup(identifier);
  }
  
  return false;
}

int SymTable::insert(string identifier, Type* type)
{
  auto it = _entries.find(identifier);
  if(it != _entries.end()) return -1;
  _entries.insert(pair<string, Type*>(identifier, type));
  return 0;
}

string SymTable::getValue(void) const {return _value;}

Type* SymTable::getClassType() const
{
  //rely on the fact that you are at a class type when the next node is root.
  if(_parent == 0)
  {
    return this->lookup(_value);
  }
  else
  {
    return _parent->getClassType();
  }
}

string SymTable::findFunc() const
{
  
  //rely on mangling, every func starts with _Z
  if(_value.substr(0, 2) != "_Z")
  {
    return _parent->findFunc();
  }
  return _value;
}

void SymTable::print(ostream* out, int level)
{
  
  for(auto it = _entries.begin(); it != _entries.end(); it++)
  {
    //print an entry. like foo class type
    string spaces = "";
    for(int i = 0; i < level; i++)
    {
      spaces.append("  ");
    }
    *out << spaces << nameUnMangle(it->first) << " ";
    it->second->print(out);
    *out << endl;

    //check to see if entry is a child as well then print it out
    auto it2 = _children.find(it->first);
    if(it2 != _children.end())
    {
      it2->second->print(out, level + 1);
      _children.erase(it2);
    }    
  }
  //print the rest of the children of this scope
  
  for(auto it2 = _children.begin(); it2!= _children.end(); it2++)
  {
    it2->second->print(out, level + 1);
  }
}

/******************************************************************************/

Node::Node(string value = "", string name = "", int kind = 0)
:_value(value), _nodeName(name), _kind(kind), _err(false)
{}
Node::~Node()
{
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    delete _subNodes[i];
  }
}
void Node::setErr() {
  _err = true;
}
bool Node::getErr() {return _err;}

int Node::getNodeKind(void) const {return _kind;}

string Node::getNodeName(void) const
{
  return _nodeName;
}

string Node::getValue(void) const {return _value;}

Type* Node::getTypeCheck(SymTable* table) {return 0;}

void Node::buildTable(SymTable* table) {return;}

bool Node::typeCheck(SymTable* table) {return true;}

/******************************************************************************/

ClassDec::ClassDec(string value, Node* node1):Node(value, "ClassDec")
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

void ClassDec::buildTable(SymTable* table)
{
  int ret;
  Type* mytype = new Type("", "", 0, _value);
  //add yourself to root table
  ret = table->insert(_value, mytype);
  if(ret == -1)
  {
    //TODO: error
  }
  
  //create a new symbol table - my symbol table 
  SymTable* myTable = new SymTable(table, _value);
  ret = table->addChild(myTable);
  if(ret == -1)
  {
    //TODO: error
  }
  //call build table on your child - the classbody
  _subNodes[0]->buildTable(myTable);
}

bool ClassDec::typeCheck(SymTable* table)
{
  return _subNodes[0]->typeCheck(table);
}

void ClassDec::print(ostream* out)
{
  if(_err) return;
  
  *out << "<ClassDec> --> class " << (PDebug ? _value : "ID") << " <ClassBody>" << endl;
  
  if(EasyReading) *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
  
  if(EasyReading) *out << endl;
}

/******************************************************************************/

ClassBody::ClassBody(int kind):Node("", "ClassBody", kind)
{}

ClassBody::ClassBody(Node* node1, int kind):Node("", "ClassBody", kind)
{
  _subNodes.push_back(node1);
}

ClassBody::ClassBody(Node* node1, Node* node2, int kind)
:Node("", "ClassBody", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
}

ClassBody::ClassBody(Node* node1, Node* node2, Node* node3, int kind)
:Node("", "ClassBody", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
  
  _subNodes.push_back(node3);
}

bool ClassBody::typeCheck(SymTable* table)
{
  bool ret = true;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    bool temp = _subNodes[i]->typeCheck(table);
    if(!temp) ret = false;
  }
  return ret;
}

void ClassBody::buildTable(SymTable* table) 
{
  // call build table on all children, which are Rnodes
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->buildTable(table);
  }
}

void ClassBody::print(ostream* out)
{
  if(_err) return;
  
  *out << "<ClassBody> --> {";
    
    switch(_kind)
    {
      case CLASSBODYEMPTY:
      {
        break;
      }
      case CLASSBODYVAR:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<VarDecs>";
        else *out << "<" << _subNodes[0]->getNodeName() << ">";
        break;
      }
      case CLASSBODYVARCON:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<VarDecs> ";
        else *out << "<" << _subNodes[0]->getNodeName() << "> ";
        
        if(_subNodes[1]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[1]->getNodeName()== "RNode") *out << "<ConstructorDecs>";
        else *out << "<" << _subNodes[1]->getNodeName() << ">";
        break;
      }
      case CLASSBODYVARCONMET:
      {
        
        if(_subNodes[0]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<VarDecs> ";
        else *out << "<" << _subNodes[0]->getNodeName() << "> ";
        
        if(_subNodes[1]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[1]->getNodeName()== "RNode") *out << "<ConstructorDecs> ";
        else *out << "<" << _subNodes[1]->getNodeName() << "> ";
        
        if(_subNodes[2]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[2]->getNodeName()== "RNode") *out << "<MethodDecs>";
        else *out << "<" << _subNodes[2]->getNodeName() << ">";
        
        break;
      }
      case CLASSBODYCONMET:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<ConstructorDecs> ";
        else *out << "<" << _subNodes[0]->getNodeName() << "> ";
        
        if(_subNodes[1]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[1]->getNodeName()== "RNode") *out << "<MethodDecs>";
        else *out << "<" << _subNodes[1]->getNodeName() << ">";
        
        break;
      }
      case CLASSBODYMET:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<MethodDecs>";
        else *out << "<" << _subNodes[0]->getNodeName() << ">";
        break;
      }
      case CLASSBODYCON:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<ConstructorDecs>";
        else *out << "<" << _subNodes[0]->getNodeName() << ">";
        break;
      }
      case CLASSBODYVARMET:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[0]->getNodeName()== "RNode") *out << "<VarDecs>";
        else *out << "<" << _subNodes[0]->getNodeName() << "> ";
        
        if(_subNodes[1]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[1]->getNodeName()== "RNode") *out << "<MethodDecs>";
        else *out << "<" << _subNodes[1]->getNodeName() << ">";
        
        break;
      }
      default:
      {
        cerr << "FATAL ERROR ClassBody" << endl;
        exit(1);
      }
    }
    
    *out << "}" << endl;
    for(unsigned int i = 0; i < _subNodes.size(); i++)
    {
      _subNodes[i]->print(out);
    }
}
/******************************************************************************/

Statement::Statement(Node* node1, Node* node2, int kind)
:Node("", "Statement", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
  
  _subNodes.push_back(node2);
  if(node2->getErr()) _err = true;
  
}
Statement::Statement(Node* node1, int kind):Node("", "Statement", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

Statement::Statement(int kind):Node("", "Statement", kind)
{}

void Statement::buildTable(SymTable* table)
{
  int ret;
  int childi = -1;
  if(_kind == STMNTWHILE) childi = 1;
  if(_kind == STMNTCOND) childi = 0;
  if(_kind == STMNTBLOCK) 
  {
    childi = 0;
    //create a new scope
    int randname = rand();
    
    string name = to_string(randname);
    
    SymTable* myTable = new SymTable(table, name);
    table->addChild(myTable);
    if(ret == -1)
    {
      //TODO: error
    }
    _subNodes[childi]->buildTable(myTable);
  }
  else
  {
    _myTable = table;
    if(childi != -1) _subNodes[childi]->buildTable(table);
  }
  
}

bool Statement::typeCheck(SymTable* table)
{
  switch(_kind)
  {
    case STMNTNAMEEXP:
    {
      //get type/type check  name 
      Type* nameType = ((Name*)_subNodes[0])->getTypeCheck(_myTable, "");
      if(nameType == 0) return false;
  
      //get type of expression
      Type* expType = _subNodes[1]->getTypeCheck(_myTable);
      if(expType == 0) return false;
      
      //compare types
      
      if(nameType->getlval() == "")
      {
        //TODO: error
        return false;
      }
      if(expType->getrval() == "") 
      {
        //TODO: error
        return false;
      }
      
      if(nameType->getlval() != expType->getrval())
      {
        //TODO: error
        return false;
      }
      //TODO: possible memory leak when expType is just a int that was just a number
      break;
    }
    case STMNTNAMEARGL:
    {
      //function call
      
      //get the types of the arguments 
      Type* argsType = _subNodes[1]->getTypeCheck(_myTable);
      if(argsType == 0) return false;
      
      //get type of function name from symbol table
      string mangledFuncName = nameMangle(((Name*)_subNodes[0])->getFuncName(), argsType->getParams());
      Type* funcType = ((Name*)_subNodes[0])->getTypeCheck(_myTable, mangledFuncName);
      if(funcType == 0) return false;
      
      //compare function type with arguments passed
      if(argsType->getParams()->size() != funcType->getParams()->size())
      {
        //TODO error
        return false;
      }
      
      //compare each type one by one
      for(unsigned int i = 0; i < funcType->getParams()->size(); i++)
      {
        if(funcType->getParams()->at(i) != argsType->getParams()->at(i))
        {
          //TODO error
          return false;
        }
      }
      
      delete argsType;
      
      break;
    }
    case STMNTPRNTARGL:
    {
      return true;
      break;
    }
    case STMNTWHILE:
    {
      //type check expression/get type of expression
      //type of exp must be int
      Type* expType = _subNodes[0]->getTypeCheck(_myTable);
      if(expType == 0) return false;
      if(expType->getrval() != "int")
      {
        //TODO: error
        return false;
      }
      
      //type check Statement
      _subNodes[1]->typeCheck(_myTable);
      break;
    }
    case SMTNTRETURN:
    {
      // get the function name from SymTable _value
      Type* funcType = table->lookup(table->findFunc());
      // still give blocks random names, but rely on mangling, every func starts with _Z
      
      //get type of expression
      Type* expType = _subNodes[0]->getTypeCheck(_myTable);
      if(expType == 0) return false;
      if(expType->getrval() != "int")
      {
        //TODO: error
        return false;
      }
      
      // check function ret type matches type of the expression
      if(funcType->getrval() != expType->getrval())
      {
        //TODO: error
        return false;
      }
      break;
    }
    case STMNTCOND:
    {
      //type check CondStatement
      return _subNodes[0]->typeCheck(_myTable);
    }
    case STMNTBLOCK:
    {
      //typecheck block
      return _subNodes[0]->typeCheck(_myTable);
    }
    case STMNTNAMEEMPTY:
    {
      //empty param list function call
      //get type of function name from symbol table
      string mangledFuncName = nameMangle(((Name*)_subNodes[0])->getFuncName(), 0);
      Type* funcType = ((Name*)_subNodes[0])->getTypeCheck(_myTable, mangledFuncName);
      if(funcType == 0) return false;
      break;
    }
  }
  
  return true;
}

void Statement::print(ostream* out)
{
  if(_err) return;
  
  *out << "<Statement> --> ";
  switch(_kind)
  {
    case STMNTNAMEEXP:
    {
      *out << "<Name> = <Expression> ;";
      break;
    }
    case STMNTNAMEARGL:
    {
      *out << "<Name> ";
      if(_subNodes.size() == 2) *out << "<Name> (<ArgList>) ;";
      else *out << "() ;";
      break;
    }
    case STMNTPRNTARGL:
    {
      *out << "print ";
      if(_subNodes.size() == 1 ) *out << "(<ArgList>) ;";
      else *out << "() ;";
      break;
    }
    case STMNTWHILE:
    {
      *out << "while (<Expression>) <Statement> ";
      break;
    }
    case SMTNTRETURN:
    {
      *out << "return ";
      if(_subNodes.size() == 1) *out << "<Expression> ";
      *out << ";";
      break;
    }
    case STMNTCOND:
    {
      *out << "<ConditionalStatement>";
      break;
    }
    case STMNTBLOCK:
    {
      *out << "<Block>";
      break;
    }
    case STMNTVARDEC:
    {
      *out << "<VarDec>";
      break;
    }
    case STMNTSEMI:
    {
      *out << ";";
      break;
    }
    case STMNTNAMEEMPTY:
    {
      *out << "<Name>()";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR!! Statement" << endl;
      exit(1);
    }
  }
  
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
  
  if(EasyReading) *out << endl << endl;
  
}
/******************************************************************************/
Block::Block(int kind):Node("", "Block", kind)
{}

Block::Block(Node* node1, int kind):Node("", "Block", kind)
{
  _subNodes.push_back(node1);
}

Block::Block(Node* node1, Node* node2, int kind):Node("", "Block", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
}
void Block::buildTable(SymTable* table)
{
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    // call buildTable on children, only up to 2 Rnodes
    _subNodes[i]->buildTable(table);
  }
}

bool Block::typeCheck(SymTable* table)
{
  //call typeCheck on all mychildren
  bool ret = true;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    bool temp = _subNodes[i]->typeCheck(table);
    if(!temp) ret = false;
  }
  return ret;
}

void Block::print(ostream* out)
{
  if(_err) return;
  
  *out << "<Block> --> {";
    switch(_kind)
    {
      case BLOCKVARDEC:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[0]->getNodeName()== "RNode") 
          *out << "<VarDecs>";
        else *out << " <" << _subNodes[0]->getNodeName() << ">";
        break;
      }
      case BLOCKSTMNT:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[0]->getNodeName()== "RNode") 
          *out << "<Statements>";
        else 
        {
          *out << "<" <<  _subNodes[0]->getNodeName() << ">";
        }
        break;
      }
      case BLOCKVARSTMNT:
      {
        if(_subNodes[0]->getErr()) *out << "<ErrNode> ";
        else if(_subNodes[0]->getNodeName()== "RNode") 
          *out << "<VarDecs>";
        else 
        {
          *out << "<" <<  _subNodes[0]->getNodeName() << "> ";
        }
        
        if(_subNodes[1]->getErr()) *out << "<ErrNode>";
        else if(_subNodes[1]->getNodeName()== "RNode") 
          *out << "<Statements>";
        else 
        {
          *out << "<" <<  _subNodes[1]->getNodeName() << ">";
        }
        
        break;
      }
      case BLOCKEMPTY:
      {
        break;
      }
      default:
      {
        cerr << "FATAL ERROR Block" <<endl;
        exit(1);
      }
    }
    
    *out << "}" << endl;
    for(unsigned int i = 0; i < _subNodes.size(); i++)
    {
      _subNodes[i]->print(out);
    }
}

/******************************************************************************/

RNode::RNode(int kind) : Node("", "RNode", kind) {}

void RNode::add(Node* child)
{
  _subNodes.push_back(child);
}

void RNode::buildTable(SymTable* table)
{
  //call build table on all my children
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->buildTable(table);
  }
}

vector<string>* RNode::getParamTypes() const
{
  vector<string>* ret = new vector<string>;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    ret->push_back(((Param*)_subNodes[i])->getType());
  }
  
  return ret;
}

vector<string>* RNode::getParamNames() const
{
  vector<string>* ret = new vector<string>;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    ret->push_back(((Param*)_subNodes[i])->getParamName());
    
  }
  return ret;
}

Type* RNode::getTypeCheck(SymTable* table)
{

    if(_kind == RECARG)
    {
      //get all arg types
      vector<string>* argTypes = new vector<string>();
      for(unsigned int i = 0; i < _subNodes.size(); i++)
      {
        argTypes->push_back(_subNodes[i]->getTypeCheck(table)->getrval());
      }
      return new Type("","",argTypes);
    }
    if(_kind == RECBRACKEXP)
    {
//       *out << "<BracketedExpression> --> " << _subNodes.size() << " <Expression>" 
//       << endl;
    }
    return 0;
  }


bool RNode::typeCheck(SymTable* table)
{
  //call typeCheck on all mychildren
  bool ret = true;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    bool temp = _subNodes[i]->typeCheck(table);
    if(!temp) ret = 0;
  }
  return ret;
}

void RNode::print(ostream* out)
{
  switch(_kind)
  {
    case RECVARDEC:
    {
      *out << "<VarDecs> --> " << _subNodes.size() << " <VarDec>" << endl;
      break;
    }
    case RECSTMNT:
    {
      *out << "<Statements> --> " << _subNodes.size() << " <Statement>" << endl;
      break;
    }
    case RECCONDEC:
    {
      *out << "<ConstructorDecs> --> " << _subNodes.size() << " <ConstructorDec>" << endl;
      break;
    }
    case RECMETDEC:
    {
      *out << "<MethodDecs> --> " << _subNodes.size() << " <MethodDec>" << endl;
      break;
    }
    case RECPARAM:
    {
      *out << "<ParamList> --> " << _subNodes.size() << " <Param>" << endl; 
      break;
    }
    case RECARG:
    {
      *out << "<ArgList> --> " << _subNodes.size() << " <Expression>" << endl;
      break;
    }
    case RECBRACKEXP:
    {
      *out << "<BracketedExpression> --> " << _subNodes.size() << " <Expression>" 
      << endl;
      break;
    }
    default:
    {
      cerr << "Fatal Error RNode!" << endl;
      exit(1);
    }
  }
  
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
  if(EasyReading) *out << endl;
  
}
/******************************************************************************/

CondStatement::CondStatement(Node* node1, Node* node2, int kind)
:Node("", "CondStatement", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
  
  _subNodes.push_back(node2);
  if(node2->getErr()) _err = true;
}

CondStatement::CondStatement(Node* node1, Node* node2, Node* node3, int kind)
:Node("", "CondStatement", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
  
  _subNodes.push_back(node2);
  if(node2->getErr()) _err = true;
  
  _subNodes.push_back(node3);
  if(node3->getErr()) _err = true;
}

void CondStatement::buildTable(SymTable* table)
{
  for(unsigned int i = 1; i < _subNodes.size(); i++)
  {
    _subNodes[i]->buildTable(table);
  }
} 

bool CondStatement::typeCheck(SymTable* table)
{
  //type check/get type of expression child
  Type* expType = _subNodes[0]->getTypeCheck(table);
  if(expType == 0) return false;
  
  //type check Statement child/children
  bool stmnt1ret = _subNodes[1]->typeCheck(table);
  if(!stmnt1ret) return false;
  
  if(_kind == CONDSTMNTELSE)
  {
    bool stmnt2ret = _subNodes[2]->typeCheck(table);
    if(!stmnt2ret) return false;
  }
    
  return true;
}

void CondStatement::print(ostream* out)
{
  if(_err) return;
  
  *out << "<ConditionalStatement> --> ";
  switch(_kind)
  {
    case CONDSTMNT:
    {
      *out << "if (<Expression>) <Statement>";
      break;
    }
    case CONDSTMNTELSE:
    {
      *out << "if (<Expression>) <Statement> else <Statement>";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR!! CondStatement" << endl;
      exit(1);
    }
  }
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
}

/******************************************************************************/
Name::Name(string value, int kind):Node(value, "Name", kind)
{}
Name::Name(Node* name, string value, int kind):Node(value, "Name", kind)
{
  _subNodes.push_back(name);
  if(name->getErr()) _err = true;
  
}

Name::Name(Node* name, Node* expression, int kind):Node("", "Name", kind)
{
  _subNodes.push_back(name);
  if(name->getErr()) _err = true;
  
  _subNodes.push_back(expression);
  if(expression->getErr()) _err = true;
  
}

string Name::getFuncName()
{
  if(_kind == NAMEDOTID || _kind == NAMEID)
    return _value;
  return "";
}

Type* Name::getTypeCheck(SymTable* table, string mangledName = "")
{
  switch(_kind)
  {  
    case NAMETHIS:
    {
      return table->getClassType();
    }
    case NAMEID:
    {
      if(mangledName == "") 
        return table->lookup(_value);
      else
        return table->lookup(mangledName);
    }
    case NAMEDOTID:
    {
      Type* nameType = ((Name*)_subNodes[0])->getTypeCheck(table, "");
      if(nameType == 0) return 0;
      
      if(nameType->getlval() == "")
      {
        //TODO error
        return 0;
      }
      if(mangledName == "") 
        return table->lookup(nameType->getlval(), _value);
      else
        return table->lookup(nameType->getlval(), mangledName);
      break;
    }
    case NAMEEXP:
    {
      //TODO: something like x.y.z[2] = 5 or x = x.y.z[2]
//       *out << "<Name> [<Expression>]";
      break;
    }
    case NAMEIDEXP:
    {
      //TODO: something like z[2] = 5 or x = z[2];
//       *out << (PDebug ? _value : "ID") << " [<Expression>]";
      break;
    }
  }
  
  return 0;
}

void Name::print(ostream* out)
{
  if(_err) return;
  
  *out << "<Name> --> ";
  switch(_kind)
  {  
    case NAMETHIS:
    {
      *out << _value;
      break;
    }
    case NAMEID:
    {
      *out << (PDebug ? _value : "ID");
      break;
    }
    case NAMEDOTID:
    {
      *out << "<Name>." << (PDebug ? _value : "ID");
      break;
    }
    case NAMEEXP:
    {
      *out << "<Name> [<Expression>]";
      break;
    }
    case NAMEIDEXP:
    {
      *out << (PDebug ? _value : "ID") << " [<Expression>]";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR!! NAME" << endl;
      exit(1);
    }
  }
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
  
}
/******************************************************************************/

Expression::Expression(Node* next, int kind):Node("", "Expression", kind)
{
  _subNodes.push_back(next);
  if(next->getErr()) _err = true;
  
}

Expression::Expression(string value, int kind):Node(value, "Expression", kind)
{}

Expression::Expression(Node* unaryop, Node* expression, int kind)
:Node("", "Expression", kind)
{
  _subNodes.push_back(unaryop);
  if(unaryop->getErr()) _err = true;
  

  _subNodes.push_back(expression);
  if(expression->getErr()) _err = true;
  
}

Expression::Expression(Node* expression1, Node* op, Node* expression2, int kind)
:Node("", "Expression", kind)
{
  _subNodes.push_back(expression1);
  if(expression1->getErr()) _err = true;
  
  _subNodes.push_back(op);
  if(op->getErr()) _err = true;
  
  _subNodes.push_back(expression2);
  if(expression2->getErr()) _err = true;
  
}

Type* Expression::getTypeCheck(SymTable* table)
{
  switch(_kind)
  {
    case EXPNUM:
    {
      return &numLiteralType;
    }
    case EXPNULL:
    {
      return &nullType;
    }
    case EXPREAD:
    {
      return &numLiteralType;
    }
    case EXPUNARY:
    {
      Type* expType = _subNodes[1]->getTypeCheck(table);
      if(expType == 0) return 0;
      
      return expType;
    }
    case EXPRELATION:
    case EXPPRODUCT:
    case EXPSUMOP:
    {
      Type* expType1 = _subNodes[0]->getTypeCheck(table);
      if(expType1 == 0) return 0;
      
      Type* expType2 = _subNodes[2]->getTypeCheck(table);
      
      if(expType2 == 0) return 0;
      
      if(expType1->getrval() != expType2->getrval())
      {
        //TODO: error
        return 0;
      }
      
      return expType1;
    }
    case EXPPAREN:
    {
      return _subNodes[0]->getTypeCheck(table);
    }
    case EXPNEW:
    {
      return _subNodes[0]->getTypeCheck(table);
    }
    case EXPNAME:
    {
      return ((Name*)_subNodes[0])->getTypeCheck(table, "");
    }
    case EXPNAMEARG:
    {
      //function call
      
      //get the types of the arguments 
      Type* argsType = _subNodes[1]->getTypeCheck(table);
      if(argsType == 0) return 0;
      
      //get type of function name from symbol table
      string mangledFuncName = nameMangle(((Name*)_subNodes[0])->getFuncName(), argsType->getParams());
      Type* funcType = ((Name*)_subNodes[0])->getTypeCheck(table, mangledFuncName);
      if(funcType == 0) return 0;
      
      //compare function type with arguments passed
      if(argsType->getParams()->size() != funcType->getParams()->size())
      {
        //TODO error
        return 0;
      }
      
      //compare each type one by one
      for(unsigned int i = 0; i < funcType->getParams()->size(); i++)
      {
        if(funcType->getParams()->at(i) != argsType->getParams()->at(i))
        {
          //TODO error
          return 0;
        }
      }
      
      delete argsType;
      
      return funcType;
    }
    case EXPNAMEEMPTY:
    {
      //empty param list function call
      //get type of function name from symbol table
      string mangledFuncName = nameMangle(((Name*)_subNodes[0])->getFuncName(), 0);
      Type* funcType = ((Name*)_subNodes[0])->getTypeCheck(table, mangledFuncName);
      if(funcType == 0) return 0;
      
      return funcType;
    }
  }
  return 0;
}

void Expression::print(ostream* out)
{
  if(_err) return;
  
  *out << "<Expression> --> ";
  switch(_kind)
  {
    case EXPNUM:
    {
      *out << (PDebug ? _value : "NUM");
      break;
    }
    case EXPNULL:
    {
      *out << _value;
      break;
    }
    case EXPREAD:
    {
      *out << _value;
      break;
    }
    case EXPUNARY:
    {
      *out << "<UnaryOp> <Expression>";
      break;
    }
    case EXPRELATION:
    {
      *out << "<Expression> <RelationOp> <Expression>";
      break;
    }
    case EXPPRODUCT:
    {
      *out << "<Expression> <ProductOp> <Expression>";
      break;
    }
    case EXPSUMOP:
    {
      *out << "<Expression> <SumOp> <Expression>";
      break;
    }
    case EXPPAREN:
    {
      *out << "(<Expression>)";
      break;
    }
    case EXPNEW:
    {
      *out << "<NewExpression>";
      break;
    }
    case EXPNAME:
    {
      *out << "<Name>";
      break;
    }
    case EXPNAMEARG:
    {
      *out << "<Name>(<ArgList>)";
      break;
    }
    case EXPNAMEEMPTY:
    {
      *out << "<Name>()";
      break;
    }
    default:
      cerr << "FATAL ERROR!!" << endl;
      exit(1);
  }
  *out << endl;
  
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
  
  if(EasyReading) *out << endl;
}

/******************************************************************************/

Param::Param(Node* node1, string value)
:Node(value, "Param")
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

string Param::getType() const
{
  return ((TypeNode*)_subNodes[0])->getType();
}

string Param::getParamName() const { return _value; }

void Param::print(ostream* out)
{
  if(_err) return;
  
  *out << "<Param> --> " << " <TypeNode> " << (PDebug ? _value : "ID");
  
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
  }
}
/******************************************************************************/

NewExpression::NewExpression(int kind):Node("int", "NewExpression", kind) {}
NewExpression::NewExpression(string value, int kind)
:Node(value, "NewExpression", kind) {}

NewExpression::NewExpression(Node* node1, int kind)
:Node("int", "NewExpression", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

NewExpression::NewExpression(Node* node1, Node* node2, int kind)
:Node("int", "NewExpression", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
  
  _subNodes.push_back(node2);
  if(node2->getErr()) _err = true;
}

NewExpression::NewExpression(string simpletype, Node* arglist, int kind)
:Node(simpletype, "NewExpression", kind)
{
  _subNodes.push_back(arglist);
  if(arglist->getErr()) _err = true;
  
}

NewExpression::NewExpression(string simpletype, Node* type2, Node* brackexp, int kind)
:Node(simpletype, "NewExpression", kind)
{
  _subNodes.push_back(type2);
  if(type2->getErr()) _err = true;
  
  _subNodes.push_back(brackexp);
  if(brackexp->getErr()) _err = true;
}

Type* NewExpression::getTypeCheck(SymTable* table)
{
  switch(_kind)
  {
    case NEWEXPARG:
    {
      //constructor call
      
      //doesn't make sense for int, int has no constructor
      if(_value == "int")
      {
        //TODO: error
        return 0;
      }
      
      //check to see if type even exists
      if(!table->classLookup(_value))
      {
        //TODO: error
        return 0;
      }
      
      //type exists, check to see call matches a Constructor in the class
      
      //get the types of the arguments 
      Type* argsType = _subNodes[0]->getTypeCheck(table);
      if(argsType == 0) return 0;
      
      //get type of constructor from symbol table
      string mangledConsName = nameMangle(_value, argsType->getParams());
      Type* consType = table->lookup(_value, mangledConsName);
      if(consType == 0) return 0;
      
      //compare function type with arguments passed
      if(argsType->getParams()->size() != consType->getParams()->size())
      {
        //TODO error
        return 0;
      }
      
      //compare each type one by one
      for(unsigned int i = 0; i < consType->getParams()->size(); i++)
      {
        if(consType->getParams()->at(i) != argsType->getParams()->at(i))
        {
          //TODO error
          return 0;
        }
      }
      
      delete argsType;
      
      return consType;
    }
    case NEWEXPBRACK:
    {
      //TODO: find out if expression must evaluate to int, x = new A[expression]
//       *out << "<[Expression]>";
      
      break;
    }
    case NEWEXPBRACKMULTI:
    {
      //TODO: what the hell does new A[5][] even mean? or new A[5][][], and what does it mean to match the type of the lhs?
//       *out << "[<BracketedExpression>] <RecursiveBrackets>";
      break;
    }
    case NEWEXPMULTI:
    {
      //TODO: what does new A[][] mean?
//       *out << "<RecursiveBrackets>";
      break;
    }
    case NEWEXPEMPTY:
    {
      //TODO: ask if x = new A; is allowed. or just x = new int;
      break;
    }
    case NEWEXPPAREN:
    {
      //default constructor call
      
      //doesn't make sense for int, int has no constructor
      if(_value == "int")
      {
        //TODO: error
        return 0;
      }
      
      //check to see if type even exists
      if(!table->classLookup(_value))
      {
        //TODO: error
        return 0;
      }
      
      //type exists, check to see call matches a Constructor in the class
      
      //no arguments
      
      //get type of constructor from symbol table
      string mangledConsName = nameMangle(_value, 0);
      Type* consType = table->lookup(_value, mangledConsName);
      if(consType == 0) return 0;
      
      
      return consType;
    }
  }
  return 0;
}

void NewExpression::print(ostream* out)
{
  if(_err) return;
  
  if(_value == "int") *out << "<NewExpression> --> new int ";
  else *out << "<NewExpression> --> new " << (PDebug ? _value : "ID") << " ";
  
  switch(_kind)
  {
    case NEWEXPARG:
    {
      *out << "(<ArgList>)";
      break;
    }
    case NEWEXPBRACK:
    {
      *out << "<[Expression]>";
      break;
    }
    case NEWEXPBRACKMULTI:
    {
      *out << "[<BracketedExpression>] <RecursiveBrackets>";
      break;
    }
    case NEWEXPMULTI:
    {
      *out << "<RecursiveBrackets>";
      break;
    }
    case NEWEXPEMPTY:
    {
      break;
    }
    case NEWEXPPAREN:
    {
      *out << "()";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR!!! NewExpression" << endl;
      exit(1);
    }
  }
  
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
    
  }
}


/******************************************************************************/

ConstructorDec::ConstructorDec(string value, Node* node1, Node* node2, int kind)
:Node(value, "ConstructorDec", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
}

ConstructorDec::ConstructorDec(string value, Node* node1, int kind)
:Node(value, "ConstructorDec", kind)
{
  _subNodes.push_back(node1);
}

void ConstructorDec::buildTable(SymTable* table)
{
  // get parameter types from param list child
  int ret;
  vector<string>* paramTypes = 0;
  vector<string>* paramNames = 0;
  string myRetType;
  
  int paramChildi = -1;
  int blockChildi = -1;
  
  if(table->getValue() != _value)
  {
    cerr << "Constructor name not equal to class name" << endl;
  }
  
  if(_kind == CONSTDEC)
  {
    paramChildi = 0;
    blockChildi = 1;
  }
  if(_kind == CONSTDECEMPTY)
  {
    blockChildi = 0;
  }
  
  if(paramChildi >= 0 )
  {
    paramTypes = ((RNode*)_subNodes[paramChildi])->getParamTypes();
    paramNames = ((RNode*)_subNodes[paramChildi])->getParamNames();
  }
  
  //create mytype
  Type* myType = new Type("", "", paramTypes);
  ret = table->insert(nameMangle(_value, paramTypes), myType);
  if(ret == -1)
  {
    //TODO: error
  }
  //create my symbol table
  SymTable* myTable = new SymTable(table, nameMangle(_value, paramTypes));
  ret = table->addChild(myTable);
  if(ret == -1)
  {
    //TODO: error
  }
  //add my paramters to my table for the code in the block, if I have any
  if(paramChildi >= 0)
  {
    for(unsigned int i = 0; i < paramTypes->size(); i++)
    {
      Type* paramType = new Type(paramTypes->at(i), paramTypes->at(i));
      ret = myTable->insert(paramNames->at(i), paramType);
      if(ret == -1)
      {
        //TODO: error
      }
    }
  }
  
  //call buildTable on my child - the block
  _subNodes[blockChildi]->buildTable(myTable);
  
  delete paramNames;
}

bool ConstructorDec::typeCheck(SymTable* table)
{
  int blockChildi = -1;
  int paramChildi = -1;
  
  if(_kind == CONSTDEC)
  {
    blockChildi = 1;
    paramChildi = 0;
  }
  if(_kind == CONSTDECEMPTY)
  {
    blockChildi = 0;
  }
  
  
  if(paramChildi != -1)
  {
    //validate paramter types
    vector<string>* paramTypes = ((RNode*)_subNodes[paramChildi])->getParamTypes();
    for(unsigned int i = 0; i < paramTypes->size(); i++)
    {
      string type = paramTypes->at(i);
      
      //remove any []
      unsigned int arrayPos = type.find_first_of("[");
      if(arrayPos != string::npos)
        type = type.substr(0,arrayPos);
      
      if(type != "int")
      {
        if(!table->classLookup(type))
        {
          //TODO: error
          return false;
        }
      }
    }
    
    delete paramTypes;
  }
  
  return _subNodes[blockChildi]->typeCheck(table);
  
}

void ConstructorDec::print(ostream* out)
{
  if(_err) return;
  
  *out << "<ConstructorDec> --> " << (PDebug ? _value : "ID");
  
  switch(_kind)
  {
    case CONSTDEC:
    {
      *out << "(<ParamList>)";
      break;
    }
    case CONSTDECEMPTY:
    {
      *out << "()";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR ConstructorDec" << endl;
      exit(1);
    }
  }
  
  if(_subNodes.size() > 0) 
    *out << " <" << _subNodes[_subNodes.size() - 1]->getNodeName() << ">" << endl;
  
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
    
  }
  if(EasyReading) *out << endl;
}

/******************************************************************************/

MethodDec::MethodDec(Node* node1, string value, Node* node2, Node* node3, int kind)
:Node(value, "MethodDec", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
  
  _subNodes.push_back(node3);
}

MethodDec::MethodDec(Node* node1, string value, Node* node2, int kind)
:Node(value, "MethodDec", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
}

MethodDec::MethodDec(string value, Node* node1, Node* node2, int kind)
:Node(value, "MethodDec", kind)
{
  _subNodes.push_back(node1);
  
  _subNodes.push_back(node2);
}

MethodDec::MethodDec(string value, Node* node1, int kind)
:Node(value, "MethodDec", kind)
{
  _subNodes.push_back(node1);      
}

void MethodDec::buildTable(SymTable* table)
{
  // get parameter types from param list child
  vector<string>* paramTypes = 0;
  vector<string>* paramNames = 0;
  string myRetType;
  int ret;

  int paramChildi = -1;
  int typeChildi = -1;
  int blockChildi = -1;
  switch(_kind)
  {
    case METHODDECVOID:
    {
      paramChildi = 0;
      blockChildi = 1;
      break;
    }
    case METHODDECTYPE:
    {
      typeChildi = 0;
      paramChildi = 1;
      blockChildi = 2;
      break;
    }
    case METHODDECTYPEEMPTY:
    {
      typeChildi = 0;
      blockChildi = 1;
      break;
    }
    case METHODDECVOIDEMPTY:
    {
      blockChildi = 0;
      break; 
    }
  }
  
  if(paramChildi >= 0 )
  {
    paramTypes = ((RNode*)_subNodes[paramChildi])->getParamTypes();
    paramNames = ((RNode*)_subNodes[paramChildi])->getParamNames();
  }
  
  if(typeChildi >= 0)
    myRetType = ((TypeNode*)_subNodes[0])->getType();
  else myRetType = "void";
  
  
  //create mytype
  Type* myType = new Type("", myRetType, paramTypes);
  ret = table->insert(nameMangle(_value, paramTypes), myType);
  if(ret == -1)
  {
    //TODO: error
  }
  
  //create my symbol table
  SymTable* myTable = new SymTable(table, nameMangle(_value, paramTypes));
  ret = table->addChild(myTable);
  if(ret == -1)
  {
    //TODO: error
  }
  //add my paramters to my table for the code in the block, if I have any
  if(paramChildi >= 0)
  {
    for(unsigned int i = 0; i < paramTypes->size(); i++)
    {
      Type* paramType = new Type(paramTypes->at(i), paramTypes->at(i));
      ret = myTable->insert(paramNames->at(i), paramType);
      if(ret == -1)
      {
        //TODO: error
      }
    }
  }
  
  //call buildTable on my child - the block
  _subNodes[blockChildi]->buildTable(myTable);
  
  delete paramNames;
  
}

bool MethodDec::typeCheck(SymTable* table)
{
  int typeChildi = -1;
  int paramChildi = -1;
  int blockChildi = -1;
  switch(_kind)
  {
    case METHODDECVOID:
    {
      paramChildi = 0;
      blockChildi = 1;
      break;
    }
    case METHODDECTYPE:
    {
      typeChildi = 0;
      paramChildi = 1;
      blockChildi = 2;
      break;
    }
    case METHODDECTYPEEMPTY:
    {
      typeChildi = 0;
      blockChildi = 1;
      break;
    }
    case METHODDECVOIDEMPTY:
    {
      blockChildi = 0;
      break; 
    }
  }
  
  if(typeChildi!= -1)
  {
    // need to validate type
    //get my type from my child typenode
    string type = ((TypeNode*)_subNodes[typeChildi])->getType();
    
    //remove any []
    unsigned int arrayPos = type.find_first_of("[");
    if(arrayPos != string::npos)
      type = type.substr(0, arrayPos);
    
    if(type != "int")
    {
      if(!table->classLookup(type))
      {
        //TODO: error
        return false;
      }
    }
  }
  
  if(paramChildi != -1)
  {
    //validate paramter types
    vector<string>* paramTypes = ((RNode*)_subNodes[paramChildi])->getParamTypes();
    for(unsigned int i = 0; i < paramTypes->size(); i++)
    {
      string type = paramTypes->at(i);
      
      //remove any []
      unsigned int arrayPos = type.find_first_of("[");
      if(arrayPos != string::npos)
        type = type.substr(0, arrayPos);
      
      if(type != "int")
      {
        if(!table->classLookup(type))
        {
          //TODO: error
          return false;
        }
      }
    }
    
    delete paramTypes;
  }
  
  return _subNodes[blockChildi]->typeCheck(table);
}

void MethodDec::print(ostream* out)
{
  if(_err) return;
  
  *out << "<MethodDec> --> "; 
  switch(_kind)
  {
    case METHODDECVOID:
    {
      *out << "void " << (PDebug ? _value : "ID") << " (<ParameterList>) "; 
      break;
    }
    case METHODDECTYPE:
    {
      *out << "<TypeNode> " << (PDebug ? _value : "ID") << " (<ParameterList>) ";
      break;
    }
    case METHODDECTYPEEMPTY:
    {
      *out << "<TypeNode> " << (PDebug ? _value : "ID") << " () ";
      break;
    }
    case METHODDECVOIDEMPTY:
    {
      *out << "void " << (PDebug ? _value : "ID") << " () ";
      break;
    }
    default:
    {
      cerr << "FATAL ERROR MethodDec" << endl;
      exit(1);
    }
  }
  if(_subNodes.size() > 0) 
    *out << "<" << _subNodes[_subNodes.size() - 1]->getNodeName() << ">" << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
    
  }
  if(EasyReading) *out << endl;
}

/******************************************************************************/

VarDec::VarDec(Node* node1, string value): Node(value, "VarDec")
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

void VarDec::buildTable(SymTable* table)
{
  int ret;
  //get my type from my child typenode
  string type = ((TypeNode*)_subNodes[0])->getType();
  //create mytype
  Type* mytype = new Type(type, type);
  // add myself to symbol table
  ret = table->insert(_value, mytype);
  if(ret == -1) 
  {
    //TODO: error
    cerr << "Var declared twice" << endl;
  }
}

bool VarDec::typeCheck(SymTable* table)
{
  // need to validate type
  //get my type from my child typenode
  string type = ((TypeNode*)_subNodes[0])->getType();
  
  //remove any []
  unsigned int arrayPos = type.find_first_of("[");
  if(arrayPos != string::npos)
    type = type.substr(0, arrayPos);
  
  if(type != "int")
  {
    if(!table->classLookup(type))
    {
      //TODO: error
      return false;
    }
  }
  return true;
}

void VarDec::print(ostream* out)
{
  if(_err) return;
  
  *out << "<VarDec> --> <TypeNode> " << (PDebug ? _value : "ID") << " ;" << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
    
  }
}

/******************************************************************************/

TypeNode::TypeNode(string value, int kind):Node(value, "TypeNode", kind)
{}

TypeNode::TypeNode(string value, Node* node1, int kind):Node(value, "TypeNode", kind)
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
}

string TypeNode::getType()
{
  string mytype = "";
  mytype.append(_value);
  string temp;
  if(_subNodes.size() > 0) temp = ((Multibracks*)_subNodes[0])->getType();
  mytype.append(temp);
  return mytype;
}

void TypeNode::print(ostream* out)
{
  if(_err) return;
  
  *out << "<TypeNode> --> ";
  switch(_kind)
  {
    case TYPE:
    {
      *out << (PDebug ? _value : "ID");
      break;
    }
    case TYPEBRACKS:
    {
      *out << (PDebug ? _value : "ID") << " <RecursiveBrackets>"; 
      break;
    }
    default:
    {
      cerr << "FATAL ERROR Type" << endl;
      exit(1);
    }
  }
  
  *out << endl;
  for(unsigned int i = 0; i < _subNodes.size(); i++)
  {
    _subNodes[i]->print(out);
    
  }
}
/******************************************************************************/

Multibracks::Multibracks():Node("", "Multibracks")
{}

void Multibracks::add()
{
  _count++;
}

string Multibracks::getType(void)
{
  string ret;
  for(int i = 0; i < _count; i++)
  {
    ret.append("[]");
  }
  return ret;
}

void Multibracks::print(ostream* out)
{
  if(_err) return;
  
  *out << "<RecursiveBrackets> --> ";
  for(int i = 0; i < _count; i++)
  {
    *out << "[]";
  }
  * out << endl;
}


/******************************************************************************/

UnaryOp::UnaryOp(string value):Node(value, "UnaryOp")
{}
void UnaryOp::print(ostream* out)
{
  if(_err) return;
  
  *out << "<UnaryOp> --> " << _value << endl;
}


/******************************************************************************/

RelationOp::RelationOp(string value):Node(value, "RelationOp")
{}
void RelationOp::print(ostream* out)
{
  if(_err) return;
  
  *out << "<RelationOp> --> " << _value << endl;
}

/******************************************************************************/

ProductOp::ProductOp(string value):Node(value, "ProductOp")
{}
void ProductOp::print(ostream* out)
{
  if(_err) return;
  
  *out << "<ProductOp> --> " << _value << endl;
}

/******************************************************************************/

SumOp::SumOp(string value):Node(value, "SumOp")
{}
void SumOp::print(ostream* out)
{
  if(_err) return;
  
  *out << "<SumOp> --> " << _value << endl;
}

/******************************************************************************/
ErrNode::ErrNode():Node("", "ErrNode")
{
  _err = true;
}
void ErrNode::print(ostream* out)
{
  return;
}

