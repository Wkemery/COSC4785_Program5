class ClassA
{
  int myint1;
  int myint2;
//   int myint2; //error var declared twice
  int[] myintarr1;
  int [][] myintarr2;
  ClassA myClassA1;
//   ClassA myClassA1; //error var declared twice
  ClassB myClassB1;
//   int myClassB1; //error var declared twice
//   ClassNone mybadvar;//error invalid type
  
  ClassA(){}
//   ClassA(){}//error constructor delcared twice
  
//   A(){}//Error constructor name does not match class name
//   ClassA(int var1, ClassNone var2){}//error invalid type classNone
//   ClassA(int var1, int var1){} //error var1 declared twice
  
  void method1()
  {
    
    
//     this(5);//error no mlathcing constructure
//     myint1 = null; //error type mismatch
//     if(myint1 == null){}//error type mismatch
//     if(myint1 + null){} //error type mismatch
//     if(myClassA1 + null){} //error type does not evaluate to boolean
//     if((myClassA1 == null) && (myint1 == null)){}//error expressions not of same tyupe
// //     myint1 = new int;//syntax error
//     method1 = myint1; //error unrecognized identifier method1
//     myint1 = method1; //error unrecognized identifier method1
//     myClassA1 = myint1; //error type mismatch
//     myint1 = myClassA1; //error type mismatch
//     method2(myint1, myint2, myClassB1);//error no matching function call
//     while(myClassA1){}//error expression does not evaluate to boolean
//     if(myClassA1){}//error expression does not evaluate to boolean
//     return 0; //error does not match func return type
//     myClassB1.x = 5;//error class b does not have a member named x
//     myintarr1 = new int[myClassA1];//error [doesnt evaluate to int]
//     myintarr1 = new int[new int[]];//error [doesnt evaluate to int]
// //     myintarr1[new int] = 5;//syntax error [doesnt evaluate to int]
//     myintarr1[new ClassA()] = 5; //error [doesnt evaluate to int]
//     myintarr1[myClassA1] = 5;//error [doesnt evaluate to int]
//     myint1 = myint2 + myClassA1;//errro expressions different types
//     myint1 = new ClassNone();//error classnone dne
//     myint1 = new ClassNone(myint1);//error classnone dne
//     myint1 = new ClassNone[][];//error classnone dne
//     myint1 = new ClassNone[5][];//error classnone dne
//     this.x = 0; //errror x not declared within ClassA

  }
  
  int method2(int x, ClassA y, ClassB c)
  {
//     return; //error does not match func return type
  }
//   int method2(int a, ClassA b, ClassB c){} //error method redeclared
//   void method2(int a, ClassA b, ClassB c){}//error method redeclared
//   int method3(ClassA x, ClassA x){} //error x declared twice
//   ClassNone method4(){} //error invalid type
//   void method5(ClassNone x, ClassA y){}//error invalid type
}

// class ClassA{} //error class declared twice

class ClassB
{}

class ClassC
{}
