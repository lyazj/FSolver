#include <iomanip>
#include <iostream>
#include <vector>

#include "FSolver.h"

// --------------------------------------------------------------------------------
// This macro needs Python3 include path. Set it up with:
//     export CPLUS_INCLUDE_PATH=/usr/include/python3.12
// Change the version number above on your own need.
// Maybe useful to have Python3 inside conda env:
//     export LD_LIBRARY_PATH=$CONDA_PREFIX/lib
// --------------------------------------------------------------------------------

void FSolverMacro()
{
  FSolver fSolver;
  fSolver.Exec(R"(
def func(x, a):
    x, y = x
    a, b = a
    return x*x + y*y - a*a, x + y - b
  )");
  std::vector<double> x = { 0, 0 }, a = { 2, 1 };
  fSolver.Solve(x, a);
  for(double x0 : x) std::cout << std::fixed << std::setprecision(3) << " " << x0;
  std::cout << std::endl;
  a = {2, 4};  // no solution
  fSolver.Solve(x, a);
  for(double x0 : x) std::cout << std::fixed << std::setprecision(3) << " " << x0;
  std::cout << std::endl;
}

#include "FSolver.cpp"
