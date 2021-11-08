#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <set>
#include <queue>
#include <bitset>
#include <fstream>
#include <memory>
#include <iomanip>

constexpr int N = 3;
constexpr int BOARD_SIZE = 18;
using ulli = unsigned long long int;
using lli = unsigned long long int;

enum class STONE {
	NONE = 0b00,
	WHITE = 0b01,
	BLACK = 0b10
};


using Board = std::bitset<BOARD_SIZE>;
using Hand = std::pair<int, STONE>;


enum class GameState {
	DRAW = 0,
	CONTINUE = 1,
	WHITE_WIN = 2,
	BLACK_WIN = 3
};

class Node {
public:
	std::vector<int> parents;
	std::vector<int> children;

	//�[���B
	int depth = 0;
	//�{�[�h�̏�ԁA2�i���ł���킳���
	Board state;
	//�W�J�ς݂��ǂ���
	bool hasExpansion = false;
	//�I�[����
	bool isLeaf = false;
	//�Q�[���l
	double gameValue = 0;
	//�Q�[���l���^����ꂽ���ǂ���
	bool hasGameValue = false;
};

struct BoardOpe {

	bool operator()(const Board& x, const Board& y) const
	{
		constexpr int board_size = 2 * N * N;
		for (int i = board_size - 1; i >= 0; i--) {
			if (x[i] ^ y[i]) return y[i];
		}
		return false;
	}

};

using HashTable = std::map<Board, Node, BoardOpe>;
using NodeTable = std::vector<Node>;

/*
1 2 3
4 5 6
7 8 9
*/
class PlayerBase {
public:

	virtual Board compute(const HashTable& nodes, Board now) = 0;
private:

};

class TicTacToeGame {
public:

	TicTacToeGame() {

	}

	static bool Play(HashTable& m, std::unique_ptr<PlayerBase> first, std::unique_ptr<PlayerBase> second) {
		Board gameBoard;

		while (TicTacToeGame::Check(gameBoard) == GameState::CONTINUE) {

			std::cout << "�^�[���F" << m[gameBoard].depth << std::endl;
			if (true) { //debug
				std::cout << "child size�F" << m[gameBoard].children.size() << std::endl;
				for (const Board& b : m[gameBoard].children) {
					std::cout << std::setprecision(2) << m[b].gameValue << " ";
				}
				std::cout << std::endl;
				std::cout << "game value�F" << m[gameBoard].gameValue << std::endl;

			}
			TicTacToeGame::DisplayBoard(gameBoard, std::cout);

			if (m[gameBoard].depth % 2 == 0) {
				gameBoard = first->compute(m, gameBoard);
			}
			else {
				gameBoard = second->compute(m, gameBoard);
			}
			if (!m.contains(gameBoard)) {

				std::cerr << "�G���[�F�s���ȃ{�[�h�ł�" << std::endl;
				return false;
			}

		}

		GameState state = TicTacToeGame::Check(gameBoard);

		switch (state)
		{
		case GameState::DRAW:
			std::cout << "���������ł�" << std::endl;
			break;
		case GameState::WHITE_WIN:
			std::cout << "��U�̏����ł�" << std::endl;
			break;
		case GameState::BLACK_WIN:
			std::cout << "��U�̏����ł�" << std::endl;
			break;
		default:
			break;
		}

		return true;
	}

	static GameState AutoPlay(HashTable& m, std::unique_ptr<PlayerBase> first, std::unique_ptr<PlayerBase> second) {
		Board gameBoard;

		while (TicTacToeGame::Check(gameBoard) == GameState::CONTINUE) {

			if (m[gameBoard].depth % 2 == 0) {
				gameBoard = first->compute(m, gameBoard);
			}
			else {
				gameBoard = second->compute(m, gameBoard);
			}
			if (!m.contains(gameBoard)) {

				std::cerr << "�G���[�F�s���ȃ{�[�h�ł�" << std::endl;
				return GameState::CONTINUE;
			}

		}

		return TicTacToeGame::Check(gameBoard);
	}

	static const STONE get(const Board& b, const int p) {
		return (STONE)(b[2 * p - 2] + b[2 * p - 1] * 2);
	}

	static const Board& set(Board& b, const int p, STONE s) {
		switch (s)
		{
		case STONE::NONE:
			b[2 * p - 2] = 0;
			b[2 * p - 1] = 0;
			break;
		case STONE::WHITE:
			b[2 * p - 2] = 1;
			b[2 * p - 1] = 0;
			break;
		case STONE::BLACK:
			b[2 * p - 2] = 0;
			b[2 * p - 1] = 1;
			break;
		default:
			break;
		}

		return b;
	}

	static GameState Check(const Board& b) {

		int lines[8][3] = { {1,2,3}, {4,5,6},  {7,8,9}, {1,4,7},  {2,5,8}, {3,6,9},  {1,5,9}, {3,5,7} };

		for (int i = 0; i < std::extent<decltype(lines)>::value; i++) {
			const bool cond1 = get(b, lines[i][0]) == get(b, lines[i][1]);
			const bool cond2 = get(b, lines[i][0]) == get(b, lines[i][2]);

			if (cond1 && cond2) {
				if (get(b, lines[i][0]) == STONE::BLACK) {
					return GameState::BLACK_WIN;
				}
				else if (get(b, lines[i][0]) == STONE::WHITE) {
					return GameState::WHITE_WIN;
				}
			}
		}

		for (int i = 0; i < N * N; i++) {
			if (get(b, i + 1) == STONE::NONE) {
				return GameState::CONTINUE;
			}
		}

		return GameState::DRAW;
	}

	static const Board Operate(const Board& b, Hand hand) {

		if (get(b, hand.first) == STONE::NONE) {
			Board result = b;
			set(result, hand.first, hand.second);
			return result;
		}
		else {
			return b;
		}
	}

	static const char GetChar(const STONE& s) {

		switch (s)
		{
		case STONE::NONE:
			return '_';
			break;
		case STONE::WHITE:
			return 'O';
			break;
		case STONE::BLACK:
			return 'X';
			break;
		default:
			break;
		}
		return '?';
	}

	static void DisplayCSV(const Node& n, std::ostream& ost) {


		if (n.depth != N * N) return;

		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				STONE s = get(n.state, y * 3 + x + 1);
				ost << GetChar(s);
				ost << ",";
			}
			ost << std::endl;
		}
		ost << std::endl;
		ost << n.depth << "," << n.gameValue << "," << (int)Check(n.state) << std::endl << std::endl;
	}


	static void DisplayBoard(const Board& b, std::ostream& ost) {


		for (int y = 0; y < N; y++) {
			for (int x = 0; x < N; x++) {
				STONE s = TicTacToeGame::get(b, y * 3 + x + 1);
				switch (s)
				{
				case STONE::NONE:
					std::cout << y * 3 + x + 1;
					break;
				case STONE::WHITE:
					std::cout << TicTacToeGame::GetChar(s);
					break;
				case STONE::BLACK:
					std::cout << TicTacToeGame::GetChar(s);
					break;
				default:
					break;
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}

	}
};

double calcGameValue(const Node& n) {

	GameState s = TicTacToeGame::Check(n.state);

	switch (s)
	{
	case GameState::DRAW:
		return 0;
		break;
	case  GameState::CONTINUE:
		break;
	case  GameState::WHITE_WIN:
		return 1 * (1.0 / n.depth);
		break;
	case  GameState::BLACK_WIN:
		return -1 * (1.0 / n.depth);
		break;
	default:
		break;
	}

	return 0;
}

class HumanPlayer :public PlayerBase {
public:
	Board compute(const HashTable& nodes, Board now) {
		Hand hand;

		if (nodes.find(now)->second.depth % 2 == 0) {
			hand.second = STONE::WHITE;
		}
		else {
			hand.second = STONE::BLACK;
		}

		std::cout << "���Ȃ��́A" << TicTacToeGame::GetChar(hand.second) << std::endl;
		std::cin >> hand.first;

		return TicTacToeGame::Operate(now, hand);
	}
};

class RandomAI :public PlayerBase {
public:
	Board compute(const HashTable& nodes, Board now) {

		const Node& nowNode = nodes.find(now)->second;

		std::random_device rd;
		std::default_random_engine engine(rd());
		std::uniform_int<> dis(0, nowNode.children.size() - 1);
		return nowNode.children[dis(engine)];
	}
private:

};

class StrongAI :public PlayerBase {
public:
	Board compute(const HashTable& nodes, Board now) {

		const Node& nowNode = nodes.find(now)->second;

		int result = 0;
		double value;

		if (nowNode.depth % 2 == 0) {
			value = -1;
			int i = 0;
			for (const Board& child : nowNode.children) {
				const Node& childNode = nodes.find(child)->second;
				if (value < childNode.gameValue) {
					value = childNode.gameValue;
					result = i;
				}
				i++;
			}
		}
		else {
			value = 1;
			int i = 0;
			for (const Board& child : nowNode.children) {
				const Node& childNode = nodes.find(child)->second;
				if (value > childNode.gameValue) {
					value = childNode.gameValue;
					result = i;
				}
				i++;
			}
		}
		return nowNode.children[result];
	}
private:

};

class WeekAI :public PlayerBase {
public:
	Board compute(const HashTable& nodes, Board now) {

		const Node& nowNode = nodes.find(now)->second;

		int result = 0;
		double value;

		if (nowNode.depth % 2 == 0) {
			value = 1;
			int i = 0;
			for (const Board& child : nowNode.children) {
				const Node& childNode = nodes.find(child)->second;
				if (value > childNode.gameValue) {
					value = childNode.gameValue;
					result = i;
				}
				i++;
			}
		}
		else {
			value = -1;
			int i = 0;
			for (const Board& child : nowNode.children) {
				const Node& childNode = nodes.find(child)->second;
				if (value < childNode.gameValue) {
					value = childNode.gameValue;
					result = i;
				}
				i++;
			}
		}
		return nowNode.children[result];
	}
private:

};


//�𒼂ɒT�������9!�����邽�߁ADP�i�g�����X�|�W�V�����e�[�u���j��p�������������s�����Ƃɂ����B


//�n�b�V���֐�
// �ł���Έȉ��̗l�Ɏ�������ׂ������A�Z�~�Q�[�������炳�ڂ���
//https://en.wikipedia.org/wiki/Zobrist_hashing

int main() {

	//////////////������

	//�n�b�V���e�[�u����p����������
	//https://en.wikipedia.org/wiki/Transposition_table
	NodeTable m;
	//���D��T�����g���ꍇ�AQueue��p����ׂ��B
	std::queue<size_t> updateQueue;
	const Board startHashValue = Board();
	{
		Node start;
		start.depth = 0;
		start.state = 0;

		m.push_back(start);
		updateQueue.push(0);
	}

	m.reserve(5*9*8*7*6*5*4*3*2*1);

	////////////�Q�[���؂̓W�J
	while (!updateQueue.empty()) {

		const size_t top = updateQueue.front();
		Node& now = m[top];
		updateQueue.pop();

		if (!now.hasExpansion) {
			now.hasExpansion = true;

			//�W�J����
			for (int i = 0; i < N * N; i++) {
				if (TicTacToeGame::get(now.state, i + 1) == STONE::NONE) {

					const STONE& stone = now.depth % 2 == 0 ? STONE::WHITE : STONE::BLACK;
					//STONE next_stone = now.depth % 2 == 1 ? STONE::WHITE : STONE::BLACK;

					Node child;
					child.state = TicTacToeGame::Operate(now.state, Hand(i + 1, stone));
					child.depth = now.depth + 1;
					if (TicTacToeGame::Check(child.state) != GameState::CONTINUE) { //�I�[�ł��邽�ߏ�������
						child.isLeaf = true;
						child.hasGameValue = true;
						child.gameValue = ::calcGameValue(child);
					}
					child.parents.push_back(top);
					now.children.push_back(m.size());
					updateQueue.push(m.size());
					m.push_back(child);

				}
			}
		}
	}

	///////////�Q�[���؂̌�މ��(backtrack)

	//�[������̎��́AMinMax�ōŏ��ȂƂ������邽�߁A�ŏ��l�B
	//�[���������̎��́AMinMax�ōő�ȂƂ������邽�߁A�ő�l�B

	//updateQueue���g���܂킷�B�I�[����A�Q�[���l���������A�[�@�ŋ��߂�B�ڂ����́h�Q�[�����_�i���c�@�́j�h
	for (const auto& n : m) {
		if (n.isLeaf) {
			for (int i = 0; i < n.parents.size(); i++)
				updateQueue.push(n.parents[i]);
		}
	}

	int countHasGameValue = 0;
	int countDoesntHaveGameValue = 0; //���̒l��0�̎��������B

	while (!updateQueue.empty()) {
		const int top = updateQueue.front();
		Node& now = m[top];
		updateQueue.pop();

		if (!now.hasGameValue) {

			bool check = true;

			for (int i = 0; i < now.children.size(); i++) {
				if (!m[now.children[i]].hasGameValue) {
					check = false;
				}
			}

			if (check) {
				now.hasGameValue = true;

				if (now.depth % 2 == 0) { //�ő�l
					double maxValue = -1;
					for (int i = 0; i < now.children.size(); i++) {
						maxValue = std::max(maxValue, m[now.children[i]].gameValue);
					}
					now.gameValue = maxValue;
				}
				else { //�ŏ��l
					double minValue = 1;
					for (int i = 0; i < now.children.size(); i++) {
						minValue = std::min(minValue, m[now.children[i]].gameValue);
					}
					now.gameValue = minValue;
				}

				for (int i = 0; i < now.parents.size(); i++)
					updateQueue.push(now.parents[i]);
			}
			else {
				updateQueue.push(top);
			}
		}
	}

	///////////���ʋy�уQ�[���؂̕\��
	std::cout << "�Q�[���S�̂͏���(0<a)or����(a<0)or��������(a=0)�ł��B:" << m[0].gameValue << std::endl;
	{
		int depthList[N * N + 1] = {};
		for (const auto& n : m) {

			if (n.hasGameValue)
				countHasGameValue++;
			else {
				countDoesntHaveGameValue++;
			}

			depthList[n.depth]++;

			if (false) {//���ʂ̕\�����K�v�ȂƂ��͕t���āB���������Ȃ�o�܂��B
				TicTacToeGame::DisplayCSV(n, std::cout);
			}

		}

		for (int i = 0; i < N * N + 1; i++) {
			std::cout << depthList[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "��މ�͂����ꂽ:" << countHasGameValue << std::endl;
		std::cout << "��މ�͂�����Ȃ�:" << countDoesntHaveGameValue << std::endl;
	}


	////////////�Q�[�����v���C����B�l�ԓ��m�Ńv���C����Ȃ炱�����A���R�����g
	//TicTacToeGame::Play(m, std::unique_ptr<PlayerBase>(new HumanPlayer()), std::unique_ptr<PlayerBase>(new StrongAI()));
}

