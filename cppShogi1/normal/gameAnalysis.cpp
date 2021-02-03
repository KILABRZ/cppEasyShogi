#include "shogi.hpp"

double G[50] = {10, 0.1, 0.1, 0.1, 10};

int gomaStrength(Shogi s, vector<char> jV){
	int offset = 0;
	int senteS = 0;
	int goteS = 0;
	for(int i=0;i<40;i++){
		if(s.gomaPos[i] == -1)continue;
		if(gomakindChesser(s.gomaKind[i]) == SENTE){
			senteS += jV[offset + gomakindEID(s.gomaKind[i])];
		}else {
			goteS += jV[offset + gomakindEID(s.gomaKind[i])];
		}
	}
	for(int i=0;i<8;i++){
		senteS += jV[offset + i] * s.gomaTable[i].size();
		goteS += jV[offset + i] * s.gomaTable[i+8].size();
	}
	return (int)((double)(senteS - goteS) * G[0]);
}

int aggressing(Shogi s, vector<char> jV){
	int offset = 14;
	int senteS = 0;
	int goteS = 0;
	for(int i=0;i<40;i++){
		if(s.gomaPos[i] == -1)continue;
		if(gomakindChesser(s.gomaKind[i]) == SENTE){
			senteS += (10 - posDan(s.gomaPos[i])) * jV[offset + gomakindEID(s.gomaKind[i])];
		}else {
			goteS += (posDan(s.gomaPos[i])) * jV[offset + gomakindEID(s.gomaKind[i])];
		}
	}
	return (int)((double)(senteS - goteS) * G[1]);
}

int spacing(Shogi s, vector<char> jV){
	int offset = 28;
	int senteS = 0;
	int goteS = 0;
	for(int pos=0;pos<81;pos++){

		if(s.boardFixedAttacking[SENTE][pos].size() != 0){
			for(int watchup : s.boardFixedAttacking[SENTE][pos]){
				int attacker = watchupAttacker(watchup);
				int gomakind = gomakindEID(s.gomaKind[attacker]);
				senteS += jV[offset+gomakind];
			}
		}
		if(s.boardFlowAttacking[SENTE][pos].size() != 0){
			for(int watchup : s.boardFixedAttacking[SENTE][pos]){
				int attacker = watchupAttacker(watchup);
				int gomakind = gomakindEID(s.gomaKind[attacker]);
				senteS += jV[offset+gomakind];
			}
		}
		if(s.boardFixedAttacking[GOTE][pos].size() != 0){
			for(int watchup : s.boardFixedAttacking[GOTE][pos]){
				int attacker = watchupAttacker(watchup);
				int gomakind = gomakindEID(s.gomaKind[attacker]);
				goteS += jV[offset+gomakind];
			}
		}
		if(s.boardFixedAttacking[GOTE][pos].size() != 0){
			for(int watchup : s.boardFixedAttacking[GOTE][pos]){
				int attacker = watchupAttacker(watchup);
				int gomakind = gomakindEID(s.gomaKind[attacker]);
				goteS += jV[offset+gomakind];
			}
		}
	}
	return (int)((double)(senteS - goteS) * G[2]);
}

int posDistance(int posA, int posB){
	int asuji = posSuji(posA);
	int adan = posDan(posA);
	int bsuji = posSuji(posB);
	int bdan = posDan(posB);
	return ((asuji - adan)*(asuji - adan) + (bsuji - bdan)*(bsuji - bdan)) / 10;
}

int generalAttacking(Shogi s, vector<char> jV){
	int senteKingpos = s.gomaPos[s.SENTEKINGNUM];
	int goteKingpos = s.gomaPos[s.GOTEKINGNUM];
	int offset = 42;
	int senteS = 0;
	int goteS = 0;
	for(int i=0;i<40;i++){
		if(s.gomaPos[i] == -1)continue;
		if(gomakindChesser(s.gomaKind[i]) == SENTE){
			senteS += posDistance(s.gomaPos[i], goteKingpos) * jV[offset + gomakindEID(s.gomaKind[i])];
		}else {
			goteS += posDistance(s.gomaPos[i], senteKingpos) * jV[offset + gomakindEID(s.gomaKind[i])];
		}
	}
	return (int)((double)(senteS - goteS) * G[3]);
}

int kingAttacking(Shogi s, vector<char> jV){
	int senteKingpos = s.gomaPos[s.SENTEKINGNUM];
	int goteKingpos = s.gomaPos[s.GOTEKINGNUM];
	int offset = 56;
	int senteS = 0;
	int goteS = 0;
	for(int watchup : s.boardFixedAttacking[SENTE][goteKingpos]){
		int attacker = watchupAttacker(watchup);
		int gomakind = s.gomaKind[attacker];
		int attackerpos = s.gomaPos[attacker];

		int attackingPosSafe = 
			s.boardFixedAttacking[SENTE][attackerpos].size() +
			s.boardFlowAttacking[SENTE][attackerpos].size();

		if(attackingPosSafe != 0){
			senteS += jV[offset + gomakind] * jV[offset + 15];
		}else{
			senteS += jV[offset + gomakind] * jV[offset + 14];
		}
	}
	for(int watchup : s.boardFlowAttacking[SENTE][goteKingpos]){
		int attacker = watchupAttacker(watchup);
		int gomakind = s.gomaKind[attacker];
		int attackerpos = s.gomaPos[attacker];

		int attackingPosSafe = 
			s.boardFixedAttacking[SENTE][attackerpos].size() +
			s.boardFlowAttacking[SENTE][attackerpos].size();

		if(attackingPosSafe != 0){
			senteS += jV[offset + gomakind] * jV[offset + 15];
		}else{
			senteS += jV[offset + gomakind] * jV[offset + 14];
		}
	}
	for(int watchup : s.boardFixedAttacking[GOTE][senteKingpos]){
		int attacker = watchupAttacker(watchup);
		int gomakind = s.gomaKind[attacker];
		int attackerpos = s.gomaPos[attacker];

		int attackingPosSafe = 
			s.boardFixedAttacking[GOTE][attackerpos].size() +
			s.boardFlowAttacking[GOTE][attackerpos].size();

		if(attackingPosSafe != 0){
			goteS += jV[offset + gomakind] * jV[offset + 15];
		}else{
			goteS += jV[offset + gomakind] * jV[offset + 14];
		}
	}
	for(int watchup : s.boardFlowAttacking[GOTE][senteKingpos]){
		int attacker = watchupAttacker(watchup);
		int gomakind = s.gomaKind[attacker];
		int attackerpos = s.gomaPos[attacker];

		int attackingPosSafe = 
			s.boardFixedAttacking[GOTE][attackerpos].size() +
			s.boardFlowAttacking[GOTE][attackerpos].size();

		if(attackingPosSafe != 0){
			goteS += jV[offset + gomakind] * jV[offset + 15];
		}else{
			goteS += jV[offset + gomakind] * jV[offset + 14];
		}
	}

	return (int)((double)(senteS - goteS) * G[4]);
}