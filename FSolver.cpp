#include "FSolver.h"

#include <cstdlib>
#include <string>

#define PY_SSIZE_T_CLEAN
#include "Python.h"

static PyObject *Py_Resolve(const char *symbol)
{
  PyObject *object = PyDict_GetItemString(PyModule_GetDict(PyImport_AddModule("__main__")), symbol);
  if(!object) fprintf(stderr, "undefined symbol: %s\n", symbol), exit(EXIT_FAILURE);
  return object;
}

static PyObject *PyList_FromVector(const std::vector<double> &v)
{
  PyObject *list = PyList_New(v.size());
  for(size_t i = 0; i < v.size(); i++) PyList_SetItem(list, i, PyFloat_FromDouble(v[i]));
  return list;
}

FSolver::FSolver() : fFsolve(nullptr), fFunc(nullptr)
{
  Py_Initialize();
  Exec(R"(
import scipy.optimize as opt
def fsolve(func, x, a):
    return list(opt.fsolve(func, x, a))
  )");
  fFsolve = Py_Resolve("fsolve");
  Py_INCREF(fFsolve);
}

FSolver::~FSolver()
{
  Py_DECREF(fFsolve);
  Py_XDECREF(fFunc);
  Py_Finalize();
}

void FSolver::UpdateFunc(const char *func)
{
  Py_XDECREF(fFunc);
  fFunc = Py_Resolve(func);
  Py_INCREF(fFunc);
}

bool FSolver::Solve(std::vector<double> &x, const std::vector<double> &a)
{
  // func
  if(fFunc == nullptr) UpdateFunc();
  PyObject *func = (PyObject *)fFunc;
  Py_INCREF(func);

  // (a,)
  PyObject *args = PyTuple_New(1);
  PyTuple_SetItem(args, 0, PyList_FromVector(a));

  // (func, x, (a,))
  PyObject *tuple = PyTuple_New(3);
  PyTuple_SetItem(tuple, 0, func);
  PyTuple_SetItem(tuple, 1, PyList_FromVector(x));
  PyTuple_SetItem(tuple, 2, args);

  // fsolve(func, x, (a,))
  PyObject *result = PyObject_CallObject((PyObject *)fFsolve, tuple);
  Py_DECREF(tuple);
  if(!result) {
    PyErr_Print();
    return false;
  }

  // x = fsolve(func, x, (a,))
  Py_ssize_t n = PyList_Size(result);
  if(n < 0) PyErr_Print(), exit(EXIT_FAILURE);
  if((size_t)n != x.size()) fprintf(stderr, "size mismatch: %zu != %zu\n", (size_t)n, x.size()), exit(EXIT_FAILURE);
  for(size_t i = 0; i < x.size(); i++) {
    x[i] = PyFloat_AsDouble(PyList_GetItem(result, i));
    if(PyErr_Occurred()) PyErr_Print(), exit(EXIT_FAILURE);
  }
  Py_DECREF(result);
  return true;
}

void FSolver::Exec(const char *source) const
{
  if(PyRun_SimpleString(source)) exit(EXIT_FAILURE);
}
