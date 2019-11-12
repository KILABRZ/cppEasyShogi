#include "shogi.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


int main(){
	srand(time(0));
	int prepos = genPos(5, 5);
	int newpos = genPos(9, 9);
	int NFlag = true;
	int UFlag = false;
	int move = genMove(prepos, newpos, UFlag, NFlag);

	Shogigame shogi;
	shogi.initialBoard();
	while(true){
		shogi.easyPrintTable(); cout << "\n";
		shogi.shogiFetchMove();
		shogi.shogiMakeMove(rand()%shogi.moveListLength);
		shogi.changeChesser();
		if(shogi.gomaPos[0] == -1 or shogi.gomaPos[20] == -1)break;
	}
	cout << shogi.round << "\n";
	
} 