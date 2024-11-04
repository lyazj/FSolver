#pragma once
#include <vector>

// A C++ wrapper for scipy.optimize.fsolve(func, x0, args=(), ...) in Python.
class FSolver {
public:
  // Controls a process-associated embedded Python interpreter.
  // Use them carefully to avoid conflicts with other embeddings.
  FSolver();
  ~FSolver();

  // Resolve symbol func for fsolve.
  // Call it before Solve().
  // Also call it once one wants to update the function definition.
  void UpdateFunc(const char *func = "func");

  // Solve the equation func(x) = 0 given a starting point x0.
  // Pass additional arguments a to func as func(x, a).
  // Returns false on error.
  bool Solve(std::vector<double> &x, const std::vector<double> &a);

  // Underlying interpreter operations.
  // Use this for function definition and user-specified initialization.
  void Exec(const char *source) const;

private:
  void *fFsolve, *fFunc;
};
