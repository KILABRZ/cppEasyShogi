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
246,244,10,12,4,3,6,1,
2,0,5,7,1,255,11,245
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
0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0
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

	uint8_t direct_attack_graph[97]			=	{0};
	uint8_t first_flow_attack_graph[97]		=	{0};
	uint8_t second_flow_attack_graph[97]	=	{0};
	uint8_t owner_fatal_weakness[8]			=	{0};
	uint8_t rival_fatal_weakness[8]			=	{0};

	uint8_t chesser = (round & 1);

	uint8_t placing_goma[8] = {0};
	uint8_t mdr = (chesser == 0 ? 1 : -1);
	uint8_t nifu[9] = {0};

	// basic moving rule procedure
	for(uint8_t goma_idx = 1; goma_idx <= 40; goma_idx++) {
		
		uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
		uint8_t owner = ((goma_cde[goma_idx] & MASK_goma_owner) >> 4);
		uint8_t fulid = ((goma_cde[goma_idx] & MASK_goma_fulid));
		uint8_t dr = (owner == 0 ? 1 : -1);

		bool addmove_flag = (owner == chesser);

		if(!notonboard) {
			uint8_t pos = goma_pos[goma_idx];
			if(fulid == 0 and owner == chesser) {
				nifu[pos % 11] = 1;
			}

			for(int idx = goma_index_vector[fulid]; idx < goma_index_vector[fulid+1]; idx++) {
				if(goma_flow_vector[idx]) {
					uint8_t uv = uv_table[goma_move_vector[idx]] * dr;
					uint8_t npos = pos + uv;
					bool second_flow_start = false;
					uint8_t tmp_blocker = 0;

					while(!outboard(npos)) {
						uint8_t unit = board[npos];
						if(second_flow_start) second_flow_attack_graph[npos] |= (owner+1);
						else first_flow_attack_graph[npos] += 1 + owner * 15;

						if(unit > 2) {
							if(second_flow_start) break;
							else {
								second_flow_start = true;
								tmp_blocker = unit;
								if((((goma_cde[unit] & MASK_goma_owner) >> 4) != owner) && addmove_flag) {
									if(fulid < 6) {
										if((uint8_t)(96 * owner + pos * dr) < 31 || (uint8_t)(96 * owner + npos * dr) < 31) {
											movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
										}
									}
									if(fulid != 1 || (uint8_t)(96 * owner + npos * dr) > 9) {
										movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);	
									}
								}
							}
						} else {
							if(unit == 0){
								if(addmove_flag && !second_flow_start) {
									if(fulid < 6) {
										if((uint8_t)(96 * owner + pos * dr) < 31 || (uint8_t)(96 * owner + npos * dr) < 31) {
											movelist.push_back(16384 + ((uint16_t)npos << 7) + (uint16_t)pos);
										}
									}
									if(fulid != 1 || (uint8_t)(96 * owner + npos * dr) > 9) {
										movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);	
									}
								}
							}
							else if(unit == owner+1) {
								if(second_flow_start) break;
								else {
									second_flow_start = true;
									tmp_blocker = unit;
								}
							} else {
								if(second_flow_start) {
									if(owner == chesser) {
										rival_fatal_weakness[goma_move_vector[idx]] = tmp_blocker;
									} else {
										owner_fatal_weakness[goma_move_vector[idx]] = tmp_blocker;
									}
									break;
								}
								// else (first flow attack rival king ? --> ignore it as space)
								// it must not be movable (you can never attack your oppoent's king)
							}
						}
						npos += uv;
					}
				} else {

					uint8_t npos = pos + goma_move_vector[idx] * dr;
					if(outboard(npos)) continue;
					
					uint8_t unit = board[npos];
					direct_attack_graph[npos] |= (owner+1);
					if(!addmove_flag) continue;

					if(unit == 0 ? 0 : (((goma_cde[unit] & MASK_goma_owner) >> 4) == owner)) continue;
					if(fulid < 4){
						if((uint8_t)(96 * owner + pos * dr) < 31 || (uint8_t)(96 * owner + npos * dr) < 31) {
							movelist.push_back(16384 + ((unsigned short)npos << 7) + (unsigned short)pos);
						}
					}

					if(fulid == 0 && (uint8_t)(96 * owner + pos * dr) < 20) continue;
					if(fulid == 2 && (uint8_t)(96 * owner + pos * dr) < 42) continue;
					
					movelist.push_back(((uint16_t)npos << 7) + (uint16_t)pos);

				}
			}
		} else {
			if(owner != chesser) continue;
			if(placing_goma[fulid]) continue;
			placing_goma[fulid] = goma_idx;
		}
	}


	for(uint8_t pos = 0; pos < 97; pos++) {
		if(outboard(pos))continue;
		if(board[pos])continue;

		if(placing_goma[0] and !nifu[pos % 11] and (uint8_t)(96 * chesser + pos * mdr) > 10) {
			movelist.push_back(32768 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[1] and (uint8_t)(96 * chesser + pos * mdr) > 10) {
			movelist.push_back(32769 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[2] and (uint8_t)(96 * chesser + pos * mdr) > 21) {
			movelist.push_back(32770 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[3]) {
			movelist.push_back(32771 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[4]) {
			movelist.push_back(32772 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[5]) {
			movelist.push_back(32773 + ((uint16_t)pos << 7));
		}
		else if(placing_goma[6]) {
			movelist.push_back(32774 + ((uint16_t)pos << 7));
		}
	}

	if(mode == 0) return movelist;

	// Filter move that ignores checkmate (or even worse, cause checkmated)
	
	vector<uint16_t> nmovelist;
	nmovelist.reserve(movelist.size());

	uint8_t chesser_idc = chesser + 1;
	uint8_t chesser_fmask = 15 + 105 * chesser;
	uint8_t kpos = goma_pos[chesser_idc];
	
	uint8_t direct_attack = ((direct_attack_graph[kpos] & chesser_idc) ? 1 : 0);
	uint8_t fflow_attack = ((first_flow_attack_graph[kpos] & chesser_fmask) >> (4*chesser));

	if(direct_attack + fflow_attack != 0) {

	}

	




	// Test used
	/*
	cout << "Direct Attack\n";

	for(uint8_t p=0;p<97;p++) {
		if(outboard(p)) {
			cout << "\n";
			continue;
		}
		uint8_t unit = board[p];

		cout << (int)direct_attack_graph[p] << "  ";
	}
	cout << "\n\n";

	cout << "First Flow Attack\n";

	for(uint8_t p=0;p<97;p++) {
		if(outboard(p)) {
			cout << "\n";
			continue;
		}
		uint8_t unit = board[p];

		cout << (int)first_flow_attack_graph[p] << "  ";
	}
	cout << "\n\n";

	cout << "Second Flow Attack\n";

	for(uint8_t p=0;p<97;p++) {
		if(outboard(p)) {
			cout << "\n";
			continue;
		}
		uint8_t unit = board[p];

		cout << (int)second_flow_attack_graph[p] << "  ";
	}
	cout << "\n";
	*/
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