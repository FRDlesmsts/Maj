#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include <algorithm>
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::cin;
using std::cout;
using std::endl;
using std::sort;
using std::min;
using std::max;

int countAll = 0;
float allStepTo = 0;
float averageStepTo = 0;

struct Tile {
	Tile(int num, char type) : num(num), rank(num == 0 ? 5 : num), type(type) {};
	int num;
	int rank;
	char type;
	void out() {
		cout << num << type;
	}
	void outNum() {
		cout << num;
	}
	Tile next() {
		if (type == 'z') {
			return Tile(rank == 4 ? 1 : (rank == 7 ? 5 : rank + 1), type);
		}
		else {
			return Tile(rank == 9 ? 1 : rank + 1, type);
		}
	}
	bool isOneNine() {
		if (type == 'z' || num == 1 || num == 9) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator < (const Tile& t)const {
		if (type == t.type) {
			return rank < t.rank;
		}
		else {
			return type < t.type;
		}
	}
	bool operator > (const Tile& t)const {
		if (type == t.type) {
			return rank > t.rank;
		}
		else {
			return type > t.type;
		}
	}
	bool operator == (const Tile& t)const {
		return (rank == t.rank) && (type == t.type);
	}
	bool operator != (const Tile& t)const {
		return (rank != t.rank) || (type != t.type);
	}
};

bool compareSmall(Tile t1, Tile t2) {
	return t1 < t2;
}

bool compareLarge(Tile t1, Tile t2) {
	return t1 > t2;
}

int distance(Tile t1, Tile t2) {
	if (t1.type == t2.type && t1.type != 'z' && t2.type != 'z') {
		if (abs(t1.num - t2.num) == 1) {
			return 1;
		}
		if (abs(t1.num - t2.num) == 2) {
			return 2;
		}
	}
	return -1;
}

struct Game {
	struct Database {
		vector<Tile> tiles;
		int size;
		void init() {
			for (int i = 1; i <= 4; i++) {
				for (int n = 1; n <= 4; n++) {
					tiles.push_back(Tile(i, 'm'));
					tiles.push_back(Tile(i, 'p'));
					tiles.push_back(Tile(i, 's'));
				}
			}
			for (int n = 1; n <= 3; n++) {
				tiles.push_back(Tile(5, 'm'));
				tiles.push_back(Tile(5, 'p'));
				tiles.push_back(Tile(5, 's'));
			}
			tiles.push_back(Tile(0, 'm'));
			tiles.push_back(Tile(0, 'p'));
			tiles.push_back(Tile(0, 's'));
			for (int i = 6; i <= 9; i++) {
				for (int n = 1; n <= 4; n++) {
					tiles.push_back(Tile(i, 'm'));
					tiles.push_back(Tile(i, 'p'));
					tiles.push_back(Tile(i, 's'));
				}
			}
			for (int i = 1; i <= 7; i++) {
				for (int n = 1; n <= 4; n++) {
					tiles.push_back(Tile(i, 'z'));
				}
			}
			size = tiles.size();
		}
		void out() {
			for (int i = 0; i < tiles.size(); i++) {
				tiles[i].out();
			}
		}
		void disorder() {
			vector<Tile> temp;
			for (int i = 0; i < size; i++) {
				int n = rand() % tiles.size();
				temp.push_back(tiles[n]);
				tiles.erase(tiles.begin() + n);
			}
			tiles.clear();
			for (int i = 0; i < size; i++) {
				tiles.push_back(temp[i]);
			}
		}
		void sortSmall() {
			sort(tiles.begin(), tiles.end(), compareSmall);
		}
		void sortLarge() {
			sort(tiles.begin(), tiles.end(), compareLarge);
		}
	};

	struct Player {
		vector<Tile> hand;
		int size() {
			return hand.size();
		}
		void getTile(Tile tile) {
			hand.push_back(tile);
		}
		void playTile(Tile tile) {
			for (int i = 0; i < size(); i++) {
				if (hand[i].num == tile.num && hand[i].type == tile.type) {
					if (i != size() - 1) {
						hand.erase(hand.begin() + i);
					}
					else {
						hand.pop_back();
					}
				}
			}
		}
		void playTile(int i) {
			if (i != size() - 1) {
				hand.erase(hand.begin() + i);
			}
			else {
				hand.pop_back();
			}
		}
		void sortSmall() {
			sort(hand.begin(), hand.end(), compareSmall);
		}
		void out() {
			for (int i = 0; i < size(); i++) {
				if (i != size() - 1 && hand[i + 1].type == hand[i].type) {
					hand[i].outNum();
				}
				else {
					hand[i].out();
				}
			}
		}
		int doraNum(vector<Tile> doras) {
			int C = 0;
			for (int h = 0; h < size(); h++) {
				for (int i = 0; i < doras.size(); i++) {
					if (hand[h] == doras[i].next()) {
						C++;
					}
				}
				if (hand[h].num == 0) {
					C++;
				}
			}
			return C;
		}
		int doraNum(Tile dora) {
			int C = 0;
			for (int h = 0; h < size(); h++) {
				if (hand[h] == dora.next()) {
					C++;
				}
				if (hand[h].num == 0) {
					C++;
				}
			}
			return C;
		}
		int maxTypeNum() {
			unordered_map<char, int> map;
			map.insert(pair<char, int>('m', 0));
			map.insert(pair<char, int>('p', 0));
			map.insert(pair<char, int>('s', 0));
			map.insert(pair<char, int>('z', 0));
			for (int i = 0; i < size(); i++) {
				map[hand[i].type]++;
			}
			int M = 0;
			for (auto it = map.begin(); it != map.end(); it++) {
				M = it->second >= M ? it->second : M;
			}
			return M;
		}
		int stepToAll() {
			sortSmall();
			int S = min(stepToOrph(), stepToHeads());
			S = min(S, stepTo());
			return S;
		}
		int stepToHeads() {
			sortSmall();
			int S;
			vector<Tile> heads;
			for (int i = 0; i < size(); i++) {
				if (i != 0 && hand[i] == hand[i - 1]) {
					if (heads.size() != 0 && hand[i] == heads[heads.size() - 1]) {
						continue;
					}
					heads.push_back(hand[i]);
				}
			}
			S = 6 - heads.size();
			return S;
		}
		int stepToOrph() {
			sortSmall();
			int S;
			vector<Tile> oneNines;
			int head = 0;
			for (int i = 0; i < size(); i++) {
				if (hand[i].isOneNine()) {
					if (i == 0) {
						oneNines.push_back(hand[i]);
					}
					else if (hand[i] != hand[i - 1]) {
						oneNines.push_back(hand[i]);
					}
					else if (head == 0) {
						oneNines.push_back(hand[i]);
						head++;
						continue;
					}
				}
			}
			S = 13 - oneNines.size();
			return S;
		}
		// https://zhuanlan.zhihu.com/p/31000381
		int stepTo() {
			sortSmall();
			int S = 8;
			int C_max = 0;
			int C_rem = size();
			int K = (C_rem - 2) / 3;
			getHead(hand, C_rem, S, C_max, K);
			return S;
		}
		void getHead(vector<Tile> tiles, int C_rem, int& S, int& C_max, int K) {
			for (int i = 0; i < tiles.size() - 1; i++) {
				if (tiles[i] == tiles[i + 1]) {
					vector<Tile> tempTiles(tiles);
					tempTiles.erase(tempTiles.begin() + i);
					if (i == tempTiles.size() - 1) {
						tempTiles.pop_back();
					}
					else {
						tempTiles.erase(tempTiles.begin() + i + 1 - 1);
					}
					getBody(tempTiles, 0, C_rem - 2, S, C_max, K, 1, 0);
				}
			}
			getBody(tiles, 0, C_rem, S, C_max, K, 0, 0);
			return;
		}
		void getBody(vector<Tile> tiles, int i, int C_rem, int& S, int& C_max, int K, int P, int G) {
			if (i >= tiles.size()) {
				getBodyLack(tiles, 0, C_rem, S, C_max, K, P, G, 0);
				return;
			}
			for (int j = i + 1; j < tiles.size() - 1; j++) {
				for (int k = j + 1; k < tiles.size(); k++) {
					if (tiles[i] == tiles[j] && tiles[k] == tiles[j]) {
						vector<Tile> tempTiles(tiles);
						tempTiles.erase(tempTiles.begin() + i);
						tempTiles.erase(tempTiles.begin() + j - 1);
						if (k - 1 - 1 == tempTiles.size() - 1) {
							tempTiles.pop_back();
						}
						else {
							tempTiles.erase(tempTiles.begin() + k - 1 - 1);
						}
						getBody(tempTiles, i, C_rem - 3, S, C_max, K, P, G + 1);
					}
					if (distance(tiles[j], tiles[i]) == 1 && distance(tiles[k], tiles[j]) == 1) {
						vector<Tile> tempTiles(tiles);
						tempTiles.erase(tempTiles.begin() + i);
						tempTiles.erase(tempTiles.begin() + j - 1);
						if (k - 1 - 1 == tempTiles.size() - 1) {
							tempTiles.pop_back();
						}
						else {
							tempTiles.erase(tempTiles.begin() + k - 1 - 1);
						}
						getBody(tempTiles, i, C_rem - 3, S, C_max, K, P, G + 1);
					}
				}
			}
			getBody(tiles, i + 1, C_rem, S, C_max, K, P, G);
			return;
		}
		void getBodyLack(vector<Tile> tiles, int i, int C_rem, int& S, int& C_max, int K, int P, int G, int G_) {
			if (S == -1) {
				return;
			}
			if (G + G_ > K) {
				return;
			}
			int C = 3 * G + 2 * G_ + 2 * P;
			if (C_rem < C_max - C) {
				return;
			}
			if (C_rem == 0) {
				S = min(S, 2 * (K - G) - G_ - P);
				C_max = max(C_max, C);
				return;
			}
			for (int j = i + 1; j < tiles.size(); j++) {
				if (tiles[i] == tiles[j]) {
					vector<Tile> tempTiles(tiles);
					tempTiles.erase(tempTiles.begin() + i);
					if (j - 1 == tempTiles.size() - 1) {
						tempTiles.pop_back();
					}
					else {
						tempTiles.erase(tempTiles.begin() + j - 1);
					}
					getBodyLack(tempTiles, i, C_rem - 2, S, C_max, K, P, G, G_ + 1);
				}
				if (distance(tiles[j], tiles[i]) != -1) {
					vector<Tile> tempTiles(tiles);
					tempTiles.erase(tempTiles.begin() + i);
					if (j - 1 == tempTiles.size() - 1) {
						tempTiles.pop_back();
					}
					else {
						tempTiles.erase(tempTiles.begin() + j - 1);
					}
					getBodyLack(tempTiles, i, C_rem - 2, S, C_max, K, P, G, G_ + 1);
				}
			}
			getBodyLack(tiles, i + 1, C_rem - 1, S, C_max, K, P, G, G_);
			return;
		}
	};
	Player players[4];
	Database database;
	vector<Tile> doras;
	int pos = 0;
	int playerNum;

	void init() {
		database.init();
		database.disorder();
		doras.push_back(database.tiles[database.tiles.size() - 6]);
		playerNum = rand() % 4;
	}
	void outDoras() {
		cout << endl;
		cout << "dora: ";
		for (int i = 0; i < doras.size(); i++) {
			doras[i].next().out();
			cout << " ";
		}
	}
	void deal() {
		for (int t = 1; t <= 3; t++) {
			for (int p = 0; p < 4; p++) {
				for (int k = 1; k <= 4; k++) {
					players[p].getTile(database.tiles[pos]);
					pos++;
				}
			}
		}
		for (int p = 0; p < 4; p++) {
			players[p].getTile(database.tiles[pos]);
			pos++;
		}
		for (int p = 0; p < 4; p++) {
			players[p].sortSmall();
		}
	}
	void playTile(int p) {
		if (p == playerNum) {
			cout << "Play: ";
			int num;
			char type;
			cin >> num >> type;
			Tile play(num, type);
			players[p].playTile(play);
		}
		else {
			//TODO AI Play
		}
	}
	void info(int p) {
		int stepToAll = players[p].stepToAll();
		int doraNum = players[p].doraNum(doras[0]);
		int maxTypeNum = players[p].maxTypeNum();
		cout << endl;
		cout << "stepToAll: " << stepToAll;
		cout << endl;
		cout << "doraNum: " << doraNum;
		cout << endl;
		cout << "maxTypeNum: " << maxTypeNum;
		cout << endl;
	}
	void start() {
		outDoras();
		cout << endl;
		cout << "playerNum: " << playerNum;
		cout << endl;
		cout << endl;
		for (int p = 0; p < 4; p++) {
			cout << p << " ";
			players[p].out();
			cout << " ";
			database.tiles[pos].out();
			cout << endl;

			players[p].getTile(database.tiles[pos]);
			players[p].sortSmall();
			playTile(p);

			cout << p << " ";
			players[p].out();
			cout << endl;
			cout << endl;
		}
		cout << endl;
	}
};

void main() {
	int count = 0;
	unsigned seed;
	seed = time(0);
	srand(seed);
	while (count <= 0) {
		printf("\rGame: %d", count);
		Game game;
		game.init();
		game.deal();
		game.start();
		count++;
	}
}