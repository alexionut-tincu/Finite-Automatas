#include <fstream>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#define LAMBDA '.'

struct LambdaNFA {
	int states_count;
	std::set<int> states;
	std::unordered_map<int, std::unordered_map<char, std::vector<int>>>
		transition_function;
	std::unordered_map<int, std::set<int>> lambda_closure;
	std::set<int> final_states;
	int init_state;

	LambdaNFA(const int states_count) : states_count(states_count) {};

	void AddState(const int state)
	{
		states.insert(state);
	}

	void AddTransition(const int start_state,
			   const char letter,
			   const int end_state)
	{
		transition_function[start_state][letter].push_back(end_state);
	}

	void SetInitState(const int state)
	{
		init_state = state;
	}

	void SetFinalState(const int state)
	{
		final_states.insert(state);
	}

	void GenerateLambdaClosures()
	{
		// Do DFS to find where all states can
		// potentially arrive through lambda-transitions
		for (int state : states) {
			std::set<int> closure;
			std::stack<int> stack;
			stack.push(state);

			while (stack.empty() == false) {
				int current_state = stack.top();
				stack.pop();
				closure.insert(current_state);

				if (transition_function[current_state].count(LAMBDA) > 0) {
					for (int next_state : transition_function[current_state][LAMBDA]) {
						// Check for potential lambda-loops
						if(closure.count(next_state) == 0) {
							stack.push(next_state);
						}
					}
				}
			}

			lambda_closure[state] = closure;
		}
	}

	bool AcceptWord(std::string &word)
	{
		std::set<int> current_states = lambda_closure[init_state];
		
		for (char letter : word) {
			std::set<int> next_states;
			for (int state : current_states) {
				if (transition_function[state].count(letter) > 0) {
					for (int next_state : transition_function[state][letter]) {
						next_states.insert(next_state);
						next_states.insert(lambda_closure[next_state].begin(),
							lambda_closure[next_state].end());
					}
				}
			}
			current_states = next_states;
		}

		for (int state : current_states) {
			if (final_states.count(state) > 0) {
				return true;
			}
		}

		return false;
	}
};

int main()
{
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	// Read states count, states and init lnfa
	int states_count;
	fin >> states_count;
	LambdaNFA lnfa(states_count);
	for (int i = 0; i < states_count; ++i) {
		int state;
		fin >> state;
		lnfa.AddState(state);
	}

	// Read transition count and transitions
	int transtion_count;
	fin >> transtion_count;
	for (int i = 0; i < transtion_count; ++i) {
		int start_state, end_state;
		char letter;
		fin >> start_state >> end_state >> letter;
		lnfa.AddTransition(start_state, letter, end_state);
	}

	// Read init state
	int init_state;
	fin >> init_state;
	lnfa.SetInitState(init_state);

	// Read final states count and final states
	int final_states_count;
	fin >> final_states_count;
	for (int i = 1; i <= final_states_count; ++i) {
		int final_state;
		fin >> final_state;
		lnfa.SetFinalState(final_state);
	}

	lnfa.GenerateLambdaClosures();

	// Read number of words and words
	int words_count;
	fin >> words_count;
	for (int i = 0; i < words_count; ++i) {
		std::string word;
		fin >> word;
		fout << (lnfa.AcceptWord(word) ? "DA\n" : "NU\n");
	}

	fin.close();
	fout.close();
	return 0;
}