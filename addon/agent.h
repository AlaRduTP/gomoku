#ifndef AGENT_H
#define AGENT_H

#include <random>
#include <vector>

#include <node.h>
#include <node_object_wrap.h>

namespace agent {

using std::mt19937;
using std::random_device;
using std::vector;

class Agent : public node::ObjectWrap {
public:
	// init addon module
	static void Init(v8::Local<v8::Object> exports);
private:
	explicit Agent();
	~Agent();

	// ctor for addon
	static void New(const v8::FunctionCallbackInfo<v8::Value> & args);

	// methods for addon
	static void NewGame(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void Play(const v8::FunctionCallbackInfo<v8::Value> & args);

	// methods for AI algo
	int RandRange(const int max);
	int Score();
	vector<int> BestMoves(const int * my_scores, const int * op_score);
	int Evaluate();
	int MinMax();
	int mmMax(const int depth, const int move, int alpha, int beta);
	int mmMin(const int depth, const int move, int alpha, int beta);

	bool black_board_[289];  //        I am always black.
	bool white_board_[289];  // Opponent is always white.

	int black_scores_[289];
	int white_scores_[289];

	int max_depth;
	size_t max_breadth;

	random_device rd_;
	mt19937 rng_;
};

} // namespace agent

#endif