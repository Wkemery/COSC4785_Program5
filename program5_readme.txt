# COSC4785_Program3
Valid input:
  The Decaf grammar as it is described in the document
  
Changes in the Grammar:
  The biggest change I made was in how a variable is declared.
  First off, I made a production called multibracks. I completely eliminated 
  simpletype. 
  
How I print things out:
  Everything should be printing as described in the instructions except for the 
  way I print expressions.  For example given an expression of the form 2+2, my 
  program will print out:
  <Expression> --> <Expression> <SumOp> <Expression>
  <Expression> --> NUM
  <SumOp> --> +
  <Expression> --> NUM
  
I did talk to you about this, but it is just a reminder.
  
  I did this because sum op could be a number of things and so I made it a node
  in my tree. Mine does not print:
  <Expression> --> <Expression> + <Expression>
  <Expression> --> NUM
  <Expression> --> NUM
  
Errors sometimes change the printing slightly. By this, I mean on some nodes
for some productions, you may get something like:
  <ClassDec> --> class ID <ClassBody>
  <ClassBody> --> {<ErrNode>}

This allows me to keep the class declaration as a valid node in the tree, for 
type checking later. Obviously the ErrNode has no legitimate values and is 
therefore never printed out. But, it will appear like that in the printing
sometimes and depending on the error.
  
  
When I create nodes, I give almost all of them a _kind. It is an integer value 
with macros defined in the Node.h file. I don't necessarily need all of them. 
However, I am using them because I have a feeling they may be helpful in the 
future. If not, I will probably remove what I can then. Overall though, it has 
made my printing much nicer and easier to follow, with case statements. 
  
One final note: I do allow an empty input because it makes sense to me.