#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Shogi{
public:

	bool activated_flag;
	uint8_t board[97];
	uint8_t goma_cde[41];
	uint8_t goma_pos[41];
	
	uint8_t round;

	Shogi() {
		activated_flag = false;
	}

	void BoardInit();


	vector<uint16_t> FetchMoves(uint8_t mode);
	void MakeMove(uint16_t move);
	void EasyBoardPrint();
	//	Shogi TryMove(uint16_t move);
};

#define outboard(x) ((x>96)||(x%11>8))
const uint8_t MASK_goma_ezyid = 7;
const uint8_t MASK_goma_upgrade = 8;
const uint8_t MASK_goma_fulid = 15;
const uint8_t MASK_goma_owner = 16;
const uint8_t MASK_goma_onboard = 32;

const uint16_t MASK_move_placing = 32768;
const uint16_t MASK_move_upgrade = 16384;
const uint16_t MASK_move_npos = 16256;
const uint16_t MASK_move_pos = 127;

void MoveExplainer(uint16_t move);
void PosExplainer(uint8_t pos);