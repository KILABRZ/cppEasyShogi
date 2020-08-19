#include "shogi.hpp"
#include <cstdlib>
#include <ctime> 

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

int main(){
	srand(time(0));
	speedtest(10);
}
