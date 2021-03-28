#include "shogi.hpp"
#include <string>


uint16_t DescribeToMove(string describe) {

	uint8_t pre_suji = describe[0] - '0';
	uint8_t pre_dan = describe[1] - '0';
	uint8_t new_suji = describe[2] - '0';
	uint8_t new_dan = describe[3] - '0';
	uint8_t upgrade = (describe[4] == '+');
	uint8_t placing = (describe[4] == 'p');

	uint8_t pre_pos;

	if(pre_suji != 0) pre_pos = (9 - pre_suji) + (pre_dan - 1) * 11;
	else pre_pos = pre_dan;

	uint8_t new_pos = (9 - new_suji) + (new_dan - 1) * 11;

	uint16_t move = ((uint16_t)new_pos << 7) + (uint16_t)pre_pos 
	+ 32768 * (uint16_t)placing + 16384 * (uint16_t)upgrade;

	return move;
}

string MoveToDescribe(uint16_t move) {
	uint8_t upgrade = ((move & MASK_move_upgrade) >> 9);
	uint8_t placing = ((move & MASK_move_placing) >> 8);
	uint8_t new_pos = ((move & MASK_move_npos) >> 7);
	uint8_t pre_pos = (move & MASK_move_pos);

	string describe = "0000*";	
	if(placing) {
		describe[1] += pre_pos;
	} else {
		describe[0] += 9 - (pre_pos % 11);
		describe[1] += pre_pos / 11 + 1;
	}

	describe[2] += 9 - (new_pos % 11);
	describe[3] += new_pos / 11 + 1;
	
	if(upgrade) describe[4] = '+';
	else if(placing) describe[4] = 'p';
	else describe[4] = '-';

	return describe;
}


void Shogi::StrMakeMove(string move) {
	uint16_t m = DescribeToMove(move);
	MakeMove(m);
}

vector<string> Shogi::StrFetchMoves(uint8_t mode) {
	vector<uint16_t> movelist = FetchMoves(mode);

	vector<string> describelist;
	describelist.reserve(movelist.size());

	for(uint16_t move : movelist) {
		describelist.push_back(MoveToDescribe(move));
	}

	return describelist;
}


// S for Sente, G for Gote
// S343 -> Sente Silver at 4 3
// Seperate with -, or not? not, every memory is 4 byte, good
// round-S343G412S481SA51...

string Shogi::StrSaveBoardState() {

	char hex_note[17] = "0123456789ABCDEF";

	string full_state = to_string(round) + "-";

	for(uint8_t i=1;i<=40;i++) {
		string describe("S000");
		if(goma_cde[i] & MASK_goma_owner) describe[0] = 'G';
		describe[1] = hex_note[(goma_cde[i] & MASK_goma_fulid)];

		if(!(goma_cde[i] & MASK_goma_onboard)) {
			describe[2] = 57 - (goma_pos[i] % 11);
			describe[3] = goma_pos[i] / 11 + 49;
		}
		full_state += describe;
	}

	return full_state;
}

void Shogi::StrLoadBoardState(string state) {

	activated_flag = true;

	int pos = state.find("-");
	string r = state.substr(0, pos);
	round = (uint16_t)stoi(r);
	
	int c = 1;

	for(int i=0;i<97;i++) board[i] = 0;

	for(int i=pos+1;i<state.length();i+=4) {
		goma_cde[c] = 0;
		goma_pos[c] = 0;
		if(state[i] == 'G') {
			goma_cde[c] |= 16;
		}

		if(state[i+1] >= 'A') goma_cde[c] |= state[i+1] - 55;
		else goma_cde[c] |= state[i+1] - 48;

		if(state[i+2] == '0') {
			goma_cde[c] |= 32;
		} else {
			goma_pos[c] = (57 - state[i+2]) + (state[i+3] - 49) * 11;
			board[goma_pos[c]] = c;
		}
		c++;
	}	
}