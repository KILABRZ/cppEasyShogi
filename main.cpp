#include "shogi.hpp"
#include <cstdlib>
#include <ctime> 

void saveLoadTest(Shogi s){
	s.EasyBoardPrint();
	cout << s.round << "\n";
	s.LoadGame(s.SaveGame());
	s.EasyBoardPrint();
	cout << s.round << "\n";
}

string vectorHexdump(vector<unsigned char> k){
	string s;
	char charset[17] = "0123456789ABCDEF";
	for(char a : k){
		unsigned char c = a;
		s.append(1, charset[c/16]);
		s.append(1, charset[c%16]);
	}
	return s;
}

string vectorHexdump(vector<char> k){
	string s;
	char charset[17] = "0123456789ABCDEF";
	for(char a : k){
		unsigned char c = a;
		s.append(1, charset[c/16]);
		s.append(1, charset[c%16]);
	}
	return s;
}

vector<char> vectorHexload(string s){
	vector<char> c;
	c.reserve(s.length()/2);
	for(int i=0;i<s.length();i+=2){
		char v = 0;
		v += (s[i] - (s[i] >= 'A' ? ('A'-10) : ('0')))*16;
		v += (s[i+1] - (s[i+1] >= 'A' ? ('A'-10) : ('0')));
		c.push_back(v);
	}
	return c;
}

vector<unsigned char> UvectorHexload(string s){
	vector<unsigned char> c;
	c.reserve(s.length()/2);
	for(int i=0;i<s.length();i+=2){
		char v = 0;
		v += (s[i] - (s[i] >= 'A' ? ('A'-10) : ('0')))*16;
		v += (s[i+1] - (s[i+1] >= 'A' ? ('A'-10) : ('0')));
		c.push_back(v);
	}
	return c;
}


void random_playing_test_visible(int n){
	Shogi s;
	s.Init();

	for(int i=0;i<n;i++){
		
		vector<int> moveList = s.FetchMove(3);
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

vector<char> randomlyChange(vector<char> k, int c){
	vector<char> r;
	r.reserve(k.size());
	for(char d : k){
		char f = d + (char)(rand() % (2*c+1)) + (char)c;
		r.push_back(f);
	}
	return r;
}

int fight(ShogiBot sente, ShogiBot gote, int maxRound, int depth, double randomP){
	Shogi s;
	s.Init();
	while(s.round < maxRound){
//		cout << s.round << "\n";
		s.EasyBoardPrint();
		if(s.round % 2 == 0){
			int move = sente.decideMove(s, depth, randomP);
			if(move == -1){
				return -1;
			}
			s.MakeMove(move);
		}else{
			int move = gote.decideMove(s, depth, randomP);
			if(move == -1){
				return 1;
			}
			s.MakeMove(move);
		}
	}
	return 0;
}

vector<char> crossover(vector<char> a, vector<char> b){
	vector<char> c;
	c.reserve(a.size());

	for(int i=0;i<a.size();i++){
		int dice = rand() % 20;
		if(dice < 9) c.push_back(a[i]);
		else if(dice < 18) c.push_back(b[i]);
		else c.push_back((char)(rand() % 256));
	}
	return c;
}

void findbestvector(int N, int T, double R){
	cout << "Start, generate vector\n";
	cout << "N = " << N << ", T = " << T << ", R = " << R << "\n";
	ShogiBot* players = new ShogiBot[N];
	ShogiBot* tmp = new ShogiBot[N];
	string specialVector = "054B322864647850505050507D7D0103FFFF0101FEFF010101010303FD03FDFD640AF6FBFBFBFBFBFBFB0A0A03030A05FF0A0F0F0F0F0F5A0505050505050505050505050505011E";
	for(int i=0;i<N;i++){
		players[i].SpecialInit(randomlyChange(vectorHexload(specialVector), 5));
	}
	vector<char> bestVector;
	for(int i=0;i<T;i++){
		int winRecord[1000] = {0};
		cout << "- Tier " << i+1 << " start -\n";
		for(int i=0;i<N;i++){
			cout << i << ": " << vectorHexdump(players[i].judgingVector) << "\n";
		}
		for(int i=0;i<2*N;i++){
			int sente = rand() % N;
			int gote = (sente + rand() % (N-1) + 1) % N;
			int result = fight(players[sente], players[gote], 500, 2, R);
			winRecord[sente] += result;
			winRecord[gote] -= result;
			if(result == 1)cout << sente << " win " << gote << "\n";
			else if(result == -1) cout << gote << " win " << sente << "\n";
			else cout << sente << " tie " << gote << "\n";
		}
		cout << "- Tier " << i+1 << " result -\n";
		
		int mostWin = 0;
		for(int i=0;i<N;i++){
			cout << vectorHexdump(players[i].judgingVector) << ": " << winRecord[i] << "\n";
			if(winRecord[i] > mostWin){
				mostWin = winRecord[i];
				bestVector = players[i].judgingVector;
			}
		}
		double lastGate = 0.6;
		int lastCount = 0;
		int threhold = mostWin;

		while((double)(lastCount)/(double)(N) < lastGate){
			for(int i=0;i<N;i++){
				int dice = rand() % 6;
				if(winRecord[i] >= threhold and dice > 2){
					tmp[lastCount++] = players[i];
					winRecord[i] = -600;
				}
			}
			threhold--;
		}

		while(lastCount < N){
			int a = rand() % N;
			int b = (a + rand() % (N-1) + 1) % N;
			tmp[lastCount++].SpecialInit(
				crossover(players[a].judgingVector, players[b].judgingVector)
			);
		}
		for(int i=0;i<N;i++){
			players[i] = tmp[i];
			tmp[i].judgingVector.clear();
		}
	}

	cout << "best vector = " << vectorHexdump(bestVector) << "\n";

	delete players;
	delete tmp;
}

void botTest(){
	ShogiBot sente;
	ShogiBot gote;
	sente.RandomInit();
	gote.RandomInit();

	vector<char> testV1 = vectorHexload("0A503E526E3E8140025B7F5E1D89090809D9EA8E0EC074070C070D0EB330BEF86CFC557D2B88D14174557C45C5104169041F1C481A3DBB8CC3C849E18E0E1211122D3A3486F67F29");
	vector<char> testV2 = vectorHexload("054B322864647850505050507D7D0103FFFF0101FEFF010101010303FD03FDFD640AF6FBFBFBFBFBFBFB0A0A03030A05FF0A0F0F0F0F0F5A0505050505050505050505050505011E");

	sente.SpecialInit(testV1);
	gote.SpecialInit(testV2);
	vectorHexdump(testV1);
	vectorHexdump(testV2);
	Shogi s;
	s.Init();

	int result = fight(sente, gote, 1000, 3, 1);
	cout << result << "\n";
	
}

void humanFightBot(){
	ShogiBot gote;
	gote.RandomInit();
	vector<char> testV2 = vectorHexload("054B322864647850505050507D7D0103FFFF0101FEFF010101010303FD03FDFD640AF6FBFBFBFBFBFBFB0A0A03030A05FF0A0F0F0F0F0F5A0505050505050505050505050505011E");
	gote.SpecialInit(testV2);
	Shogi s;
	s.Init();

	while(true){
		s.EasyBoardPrint();
		vector<int> killingMoveList = s.FetchMove(4);
		for(int move : killingMoveList){
			printMove(move);
		}cout << killingMoveList.size() << "\n";
		vector<int> moveList = s.FetchMove(3);
		if(moveList.size() == 0){
			cout << "End\n";
			break;
		}
		if(s.round % 2 == 0){
			int move;
			bool finish = false;
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
			s.MakeMove(move);
		}else{
			int move = gote.decideMove(s, 3, 0.4);
			s.MakeMove(move);
		}
		
	}
}

void setGame(){
	string cmd;
	Shogi s;
	s.WhiteInit();
	queue<int> godTable[8];
	for(int i=0;i<40;i++){
		int gomaid = gomakindID(s.gomaKind[i]);
		godTable[gomaid].push(i);
	}

	while(true){
		cout << "Foot    Silver  Cassia  Chariot Flying  Angle   King   Gold  \n";
		for(int i=0;i<8;i++){
			cout << godTable[i].size() << "       ";
		}
		cout << "\n";
		s.EasyBoardPrint();
		
		cout << "\n";
		cin >> cmd;
		if(cmd == "exit"){
			break;
		}else if(cmd == "set"){
			int gomacode, suji, dan, chesser, upgrade, gomatable;
			cin >> suji >> dan >> gomacode >>  chesser >> upgrade >> gomatable;
			int pos = genPos(suji, dan);
			int gomanum = godTable[gomacode].front();
			godTable[gomacode].pop();
			s.SetGoma(gomanum, chesser, upgrade, pos, gomatable);
		}else if(cmd == "save"){
			cout << vectorHexdump(s.SaveGame()) << "\n";
		}else if(cmd == "all"){
			for(int i=0;i<8;i++){
				if(i == 6)continue;
				while(!godTable[i].empty()){
					int gomanum = godTable[i].front();
					godTable[i].pop();
					s.SetGoma(gomanum, GOTE, NORMAL, 0, 1);
				}
			}
		}else if(cmd == "remove"){
			int table, suji, dan, pos;
			cin >> table >> suji >> dan;
			if(table){
				pos = genUPos(suji, dan);
			}else{
				pos = genPos(suji, dan);
			}
			int getGoma = s.RemoveGoma(pos, table);
			int gomaid = gomakindID(s.gomaKind[getGoma]);
			godTable[gomaid].push(getGoma);
		}
	}
}

vector<int> killOrder(vector<int> moveList, Shogi s){
	vector<int> newMoveList;
	newMoveList.reserve(moveList.size());

	for(int move : moveList){
		int newPos = moveNewpos(move);
		if(s.boardChesser[newPos] != -1)newMoveList.push_back(move);
	}
	for(int move : moveList){
		int newPos = moveNewpos(move);
		if(s.boardChesser[newPos] == -1)newMoveList.push_back(move);
	}
	return newMoveList;
}

int tsumeWalk(Shogi s, int depth){
	int chesser = s.round % 2;
	if(depth < 0){
		return -1;
	}
	
	vector<int> moveList;
	if(chesser == SENTE){
		if(s.gomaPos[s.SENTEKINGNUM] == -1){
			moveList = s.FetchMove(5);
		}else{
			moveList = s.FetchMove(4);
		}	 
	}else{
		moveList = s.FetchMove(3);
	}

	if(chesser == GOTE){
		moveList = killOrder(moveList, s);
	}
	

	int bestAnswer = (chesser == SENTE ? -1 : 0);
	int bestMove = -1;

	for(int move : moveList){
		Shogi ns = s;
		ns.MakeMove(move);		
		int answer = tsumeWalk(ns, depth-1);

		if(chesser == SENTE and answer != -1){
			if(bestAnswer == -1){
				bestAnswer = answer+1;
				bestMove = move;
			}
			else if(bestAnswer > answer+1){
				bestAnswer = answer+1;
				bestMove = move;
			}
		}
		if(chesser == GOTE and bestAnswer != -1){
			if(answer == -1){
				bestAnswer = -1;
				bestMove = move;
				break;
			}
			else if(answer+1 > bestAnswer){
				bestAnswer = answer+1;
				bestMove = move;
			}
		}
	}
	
	return bestAnswer;
}

bool tsumeWalkB(Shogi s, int depth){
	int chesser = s.round % 2;
	if(depth < 0){
		return false;
	}
	
	vector<int> moveList;
	if(chesser == SENTE){
		if(s.gomaPos[s.SENTEKINGNUM] == -1){
			moveList = s.FetchMove(5);
		}else{
			moveList = s.FetchMove(4);
		}	 
	}else{
		moveList = s.FetchMove(3);
	}

	if(chesser == GOTE){
		moveList = killOrder(moveList, s);
	}

	for(int move : moveList){
		Shogi ns = s;
		ns.MakeMove(move);		
		bool answer = tsumeWalkB(ns, depth-1);
		if(chesser == SENTE and answer){
			return true;
		}
		if(chesser == GOTE and !answer){
			return false;
		}
	}
	if(chesser == SENTE)return false;
	else return true;
}

void solveTsumeShogi(){
	vector<unsigned char> gamecode = UvectorHexload("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF11FFFFFFFFFFFFFFFF16FF01FFFFFFFFFFFF11FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF05FFFFFF000100000000000012000404020100040000");
	vector<unsigned char> gamecode2 = UvectorHexload("FFFF1DFFFFFFFFFFFFFF16FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF08FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000100000100000111030404010100030000");
	vector<unsigned char> gamecode3 = UvectorHexload("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF11FF16171002FFFFFF05FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000010100000011030303010100030000");

	Shogi s;
	s.Init();
	s.LoadGame(gamecode3);

	cout << tsumeWalkB(s, 11) << "\n";
	s.EasyBoardPrint();
}


int main(){
	srand(time(0));
//	findbestvector(100, 50, 0.35);
	solveTsumeShogi();
}
