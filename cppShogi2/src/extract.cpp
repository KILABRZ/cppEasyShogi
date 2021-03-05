#include "shogi.hpp"

vector<vector<int8_t>> Shogi::BoardKnowledgeExtract() {
	vector<vector<int8_t>> extractions;

	for(int8_t dan = 0; dan <= 88; dan+=11) {
		for(int8_t suji = 0; suji <= 8; suji++) {
			uint8_t pos = dan + suji;
			// owner, (one-hot-encode)
			vector<int8_t> encoding(16, 0);
			if(board[pos] == 0) {
				extractions.push_back(encoding);
			} else {
				uint8_t owner = ((goma_cde[board[pos]] & MASK_goma_owner) >> 4);
				uint8_t fulid = ((goma_cde[board[pos]] & MASK_goma_fulid));
				if(owner == 0) encoding[0] = 1;
				else encoding[1] = -1;
				encoding[fulid+1] = 1;
				extractions.push_back(encoding);
			}
		}
	}

	vector<int8_t> sente(16, 0);
	vector<int8_t> gote(16, 0);
	sente[0] = 1;
	gote[0] = -1;

	for(uint8_t goma_idx = 1; goma_idx <= 40; goma_idx++) {
		uint8_t notonboard = ((goma_cde[goma_idx] & MASK_goma_onboard) >> 5);
		uint8_t pos = goma_pos[goma_idx];
		if(notonboard) {
			uint8_t owner = ((goma_cde[board[pos]] & MASK_goma_owner) >> 4);
			uint8_t fulid = ((goma_cde[board[pos]] & MASK_goma_fulid));
			if(owner == 0)sente[fulid+1]++;
			else gote[fulid+1]++;
		}
	}

	extractions.push_back(sente);
	extractions.push_back(gote);

	return extractions;
}