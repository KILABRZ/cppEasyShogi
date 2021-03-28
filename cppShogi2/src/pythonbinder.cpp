#undef __STRICT_ANSI__
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "shogi.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ShogiAgent, m){
	py::class_<Shogi>(m, "Shogi")
		.def(py::init<>())
		.def("BoardInit", &Shogi::BoardInit)
		.def("FetchMoves", &Shogi::FetchMoves)
		.def("MakeMove", &Shogi::MakeMove)
		.def("EasyBoardPrint", &Shogi::EasyBoardPrint)
		.def("BoardKnowledgeExtract", &Shogi::BoardKnowledgeExtract)
		.def("StrMakeMove", &Shogi::StrMakeMove)
		.def("StrFetchMoves", &Shogi::StrFetchMoves)
		.def("StrSaveBoardState", &Shogi::StrSaveBoardState)
		.def("StrLoadBoardState", &Shogi::StrLoadBoardState);
}