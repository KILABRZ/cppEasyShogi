class Shogigame{
public:

/* game processing */
	int board[81];
	int gomatable[81];
	
	int moveList[10000];
	int moveListLength;

	int gomaKinds[40];
	int gomaPos[40];

	int round;

/* game processing */
	Shogigame();
	void initialBoard();
	void shogiFetchMove();
	void shogiMakeMove(int moveN);
	void changeChesser();
	void easyPrintTable();

};

int posSuji(int pos);
int posDan(int pos);
int moveNewPos(int move);
int movePrePos(int move);
int moveUFlag(int move);
int moveNFlag(int move);
int genPos(int suji, int dan);
bool truePos(int pos);
int genUPos(int gomaNumber, int chesser);
int genMove(int prepos, int newpos, int UFlag, int NFlag);