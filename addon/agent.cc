#include <algorithm>
#include <limits>
#include <random>

#include <iostream>

#include "agent.h"

namespace agent {

using std::uniform_int_distribution;
using std::max;
using std::min;
using std::numeric_limits;

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::ObjectTemplate;
using v8::String;
using v8::Value;

constexpr unsigned char scores[] = {
	1, 1, 2, 2, 2, 2, 3, 3, 2, 2, 3, 3, 3, 3, 4, 5,
	2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5,
	2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
	2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 3, 3, 3, 4, 5,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
	1, 1, 2, 2, 2, 2, 3, 3, 2, 2, 3, 3, 3, 3, 4, 5,
	2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5,
	2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5,
	3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 4, 4, 5,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

constexpr unsigned char masks[] = {
	 24,  14,  29,  12,  26,  10,  25,   8,  20,   6,  21,   4,  18,   2,  17,   0,
	 40,  40,  12,  12,  42,  42,   8,   8, 102, 102,   4,   4,  34,  34,   0,   0,
	 88,  88,  28,  28,  24,  24,  25,   8,  84,  84,  84,   4,  16,  16,  16,  16,
	 72,  72,  72,  72,   8,   8,   8,   8,  68,  68,  68,  68,   0,   0,   0,   0,
	 56,  56,  60,  12,  26,  26,  25,   8,  52,  52,  21,   4,  18,   2,  17,   0,
	168, 168, 168, 168, 170, 170,   4,   4,  32,  32,  32,  32,  34,  34,   0,   0,
	152, 152, 152, 152, 152, 152, 153,   8,  16,  16,  16,  16,  16,  16,  17,   0,
	136, 136, 136, 136, 136, 136, 136, 136,   0,   0,   0,   0,   0,   0,   0,   0,
	 24,  30,  29,  12,  26,  10,  25,   8,  20,   6,  21,   4,  18,   2,  17,   0,
	 40,  40,  12,  12,  42,  42,   8,   8, 102, 102,  37,   4,  34,  34,   0,   0,
	 80,  80,  80,  80,  24,  24,  25,   8,  84,  84,  21,   4,  16,   2,  17,   0,
	 64,  64,  64,  64,  64,  64,  64,  64,  68,  68,  68,  68,   0,   0,   0,   0,
	 48,  48,  48,  48,  48,  48,  48,  48,  48,   6,  21,   4,  18,   2,  17,   0,
	 32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,   4,  34,  34,   0,   0,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  17,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

static int ConutBit(unsigned char n) {
	int count = 0;
	while (n) {
		count += n & 1;
		n >>= 1;
	}
	return count;
}

static int Score_(const size_t i17, const bool * my_board, const bool * op_board) {
	int score = 0;
	constexpr size_t moves[] = {1, 16, 17, 18};
	for (size_t m : moves) {
		unsigned char b = 0;
		unsigned char w = 0;
		for (size_t i = 1; i <= 4; ++i) {
			const size_t index = i17 + m*i;
			w |= op_board[index] << (4 - i);
			if (op_board[index]) {
				break;
			}
			b |= my_board[index] << (4 - i);
		}
		for (size_t i = 1; i <= 4; ++i) {
			const size_t index = i17 - m*i;
			w |= op_board[index] << (3 + i);
			if (op_board[index]) {
				break;
			}
			b |= my_board[index] << (3 + i);
		}
		int s = 1 << (scores[b] - min(ConutBit(w & masks[b]), 2));
		if (score == s) {
			score = (score << 1) - 1;
		} else {
			score = max(score, s); 
		}
	}
	return score;
}

Agent::Agent() : max_depth(10), max_breadth(10), rng_(rd_()) {}

Agent::~Agent() {}

void Agent::Init(Local<Object> exports) {
	Isolate * isolate = exports->GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	Local<ObjectTemplate> addon_data_tpl = ObjectTemplate::New(isolate);
	addon_data_tpl->SetInternalFieldCount(1);  // 1 field for the Agent::New()
	Local<Object> addon_data = addon_data_tpl->NewInstance(context).ToLocalChecked();

	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New, addon_data);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Agent").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tpl, "newGame", NewGame);
	NODE_SET_PROTOTYPE_METHOD(tpl, "play", Play);

	Local<Function> constructor = tpl->GetFunction(context).ToLocalChecked();
	addon_data->SetInternalField(0, constructor);
	exports->Set(
		context,
		String::NewFromUtf8(isolate, "Agent").ToLocalChecked(),
		constructor
	).FromJust();
}

void Agent::New(const FunctionCallbackInfo<Value> & args) {
	Agent * agent = new Agent();
	agent->Wrap(args.This());
	args.GetReturnValue().Set(args.This());
}

void Agent::NewGame(const v8::FunctionCallbackInfo<v8::Value> & args) {
	Agent * agent = ObjectWrap::Unwrap<Agent>(args.Holder());
	
	for (size_t i = 0; i < 17; ++i) {
		agent->black_board_[i] = agent->black_board_[272 + i] = true;
		agent->white_board_[i] = agent->white_board_[272 + i] = true;
	}
	for (size_t i = 1; i < 16; ++i) {
		agent->black_board_[i*17] = agent->black_board_[i*17 + 16] = true;
		agent->white_board_[i*17] = agent->white_board_[i*17 + 16] = true;
		for (size_t j = 1; j < 16; ++j) {
			agent->black_board_[i*17 + j] = false;
			agent->white_board_[i*17 + j] = false;
		}
	}
}

void Agent::Play(const v8::FunctionCallbackInfo<v8::Value> & args) {
	Isolate * isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();
	Agent * agent = ObjectWrap::Unwrap<Agent>(args.Holder());
	
	int white = args[0]->NumberValue(context).FromMaybe(0);
	agent->white_board_[white] = true;

	int black = agent->MinMax();
	agent->black_board_[black] = true;

	args.GetReturnValue().Set(Number::New(isolate, black));
}

int Agent::RandRange(const int range) {
	uniform_int_distribution<int> udist(0, range - 1);
	return udist(rng_);
}

int Agent::Score() {
	for (size_t i = 0; i < 289; ++i) {
		black_scores_[i] = white_scores_[i] = numeric_limits<int>::min();
	}

	int max_score = 0;
	for (size_t i15 = 0; i15 < 225; ++i15) {
		const size_t i17 = i15 + 18 + 2*(i15 / 15);
		if (black_board_[i17] || white_board_[i17]) {
			continue;
		}
		black_scores_[i17] = Score_(i17, black_board_, white_board_);
		white_scores_[i17] = Score_(i17, white_board_, black_board_);
		max_score = max(max_score, max(black_scores_[i17], white_scores_[i17]));
	}
	return max_score;
}

vector<int> Agent::BestMoves(const int * my_scores, const int * op_score) {
	const int score = Score();
	
	vector<int> moves;
	for (size_t i = 18; i < 271; ++i) {
		if (my_scores[i] == score) {
			moves.push_back(i);
		}
	}

	if (moves.size()) {
		return moves;
	}

	for (size_t i = 18; i < 271; ++i) {
		if (op_score[i] == score) {
			moves.push_back(i);
		}
	}
	return moves;
}

int Agent::Evaluate() {
	int b = numeric_limits<int>::min();
	int w = numeric_limits<int>::min();
	for (size_t i15 = 0; i15 < 225; ++i15) {
		const size_t i17 = i15 + 18 + 2*(i15 / 15);
		if (black_board_[i17] || white_board_[i17]) {
			continue;
		}
		b = max(b, Score_(i17, black_board_, white_board_));
		w = max(w, Score_(i17, white_board_, black_board_));
	}
	return b > w ? b : -w;
}

int Agent::MinMax() {
	const vector<int> & moves = BestMoves(black_scores_, white_scores_);
	return moves[RandRange(moves.size())];

	int move = 0;
	int max_eval = numeric_limits<int>::min();

	int alpha = numeric_limits<int>::min();
	int beta = numeric_limits<int>::min();

	size_t breadth = 0;
	for (int m : moves) {
		black_board_[m] = true;
		int eval = mmMin(max_depth, m, alpha, beta);
		black_board_[m] = false;
		if (eval > max_eval) {
			move = m;
			max_eval = eval;
		}

		alpha = max(alpha, max_eval);
		if (++breadth >= max_breadth || beta <= alpha) {
			break;
		}
	}

	return move;
}

int Agent::mmMax(const int depth, const int move, int alpha, int beta) {
	int b = numeric_limits<int>::min();

	for (size_t i15 = 0; i15 < 225; ++i15) {
		const size_t i17 = i15 + 18 + 2*(i15 / 15);
		if (black_board_[i17] || white_board_[i17]) {
			continue;
		}
		b = max(b, Score_(i17, black_board_, white_board_));
	}

	if (!depth || b >= 32) {
		return b;
	}

	const vector<int> & moves = BestMoves(black_scores_, white_scores_);
	
	int max_eval = numeric_limits<int>::min();
	size_t breadth = 0;
	
	
	for (int m : moves) {
		black_board_[m] = true;
		max_eval = max(max_eval, mmMin(depth - 1, m, alpha, beta));
		black_board_[m] = false;
		alpha = max(alpha, max_eval);
		if (++breadth >= max_breadth || beta <= alpha) {
			break;
		}
	}

	return max_eval;
}

int Agent::mmMin(const int depth, const int move, int alpha, int beta) {
	int w = numeric_limits<int>::min();

	for (size_t i15 = 0; i15 < 225; ++i15) {
		const size_t i17 = i15 + 18 + 2*(i15 / 15);
		if (black_board_[i17] || white_board_[i17]) {
			continue;
		}
		w = max(w, Score_(i17, white_board_, black_board_));
	}

	if (!depth || w >= 32) {
		return -w;
	}

	const vector<int> & moves = BestMoves(white_scores_, black_scores_);
	
	int min_eval = numeric_limits<int>::max();
	size_t breadth = 0;


	for (int m : moves) {
		white_board_[m] = true;
		min_eval = min(min_eval, mmMax(depth - 1, m, alpha, beta));
		white_board_[m] = false;
		beta = min(beta, min_eval);
		if (++breadth >= max_breadth || beta <= alpha) {
			break;
		}
	}


	return min_eval;
}

} // namespace agent
