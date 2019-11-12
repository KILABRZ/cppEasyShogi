#include "shogi.hpp"
#include <iostream>
using namespace std;

Shogigame::Shogigame(){
	for(int i=0;i<81;i++)board[i] = 0;
	for(int i=0;i<81;i++)gomatable[i] = -1;
	
	for(int i=0;i<10000;i++)moveList[i] = 0;
	moveListLength = 0;
	for(int i=0;i<40;i++)gomaKinds[i] = 0;
	for(int i=0;i<40;i++)gomaPos[i] = 0;
	round = 0;
}

void Shogigame::changeChesser(){
	round++;
}

void Shogigame::initialBoard(){

	gomaKinds[ 0] = 0 + 0 + 0;  gomaPos[ 0] = genPos(5, 9);
	gomaKinds[ 1] = 0 + 0 + 1;  gomaPos[ 1] = genPos(2, 8);
	gomaKinds[ 2] = 0 + 0 + 2;  gomaPos[ 2] = genPos(8, 8);
	gomaKinds[ 3] = 0 + 0 + 3;  gomaPos[ 3] = genPos(4, 9);
	gomaKinds[ 4] = 0 + 0 + 3;  gomaPos[ 4] = genPos(6, 9);
	gomaKinds[ 5] = 0 + 0 + 4;  gomaPos[ 5] = genPos(3, 9);
	gomaKinds[ 6] = 0 + 0 + 4;  gomaPos[ 6] = genPos(7, 9);
	gomaKinds[ 7] = 0 + 0 + 5;  gomaPos[ 7] = genPos(2, 9);
	gomaKinds[ 8] = 0 + 0 + 5;  gomaPos[ 8] = genPos(8, 9);
	gomaKinds[ 9] = 0 + 0 + 6;  gomaPos[ 9] = genPos(1, 9);
	gomaKinds[10] = 0 + 0 + 6;  gomaPos[10] = genPos(9, 9);
	gomaKinds[11] = 0 + 0 + 7;  gomaPos[11] = genPos(1, 7);
	gomaKinds[12] = 0 + 0 + 7;  gomaPos[12] = genPos(2, 7);
	gomaKinds[13] = 0 + 0 + 7;  gomaPos[13] = genPos(3, 7);
	gomaKinds[14] = 0 + 0 + 7;  gomaPos[14] = genPos(4, 7);
	gomaKinds[15] = 0 + 0 + 7;  gomaPos[15] = genPos(5, 7);
	gomaKinds[16] = 0 + 0 + 7;  gomaPos[16] = genPos(6, 7);
	gomaKinds[17] = 0 + 0 + 7;  gomaPos[17] = genPos(7, 7);
	gomaKinds[18] = 0 + 0 + 7;  gomaPos[18] = genPos(8, 7);
	gomaKinds[19] = 0 + 0 + 7;  gomaPos[19] = genPos(9, 7);
	
	gomaKinds[20] = 16 + 0 + 0;  gomaPos[20] = genPos(5, 1);
	gomaKinds[21] = 16 + 0 + 1;  gomaPos[21] = genPos(8, 2);
	gomaKinds[22] = 16 + 0 + 2;  gomaPos[22] = genPos(2, 2);
	gomaKinds[23] = 16 + 0 + 3;  gomaPos[23] = genPos(4, 1);
	gomaKinds[24] = 16 + 0 + 3;  gomaPos[24] = genPos(6, 1);
	gomaKinds[25] = 16 + 0 + 4;  gomaPos[25] = genPos(3, 1);
	gomaKinds[26] = 16 + 0 + 4;  gomaPos[26] = genPos(7, 1);
	gomaKinds[27] = 16 + 0 + 5;  gomaPos[27] = genPos(2, 1);
	gomaKinds[28] = 16 + 0 + 5;  gomaPos[28] = genPos(8, 1);
	gomaKinds[29] = 16 + 0 + 6;  gomaPos[29] = genPos(1, 1);
	gomaKinds[30] = 16 + 0 + 6;  gomaPos[30] = genPos(9, 1);
	gomaKinds[31] = 16 + 0 + 7;  gomaPos[31] = genPos(1, 3);
	gomaKinds[32] = 16 + 0 + 7;  gomaPos[32] = genPos(2, 3);
	gomaKinds[33] = 16 + 0 + 7;  gomaPos[33] = genPos(3, 3);
	gomaKinds[34] = 16 + 0 + 7;  gomaPos[34] = genPos(4, 3);
	gomaKinds[35] = 16 + 0 + 7;  gomaPos[35] = genPos(5, 3);
	gomaKinds[36] = 16 + 0 + 7;  gomaPos[36] = genPos(6, 3);
	gomaKinds[37] = 16 + 0 + 7;  gomaPos[37] = genPos(7, 3);
	gomaKinds[38] = 16 + 0 + 7;  gomaPos[38] = genPos(8, 3);
	gomaKinds[39] = 16 + 0 + 7;  gomaPos[39] = genPos(9, 3);
	
	for(int suji=1;suji<=9;suji++){
		for(int dan=1;dan<=9;dan++){
			board[genPos(suji, dan)] = -1;
		}
	}

	for(int gomaI=0;gomaI<40;gomaI++){
		board[gomaPos[gomaI]] = gomaI;
	}
}

void Shogigame::easyPrintTable(){

	char gomaExpress[20] = "KFXGSJAT #* sja&";

	for(int dan=1;dan<=9;dan++){
		for(int suji=9;suji>=1;suji--){
			if(board[genPos(suji, dan)] == -1) cout << ".. ";
			else {
				if(( gomaKinds[board[genPos(suji, dan)]] & 16 ))cout << "-";
				else cout << "+";
				cout << gomaExpress[( gomaKinds[board[genPos(suji, dan)]] & 15 )] << " ";
			}
		}
		cout << "\n";
	}
}

void Shogigame::shogiMakeMove(int moveN){
	if(moveN >= moveListLength)return;

	int themove = moveList[moveN];
	int prepos = movePrePos(themove);
	int newpos = moveNewPos(themove);
	bool uflag = moveUFlag(themove);
	bool nflag = moveNFlag(themove);

	int chesser = (round & 1);

	if(!uflag){
		if(board[newpos] == -1){
			board[newpos] = board[prepos];
			board[prepos] = -1;
			gomaPos[board[newpos]] = newpos;
			gomaKinds[board[newpos]] |= nflag * 8;
		}else{
			gomatable[genUPos(board[newpos], chesser)] = board[newpos];
			gomaKinds[board[newpos]] = (gomaKinds[board[newpos]] & 7) + chesser * 16;
			gomaPos[board[newpos]] = -1; 

			board[newpos] = board[prepos];
			board[prepos] = -1;
			gomaPos[board[newpos]] = newpos;
			gomaKinds[board[newpos]] |= nflag * 8;
		}
	}else{
		int uchigoma = gomatable[prepos];
		board[newpos] = uchigoma;
		gomaPos[uchigoma] = newpos;
		gomatable[prepos] = -1;
	}
	
}

void Shogigame::shogiFetchMove(){
	moveListLength = 0;
	int chesser = (round & 1);

	bool uchiFu_suji[10] = {false};

	for(int suji=1;suji<=9;suji++){
		for(int dan=1;dan<=9;dan++){
			int pos = genPos(suji, dan);
			if(board[pos] == -1)continue;
			if((gomaKinds[board[pos]] & 16) == chesser*16 and (gomaKinds[board[pos]] & 7) == 7){
				uchiFu_suji[suji] = true;
			}
		}
	}

	for(int gomaN=0;gomaN<40;gomaN++){
		if(gomatable[gomaN+chesser*40] != -1){
			int uchigoma = gomatable[genUPos(gomaN, chesser)];	// though total identical to gomaN
			int gomakind = (gomaKinds[uchigoma] & 7);

			for(int pos=0;pos<81;pos++){
				if(board[pos] != -1)continue;
				if(gomakind == 7 and uchiFu_suji[posSuji(pos)])continue;
				if((gomakind == 7 or gomakind == 6 or gomakind == 5) and posDan(pos) == 8*chesser+1)continue;
				if((gomakind == 5) and posDan(pos) == 6*chesser+2)continue;
				moveList[moveListLength++] = genMove(genUPos(gomaN, chesser), pos, true, false);
			}
		}
	}

	for(int dan=1;dan<=9;dan++){
		for(int suji=1;suji<=9;suji++){
			int pos = genPos(suji, dan);
			int goma = board[pos];
			if(goma == -1)continue;
			if(((gomaKinds[goma] & 16) >> 4)!= chesser)continue;
			int gomakind = (gomaKinds[goma] & 15);
			if(chesser == 0){

				if(gomakind == 7){
					int newpos = genPos(suji, dan-1), newgoma;
					if(!truePos(newpos))continue;
					else newgoma = board[newpos];

					if(newgoma == -1){
						if(posDan(newpos) != 1)moveList[moveListLength++] = genMove(pos, newpos, false, false);
						if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
					}
					else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
						if(posDan(newpos) != 1)moveList[moveListLength++] = genMove(pos, newpos, false, false);
						if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
					}else continue;
				}

				if(gomakind == 6){

					int vector_suji[1] = {0};
					int vector_dan[1] = {-1};

					for(int v=0;v<1;v++){
						int straightX = 1;
						while(straightX <= 8){

							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								if(posDan(newpos) != 1)moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								if(posDan(newpos) != 1)moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
					continue;
				}

				if(gomakind == 5){
					int newpos, newgoma;

					int vector_suji[2] = {-1, 1};
					int vector_dan[2] = {-2, -2};
					
					for(int v=0;v<2;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];
						
						if(newgoma == -1){
							if(posDan(newpos) > 2)moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							if(posDan(newpos) > 2)moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
					}
					continue;
					
				}

				if(gomakind == 4){
					int newpos, newgoma;

					int vector_suji[5] = {-1, 0, 1, -1, 1};
					int vector_dan[5] = {-1, -1, -1, 1, 1};
					
					for(int v=0;v<5;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) <= 3)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
					}
					continue;
				}

				if(gomakind == 3 or gomakind == 12 or gomakind == 13 or gomakind == 14 or gomakind == 15){
					int newpos, newgoma;

					int vector_suji[6] = {-1, 0, 1, 1, -1, 0};
					int vector_dan[6] = {-1, -1, -1, 0, 0, 1};
					
					for(int v=0;v<6;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				if(gomakind == 1 || gomakind == 9){

					int vector_suji[4] = {1, 0, 0, -1};
					int vector_dan[4] = {0, 1, -1, 0};

					for(int v=0;v<4;v++){
						int straightX = 1;
						while(straightX <= 8){
							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
				}

				if(gomakind == 9){
					int newpos, newgoma;
					int vector_suji[4] = {-1, 1, -1, 1};
					int vector_dan[4] = {-1, -1, 1, 1};
					
					for(int v=0;v<4;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				if(gomakind == 2 || gomakind == 10){
					int vector_suji[4] = {1, 1, -1, -1};
					int vector_dan[4] = {-1, 1, -1, 1};

					for(int v=0;v<4;v++){
						int straightX = 1;
						while(straightX <= 8){
							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) <= 3 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
					continue;
				}

				if(gomakind == 10){
					int newpos, newgoma;

					int vector_suji[4] = {1, 0, 0, -1};
					int vector_dan[4] = {0, 1, -1, 0};
					
					for(int v=0;v<4;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				

				if(gomakind == 0){
					int newpos, newgoma;

					int vector_suji[8] = {-1, 0, 1, 1, -1, -1, 0, 1};
					int vector_dan[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
					
					for(int v=0;v<8;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

/*
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
*/
			}else{


				if(gomakind == 7){
					int newpos = genPos(suji, dan+1), newgoma;
					if(!truePos(newpos))continue;
					else newgoma = board[newpos];

					if(newgoma == -1){
						if(posDan(newpos) != 9)moveList[moveListLength++] = genMove(pos, newpos, false, false);
						if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
					}
					else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
						if(posDan(newpos) != 9)moveList[moveListLength++] = genMove(pos, newpos, false, false);
						if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
					}else continue;
				}

				if(gomakind == 6){

					int vector_suji[1] = {0};
					int vector_dan[1] = {1};

					for(int v=0;v<1;v++){
						int straightX = 1;
						while(straightX <= 8){

							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								if(posDan(newpos) != 9)moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								if(posDan(newpos) != 9)moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
					continue;
				}

				if(gomakind == 5){
					int newpos, newgoma;

					int vector_suji[2] = {-1, 1};
					int vector_dan[2] = {2, 2};
					
					for(int v=0;v<2;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];
						
						if(newgoma == -1){
							if(posDan(newpos) < 8)moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							if(posDan(newpos) < 8)moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
					}
					continue;
					
				}

				if(gomakind == 4){
					int newpos, newgoma;

					int vector_suji[5] = {-1, 0, 1, -1, 1};
					int vector_dan[5] = {1, 1, 1, -1, -1};
					
					for(int v=0;v<5;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
							if(posDan(newpos) >= 7)moveList[moveListLength++] = genMove(pos, newpos, false, true);
						}
					}
					continue;
				}

				if(gomakind == 3 or gomakind == 12 or gomakind == 13 or gomakind == 14 or gomakind == 15){
					int newpos, newgoma;

					int vector_suji[6] = {-1, 0, 1, 1, -1, 0};
					int vector_dan[6] = {1, 1, 1, 0, 0, -1};
					
					for(int v=0;v<6;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				if(gomakind == 1 || gomakind == 9){

					int vector_suji[4] = {1, 0, 0, -1};
					int vector_dan[4] = {0, 1, -1, 0};

					for(int v=0;v<4;v++){
						int straightX = 1;
						while(straightX <= 8){
							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
				}

				if(gomakind == 9){
					int newpos, newgoma;
					int vector_suji[4] = {-1, 1, -1, 1};
					int vector_dan[4] = {-1, -1, 1, 1};
					
					for(int v=0;v<4;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				if(gomakind == 2 || gomakind == 10){
					int vector_suji[4] = {1, 1, -1, -1};
					int vector_dan[4] = {-1, 1, -1, 1};

					for(int v=0;v<4;v++){
						int straightX = 1;
						while(straightX <= 8){
							int newpos = genPos(suji+vector_suji[v]*straightX, dan+vector_dan[v]*straightX);
							if(!truePos(newpos))break;
							int newgoma = board[newpos];

							if(newgoma == -1){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
							}
							else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
								moveList[moveListLength++] = genMove(pos, newpos, false, false);
								if(posDan(newpos) >= 7 and (gomakind & 8) != 8)moveList[moveListLength++] = genMove(pos, newpos, false, true);
								break;
							}else break;
							straightX++;
						}
					}
					continue;
				}

				if(gomakind == 10){
					int newpos, newgoma;

					int vector_suji[4] = {1, 0, 0, -1};
					int vector_dan[4] = {0, 1, -1, 0};
					
					for(int v=0;v<4;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}

				

				if(gomakind == 0){
					int newpos, newgoma;

					int vector_suji[8] = {-1, 0, 1, 1, -1, -1, 0, 1};
					int vector_dan[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
					
					for(int v=0;v<8;v++){
						newpos = genPos(suji+vector_suji[v], dan+vector_dan[v]);
						if(!truePos(newpos))continue;
						else newgoma = board[newpos];

						if(newgoma == -1){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
						else if(newgoma != -1 and ((gomaKinds[newgoma] & 16) >> 4) != chesser){
							moveList[moveListLength++] = genMove(pos, newpos, false, false);
						}
					}
					continue;
				}
			}
		}
	}
}