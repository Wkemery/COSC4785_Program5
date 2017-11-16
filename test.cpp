class ClassA
{
  int myint1;
  int myint2;
  int[] myintarr1;
  int [][] myintarr2;
  ClassA myClassA1;
  ClassB myClassB1;
  
  void method1()
  {
//     method1 = myint1; //error unrecognized identifier method1
//     myint1 = method1; //error unrecognized identifier method1
//     myClassA1 = myint1; //error type mismatch
//     myint1 = myClassA1; //error type mismatch
//     method2(myint1, myint2, myClassB1);//error no matching function call
//     while(myClassA1){}//error expression does not evaluate to boolean
//     return 0; //error does not match func return type
//     myClassB1.x = 5;//error class b does not have a member named x
//     myintarr1 = new int[myClassA1];//error [doesnt evaluate to int]
//     myintarr1 = new int[new int[]];//error [doesnt evaluate to int]
//     myintarr1[new int] = 5;//shit goddamnit new int is an int reference not a number
//     myintarr1[myClassA1] = 5;//error [doesnt evaluate to int]
//     myint1 = myint2 + myClassA1;
//     myint1 = new ClassNone();//error classnone dne
//     myint1 = new ClassNone(myint1);//error classnone dne
//     myint1 = new ClassNone[][];//error classnone dne
//     myint1 = new ClassNone[5][];//error classnone dne
    
    
  }
  
  int method2(int x, ClassA y, ClassB c)
  {
//     return; //error does not match func return type
  }
}

// class ClassA{} //error class declared twice

class ClassB
{}

class ClassC
{}
