//time to test name. stuff

class ClassA
{
  ClassB[] y;
  ClassB bs;
}


class ClassB
{
  int z;
  int[] iarr;
  int mymethod(int x, ClassA y){}
//   void mymethod(int x, ClassA y){}
}

class ClassC
{
  ClassA myClassA1;
  int myint1;
  ClassC(){}
  ClassC(int x){}
  void meth()
  {
//     ClassB[] j;
//     x.y = j;
//     x.y.z = 5;
    int a;
    int[] myarray;
    ClassC myCobject;
    ClassA[][] objArray;
//     a = x.y.mymethod(a, x);
//     myarray = x.y.iarr;
//     x.y = j;
//     myarray = myarray;
//     myarray[0] = myarray[0];
//     myarray = new int[][]; //error
//     myarray = new int[]; //correct
//     myarray = new int;//error
//     a = new int;
    
//     myCobject = new C; //error
//     myCobject = new ClassC; //error
    myCobject = new ClassC(); //correct
    myCobject = new ClassC(a);//correct
//     objArray = new ClassA[][][];//error
    objArray = new ClassA[][];//correct
    objArray = new ClassA[a][5];//correct
    objArray = new ClassA[5][5];//correct
    objArray = new ClassA[a][];//correct
    objArray = new ClassA[5][];//correct
//     objArray = new ClassA[a][myCobject];//error
//     objArray = new ClassA[a][x.bs.z];//correct
//     objArray = new ClassA[a][x.y.z];//error
//     objArray = new ClassA[a][x.y[0].z];//correct
    
    myClassA1 = null;
    myClassA1 = myClassA1 + null;
    myClassA1 = null + myClassA1;
    myClassA1 = null + null;
    if(method2()){}
    if(null == null){}
    if((myClassA1 == null) && (myint1 == 0)){}
    
//     objArray = new ClassA[myCobject][];//Error
    
  }
  
  int method2(){}
  
}













































class MethodDec : public SymbolTable
{
public:
  MethodDec(SymbolTable * p) : SymbolTable(p)
  {
    type = METHODTYPE;
  }
}



