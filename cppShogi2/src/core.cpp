#include "shogi.hpp"
#define outboard(x) (x<12||x>108)||((x+1)%11<=1)

/*
0 = 步兵 8  = 成金
1 = 香車 9  = 成香
2 = 桂馬 10 = 成桂
3 = 銀將 11 = 成銀
4 = 角行 12 = 龍馬
5 = 飛車 13 = 龍王
6 = 金將
7 = 王將
*/

short goma_move_vector[71] = {
-11,
-11,
-21,-23,
-10,-11,-12,10,12,
-12,-10,10,12,
-11,11,1,-1,
-10,-11,-12,1,-1,11,
-10,-11,-12,1,-1,10,11,12,
-10,-11,-12,1,-1,11,
-10,-11,-12,1,-1,11,
-10,-11,-12,1,-1,11,
-10,-11,-12,1,-1,11,
-10,-12,10,12,1,-1,11,-11,
-10,-12,10,12,1,-1,11,-11
};

short goma_flow_vector[71] = {
0,
1,
0,0,
0,0,0,0,0,
1,1,1,1,
1,1,1,1,
0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0
};

short goma_index_vector[14] = {0, 1, 2, 4, 9, 13, 17, 23, 31, 37, 43, 49, 55, 63, 71};

vector<unsigned short> Shogi::FetchMoves(unsigned char mode) {

	vector<unsigned short> movelist;
	movelist.reserve(1024);

	unsigned char chesser = (round & 1);

	// basic moving rule procedure
	for(int goma_idx = 1; goma_idx <= 40; goma_idx++) {
		
		unsigned char onboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 6);
		unsigned char owner = ((goma_cde[goma_idx] & MASK_goma_owner) >> 5);
		unsigned char fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));

		if(!onboard && owner != chesser) continue;

		if(onboard) {
			short dr = (owner == 0 ? 1 : -1);
			short pos = goma_pos[goma_idx];

			for(int idx = goma_index_vector[fulid]; idx < goma_index_vector[fulid+1]; idx++) {
				if(goma_flow_vector[goma_idx]) {
					// flow move
				} else {
					// direct move
				}
			}
		} else {
			// placing move
		}
	}
}