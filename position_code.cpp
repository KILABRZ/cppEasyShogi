#include "shogi.hpp"

int posSuji(int pos){
	return pos / 9 + 1;
}

int posDan(int pos){
	return pos % 9 + 1;
}

int moveNewPos(int move){
	return (move >> 2) / 81;
}

int movePrePos(int move){
	return (move >> 2) % 81;
}

int moveUFlag(int move){
	return (move & 2);
}

int moveNFlag(int move){
	return (move & 1);
}

int genPos(int suji, int dan){
	return (suji >= 1 and suji <= 9 and dan >= 1 and dan <= 9) ? (suji - 1) * 9 + (dan - 1) : -1;
}

bool truePos(int pos){
	return (pos >= 0 and pos <= 80);
}

int genUPos(int gomaNumber, int chesser){
	return chesser*40+gomaNumber;
}

int genMove(int prepos, int newpos, int UFlag, int NFlag){
	return ((newpos*81 + prepos)*4 + UFlag*2) | NFlag;
}