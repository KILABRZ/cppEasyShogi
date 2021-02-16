#include "shogi.hpp"


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

uint8_t goma_move_vector[71] = {
245,
1,
235,233,
246,245,244,10,12,
0,2,5,7,
1,6,3,4,
246,245,244,1,255,11,
246,245,244,1,255,10,11,12,
246,245,244,1,255,11,
246,245,244,1,255,11,
246,245,244,1,255,11,
246,245,244,1,255,11,
0,2,5,7,1,255,11,245,
246,244,10,12,1,6,3,4

};

uint8_t goma_flow_vector[71] = {
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
1,1,1,1,0,0,0,0,
0,0,0,0,1,1,1,1

};

uint8_t uv_table[9] = {244, 245, 246, 255, 1, 10, 11, 12};
uint8_t goma_index_vector[15] = {0, 1, 2, 4, 9, 13, 17, 23, 31, 37, 43, 49, 55, 63, 71};

void Shogi::BoardInit() {
	activated_flag = true;

	for(uint8_t i=0;i<97;i++) board[i] = 0;

	goma_pos[1] = 92;	goma_pos[2]	= 4;	goma_pos[3] = 84;	goma_pos[4] = 78;
	goma_pos[5] = 12;	goma_pos[6] = 18;	goma_pos[7] = 88;	goma_pos[8] = 89;
	goma_pos[9] = 90;	goma_pos[10] = 91;	goma_pos[11] = 93;	goma_pos[12] = 94;
	goma_pos[13] = 95;	goma_pos[14] = 96;	goma_pos[15] = 8;	goma_pos[16] = 7;
	goma_pos[17] = 6;	goma_pos[18] = 5;	goma_pos[19] = 3;	goma_pos[20] = 2;
	goma_pos[21] = 1;	goma_pos[22] = 0;	goma_pos[23] = 66;	goma_pos[24] = 67;
	goma_pos[25] = 68;	goma_pos[26] = 69;	goma_pos[27] = 70;	goma_pos[28] = 71;
	goma_pos[29] = 72;	goma_pos[30] = 73;	goma_pos[31] = 74;	goma_pos[32] = 30;
	goma_pos[33] = 29;	goma_pos[34] = 28;	goma_pos[35] = 27;	goma_pos[36] = 26;
	goma_pos[37] = 25;	goma_pos[38] = 24;	goma_pos[39] = 23;	goma_pos[40] = 22;

	goma_cde[1] = 7;	goma_cde[2] = 23;	goma_cde[3] = 5;	goma_cde[4] = 4;
	goma_cde[5] = 21;	goma_cde[6] = 20;	goma_cde[7] = 1;	goma_cde[8] = 2;
	goma_cde[9] = 3;	goma_cde[10] = 6;	goma_cde[11] = 6;	goma_cde[12] = 3;
	goma_cde[13] = 2;	goma_cde[14] = 1;	goma_cde[15] = 17;	goma_cde[16] = 18;
	goma_cde[17] = 19;	goma_cde[18] = 22;	goma_cde[19] = 22;	goma_cde[20] = 19;
	goma_cde[21] = 18;	goma_cde[22] = 17;	goma_cde[23] = 0;	goma_cde[24] = 0;
	goma_cde[25] = 0;	goma_cde[26] = 0;	goma_cde[27] = 0;	goma_cde[28] = 0;
	goma_cde[29] = 0;	goma_cde[30] = 0;	goma_cde[31] = 0;	goma_cde[32] = 16;
	goma_cde[33] = 16;	goma_cde[34] = 16;	goma_cde[35] = 16;	goma_cde[36] = 16;
	goma_cde[37] = 16;	goma_cde[38] = 16;	goma_cde[39] = 16;	goma_cde[40] = 16;

	for(uint8_t i = 1; i<=40; i++) {
		board[goma_pos[i]] = i;
	}

	round = 0;

}

vector<uint16_t> Shogi::FetchMoves(uint8_t mode) {

	vector<uint16_t> movelist;
	movelist.reserve(1024);

	uint8_t chesser = (round & 1);
	uint8_t rival = 1 - (round & 1);

	uint8_t mdr = (chesser == 0 ? 1 : -1);
	uint8_t rdr = (chesser == 0 ? -1 : 1);

	uint8_t mking = (chesser == 0 ? 1 : 2);
	uint8_t rking = (chesser == 0 ? 2 : 1);

	uint8_t rival_direct_attack_graph[97]	=	{0};
	uint8_t rival_flow_attack_graph[97]		=	{0};
	uint8_t mine_general_attack_graph[97]	=	{0};


	bool goma_checked[41] = {false};

	// Check the rival's attack graph

	uint8_t critical_blocker_list[9] = {0};
	uint8_t critical_direction_list[9] = {0};
	uint8_t cbl_length = 0;
	uint8_t cbl_counter = 0;

	for(uint8_t goma_idx = 1; goma_idx <= 40; goma_idx++) {
		
		uint8_t owner = ((goma_cde[goma_idx] & MASK_goma_owner) >> 4);
		if(owner == chesser) continue;

		goma_checked[goma_idx] = true;

		uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
		if(notonboard) continue;

		uint8_t fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));
		uint8_t pos = goma_pos[goma_idx];

		for(uint8_t idx = goma_index_vector[fulid]; idx < goma_index_vector[fulid+1]; idx++) {
			if(goma_flow_vector[idx]) {
				uint8_t uv = uv_table[goma_move_vector[idx]] * rdr;
				uint8_t npos = pos + uv;
				bool direct = true;
				uint8_t blocker = 0;
				while(!outboard(npos)) {
					uint8_t focus = board[npos];
					if(direct) rival_flow_attack_graph[npos] |= ((uint8_t)1 << goma_move_vector[idx]);
					if(focus > 0) {
						if(direct) {
							if(focus != mking) {
								direct = false;
								if(((goma_cde[focus] & MASK_goma_owner) >> 4) == chesser){
									blocker = focus;
								}
								else break;
							}
						} else {
							if(focus == mking) {
								critical_blocker_list[cbl_length] = blocker;
								critical_direction_list[cbl_length] = uv;
								cbl_length++;
							}
							break;
						}
					}
					npos += uv;
				}
			}
			else {
				uint8_t npos = pos + goma_move_vector[idx] * rdr;
				if(outboard(npos)) continue;
				uint8_t focus = board[npos];
				rival_direct_attack_graph[npos] = goma_idx;
			}
		}
	}

	// Check the true move with those given information

	uint8_t mkpos = goma_pos[mking];
	uint8_t direct_attacker = rival_direct_attack_graph[mkpos];

	uint8_t n_direct_attack = (direct_attacker != 0);
	uint8_t n_flow_attack =
	(rival_flow_attack_graph[mkpos] & 1) +
	((rival_flow_attack_graph[mkpos] & 2) >> 1) + 
	((rival_flow_attack_graph[mkpos] & 4) >> 2) + 
	((rival_flow_attack_graph[mkpos] & 8) >> 3) + 
	((rival_flow_attack_graph[mkpos] & 16) >> 4) + 
	((rival_flow_attack_graph[mkpos] & 32) >> 5) + 
	((rival_flow_attack_graph[mkpos] & 64) >> 6) + 
	((rival_flow_attack_graph[mkpos] & 128) >> 7);


	bool fast_leave_flag = false;
	bool dpos_flag = false;
	bool no_placing_flag = false;
	bool dpos[97] = {false};

	if(n_flow_attack + n_direct_attack >= 2) {
		fast_leave_flag = true;
	} else if(n_flow_attack == 1) {
		uint8_t defense_direction = 0;
		for(uint8_t i = 0; i < 8; i++) {
			if(rival_flow_attack_graph[mkpos] & (1 << i)){
				defense_direction = i;
				break;
			}
		}
		defense_direction = uv_table[defense_direction] * (-1) * rdr;
		uint8_t apos = mkpos + defense_direction;
		while(!outboard(apos)) {
			dpos[apos] = true;
			if(board[apos] != 0) break;
			apos += defense_direction;
		}
		dpos_flag = true;
	} else if(n_direct_attack == 1) {
		dpos[goma_pos[direct_attacker]] = true;
		dpos_flag = true;
		no_placing_flag = true;
	}

	for(int idx = goma_index_vector[7]; idx < goma_index_vector[8]; idx++) {
		uint8_t npos = mkpos + goma_move_vector[idx] * mdr;
		if(outboard(npos)) continue;
		mine_general_attack_graph[npos] = 1;
		if(rival_direct_attack_graph[npos] || rival_flow_attack_graph[npos]) continue;
		else {
			uint8_t focus = board[npos];
			if(focus == 0 || ((goma_cde[focus] & MASK_goma_owner) >> 4) == rival)
				movelist.push_back(((uint16_t)npos << 7) + (uint16_t)mkpos);
		}
	}
	goma_checked[mking] = true;

	if(fast_leave_flag) return movelist;
	// Fast leave of two-attack case.

	sort(critical_blocker_list, critical_blocker_list + cbl_length);

	bool placing_goma[9] = {false};
	bool nifu[9] = {false};

	for(uint8_t goma_idx = 3; goma_idx <= 40; goma_idx++) {
		if(goma_checked[goma_idx]) continue;
		uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
		uint8_t fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));
		if(notonboard) {
			placing_goma[fulid] = true;
		}else {
			if(fulid == 0) nifu[goma_pos[goma_idx] % 11] = true;
		}
	}

	for(uint8_t goma_idx = 3; goma_idx <= 40; goma_idx++) {
		if(goma_checked[goma_idx]) continue;
		uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
		uint8_t fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));

		if(notonboard) {
			if(no_placing_flag) continue;
			for(uint8_t g = 0; g < 7; g++) {
				if(!placing_goma[g])continue;
				for(uint8_t pos = 0; pos < 97; pos++) {
					if(outboard(pos)) continue;
					if(dpos_flag&&!dpos[pos]) continue;
					uint8_t focus = board[pos];
					if(focus != 0) continue;
					if(g <= 1 && (uint8_t)(96 * chesser + pos * mdr) < 9) continue;
					if(g == 2 && (uint8_t)(96 * chesser + pos * mdr) < 20) continue;
					if(g != 0 || !nifu[pos % 11]) {
						movelist.push_back(32768 + ((uint16_t)pos << 7) + g);
					}					
				}
			}
			continue;
		}

		uint8_t pos = goma_pos[goma_idx];
		
		bool critical_blocker_flag = false;
		uint8_t critical_direction = 0;
		if(critical_blocker_list[cbl_counter] == goma_idx) {
			critical_blocker_flag = true;
			critical_direction = critical_blocker_list[cbl_counter++];
		}

		for(uint8_t idx = goma_index_vector[fulid]; idx < goma_index_vector[fulid+1]; idx++) {
			if(goma_flow_vector[idx]) {

				uint8_t uv = uv_table[goma_move_vector[idx]] * mdr;
				if(critical_blocker_flag) {
					if(uv != critical_direction && uv != (uint8_t)((-1) * critical_direction)) {
						continue;
					}
				}

				for(uint8_t npos = pos + uv; !outboard(npos); npos += uv) {
					uint8_t focus = board[npos];
					mine_general_attack_graph[npos] = 1;
					
					if(dpos_flag) {
						if(!dpos[npos]) {
							if(focus != 0)break;
							else continue;
						}
						if(fulid < 7 && (((uint8_t)(96 * chesser + pos * mdr) < 31) || ((uint8_t)(96 * chesser + npos * mdr) < 31))) {
							movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
						}
						if(fulid != 1 || ((uint8_t)(96 * chesser + npos * mdr) > 10)) {
							movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);
						}
						if(focus != 0)break;
						else continue;
					}

					if(focus == 0) {
						if(fulid < 7 && (((uint8_t)(96 * chesser + pos * mdr) < 31) || ((uint8_t)(96 * chesser + npos * mdr) < 31))) {
							movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
						}
						if(fulid != 1 || ((uint8_t)(96 * chesser + npos * mdr) > 10)) {
							movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);
						}
					} else {
						if(((goma_cde[focus] & MASK_goma_owner) >> 4) == rival) {
							if(fulid < 7 && (((uint8_t)(96 * chesser + pos * mdr) < 31) || ((uint8_t)(96 * chesser + npos * mdr) < 31))) {
								movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
							}
							if(fulid != 1 || ((uint8_t)(96 * chesser + npos * mdr) > 10)) {
								movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);
							}
						}
						break;
					}
				}

			} else {
				uint8_t uv = goma_move_vector[idx] * mdr;
				uint8_t npos = pos + uv;
				if(outboard(npos)) continue;
				mine_general_attack_graph[npos] = 1;

				if(critical_blocker_flag) {
					if(uv != critical_direction && uv != (uint8_t)((-1) * critical_direction)) {
						continue;
					}
				}

				uint8_t focus = board[npos];

				if(dpos_flag&&!dpos[npos]) continue;
				if(focus != 0 && ((goma_cde[focus] & MASK_goma_owner) >> 4) != rival) continue;

				if(fulid < 4 && (((uint8_t)(96 * chesser + pos * mdr) < 31) || ((uint8_t)(96 * chesser + npos * mdr) < 31))) {
					movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
				}

				if(fulid == 0 && (uint8_t)(96 * chesser + pos * mdr) < 20) continue;
				if(fulid == 2 && (uint8_t)(96 * chesser + pos * mdr) < 42) continue;

				movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);
			}
		}
	}

	if(mode == 0) return movelist;
	return movelist;
}

void Shogi::MakeMove(uint16_t move) {
	
	uint16_t placing_flag = (move & MASK_move_placing);
	uint16_t upgrade_flag = (move & MASK_move_upgrade);
	uint16_t npos = ((move & MASK_move_npos) >> 7);
	uint16_t pos = (move & MASK_move_pos);
	uint8_t chesser = (round & 1);

	if(placing_flag) {

		uint8_t target_gomaidx = 0;
		for(int goma_idx=3;goma_idx<=40;goma_idx++) {
			uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
			uint8_t owner = ((goma_cde[goma_idx] & MASK_goma_owner) >> 4);
			uint8_t fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));

			if(!notonboard) continue;
			if(chesser != owner) continue;
			if(fulid != pos) continue;

			target_gomaidx = goma_idx;
			break;
		}

		board[npos] = target_gomaidx;
		goma_pos[target_gomaidx] = npos;
		goma_cde[target_gomaidx] ^= MASK_goma_onboard;

	} else {

		uint8_t target_gomaidx = board[pos];
		uint8_t subject_gomaidx = board[npos];
		
		goma_pos[target_gomaidx] = npos;
		board[pos] = 0;
		board[npos] = target_gomaidx;		
		if(upgrade_flag)
			goma_cde[target_gomaidx] ^= MASK_goma_upgrade;				
			
		if(subject_gomaidx != 0) {	
			goma_pos[subject_gomaidx] = 97 + chesser;
			goma_cde[subject_gomaidx] ^= MASK_goma_onboard;
			goma_cde[subject_gomaidx] &= ~MASK_goma_upgrade;
			goma_cde[subject_gomaidx] ^= MASK_goma_owner;
		}
	}

	round += 1;
}