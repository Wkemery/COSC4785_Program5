/*
 * SymTable.cpp
 * Author: Wyatt Emery
 * Date: NOV 2017
 *
 * COSC 4785, Homework5
 *
 */

/******************************************************************************/
#include"SymTable.h"
Type::Type(string lval, string rval, vector<string>* parameters, string classType, bool deleteME)
:_lval(lval), _rval(rval), _parameters(parameters), _classType(classType), _value("")
{
  if (deleteME)
  {
    extraTypes.push_back(this);
  }
}

Type::~Type()
{
  if(_parameters!= 0)
  {
    delete _parameters;
  }
}

string Type::getlval(void) const{return _lval; }

string Type::getrval(void) const {return _rval;}

vector<string>* Type::getParams(void) const { return _parameters;}

string Type::getClassType(void) const {return _classType; }

void Type::setInit(string val) { _value = val; }

void Type::print(ostream* out) const
{ 
  if(_classType != "")
    *out << "Class Type";
  else
  {
    if(_rval == "" && _lval == "")
    {
      *out << "Constructor ";
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
  for(auto it = _children.begin(); it!= _children.end(); it ++)
  {
    delete it->second;
  }
  for(auto it = _entries.begin(); it != _entries.end(); it++)
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

Type* SymTable::lookup(string className, string identifier, int linenum) const
{
  //lookup the id in the class symtable of className
  
  if(!this->classLookup(className))
  {
    cerr << "Type Error: " << "Class Name \"" << className << "\" Does Not exist"  
    << endl;
    return 0;
  }
  
  //the class does exist
  const SymTable* rootTable = this->getRoot();
  const SymTable* classTable = rootTable->lookupChild(className);
  if(classTable == 0)
  {
    cerr << "Type Error: " << "Class Name \"" << className << "\" Does Not exist"  
    << endl;
    return 0;
  }
  
  Type* idType = classTable->lookup(identifier);
  if(idType == 0)
  {
    cerr << "Type Error: " << "Identfier \"" << nameUnMangle(identifier) 
    << "\" Not Declared within " << className << " Line " << linenum  
    << endl;
    return 0;
  }
  
  if(idType->getClassType() != "")
  {
    cerr << "Fatal Internal Error" << endl; 
    exit(1);
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
    //TODO: error skip for now... i think its just a duplicate error
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

SymTable* SymTable::getParent() const { return _parent;}

Type* SymTable::getClassType() const
{
  //rely on the fact that you are at a class type when the next node is root.
  if(_parent->getParent() == 0)
  {
    return _parent->lookup(_value);
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

void SymTable::print(ostream* out, int level) const
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
    }    
  }
}