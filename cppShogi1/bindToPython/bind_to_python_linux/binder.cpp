#undef __STRICT_ANSI__
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "shogi.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ShogiAgent, m){
	py::class_<Shogi>(m, "Shogi")
		.def(py::init<>())
		.def("InitialBoard", &Shogi::InitialBoard)
		.def("FetchMove", &Shogi::FetchMove)
		.def("MakeMove", &Shogi::MakeMove)
		.def("BoardState", &Shogi::BoardState)
		.def("EasyBoardPrint", &Shogi::EasyBoardPrint);
	m.def("DecodeMove", &DecodeMove);
	m.def("EncodeMove", &EncodeMove);
}