//time to test name. stuff

class ClassA
{
  ClassB y;
}


class ClassB
{
  int z;
  int mymethod(int x, ClassA y){}
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
    a = x.y.mymethod(a, x);
  }
  
}