#include <iomanip>
#include <iostream>
#include <vector>

#include "FSolver.h"

// --------------------------------------------------------------------------------
// Compile with:
//     g++ FSolverPerf.cpp FSolver.cpp -o FSolverPerf `python3-config --embed --cflags --ldflags`
// Maybe a useful flag to have Python3 inside conda env:
//     -L$CONDA_PREFIX/lib -Wl,--rpath=$CONDA_PREFIX/lib
// --------------------------------------------------------------------------------

int main()
{
  FSolver fSolver;
  fSolver.Exec(R"(
def func(x, a):
    x = x[0]
    a = a[0]
    return x*x - a
  )");
  std::vector<double> x(1), a(1);
  for(int i = 0; i < 10000; ++i) {
    a[0] = drand48();
    x[0] = a[0];
    fSolver.Solve(x, a);
    std::cout << std::fixed << std::setprecision(3) << a[0] << " " << x[0] << std::endl;
  }
  return 0;
}
