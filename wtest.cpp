
class ClassA
{
  ClassB[] y;
  ClassB bs;
  void dummymethod()
  {
//     return 1; //error invalid return type
    return;
  }
}


class ClassB
{
  int myint;
  int[] myintarr;
  int[][] myintarr2;
  ClassA myA1;
  ClassA[][] myAarr2;
  
  int mymethod1(int x, ClassA y)
  {
    x = 5;
//     x = y; //error mismatched types
    mymethod2(myAarr2, myint);
    myint = mymethod1(x, y);
//     mymethod2(myint, myAarr2); //error no method
    return x * 2;
  }
  void mymethod2(ClassA[][] myvar1, int myvar2)
  {
    myvar1 = new ClassA[][];
    myvar1 = new ClassA[5][];
    myvar1 = new ClassA[mymethod1(myvar2, myA1)][myvar2];
    myA1 = new ClassA();
    print(5, new ClassA());
    if((myvar1 == myvar1) && (myA1 == new ClassA()))
    {}
    else
    {}
//     if((myvar1 == myA1) && (myA1 == new ClassA())){}//error exp not of same type
    while(1){}
//     myA1 = new ClassA;//syntax error
  }
}

class ClassC
{
  ClassA myClassA1;
  ClassC myClassC1;
  ClassB myClassB1;
  int myint1;
  ClassC(){
    this();
    this(myint1);
    this.myint1 = 10;
    this.myint1 = this.myint1;
  }
  ClassC(int x){}
  void meth()
  {
    ClassA[][] objArray;
    ClassA myAobj;
    int a;
    {
      int a;
      {
        int b;
        {
          int c;
          {
            int d;
          }
        }
      }
      a = this.myint1;
    }
//     a = x.y.mymethod(a, x);
//     myarray = x.y.iarr;
//     x.y = j;
//     myarray = myarray;
//     myarray[0] = myarray[0];
//     myarray = new int[]; //correct
    
//     myCobject = new ClassC(); //correct
//     myCobject = new ClassC(a);//correct
    
    myClassC1.myClassA1.y[0] = myClassB1;
    
//     myClassC1.myClassA1.y[0] = myClassB1.myint;//error type mismatch
    myClassC1.myClassA1.y[0].myintarr2[4][myint1] = myint1;
    
    objArray = new ClassA[][];//correct
    objArray = new ClassA[a][5];//correct
    objArray = new ClassA[5][5];//correct
    objArray = new ClassA[a][];//correct
    objArray = new ClassA[5][];//correct
    objArray = new ClassA[a][myAobj.y[0].myint];//correct
    
    myClassA1 = null;
    //     myClassA1 = myClassA1 + null;//error for shaya
    //     myClassA1 = null + myClassA1;//error for shaya
//     myClassA1 = null + null;//error for shaya
    if(method2()){}
    if(null == null){}
    if((myClassA1 == null) && (myint1 == 0)){}
//     this[2] = myClassC1;//error for shaya
    this.myint1 = 0;
    myClassC1 = this;
    this = myClassC1;
    
//     objArray = new ClassA[myCobject][];//Error
  }
  
  int method2(){}
  
}


