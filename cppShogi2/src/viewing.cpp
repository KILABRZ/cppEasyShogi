#include "shogi.hpp"

string note_table[32] = {
	"+步 ", "+香 ", "+桂 ", "+銀 ", "+角 ", "+飛 ", "+金 ", "+王 ", "+と", "+杏 ", "+圭 ", "+全 ", "+馬 ", "+龍 ", "nothing", "nothing",
	"-步 ", "-香 ", "-桂 ", "-銀 ", "-角 ", "-飛 ", "-金 ", "-王 ", "-と", "-杏 ", "-圭 ", "-全 ", "-馬 ", "-龍 ", "nothing", "nothing"
};

void Shogi::EasyBoardPrint() {
	for(uint8_t p=0;p<97;p++) {
		if(outboard(p)) {
			cout << "\n";
			continue;
		}
		uint8_t unit = board[p];

		if(unit == 0) {
			cout << " .. ";
		} else {
			cout << note_table[goma_cde[unit]];
		}
	}
	cout << "\n";
}