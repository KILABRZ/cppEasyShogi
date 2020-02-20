#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Shogi{
public:
	int board[82];
	int boardChesser[82];
	
	int gomaKind[82];
	int gomaPos[82];
	int round;

	int SENTEKINGNUM;
	int GOTEKINGNUM;

	vector<queue<int>> gomaTable;

	vector<int> boardFixedAttacking[2][82];
	vector<int> boardFlowAttacking[2][82];
	vector<int> boardBFlowAttacking[2][82];

	void Init();
	void NumberBoardPrint();
	void EasyBoardPrint();
	void PrintAttackBoard();
	vector<int> FetchMove(int request);
	void MakeMove(int move);

	vector<unsigned char> SaveGame();
	void LoadGame(vector<unsigned char>);

	Shogi operator=(Shogi a){
		for(int i=0;i<=81;i++){
			board[i] = a.board[i];
			boardChesser[i] = a.boardChesser[i];
			gomaKind[i] = a.gomaKind[i];
			gomaPos[i] = a.gomaPos[i];
		}
		round = a.round;
		SENTEKINGNUM = a.SENTEKINGNUM;
		GOTEKINGNUM = a.GOTEKINGNUM;
		for(int i=0;i<16;i++){
			gomaTable[i] = a.gomaTable[i];
		}
		return *this;
	}
};

const int FOOT = 0;
const int SILVER = 1;
const int CASSIA = 2;
const int CHARIOT = 3;
const int FLYING = 4;
const int ANGLE = 5;
const int KING = 6;
const int GOLD = 7;

const int NORMAL = 0;
const int UPGRADED = 1;
const int PLAYING = 1;

const int SENTE = 0;
const int GOTE = 1;



int posSuji(int pos);
int posDan(int pos);
int genPos(int suji, int dan);
int genGomakind(int id, int upgrade, int chesser);
int gomakindID(int gomakind);
int gomakindEID(int gomakind);
int gomakindChesser(int gomakind);
int genMove(int prePos, int newPos, int upgrade, int playing);
int movePrepos(int move);
int moveNewpos(int move);
int moveUpgrade(int move);
int movePlaying(int move);

int genWatchup(int blocker, int attacker);
int watchupBlocker(int watchup);
int watchupAttacker(int watchup);

const int NOBLOCKER = -1;


void printMove(int move);

bool posOnLine(int pa, int pb, int pc);
bool posInMiddle(int pl, int pm, int pr);


const int ALPHA = 0;
const int BETA = 1;

class ShogiBot{
public:

	vector<char> judgingVector;
	int bestMove;


	void RandomInit();
	void SpecialInit(vector<char> jV);
	int judging(Shogi s);
	int alphaBetaTree(Shogi s, int alpha, int beta, int player, int lastDepth, int maxDepth, double beamsize);
	int decideMove(Shogi s, int depth, double beamsize);

	ShogiBot operator=(ShogiBot a){
		judgingVector = a.judgingVector;
		bestMove = a.bestMove;
		return *this;
	}

};

int gomaStrength(Shogi s, vector<char> jV);
int aggressing(Shogi s, vector<char> jV);
int spacing(Shogi s, vector<char> jV);
int generalAttacking(Shogi s, vector<char> jV);
int kingAttacking(Shogi s, vector<char> jV);