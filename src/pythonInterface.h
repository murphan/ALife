//
// Created by Emmet on 1/18/2023.
//

#ifndef ALIFE_PYTHONINTERFACE_H
#define ALIFE_PYTHONINTERFACE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pyconfig.h"

#include <functional>
#include <initializer_list>

/**
 * simple wrapper for a python object
 * manages the memory for it so you don't have to worry about how crappy the,
 * python library is
 */
class PythonHolder {
protected:
	PyObject * object;
public:
	explicit PythonHolder(PyObject * object);
	~PythonHolder();

	auto get() -> PyObject *;
};

/**
 * created to call python code in c++
 */
class PythonScript : public PythonHolder {
public:
	explicit PythonScript(const char * filename);

	auto callFunction(const char * functionName, std::initializer_list<PyObject *> args) -> PythonHolder;

	static auto init() -> void;

	//TODO this function crashes for some reason
	[[maybe_unused]] static auto cleanup() -> void;
};

struct PythonMethodDefinition {
	const char * name;
	PyCFunction function;
};

#endif
