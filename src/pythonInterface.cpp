//
// Created by Emmet on 1/18/2023.
//

#include <Python.h>

#include <exception>
#include <sstream>
#include <filesystem>

#include "pythonInterface.h"

/* python holder */

PythonHolder::PythonHolder(PyObject * object) : object(object) {}

PythonHolder::~PythonHolder() {
	Py_XDECREF(object);
}

auto PythonHolder::get() -> PyObject * {
	return object;
}

/* program */

PythonProgram::PythonProgram(const char * filename) : PythonHolder(nullptr) {
	auto currentPath = std::filesystem::current_path().string();
	replace(currentPath.begin(), currentPath.end(), '\\', '/');

	auto command = std::stringstream();
	command << "sys.path.append(" << std::quoted(currentPath) << ")";

	PyRun_SimpleString(command.str().c_str());

	auto pName = PyUnicode_FromString(filename);

	auto module = PyImport_Import(pName);
	if (module == nullptr) throw std::exception("could not load python program");

	object = module;

	Py_DECREF(pName);
}

auto PythonProgram::init() -> void {
	Py_Initialize();
	PyRun_SimpleString("import sys");
}

auto PythonProgram::cleanup() -> void {
	Py_Finalize();
}

#include <iostream>

auto PythonProgram::callFunction(const char * functionName, std::initializer_list<PyObject *> args) -> PythonHolder {
	auto dict = PyModule_GetDict(object);

	auto functionHandle = PyDict_GetItemString(dict, functionName);

	if (functionHandle == nullptr || !PyCallable_Check(functionHandle)) {
		Py_XDECREF(functionHandle);
		for (const auto * item : args) Py_DECREF(item);

		throw std::exception("called name is not a python function");
	}

	auto passTuple = PyTuple_New((Py_ssize_clean_t)args.size());
	for (auto i = 0; i < args.size(); ++i) {
		/* reference of item gets owned by the tuple now */
		PyTuple_SetItem(passTuple, i, args.begin()[i]);
	}

	auto returnValue = PythonHolder(PyObject_CallObject(functionHandle, passTuple));

	Py_DECREF(functionHandle);
	Py_DECREF(passTuple);

	return returnValue;
}
