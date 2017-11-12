/*
 * This is a test file for Symbol Table Building
 * It does a thourough test of Symbol Table building and printing
 * By continously incrementing the complexity of the tests
 */


// Test building symbol Tables for variable declarations
//-----------------------------------------------------------------------
class A
{
  int x;
}

class B
{
  int[] x;
}

class C
{
  int[][][] x;
}

class D
{
  type x;
}

class E
{
  type[] x;
}

class F
{
  type[][][] x;
}

class G
{
  int x;
  type y;
  int[][] z;
  type[][][] w;
}

class H
{
  int[][][][][][][][][][][][][][][][] x;
  type[][][][][][][][][][][][][][][][] y;
}

// Test Building Tables for Method Declarations
//-------------------------------------------------------------------

class I
{
  void method(){}
}

class J
{
  int method(){}
}

class K
{
  return_type method(){}
}

class L
{
  int[][] method(){}
}

class M
{
  return_type[][][][][] method(){}
}

class N
{
  void method1(){}
  int method2(){}
  return_type method3(){}
  int[][] method4(){}
  return_type[][][][] method5(){}
}

class O
{
  int x;
  type[][] y;
  void method1(){}
  void method2(){}
  return_type[][] method3(){}
}

class P
{
  int x;
  void method(int o, type t, int b){}
}

class Q
{
  int y;
  return_type[][] method(type[][] r, type d){}
}
