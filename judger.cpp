#include "shogi.hpp"

void ShogiBot::RandomInit(){
	judgingVector.reserve(72);
	for(int i=0;i<72;i++){
		judgingVector.push_back(rand() % 256 - 128);
	}
}

void ShogiBot::SpecialInit(vector<char> JV){
	RandomInit();
	for(int i=0;i<JV.size();i++){
		judgingVector[i] = JV[i];
	}
}

int ShogiBot::judging(Shogi s){
	vector<int> moveList = s.FetchMove(3);
	int totalScore = 0;
	totalScore += gomaStrength(s, judgingVector);
	totalScore += aggressing(s, judgingVector);
	totalScore += spacing(s, judgingVector);
	totalScore += generalAttacking(s, judgingVector);
	totalScore += kingAttacking(s, judgingVector);

	return totalScore;
}

vector<int> moveListRearrange(Shogi s, vector<int> moveList){
	vector<int> returnVector;
	int maxSize = moveList.size();
	returnVector.reserve(maxSize);
	int order[15] = {6, 13, 12, 4, 5, 8, 9, 10, 11, 0, 1, 2, 7, 3, -1};
	for(int i=0;i<15;i++){
		if(order[i] >= 0){
			for(int move : moveList){
				if(movePlaying(move))continue;
				int prepos = movePrepos(move);
				if(gomakindEID(s.gomaKind[s.board[prepos]]) == order[i]){
					returnVector.push_back(move);
				}
			}
		}else{
			for(int move : moveList){
				if(movePlaying(move)){
					returnVector.push_back(move);
				}
			}
		}
	}
	return returnVector;
	
}

vector<int> moveListRandomDelete(vector<int> moveList, double p){
	int i=0, cnt=0, maxsize = moveList.size() * p + 1;
	if(maxsize > moveList.size()) maxsize = moveList.size();
	vector<int> returnVector;
	returnVector.reserve(maxsize);
	while(cnt < maxsize){
		int dice = rand() % 20;
		if(dice < 17){
			returnVector.push_back(moveList[i]);
			cnt++;
		}

		i++;
		i %= moveList.size();
	}
	return returnVector;
}

int ShogiBot::alphaBetaTree(Shogi s, int alpha, int beta, int player, 
	int lastDepth, int maxDepth, double beamSize=1){
	
	if(lastDepth == 0){
		int score = judging(s);
		return score;
	}
	vector<int> moveList = s.FetchMove(3);
	moveList = moveListRearrange(s, moveList);
	if(beamSize <= 0.999){
		moveList = moveListRandomDelete(moveList, beamSize);
	}
	
	int other = (player ^ 1);
	for(int move : moveList){
		Shogi ns = s;
		ns.MakeMove(move);
		int score = alphaBetaTree(ns, 
			alpha, beta, other, lastDepth-1, maxDepth, beamSize*beamSize);

		if(player == BETA and score < alpha)return alpha;
		if(player == ALPHA and score > beta)return beta;
		if(player == ALPHA and score > alpha){
			if(maxDepth == lastDepth)bestMove = move;
			alpha = score;
		}
		if(player == BETA and score < beta){
			if(maxDepth == lastDepth)bestMove = move;
			beta = score;
		}
	}
	if(player == ALPHA)return alpha;
	else return beta;
}

int SURRENDER = 1200000;
int ShogiBot::decideMove(Shogi s, int depth, double beamSize=1){
	bestMove = -1;
	int score = judging(s);
	

	if(score > SURRENDER or score < -1 * SURRENDER)bestMove = -1;
	else alphaBetaTree(s, -2147483648, 2147483647, (s.round % 2), depth, depth, beamSize);


	return bestMove;
}