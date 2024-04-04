#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

struct DFA {
	std::unordered_map<int, std::unordered_map<char, int>> transition_function;
	std::unordered_map<int, int> states;
	std::vector<bool> final_states;
	int init_state;

	DFA(const int states_count) : final_states(states_count, false) {};

	void AddState(const int alias, const int state)
	{
		states[alias] = state;
	}

	void AddTransition(const int start_state_alias, 
			   const char letter,
			   const int end_state_alias)
	{
		int start_state = states[start_state_alias];
		int end_state = states[end_state_alias];
		transition_function[start_state][letter] = end_state;
	}

	void SetInitState(const int state_alias)
	{
		int state = states[state_alias];
		init_state = state;
	}

	void SetFinalState(const int state_alias)
	{
		int state = states[state_alias];
		final_states[state] = true;
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
		return final_states[current_state];
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
	for (int i = 1; i <= states_count; ++i) {
		int state_alias;
		fin >> state_alias;
		dfa.AddState(state_alias, i);
	}

	// Read transitions count and transitions
	int transitions_count;
	fin >> transitions_count;
	for (int i = 1; i <= transitions_count; ++i) {
		int start_state_alias, end_state_alias;
		char letter;
		fin >> start_state_alias >> end_state_alias >> letter;
		dfa.AddTransition(start_state_alias, letter, end_state_alias);
	}

	// Read init state
	int init_state_alias;
	fin >> init_state_alias;
	dfa.SetInitState(init_state_alias);

	// Read final states count and final states
	int final_states_count;
	fin >> final_states_count;
	for (int i = 1; i <= final_states_count; ++i) {
		int final_state_alias;
		fin >> final_state_alias;
		dfa.SetFinalState(final_state_alias);
	}

	// Read number of words and words
	int words_count;
	fin >> words_count;
	for (int i = 1; i <= words_count; ++i) {
		std::string word;
		fin >> word;
		fout << (dfa.AcceptWord(word) ? "DA\n" : "NU\n");
	}

	fin.close();
	fout.close();
	return 0;
}