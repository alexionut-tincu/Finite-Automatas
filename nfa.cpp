#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>

struct NFA {
	std::unordered_map<int, std::unordered_map<char, std::vector<int>>>
		transition_function;
	std::unordered_map<int, int> states;
	std::vector<bool> final_states;
	int init_state;

	NFA(const int states_count) : final_states(states_count, false) {};

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
		transition_function[start_state][letter].push_back(end_state);
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
		std::set<int> current_states = {init_state};
		for (char letter : word) {
			std::set<int> next_states;
			for(int state : current_states) {
				if (transition_function.count(state) > 0) {
					for (int next_state : transition_function[state][letter]) {
						next_states.insert(next_state);
					}
				}
			}
			current_states = next_states;
		}
		for (int state : current_states) {
			if (final_states[state] == true) {
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

	// Read states count, states and init NFA
	int states_count;
	fin >> states_count;
	NFA nfa(states_count);
	for (int i = 1; i <= states_count; ++i) {
		int state_alias;
		fin >> state_alias;
		nfa.AddState(state_alias, i);
	}

	// Read transitions count and transitions
	int transitions_count;
	fin >> transitions_count;
	for (int i = 1; i <= transitions_count; ++i) {
		int start_state_alias, end_state_alias;
		char letter;
		fin >> start_state_alias >> letter >> end_state_alias;
		nfa.AddTransition(start_state_alias, letter, end_state_alias);
	}

	// Read init state
	int init_state_alias;
	fin >> init_state_alias;
	nfa.SetInitState(init_state_alias);

	// Read final states count and final states
	int final_states_count;
	fin >> final_states_count;
	for (int i = 1; i <= final_states_count; ++i) {
		int final_state_alias;
		fin >> final_state_alias;
		nfa.SetFinalState(final_state_alias);
	}

	// Read number of words and words
	int words_count;
	fin >> words_count;
	for (int i = 1; i <= words_count; ++i) {
		std::string word;
		fin >> word;
		fout << (nfa.AcceptWord(word) ? "DA\n" : "NU\n");
	}

	fin.close();
	fout.close();
	return 0;
}