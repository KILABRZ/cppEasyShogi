#include "shogi.hpp"
#include <cstdlib>
#include <ctime>
void random_playing_test_visible(int n){
	Shogi s;
	s.Init();

	for(int i=0;i<n;i++){
		
		vector<int> moveList = s.FetchMove(2);
		if(moveList.size() == 0){
			vector<int> moveList = s.FetchMove(1);
			s.EasyBoardPrint(); cout << "\n";
			s.PrintAttackBoard(); cout << "\n";
			for(int move: moveList){
				printMove(move);
			}
			system("pause");
			break;
		}
		int move = moveList[rand() % moveList.size()];
		printMove(move);

		cout << "\n------------------------------- " << s.round+1 << "\n";
		s.MakeMove(move);
//		system("cls");
	}
}

void handtest(){
	Shogi s;
	s.Init();

	while(true){
		
		vector<int> moveList = s.FetchMove(3);
		bool finish = false;
		int move = 0;

		s.EasyBoardPrint(); cout << "\n";
		for(int m : moveList){
			printMove(m);
		}
		cout << moveList.size() << "\n\n";

		while(!finish){
			int presuji, predan, newsuji, newdan, upgrade, playing;
			cin >> presuji >> predan >> newsuji >> newdan >> upgrade >> playing;
			if(playing){
				move = genMove(presuji, genPos(newsuji, newdan), upgrade, playing);
			}else{
				move = genMove(genPos(presuji, predan), genPos(newsuji, newdan), upgrade, playing);
			}
			for(int m : moveList){
				if(move == m){
					finish = true;
					break;
				}
			}
		}

		
		cout << "\n";
		cout << "\n------------------------------- " << s.round+1 << "\n";
		s.MakeMove(move);
	}
}

void speedtest(int testtimes){
	int roundCount = 0;
	for(int i=0;i<testtimes;i++){
		Shogi s;
		s.Init();
		for(int i=0;i<500000;i++){
			vector<int> movelist = s.FetchMove(3);
			if(movelist.size() == 0){
				roundCount += s.round;
				break;
			}
			int move = movelist[rand() % movelist.size()];
			s.MakeMove(move);
		}
	}
	cout << "finish.\n";
	cout << "Average round = " << (double)roundCount / (double)testtimes << "\n";
	cout << "Total Compute round = " << roundCount << "\n";
}

void fuzztest(int testtimes){
	for(int i=0;i<testtimes;i++){
		random_playing_test_visible(500);
		cout << "==============================\n";
		cout << "==============================\n\n";
	}
	
}

int main(){
	speedtest(1000);
}