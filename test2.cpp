//time to test name. stuff

class ClassA
{
  ClassB y;
}


class ClassB
{
  int z;
  int[] iarr;
  int mymethod(int x, ClassA y){}
  void mymethod(int x, ClassA y){}
}

class ClassC
{
  ClassA x; 
  void meth()
  {
//     ClassB j;
//     x.y = j;
//     x.y.z = 5;
    int a;
    int[] myarray;
    
    a = x.y.mymethod(a, x);
    myarray = x.y.iarr[0];
  }
  
}