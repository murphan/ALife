//
// Created by Emmet on 1/18/2023.
//

#ifndef ALIFE_PYTHONINTERFACE_H
#define ALIFE_PYTHONINTERFACE_H

#include <Python.h>

#include <initializer_list>

class PythonHolder {
protected:
	PyObject * object;
public:
	explicit PythonHolder(PyObject * object);
	~PythonHolder();

	auto get() -> PyObject *;
};

class PythonProgram : public PythonHolder {
public:
	explicit PythonProgram(const char * filename);

	auto callFunction(const char * functionName, std::initializer_list<PyObject *> args) -> PythonHolder;

	static auto init() -> void;
	static auto cleanup() -> void;
};

#endif//ALIFE_PYTHONINTERFACE_H
