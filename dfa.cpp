#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct DFA {
	int states_count;
	std::set<int> states;
	std::unordered_map<int, std::unordered_map<char, int>> transition_function;
	std::set<int> final_states;
	int init_state;

	DFA(const int states_count) : states_count(states_count) {};

	void AddState(const int state)
	{
		states.insert(state);
	}

	void AddTransition(const int start_state, 
			   const char letter,
			   const int end_state)
	{
		transition_function[start_state][letter] = end_state;
	}

	void SetInitState(const int state)
	{
		init_state = state;
	}

	void SetFinalState(const int state)
	{
		final_states.insert(state);
	}

	bool AcceptWord(const std::string &word)
	{
		int current_state = init_state;

		for (char letter : word) {
			current_state = transition_function[current_state][letter];
			if (current_state == 0) {
				return false;
			}
		}
		
		return (final_states.count(current_state) > 0);
	}
};

int main()
{
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	// Read states count, states and init DFA
	int states_count;
	fin >> states_count;
	DFA dfa(states_count);
	for (int i = 0; i < states_count; ++i) {
		int state;
		fin >> state;
		dfa.AddState(state);
	}

	// Read transitions count and transitions
	int transitions_count;
	fin >> transitions_count;
	for (int i = 0; i < transitions_count; ++i) {
		int start_state, end_state;
		char letter;
		fin >> start_state >> end_state >> letter;
		dfa.AddTransition(start_state, letter, end_state);
	}

	// Read init state
	int init_state;
	fin >> init_state;
	dfa.SetInitState(init_state);

	// Read final states count and final states
	int final_states_count;
	fin >> final_states_count;
	for (int i = 0; i < final_states_count; ++i) {
		int final_state;
		fin >> final_state;
		dfa.SetFinalState(final_state);
	}

	// Read number of words and words
	int words_count;
	fin >> words_count;
	for (int i = 0; i < words_count; ++i) {
		std::string word;
		fin >> word;
		fout << (dfa.AcceptWord(word) ? "DA\n" : "NU\n");
	}

	fin.close();
	fout.close();
	return 0;
}