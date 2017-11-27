# COSC4785_Program5
Valid input:
  The Decaf grammar as it is described in the document, except one caveat, listed below
  
some Notes about boolean expressions:
In my opinion, exp1 == exp2 == exp3 does not make sense, even in exp1, exp2 and 
exp3 are of the same type. Therefore, this format is almost always an error.
I do this, because == is a relational operator. It is unlike a sum operator in that
it does not return something something of the same type as its sub expressions. Instead,
it reuturns a "boolean" or in my case, something of type integer. Therefore, if exp1 and
exp2 are of type A, exp1==exp2 returns an int. if exp3 is of type A this is a type 
mismatch error.

types and null:
Since null is a valid type for all non-integer types, ie, any non integer variable
can be set to null, I allow most operators to act with null. EG
you can do something like
A x;
x = null + null;
if(x == null){}
etc...

are parenthesis optional?
Based off of my questions to you, I chose to implement the following:
This is not type check correct:
A x;
int y;
x = new A;
y = new int;
in order for the 3rd line to be correct the "()" are required.
EG
A x;
x = new A();

This means at no time is it valid to have x = new identifier;
without "()". Therefore, I consider this a syntax error, even
though it is not a syntax error as described in the grammar.
However, to me, it does not make sense to allow something syntactically that
you unconditionally deny semantically.
  
When I create nodes, I give almost all of them a _kind. It is an integer value 
with macros defined in the Node.h file. I don't necessarily need all of them. 
However, I am using them because the have been quite helpful with printing the 
parse tree, and building the symbol table and type checking 
  
One final note: I do allow an empty input because it makes sense to me.
