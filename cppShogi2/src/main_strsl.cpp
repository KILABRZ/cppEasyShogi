#include "shogi.hpp"

int main() {
	Shogi s;
	Shogi a;

	s.BoardInit();

	s.StrMakeMove("7776-");

	string k = s.StrSaveBoardState();
	cout << k << "\n";
	s.StrLoadBoardState(k);
	a.StrLoadBoardState(k);
	string c = a.StrSaveBoardState();

	s.EasyBoardPrint();
	a.EasyBoardPrint();

	cout << k << "\n";
	cout << c << "\n";


}