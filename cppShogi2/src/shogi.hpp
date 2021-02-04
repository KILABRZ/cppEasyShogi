#include <vector>
#include <iostream>
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
	bool MakeMove(uint16_t move);
	void EasyBoardPrint();
	//	Shogi TryMove(uint16_t move);
};

uint8_t MASK_goma_ezyid = 7;
uint8_t MASK_goma_fulid = 15;
uint8_t MASK_goma_owner = 16;
uint8_t MASK_goma_onboard = 32;
