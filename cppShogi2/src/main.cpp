#include "shogi.hpp"
#include <cstdlib>
#include <ctime>

int main() {
	srand(time(0));

	Shogi c;
	c.BoardInit();

	c.EasyBoardPrint();

	for(int i=0;i<10000;i++) {
		
		vector<uint16_t> movelist = c.FetchMoves(0);
		if(movelist.size() == 0)break;
		int l = rand() % movelist.size();
		c.MakeMove(movelist[l]);
		/*for(int i=0;i<movelist.size();i++) {
			if(i == l) cout << "Hit ";
			MoveExplainer(movelist[i]);
		}
		MoveExplainer(movelist[l]);
		
		c.EasyBoardPrint();*/
	}

	c.EasyBoardPrint();

}