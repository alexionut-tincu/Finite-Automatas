#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct NFA {
	int states_count;
	std::set<int> states;
	std::unordered_map<int, std::unordered_map<char, std::vector<int>>>
		transition_function;
	std::set<int> final_states;
	int init_state;

	NFA(const int states_count) : states_count(states_count) {};

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
			if (final_states.count(state) > 0) {
				return true;
			}
		}
		return false;
	}

	std::vector<int> DFS(int current_state, const std::string &word, int index, std::vector<int> path)
	{
		path.push_back(current_state);

		if (index == word.size()) {
			if (final_states.count(current_state) > 0) {
				return path;
			}
		} else if (transition_function.count(current_state) > 0) {
			for (int next_state : transition_function[current_state][word[index]]) {
				std::vector<int> result = DFS(next_state, word, index + 1, path);
				if(result.empty() == false) {
					return result;
				}
			}
		}

		return {};
	}

	std::vector<int> GetPath(const std::string &word)
	{
		return DFS(init_state, word, 0, {});
	}

	void PrintPath(const std::string &word, std::ofstream &fout)
	{
		std::vector<int> path = GetPath(word);

		for (int i = 0; i < path.size() - 1; ++i) {
			fout << path[i] << " -> ";
		}
		fout << path[path.size() - 1] << std::endl;
	}
};

int main(int argc, char *argv[])
{
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	// Read states count, states and init NFA
	int states_count;
	fin >> states_count;
	NFA nfa(states_count);
	for (int i = 0; i < states_count; ++i) {
		int state;
		fin >> state;
		nfa.AddState(state);
	}

	// Read transitions count and transitions
	int transitions_count;
	fin >> transitions_count;
	for (int i = 0; i < transitions_count; ++i) {
		int start_state, end_state;
		char letter;
		fin >> start_state >> end_state >> letter;
		nfa.AddTransition(start_state, letter, end_state);
	}

	// Read init state
	int init_state;
	fin >> init_state;
	nfa.SetInitState(init_state);

	// Read final states count and final states
	int final_states_count;
	fin >> final_states_count;
	for (int i = 0; i < final_states_count; ++i) {
		int final_state;
		fin >> final_state;
		nfa.SetFinalState(final_state);
	}

	// Read number of words and words
	int words_count;
	fin >> words_count;
	for (int i = 0; i < words_count; ++i) {
		std::string word;
		fin >> word;
		if (argc == 1) {
			fout << (nfa.AcceptWord(word) ? "DA\n" : "NU\n");
		}
		else {
			if (nfa.AcceptWord(word) == true) {
				nfa.PrintPath(word, fout);
			}
			else {
				fout << "NU\n";
			}
		}
	}

	fin.close();
	fout.close();
	return 0;
}