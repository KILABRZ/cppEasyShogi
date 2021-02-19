#include "shogi.hpp"
#include <cstdlib>
#include <ctime>

void speedTest(int n=1000) {
	srand(time(0));
	for(int i=0;i<n;i++) {
		Shogi c;
		c.BoardInit();
		for(int j=0;j<10000;j++) {
			vector<uint16_t> movelist = c.FetchMoves(1);
			if(movelist.size() == 0)break;
			int l = rand() % movelist.size();
			c.MakeMove(movelist[l]);
		}
	}
}

int main() {
	speedTest(1000);
}