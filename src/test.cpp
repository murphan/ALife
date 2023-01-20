//
// Created by Emmet on 11/9/2022.
//

#include "pythonInterface.h"

#include <iostream>
#include "genome/phenome.h"

#include "genome/genome.h"
#include "types.h"
#include <genome/gene/bodyGene.h>

auto product(PyObject * self, PyObject * args) -> PyObject * {
	auto result = PyLong_AsLong(PyTuple_GetItem(args, 0)) * PyLong_AsLong(PyTuple_GetItem(args, 1));
	return PyLong_FromLong(result);
}

static PyMethodDef aLifeMethodDefinitions[] = {
{ "product", product, METH_VARARGS, nullptr },
{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef aLifeModule = {
	PyModuleDef_HEAD_INIT,
	"alife",
	nullptr,
	-1,
	aLifeMethodDefinitions,
};

PyMODINIT_FUNC initALifeModule() {
	return PyModule_Create(&aLifeModule);
}

auto main () -> int {
	PyImport_AppendInittab("alife", initALifeModule);

	PythonScript::init();

	auto * mmm = PyImport_ImportModule("alife");
	if (mmm == nullptr) throw std::exception("bruh");

	auto * dict = PyModule_GetDict(mmm);
	auto * keys = PyDict_Keys(dict);
	for (auto i = 0; i < PyList_Size(keys); ++i) {
		std::cout << PyUnicode_AsUTF8(PyList_GetItem(keys, i)) << std::endl;
	}

	{
		auto program = PythonScript("testmodule");

		auto result = program.callFunction("add", { PyLong_FromLong(3), PyLong_FromLong(4) });

		std::cout << "result: " << PyLong_AsLong(result.get()) << std::endl;
	}

    auto g0 = Genome("AAACAAADAAABAAACAAAD");
    auto g1 = Genome("AAADAAABAAABAAACAAAC");

	for (auto i = 0; i < 20; ++i) {
		auto recombined = Genome::mutateCombine(g0, g1, 0.3_f32, 0.01_f32, 0.01_f32, 0.01_f32);
		std::cout << "Combined: " << (recombined.has_value() ? recombined->toString() : "Not compatible") << std::endl;
	}

    //auto [ r0, r1 ] = std::tuple<Genome &, Genome &> { g0, g1 };
    //auto grid = Genome::editDistance(g0, g1);
    //Util::printGrid(grid, g0.size() + 1);

    /* mutate genome test */
    auto ogGenome = Genome(std::string("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
    auto mutatedGenome = ogGenome.mutateCopy(0.01_f32, 0.01_f32, 0.01_f32);

    std::cout << ogGenome.toString() << std::endl;
	std::cout << mutatedGenome.toString() << '\n' << std::endl;

    /* create body test */
	auto creator = Genome();

	BodyGene::create(Direction::RIGHT, 2).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 3).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 4).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 5).write(creator);
	creator.writeGarbage(4, Genome::A);

	std::cout << "created: " << creator.toString() << std::endl;

    auto phenome = Phenome(creator);
    std::cout << phenome.body.debugToString() << std::endl;

    /* string decode test */
	auto genome1 = Genome("ABDACDABBDCABBBCBABCBADBCBDABBBCBDABBABDBADBCBABBDABDDACBCBCDBABC");

	std::cout << genome1.toString() << std::endl;

	//PythonScript::cleanup();
}
