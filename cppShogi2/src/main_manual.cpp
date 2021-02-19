#include "shogi.hpp"
#include <cstdlib>
#include <ctime>

void manualTest() {
	Shogi c;
	c.BoardInit();
	c.EasyBoardPrint();

	int n;
	cin >> n;

	for(int j=0;j<n;j++) {
		vector<uint16_t> movelist = c.FetchMoves(1);
		if(movelist.size() == 0){
			if(c.round % 2 == 1) {
				cout << c.round << "手，先手勝。\n";
			} else {
				cout << c.round << "手，後手勝。\n";
			}
			break;
		}

		int psuji, pdan, nsuji, ndan;
		int upgrade, placing;
		cin >> psuji >> pdan >> nsuji >> ndan >> upgrade >> placing;

		uint8_t pos = (uint8_t)((9 - psuji) + ((pdan - 1) * 11));
		uint8_t npos = (uint8_t)((9 - nsuji) + ((ndan - 1) * 11));

		if(placing) {
			pos = pdan;
		}
		uint16_t move = (uint16_t)(upgrade * 16384) + (uint16_t)(placing * 32768) + 
		(((uint16_t)npos << 7) + (uint16_t)pos);

		string goma_note[8] = {"步", "香", "桂", "銀", "角", "飛", "金", "王"};
		if(placing) {
			cout << "Placing " << goma_note[pos] << " at (" << nsuji << ", " << ndan << ")\n"; 
		} else {
			cout << "(" << psuji << ", " << pdan << ") --> " << "(" << nsuji << ", " << ndan << ")\n";
		}

		bool exist_flag = false;
		for(uint16_t em : movelist) {
			if(em == move) {
				exist_flag = true;
				break;
			}
		}

		if(exist_flag) {
			c.MakeMove(move);
			c.EasyBoardPrint();
		} else {
			cout << "Wrong Move!\n";
		}
	}
}

int main() {
	manualTest();
}