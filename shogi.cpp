#include "shogi.hpp"
#include <cmath>
/* decide moving rule */
int movingD[14][8] = {
	{0}, {0, 0, 0, 0, 0}, {0, 0}, {1}, {1, 1, 1, 1},
	{1, 1, 1, 1}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 1, 1}, {0, 0, 0, 0, 1, 1, 1, 1}
};

int movingDlength[14] = {
	1, 5, 2, 1, 4, 4, 8, 6, 6, 6, 6, 6, 8, 8
};

int danD[14][8] = {
	{-1}, {-1, -1, -1, 1, 1}, {-2, -2}, {-1}, {-1, 0, 1, 0}, {-1, -1, 1, 1},
	{-1, -1, -1, 1, 1, 1, 0, 0}, {-1, -1, -1, 0, 0, 1}, {-1, -1, -1, 0, 0, 1},
	{-1, -1, -1, 0, 0, 1}, {-1, -1, -1, 0, 0, 1}, {-1, -1, -1, 0, 0, 1}, 
	{-1, -1, 1, 1, -1, 0, 1, 0}, {-1, 0, 1, 0, -1, -1, 1, 1}
};

int sujiD[14][8] = {
	{0}, {-1, 0, 1, -1, 1}, {-1, 1}, {0}, {0, -1, 0, 1}, {1, -1, 1, -1},
	{1, 0, -1, 1, 0, -1, 1, -1}, {1, 0, -1, 1, -1, 0}, {1, 0, -1, 1, -1, 0}, 
	{1, 0, -1, 1, -1, 0}, {1, 0, -1, 1, -1, 0}, {1, 0, -1, 1, -1, 0}, 
	{1, -1, 1, -1, 0, 1, 0, -1}, {0, 1, 0, -1, 1, -1, 1, -1} 
};

void Shogi::Init(){

	for(int i=0;i<81;i++){
		board[i] = -1;
		gomaKind[i] = -1;
		gomaPos[i] = -1;
		boardChesser[i] = -1;
		boardFixedAttacking[SENTE][i].clear();
		boardFlowAttacking[SENTE][i].clear();
		boardBFlowAttacking[SENTE][i].clear();

		boardFixedAttacking[GOTE][i].clear();
		boardFlowAttacking[GOTE][i].clear();
		boardBFlowAttacking[GOTE][i].clear();

		boardFixedAttacking[SENTE][i].reserve(10);
		boardFlowAttacking[SENTE][i].reserve(10);
		boardBFlowAttacking[SENTE][i].reserve(10);

		boardFixedAttacking[GOTE][i].reserve(10);
		boardFlowAttacking[GOTE][i].reserve(10);
		boardBFlowAttacking[GOTE][i].reserve(10);
	}

	SENTEKINGNUM = 10;
	GOTEKINGNUM = 30;

	gomaKind[ 0] = genGomakind(CHARIOT, NORMAL, SENTE);		gomaPos[ 0] = genPos(9, 9);		
	gomaKind[ 1] = genGomakind(CHARIOT, NORMAL, SENTE);		gomaPos[ 1] = genPos(1, 9);
	gomaKind[ 2] = genGomakind(CASSIA, NORMAL, SENTE);		gomaPos[ 2] = genPos(8, 9);
	gomaKind[ 3] = genGomakind(CASSIA, NORMAL, SENTE);		gomaPos[ 3] = genPos(2, 9);
	gomaKind[ 4] = genGomakind(SILVER, NORMAL, SENTE);		gomaPos[ 4] = genPos(7, 9);
	gomaKind[ 5] = genGomakind(SILVER, NORMAL, SENTE);		gomaPos[ 5] = genPos(3, 9);
	gomaKind[ 6] = genGomakind(GOLD, NORMAL, SENTE);		gomaPos[ 6] = genPos(6, 9);
	gomaKind[ 7] = genGomakind(GOLD, NORMAL, SENTE);		gomaPos[ 7] = genPos(4, 9);
	gomaKind[ 8] = genGomakind(FLYING, NORMAL, SENTE);		gomaPos[ 8] = genPos(2, 8);
	gomaKind[ 9] = genGomakind(ANGLE, NORMAL, SENTE);		gomaPos[ 9] = genPos(8, 8);
	gomaKind[10] = genGomakind(KING, NORMAL, SENTE);		gomaPos[10] = genPos(5, 9);
	gomaKind[11] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[11] = genPos(9, 7);
	gomaKind[12] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[12] = genPos(8, 7);
	gomaKind[13] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[13] = genPos(7, 7);
	gomaKind[14] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[14] = genPos(6, 7);
	gomaKind[15] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[15] = genPos(5, 7);
	gomaKind[16] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[16] = genPos(4, 7);
	gomaKind[17] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[17] = genPos(3, 7);
	gomaKind[18] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[18] = genPos(2, 7);
	gomaKind[19] = genGomakind(FOOT, NORMAL, SENTE);		gomaPos[19] = genPos(1, 7);

	gomaKind[20] = genGomakind(CHARIOT, NORMAL, GOTE);		gomaPos[20] = genPos(1, 1);
	gomaKind[21] = genGomakind(CHARIOT, NORMAL, GOTE);		gomaPos[21] = genPos(9, 1);
	gomaKind[22] = genGomakind(CASSIA, NORMAL, GOTE);		gomaPos[22] = genPos(2, 1);
	gomaKind[23] = genGomakind(CASSIA, NORMAL, GOTE);		gomaPos[23] = genPos(8, 1);
	gomaKind[24] = genGomakind(SILVER, NORMAL, GOTE);		gomaPos[24] = genPos(3, 1);
	gomaKind[25] = genGomakind(SILVER, NORMAL, GOTE);		gomaPos[25] = genPos(7, 1);
	gomaKind[26] = genGomakind(GOLD, NORMAL, GOTE);			gomaPos[26] = genPos(4, 1);
	gomaKind[27] = genGomakind(GOLD, NORMAL, GOTE);			gomaPos[27] = genPos(6, 1);
	gomaKind[28] = genGomakind(FLYING, NORMAL, GOTE);		gomaPos[28] = genPos(8, 2);
	gomaKind[29] = genGomakind(ANGLE, NORMAL, GOTE);		gomaPos[29] = genPos(2, 2);
	gomaKind[30] = genGomakind(KING, NORMAL, GOTE);			gomaPos[30] = genPos(5, 1);
	gomaKind[31] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[31] = genPos(1, 3);
	gomaKind[32] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[32] = genPos(2, 3);
	gomaKind[33] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[33] = genPos(3, 3);
	gomaKind[34] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[34] = genPos(4, 3);
	gomaKind[35] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[35] = genPos(5, 3);
	gomaKind[36] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[36] = genPos(6, 3);
	gomaKind[37] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[37] = genPos(7, 3);
	gomaKind[38] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[38] = genPos(8, 3);
	gomaKind[39] = genGomakind(FOOT, NORMAL, GOTE);			gomaPos[39] = genPos(9, 3);

	for(int i=0;i<40;i++){
		board[gomaPos[i]] = i;
		boardChesser[gomaPos[i]] = gomakindChesser(gomaKind[i]);
	}

	for(int i=0;i<16;i++){
		queue<int> gomaQ;
		gomaTable.push_back(gomaQ);
	}

	round = 0;
}

double distance(double x, double y){
	return sqrt(x*x+y*y);
}

bool equal(double x, double y){
	if(x - y >= -0.0001 and x - y <= 0.0001)return true;
	else return false;
}

bool posOnLine(int posA, int posB, int posC){
	double x2 = posSuji(posA) - posSuji(posB);
	double y2 = posDan(posA) - posDan(posB);
	double x1 = posSuji(posA) - posSuji(posC);
	double y1 = posDan(posA) - posDan(posC);
	double d1 = distance(x1, y1);
	double d2 = distance(x2, y2);

	x1 /= d1;	x2 /= d2;
	y1 /= d1;	y2 /= d2;

	if(equal(x1, x2) and equal(y1, y2))return true;
	if(equal(-1*x1, x2) and equal(-1*y1, y2))return true;

	return false;
}

bool posInMiddle(int posL, int posM, int posR){
	double x2 = posSuji(posL) - posSuji(posR);
	double y2 = posDan(posL) - posDan(posR);
	double x1 = posSuji(posL) - posSuji(posM);
	double y1 = posDan(posL) - posDan(posM);
	double d1 = distance(x1, y1);
	double d2 = distance(x2, y2);
	if(equal(d1, 0) or equal(d2, 0))return false;
	if(d2 < d1)return false;

	x1 /= d1;	x2 /= d2;
	y1 /= d1;	y2 /= d2;
	if(equal(x1, x2) and equal(y1, y2))return true;

	return false;
}

void Shogi::NumberBoardPrint(){
	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(board[x] < 10 and board[x] >= 0)cout << "0";
			cout << board[x] << " ";
		}
		cout << "\n";
	}

	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(boardChesser[x] >= 0) cout <<  " ";
			cout << boardChesser[x] << " ";
		}
		cout << "\n";
	}
}

void Shogi::EasyBoardPrint(){

	string gomaPresentation[16] = {
		"f", "s", "k", "l", "t", "x", "O", "G",
		"F", "S", "K", "L", "D", "X", "O", "G"
	};

	string chesserPresentation[2] = {
		"+", "-"
	};

	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(board[x] == -1){
				cout << ".. ";
			}else{
				int eid = gomakindEID(gomaKind[board[x]]);
				int chesser = gomakindChesser(gomaKind[board[x]]);
				cout << chesserPresentation[chesser] << gomaPresentation[eid] << " ";
			}
		}
		cout << "\n";
	}
	cout << "\nGOMA  f l k s g x t\n";
	cout << "SENTE " << gomaTable[FOOT].size() << " " << gomaTable[CHARIOT].size() << " "
	 << gomaTable[CASSIA].size() << " " << gomaTable[SILVER].size() << " " << gomaTable[GOLD].size() << " " 
	 << gomaTable[ANGLE].size() << " " << gomaTable[FLYING].size() << " ";
	 cout << "\n";
	 cout << "GOTE  " << gomaTable[FOOT+8].size() << " " << gomaTable[CHARIOT+8].size() << " "
	 << gomaTable[CASSIA+8].size() << " " << gomaTable[SILVER+8].size() << " " << gomaTable[GOLD+8].size() << " " 
	 << gomaTable[ANGLE+8].size() << " " << gomaTable[FLYING+8].size() << " ";
	 cout << "\n";
}

void Shogi::PrintAttackBoard(){
	cout << "Fixed:\n";
	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(boardFixedAttacking[SENTE][x].size() == 0
				and boardFixedAttacking[GOTE][x].size() == 0) cout << ". ";
			else if(boardFixedAttacking[GOTE][x].size() == 0) cout << "^ ";
			else if(boardFixedAttacking[SENTE][x].size() == 0) cout << "v ";
			else cout << "x ";
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "Flow:\n";
	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(boardFlowAttacking[SENTE][x].size() == 0
				and boardFlowAttacking[GOTE][x].size() == 0) cout << ". ";
			else if(boardFlowAttacking[GOTE][x].size() == 0) cout << "^ ";
			else if(boardFlowAttacking[SENTE][x].size() == 0) cout << "v ";
			else cout << "x ";
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "Blocking Flow:\n";
	for(int dan = 1; dan <= 9; dan++){
		for(int suji = 9; suji >= 1; suji--){
			int x = genPos(suji, dan);
			if(boardBFlowAttacking[SENTE][x].size() == 0
				and boardBFlowAttacking[GOTE][x].size() == 0) cout << ". ";
			else if(boardBFlowAttacking[GOTE][x].size() == 0) cout << "^ ";
			else if(boardBFlowAttacking[SENTE][x].size() == 0) cout << "v ";
			else cout << "x ";
		}
		cout << "\n";
	}
}

vector<int> Shogi::FetchMove(int request){
	int chesser = round & 1;
	int other = (chesser ^ 1);

	int updatingAttackMap = (request >= 1);
	int ruleOfSafeKing = (request >= 2);
	int ruleOfFootKill = (request >= 3);
	int threateningKing = (request >= 4);
	int noking = (request >= 5);

	if(updatingAttackMap){
		for(int i=0;i<81;i++){
			boardFixedAttacking[SENTE][i].clear();
			boardFlowAttacking[SENTE][i].clear();
			boardBFlowAttacking[SENTE][i].clear();

			boardFixedAttacking[GOTE][i].clear();
			boardFlowAttacking[GOTE][i].clear();
			boardBFlowAttacking[GOTE][i].clear();

			boardFixedAttacking[SENTE][i].reserve(10);
			boardFlowAttacking[SENTE][i].reserve(10);
			boardBFlowAttacking[SENTE][i].reserve(10);

			boardFixedAttacking[GOTE][i].reserve(10);
			boardFlowAttacking[GOTE][i].reserve(10);
			boardBFlowAttacking[GOTE][i].reserve(10);
		}
	}

	vector<int> moveList;
	moveList.reserve(1000);
	for(int i=0;i<40;i++){

		if(gomaPos[i] == -1)continue;
		
		int eid = gomakindEID(gomaKind[i]);
		int owner = gomakindChesser(gomaKind[i]);
		int danReverse = (owner == SENTE) ? 1 : -1;

		if(owner != chesser and !updatingAttackMap)continue;

		int prePos = gomaPos[i];
		int preSuji = posSuji(prePos);
		int preDan = posDan(prePos);

		for(int v=0;v<movingDlength[eid];v++){
			if(movingD[eid][v] == 0){
				int newSuji = preSuji + sujiD[eid][v];
				int newDan = preDan + danD[eid][v] * danReverse;
				int newPos = genPos(newSuji, newDan);
				if(newPos == -1)continue;

				if(updatingAttackMap){
					boardFixedAttacking[owner][newPos].push_back(genWatchup(NOBLOCKER, i));
				}
				
				if(boardChesser[newPos] == owner)continue;
				if(chesser != owner)continue;

				int normalMove = genMove(prePos, newPos, NORMAL, NORMAL);
				int upgradeMove = genMove(prePos, newPos, UPGRADED, NORMAL);

				if(owner == SENTE and newDan > 3 and preDan > 3){
					moveList.push_back(normalMove);
				}else if(owner == GOTE and newDan < 7 and newDan < 7){
					moveList.push_back(normalMove);
				}
				else if(eid != FOOT and eid != CHARIOT and eid != CASSIA){
					moveList.push_back(normalMove);
					if(eid < 6){
						moveList.push_back(upgradeMove);
					}
				}
				else if(eid == FOOT or eid == CHARIOT){
					if(owner == SENTE and newDan == 1){
						moveList.push_back(upgradeMove);
					}else if(owner == GOTE and newDan == 9){
						moveList.push_back(upgradeMove);
					}else{
						moveList.push_back(normalMove);
						moveList.push_back(upgradeMove);
					}
				}else{
					if(owner == SENTE and newDan <= 2){
						moveList.push_back(upgradeMove);
					}else if(owner == GOTE and newDan >= 8){
						moveList.push_back(upgradeMove);
					}else{
						moveList.push_back(normalMove);
						moveList.push_back(upgradeMove);
					}
				}
			}else if(movingD[eid][v] == 1){
				int s = 1;
				bool blockingMode = false;
				int blocker = NOBLOCKER;
				while(true){
					int newSuji = preSuji + sujiD[eid][v] * s;
					int newDan = preDan + danD[eid][v] * danReverse * s;
					int newPos = genPos(newSuji, newDan);
					if(newPos == -1)break;

					if(updatingAttackMap){
						if(!blockingMode){
							boardFlowAttacking[owner][newPos].push_back(genWatchup(NOBLOCKER, i));
						}
						else {
							boardBFlowAttacking[owner][newPos].push_back(genWatchup(blocker, i));
						}
					}
					
					if(boardChesser[newPos] == owner)break;

					int normalMove = genMove(prePos, newPos, NORMAL, NORMAL);
					int upgradeMove = genMove(prePos, newPos, UPGRADED, NORMAL);
					if(!blockingMode){
						if(chesser != owner);
						else if(owner == SENTE and newDan > 3 and preDan > 3){
							moveList.push_back(normalMove);
						}else if(owner == GOTE and newDan < 7 and newDan < 7){
							moveList.push_back(normalMove);
						}
						else if(eid != FOOT and eid != CHARIOT and eid != CASSIA){
							if(eid < 6){
								moveList.push_back(upgradeMove);
							}
							moveList.push_back(normalMove);
						}
						else if(eid == FOOT or eid == CHARIOT){
							if(owner == SENTE and newDan == 1){
								moveList.push_back(upgradeMove);
							}else if(owner == GOTE and newDan == 9){
								moveList.push_back(upgradeMove);
							}else{
								moveList.push_back(upgradeMove);
								moveList.push_back(normalMove);
							}
						}else{
							if(owner == SENTE and newDan <= 2){
								moveList.push_back(upgradeMove);
							}else if(owner == GOTE and newDan >= 8){
								moveList.push_back(upgradeMove);
							}else{
								moveList.push_back(upgradeMove);
								moveList.push_back(normalMove);
							}
						}
					}

					if(boardChesser[newPos] != -1){
						if(blockingMode and updatingAttackMap)
							break;
						else {
							blockingMode = true;
							blocker = board[newPos];
						}
					}
					s++;
				}
			}
		}
	}

	bool nifu[10] = {false};
	for(int suji=1;suji<=9;suji++){
		for(int dan=1;dan<=9;dan++){
			int pos = genPos(suji, dan);
			if(boardChesser[pos] != chesser)continue;
			if(gomakindEID(gomaKind[board[pos]]) == FOOT){
				nifu[suji] = true;
				break;
			}
		}
	}

	int kingPos = (chesser == SENTE) ? gomaPos[SENTEKINGNUM] : gomaPos[GOTEKINGNUM];
	int otherkingPos = (chesser == GOTE) ? gomaPos[SENTEKINGNUM] : gomaPos[GOTEKINGNUM];
	int killerPos = genPos(posSuji(otherkingPos), posDan(otherkingPos) + (chesser == SENTE ? 1 : -1));
	int totalKingAttack = 
		boardFixedAttacking[other][kingPos].size() + 
		boardFlowAttacking[other][kingPos].size();
	int deceiveKingAttack = boardBFlowAttacking[other][kingPos].size();

	if(!ruleOfSafeKing or totalKingAttack < 2 or noking){
		for(int i=0;i<=7;i++){
			if(i == KING)continue;

			int I = i + chesser * 8;
			if(gomaTable[I].empty())continue;

			for(int pos=0;pos<81;pos++){
				if(boardChesser[pos] != -1)continue;
				if(nifu[posSuji(pos)] and i == FOOT)continue;

				if((i == FOOT or i == CHARIOT) and 
				  ((posDan(pos) == 1 and chesser == SENTE) or 
				  (posDan(pos) == 9 and chesser == GOTE)))continue;
				if((i == CASSIA) and 
				  ((posDan(pos) <= 2 and chesser == SENTE) or 
				  (posDan(pos) >= 8 and chesser == GOTE)))continue;

				bool nofootkill = false;

				if(i == FOOT and pos == killerPos and ruleOfFootKill){
					int otherkingSuji = posSuji(otherkingPos);
					int otherkingDan = posDan(otherkingPos);
					
					for(int np=0;np<8;np++){
						int escapeSuji = otherkingSuji + sujiD[KING][np];
						int escapeDan = otherkingDan + danD[KING][np];
						int escapePos = genPos(escapeSuji, escapeDan);
						if(escapePos == -1)continue;
						if(boardChesser[escapePos] == other)continue;
						int escapePosAttack = 
							boardFixedAttacking[chesser][escapePos].size() +
							boardFlowAttacking[chesser][escapePos].size();
						if(escapePosAttack == 0){
							nofootkill = true;
							break;
						}
					}

					if(!nofootkill){
						int killerPosAttack =
							boardFixedAttacking[other][killerPos].size() + 
							boardFlowAttacking[other][killerPos].size();

						int otherDeceiveKingAttack = 
							boardBFlowAttacking[chesser][otherkingPos].size();

						if(otherDeceiveKingAttack == 0){
							if(killerPosAttack >= 2){
								nofootkill = true;
							}else if(killerPosAttack == 1){
								if(boardFixedAttacking[other][killerPos].size() == 1){
									if(gomaPos[watchupAttacker(boardFixedAttacking[other][killerPos][0])]
									!= otherkingPos ) nofootkill = true;
								}else {
									nofootkill = true;
								}
							}
						}else{
							for(int watchup : boardFixedAttacking[other][killerPos]){
								int defenser = watchupAttacker(watchup);
								if(gomaPos[defenser] == otherkingPos)continue;
								bool gooddefenser = true;
								for(int dwatchup : boardBFlowAttacking[chesser][otherkingPos]){
									int attacker = watchupAttacker(dwatchup);
									int blocker = watchupBlocker(dwatchup);
									if(blocker == defenser){
										int attackerPos = gomaPos[attacker];
										int defenserPos = gomaPos[defenser];
										if(posOnLine(defenserPos, killerPos, attackerPos)){
											break;
										}else{
											gooddefenser = false;
											break;
										}
									}
								}
								if(gooddefenser){
									nofootkill = true;
									break;
								}
							}

							for(int watchup : boardFlowAttacking[other][killerPos]){
								int defenser = watchupAttacker(watchup);
								bool gooddefenser = true;
								for(int dwatchup : boardBFlowAttacking[chesser][otherkingPos]){
									int attacker = watchupAttacker(dwatchup);
									int blocker = watchupBlocker(dwatchup);
									if(blocker == defenser){
										int attackerPos = gomaPos[attacker];
										int defenserPos = gomaPos[defenser];
										if(posOnLine(defenserPos, killerPos, attackerPos)){
											break;
										}else{
											gooddefenser = false;
											break;
										}
									}
								}
								if(gooddefenser){
									nofootkill = true;
									break;
								}
							}

						}
					}

					if(!nofootkill){
						int u = (chesser == SENTE) ? -1 : 1;
						int l = -1, r = 1;
						int criticalPos[4] = {
							genPos(otherkingSuji+l, otherkingDan),
							genPos(otherkingSuji+r, otherkingDan),
							genPos(otherkingSuji+l, otherkingDan+u),
							genPos(otherkingSuji+r, otherkingDan+u)
						};

						for(int c = 0;c < 4;c++){
							if(criticalPos[c] != -1)continue;
							if(boardChesser[criticalPos[c]] == other)continue;

							if(boardFixedAttacking[chesser][criticalPos[c]].size() == 0 and
								boardFlowAttacking[chesser][criticalPos[c]].size() == 1){
								int watchup = boardFlowAttacking[chesser][criticalPos[c]][0];
								int attacker = watchupAttacker(watchup);
								int attackerPos = gomaPos[attacker];
								if(posInMiddle(criticalPos[c], killerPos, attackerPos)){
									nofootkill = true;
									break;
								}
							}
						}
					}

				}else {
					nofootkill = true;
				}

				if(!nofootkill)continue;

				int prePos = i;
				int newPos = pos;
				int move = genMove(prePos, newPos, NORMAL, PLAYING);
				moveList.push_back(move);
				
			}
		}
	}
	
	if(!ruleOfSafeKing)return moveList;

	vector<int> returnVector;
	returnVector.reserve(moveList.size());

	if(noking){
		returnVector = moveList;
	}
	else if(totalKingAttack == 0){
		if(deceiveKingAttack == 0){
			for(int move : moveList){
				if(movePlaying(move)){
					returnVector.push_back(move);
					continue;
				}
				int prePos = movePrepos(move);
				int newPos = moveNewpos(move);
				int newPosAttack = 
					boardFixedAttacking[other][newPos].size() +
					boardFlowAttacking[other][newPos].size();

				if(prePos == kingPos and newPosAttack > 0)continue;
				
				returnVector.push_back(move);
			}
		}else{
			for(int move : moveList){
				if(movePlaying(move)){
					returnVector.push_back(move);
					continue;
				}
				int prePos = movePrepos(move);
				int newPos = moveNewpos(move);
				int newPosAttack = 
					boardFixedAttacking[other][newPos].size() +
					boardFlowAttacking[other][newPos].size();

				if(prePos == kingPos and newPosAttack > 0)continue;
				
				bool goodmove = true;
				for(int i=0;i<deceiveKingAttack;i++){
					int watchup = boardBFlowAttacking[other][kingPos][i];
					int blocker = watchupBlocker(watchup);
					int attacker = watchupAttacker(watchup);
					if(prePos == gomaPos[blocker]){
						if(posOnLine(kingPos, prePos, newPos)){
							break;
						}else{
							goodmove = false;
							break;
						}
					}
				}

				if(goodmove){
					returnVector.push_back(move);
				}
			}
		}
	}else if(totalKingAttack >= 2){
		for(int move : moveList){
			if(movePlaying(move))continue;

			int prePos = movePrepos(move);
			int newPos = moveNewpos(move);
			int newPosAttack = 
				boardFixedAttacking[other][newPos].size() +
				boardFlowAttacking[other][newPos].size();

			int newPosDeceive = 
				boardBFlowAttacking[other][newPos].size();

			if(prePos != kingPos or newPosAttack > 0)continue;
			bool goodmove = true;

			for(int i=0;i<newPosDeceive;i++){
				int watchup = boardBFlowAttacking[other][newPos][i];
				int blocker = watchupBlocker(watchup);
				int attacker = watchupAttacker(watchup);
				if(gomaPos[blocker] == kingPos){
					goodmove = false;
					break;
				}
			}

			if(goodmove){
				returnVector.push_back(move);
			}
			
		}
	}else {
		if(deceiveKingAttack == 0){
			if(boardFixedAttacking[other][kingPos].size() == 1){
				
				int watchup = boardFixedAttacking[other][kingPos][0];
				int attacker = watchupAttacker(watchup);
				for(int move: moveList){
					if(movePlaying(move))continue;

					int prePos = movePrepos(move);
					int newPos = moveNewpos(move);

					int newPosAttack = 
						boardFixedAttacking[other][newPos].size() +
						boardFlowAttacking[other][newPos].size();

					if(newPos != gomaPos[attacker] and prePos != kingPos)continue;
					if(prePos == kingPos and newPosAttack > 0)continue;

					returnVector.push_back(move);
				}

			}else{
				int watchup = boardFlowAttacking[other][kingPos][0];
				int attacker = watchupAttacker(watchup);
				for(int move: moveList){
					int prePos = movePrepos(move);
					int newPos = moveNewpos(move);
					bool newPosBlocking = false;
					int attackerPos = gomaPos[attacker];

					if(posInMiddle(kingPos, newPos, attackerPos)){
						newPosBlocking = true;
					}

					if(movePlaying(move)){
						if(newPosBlocking){
							returnVector.push_back(move);
						}
					}else{
						int newPosAttack = 
							boardFixedAttacking[other][newPos].size() +
							boardFlowAttacking[other][newPos].size();

						if(newPos != gomaPos[attacker] and prePos != kingPos and !newPosBlocking)continue;
						if(prePos == kingPos and newPosAttack > 0)continue;

						bool goodmove = true;
						int newPosDeceive = 
							boardBFlowAttacking[other][newPos].size();

						for(int i=0;i<newPosDeceive;i++){
							int watchup = boardBFlowAttacking[other][newPos][i];
							int blocker = watchupBlocker(watchup);
							int attacker = watchupAttacker(watchup);
							if(gomaPos[blocker] == kingPos){
								goodmove = false;
								break;
							}
						}

						if(goodmove){
							returnVector.push_back(move);
						}
					}
				}
			}
		}else{
			if(boardFixedAttacking[other][kingPos].size() == 1){
				
				int watchup = boardFixedAttacking[other][kingPos][0];
				int attacker = watchupAttacker(watchup);
				for(int move: moveList){
					if(movePlaying(move))continue;

					int prePos = movePrepos(move);
					int newPos = moveNewpos(move);

					int newPosAttack = 
						boardFixedAttacking[other][newPos].size() +
						boardFlowAttacking[other][newPos].size();

					if(newPos != gomaPos[attacker] and prePos != kingPos)continue;
					if(prePos == kingPos and newPosAttack > 0)continue;

					bool goodmove = true;
					for(int i=0;i<deceiveKingAttack;i++){
						int watchup = boardBFlowAttacking[other][kingPos][i];
						int blocker = watchupBlocker(watchup);
						int attacker = watchupAttacker(watchup);
						
						if(prePos == gomaPos[blocker]){
							if(posOnLine(kingPos, prePos, newPos)){
								break;
							}else {
								goodmove = false;
								break;
							}
						}
					}

					if(goodmove){
						returnVector.push_back(move);
					}
				}
			}else{
				int watchup = boardFlowAttacking[other][kingPos][0];
				int attacker = watchupAttacker(watchup);
				for(int move: moveList){
					int prePos = movePrepos(move);
					int newPos = moveNewpos(move);
					bool newPosBlocking = false;
					int attackerPos = gomaPos[attacker];
					if(posInMiddle(kingPos, newPos, attackerPos)){
						newPosBlocking = true;
					}

					if(movePlaying(move)){
						if(newPosBlocking){
							returnVector.push_back(move);
						}
					}else{
						int newPosAttack = 
							boardFixedAttacking[other][newPos].size() +
							boardFlowAttacking[other][newPos].size();

						if(newPos != gomaPos[attacker] and prePos != kingPos and !newPosBlocking)continue;
						if(prePos == kingPos and newPosAttack > 0)continue;

						bool goodmove = true;

						int newPosDeceive = 
							boardBFlowAttacking[other][newPos].size();

						for(int i=0;i<newPosDeceive;i++){
							int watchup = boardBFlowAttacking[other][newPos][i];
							int blocker = watchupBlocker(watchup);
							int attacker = watchupAttacker(watchup);
							if(gomaPos[blocker] == kingPos){
								goodmove = false;
								break;
							}
						}

						if(!goodmove)continue;

						for(int i=0;i<deceiveKingAttack;i++){
							int watchup = boardBFlowAttacking[other][kingPos][i];
							int blocker = watchupBlocker(watchup);
							int attacker = watchupAttacker(watchup);
							
							if(prePos == gomaPos[blocker]){
								if(posOnLine(kingPos, prePos, newPos))break;
								else {
									goodmove = false;
									break;
								}
							}
						}

						if(goodmove){
							returnVector.push_back(move);
						}
					}
				}
			}
		}
	}

	if(!threateningKing) return returnVector;

	vector<int> lastReturnVector;
	lastReturnVector.reserve(returnVector.size());

	vector<int> criticalBlocker;
	criticalBlocker.reserve(6);

	int otherKingSuji = posSuji(otherkingPos);
	int otherKingDan = posDan(otherkingPos);
	int danReverse = (chesser == SENTE ? -1 : 1);

	vector<int> warningMap(82, 0);
	for(int eid=0;eid<14;eid++){
		for(int v=0;v<movingDlength[eid];v++){
			if(movingD[eid][v] == 0){
				int newSuji = otherKingSuji + sujiD[eid][v];
				int newDan = otherKingDan + danD[eid][v] * danReverse;
				int newPos = genPos(newSuji, newDan);
				if(newPos == -1)continue;

				warningMap[newPos] |= (1 << eid);

			}else if(movingD[eid][v] == 1){
				int nx = 1;
				int blocker = NOBLOCKER;
				bool blockingMode = false;

				while(true){
					int newSuji = otherKingSuji + sujiD[eid][v] * nx;
					int newDan = otherKingDan + danD[eid][v] * danReverse * nx;
					int newPos = genPos(newSuji, newDan);
					if(newPos == -1)break;
					
					if(blockingMode and gomaKind[board[newPos]] == eid and boardChesser[newPos] == chesser){
						criticalBlocker.push_back(blocker);
						break;
					}

					if(boardChesser[newPos] == chesser){
						if(blockingMode){
							break;
						}
						blocker = board[newPos];
						blockingMode = true;
					}

					if(!blockingMode){
						warningMap[newPos] |= (1 << eid);
					}

					if(boardChesser[newPos] != chesser and boardChesser[newPos] != -1)
						break;

					nx++;
				}
			}
		}
	}

/*
	for(int dan=1;dan<=9;dan++){
		for(int suji=9;suji>=1;suji--){
			int pos = genPos(suji, dan);
			for(int eid=0;eid<14;eid++){
				if(warningMap[pos] & (1 << eid))cout << hex << eid << dec;
				else cout << ".";
			}
			cout << " ";
		}
		cout << "\n";
	}
*/
	for(int move : returnVector){
		bool makekill = false;

		int prePos = movePrepos(move);
		int newPos = moveNewpos(move);
		int playing = movePlaying(move);
		int upgrade = moveUpgrade(move);

		int gomaeid = playing ? prePos : gomakindEID(gomaKind[board[prePos]]);
		if(upgrade) {
			gomaeid += 8;
		}
		if(warningMap[newPos] & (1 << gomaeid))makekill = true;
		else{
			int gomanum = board[prePos];
			for(int blocker : criticalBlocker){
				if(blocker == gomanum){
					if(!posOnLine(otherkingPos, prePos, newPos)){
						makekill = true;
						break;
					}else{
						break;
					}
				}
			}
		}

		if(makekill){
			lastReturnVector.push_back(move);
		}
	}

	return lastReturnVector;
}

void Shogi::MakeMove(int move){
	int prePos = movePrepos(move);
	int newPos = moveNewpos(move);
	int upgrade = moveUpgrade(move);
	int playing = movePlaying(move);

	int chesser = (round & 1);

	if(playing){
		int id = prePos;
		int I = prePos + chesser * 8;
		int gomanum = gomaTable[I].front();
		gomaTable[I].pop();

		board[newPos] = gomanum;
		boardChesser[newPos] = chesser;
		gomaKind[gomanum] = genGomakind(id, 0, chesser);
		gomaPos[gomanum] = newPos;
		round++;
		return;
	}

	int getGoma = -1;
	int gomanum = board[prePos];
	int gomanumN = board[newPos];

	if(board[newPos] != -1){
		int tdg = board[newPos];
		getGoma = gomakindID(gomaKind[tdg]);
		gomaPos[tdg] = -1;
	}

	board[newPos] = gomanum;
	boardChesser[newPos] = chesser;
	board[prePos] = -1;
	boardChesser[prePos] = -1;

	gomaPos[gomanum] = newPos;
	gomaKind[gomanum] += upgrade * 8;

	if(getGoma != -1){
		int I = getGoma + chesser * 8;
		gomaTable[I].push(gomanumN);
	} 
	round++;
	return;
}


vector<unsigned char> Shogi::SaveGame(){
	vector<unsigned char> gameDigest;
	gameDigest.reserve(100);
	for(int pos=0;pos<81;pos++){
		int gomaNum = board[pos];
		if(gomaNum == -1) gameDigest.push_back(255);
		else gameDigest.push_back(gomaKind[gomaNum]);
	}
	for(int chesser=0;chesser<2;chesser++){
		for(int i=0;i<8;i++){
			int I = i + chesser * 8;
			gameDigest.push_back(gomaTable[I].size());
		}
	}
	gameDigest.push_back(round / 256);
	gameDigest.push_back(round % 256);
	return gameDigest;
}

void Shogi::LoadGame(vector<unsigned char> digest){
	int gomaNumber = 0;
	int offset = 0;
	for(int pos=0;pos<81;pos++){
		if(digest[pos] == 255){
			board[pos] = -1;
			boardChesser[pos] = -1;
		}else{
			board[pos] = gomaNumber;
			boardChesser[pos] = gomakindChesser(digest[pos+offset]);
			gomaKind[gomaNumber] = digest[pos+offset];
			gomaPos[gomaNumber] = pos;
			if(gomaKind[gomaNumber] == genGomakind(KING, NORMAL, SENTE)){
				SENTEKINGNUM = gomaNumber;
			}else if(gomaKind[gomaNumber] == genGomakind(KING, NORMAL, GOTE)){
				GOTEKINGNUM = gomaNumber;
			}
			gomaNumber++;
		}
	}

	offset = 81;

	for(int chesser=0;chesser<2;chesser++){
		for(int i=0;i<8;i++){
			int I = i + chesser * 8;
			while(!gomaTable[I].empty()){
				gomaTable[I].pop();
			}

			for(int k=0;k<digest[I+offset];k++){
				int gomakind = genGomakind(i, NORMAL, chesser);
				gomaKind[gomaNumber] = gomakind;
				gomaPos[gomaNumber] = -1;
				gomaTable[I].push(gomaNumber);
				gomaNumber++;
			}
		}
	}
	offset = 97;

	if(gomaNumber != 40){
		SENTEKINGNUM = 39;
	}

	round = 0;
	round += digest[offset] * 256;
	round += digest[offset+1];
}

void Shogi::WhiteInit(){

	Init();
	for(int pos=0;pos<81;pos++){
		board[pos] = -1;
		boardChesser[pos] = -1;
	}
	for(int i=0;i<40;i++){
		gomaPos[i] = -1;
	}
}

void Shogi::SetGoma(int gomanum, int chesser, int upgrade, int position, int gomatable){
	
	if(gomatable == 0){
		board[position] = gomanum;
		boardChesser[position] = chesser;
		gomaPos[gomanum] = position;
		gomaKind[gomanum] = genGomakind(gomakindID(gomaKind[gomanum]), upgrade, chesser);
	}else{
		int pos = genUPos(chesser, gomakindID(gomaKind[gomanum]));
		gomaTable[pos].push(gomanum);
	}
}

int Shogi::RemoveGoma(int position, int gomatable){
	if(gomatable == 0){
		int rat = board[position];
		board[position] = -1;
		boardChesser[position] = -1;
		gomaPos[rat] = -1;
		return rat;
	}else{
		int rat = gomaTable[position].front();
		gomaTable[position].pop();
		return rat;
	}
}