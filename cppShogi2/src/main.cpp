#include "shogi.hpp"
#include <cstdlib>
#include <ctime>

int main() {
	srand(time(0));

	Shogi c;
	c.BoardInit();

	for(int i=0;i<100;i++) {
		c.EasyBoardPrint();
		vector<uint16_t> movelist = c.FetchMoves(2);
		int l = rand() % movelist.size();
		c.MakeMove(movelist[l]);
	}

}