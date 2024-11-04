#include <iomanip>
#include <iostream>
#include <vector>

#include "FSolver.h"

// --------------------------------------------------------------------------------
// Compile with:
//     g++ FSolverTest.cpp FSolver.cpp -o FSolverTest `python3-config --embed --cflags --libs`
// Maybe a useful flag to have Python3 inside conda env:
//     -L$CONDA_PREFIX/lib -Wl,--rpath=$CONDA_PREFIX/lib
// --------------------------------------------------------------------------------

int main()
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
  return 0;
}
