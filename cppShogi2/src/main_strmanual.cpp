#include "shogi.hpp"

int main() {
	Shogi s;

	s.BoardInit();


	while(true) {
		s.EasyBoardPrint();
		vector<string> smovelist = s.StrFetchMoves(1);
		if(smovelist.size() == 0) break;
		for(int i=0;i<smovelist.size();i++) {
			cout << smovelist[i] << "  ";
			if(i % 10 == 9) cout << "\n";
		}
		cout << "\n";
		string move;
		bool exist = false;
		while(!exist) {
			cin >> move;
			for(string m : smovelist) {
				if(move == m){
					exist = true;
					break;
				}
			}
		}
		


		s.StrMakeMove(move);
	}
	
}