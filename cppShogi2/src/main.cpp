#include "shogi.hpp"
#include <cstdlib>
#include <ctime>

int main() {
	srand(time(0));


	for(int i=0;i<1000;i++) {
		Shogi c;
		c.BoardInit();
		for(int j=0;j<10000;j++) {
			vector<uint16_t> movelist = c.FetchMoves(0);
			if(movelist.size() == 0)break;
			int l = rand() % movelist.size();
			c.MakeMove(movelist[l]);
		}
	}
	
}