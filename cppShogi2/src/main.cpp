#include "shogi.hpp"

int main() {
	Shogi c;
	c.BoardInit();
	c.EasyBoardPrint();
	vector<uint16_t> movelist = c.FetchMoves(2);

	cout << movelist.size() << "\n";
}