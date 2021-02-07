#include "shogi.hpp"

string note_table[32] = {
	"+步 ", "+香 ", "+桂 ", "+銀 ", "+角 ", "+飛 ", "+金 ", "+王 ", "+と ", "+杏 ", "+圭 ", "+全 ", "+馬 ", "+龍 ", "nothing", "nothing",
	"-步 ", "-香 ", "-桂 ", "-銀 ", "-角 ", "-飛 ", "-金 ", "-王 ", "-と ", "-杏 ", "-圭 ", "-全 ", "-馬 ", "-龍 ", "nothing", "nothing"
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

	int goma_recorder[16] = {0};

	for(uint8_t goma_idx = 3; goma_idx <= 40; goma_idx++) {
		uint8_t notonboard = (goma_cde[goma_idx] & MASK_goma_onboard);
		if(notonboard) {
			uint8_t owner = ((goma_cde[goma_idx] & MASK_goma_owner) >> 4);
			uint8_t ezyid = (goma_cde[goma_idx] & MASK_goma_ezyid);
			goma_recorder[owner*8+ezyid]++;
		}
	}
	cout << "\n--------------------------------------\n";
	cout << "駒台：  步  香  桂  銀  金  角  飛\n";
	cout << "先手：   " << goma_recorder[0] << "   " << goma_recorder[1] 
	<< "   " << goma_recorder[2] << "   " << goma_recorder[3] << "   " << goma_recorder[6] 
	<< "   " << goma_recorder[4] << "   " << goma_recorder[5] << "\n";
	cout << "後手：   " << goma_recorder[8] << "   " << goma_recorder[9] 
	<< "   " << goma_recorder[10] << "   " << goma_recorder[11] << "   " << goma_recorder[14] 
	<< "   " << goma_recorder[12] << "   " << goma_recorder[13] << "\n";
	cout << "--------------------------------------\n";
}

void MoveExplainer(uint16_t move) {
	uint16_t placing_flag = (move & MASK_move_placing);
	uint16_t upgrade_flag = (move & MASK_move_upgrade);
	uint8_t npos = ((move & MASK_move_npos) >> 7);
	uint8_t pos = (move & MASK_move_pos);

	if(placing_flag) {
		cout << "> 打入 " << note_table[pos] << "於 "; PosExplainer(npos); cout << "\n";
	} else {
		cout << "> "; PosExplainer(pos); cout <<" => "; PosExplainer(npos);
		if(upgrade_flag) cout << " 成";
		cout << "\n";
	}
}

void PosExplainer(uint8_t pos) {
	cout << "(" << (int)(9 - pos%11) << ", " << int((pos+1)/11+1) << ")";
}