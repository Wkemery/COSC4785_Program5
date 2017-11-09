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

/******************************************************************************/

Type::Type(string lval, string rval, vector<string>& parameters)
:_lval(lval), _rval(rval), _parameters(parameters)
{}

string Type::getlval(void) const{return _lval; }

string Type::getrval(void) const {return _rval;}

vector<string>* Type::getparams(void) { return &_parameters;}

/******************************************************************************/

SymTable::SymTable(SymTable* parent):_parent(parent)
{}

SymTable::~SymTable()
{
  for(auto it = _entries.begin(); it != _entries.end(); it++)
  {
    delete it->second;
  }
}

Type* SymTable::lookup(string identifier)
{
  auto it = _entries.find(identifier);
  return it->second;
}

int SymTable::insert(string identifier, Type* type)
{
  auto it = _entries.find(identifier);
  if(it == _entries.end()) return -1;
  _entries.insert(pair<string, Type*>(identifier, type));
  return 0;
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

string Node::getNodeName(void) const
{
  return _nodeName;
}

string Node::getValue(void) const {return _value;}

Type* Node::getType() const {return 0;}

Type* Node::getType(SymTable* table) const {return 0;}

void Node::buildTable(SymTable* table) {return;}
/******************************************************************************/

ClassDec::ClassDec(string value, Node* node1):Node(value, "ClassDec")
{
  _subNodes.push_back(node1);
  if(node1->getErr()) _err = true;
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
    /* Will not discount an entire block due to error value of its children */
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
    
