#include <vector>

class Shogi{
public:

	bool activated_flag;
	unsigned char board[121];
	unsigned char goma_pos[41];
	unsigned char goma_cde[41];

	Shogi() {
		activated_flag = false;
	}

	void BoardInit();


	vector<unsigned short> FetchMoves(unsigned char mode);
	bool MakeMove(unsigned short move);
	Shogi TryMove(unsigned short move);
}

unsigned char MASK_goma_easy_id = 7;
unsigned char MASK_goma_full_id = 15;
unsigned char MASK_goma_owner = 32;
unsigned char MASK_goma_onboard = 64;