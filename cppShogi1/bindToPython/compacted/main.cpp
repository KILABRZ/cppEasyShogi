#include "shogi.hpp"
#include <cstdlib>
#include <ctime> 

void speedtest(int testtimes){
	for(int i=0;i<testtimes;i++){
		Shogi s;
		s.Init();
		for(int i=0;i<10000;i++){
			vector<int> movelist = s.FetchMove(3);
			if(movelist.size() == 0){
				break;
			}
			int move = movelist[rand() % movelist.size()];
			s.MakeMove(move);
		}
	}
}

int main(){
	srand(time(0));
	speedtest(1000);
}
