#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

int get_operand(vector<int> &stack) {
	if (stack.empty()) {
		throw std::out_of_range("stack is empty!");
	}
	int operand = stack.back();
	stack.pop_back();
	return operand;
}

// Даны \alpha и слово word \in {a, b, c}*
// Найти длину самого длинного префикса word, принадлежащего L
int get_longest_prefix_length(string rpn, string word) {
	if (rpn.empty()) {
		throw logic_error("regular expression is empty!");
	}
	rpn.erase(remove_if(rpn.begin(), rpn.end(), ::isspace), rpn.end());
	int rpn_length = rpn.length();
	int word_length = word.length();
	// dp[k][i][j] = можно ли получить подслово word[i,,j) из регулярного выражения, соответствующего символу rpn[k]
	vector<vector<vector<int >>> dp(rpn_length, vector<vector<int >>(word_length + 1, vector<int>(word_length + 1, false)));

	vector<int> stack;
	try {
		for (int k = 0; k < rpn_length; ++k) {
			switch (rpn[k]) {
				case '+': {
					int operand2 = get_operand(stack);
					int operand1 = get_operand(stack);
					for (int i = 0; i <= word_length; ++i) {
						for (int j = i; j <= word_length; ++j) {
							dp[k][i][j] = dp[operand1][i][j] || dp[operand2][i][j];
						}
					}
					break;
				}
				case '.': {
					int operand2 = get_operand(stack);
					int operand1 = get_operand(stack);
					for (int i = 0; i <= word_length; ++i) {
						for (int j = i; j <= word_length; ++j) {
							for (int delimeter = i; delimeter <= j; ++delimeter) {
								dp[k][i][j] |= dp[operand1][i][delimeter] && dp[operand2][delimeter][j];
							}
						}
					}
					break;
				}
				case '*': {
					int operand = get_operand(stack);
					for (int i = 0; i <= word_length; ++i) {
						dp[k][i][i] = true;
					}
					for (int i = 0; i <= word_length; ++i) {
						for (int j = i + 1; j <= word_length; ++j) {
							for (int delimeter = i; delimeter <= j; ++delimeter) {
								dp[k][i][j] |= dp[k][i][delimeter] && dp[operand][delimeter][j];
							}
						}
					}
					break;
				}
				case '1':
					for (int i = 0; i <= word_length; ++i) {
						dp[k][i][i] = true;
					}
					break;
				default: // single letter
					if (!('a' <= rpn[k] && rpn[k] <= 'c')) {
						char buffer[rpn_length + 100];
						int size = snprintf(buffer, sizeof(buffer), "string '%s' contains invalid symbol '%c' (code %d) at position %d", rpn.c_str(), rpn[k], (int) rpn[k], k);
						assert(size <= sizeof(buffer));
						throw logic_error(buffer);
					}
					for (int i = 0; i < word_length; ++i) {
						if (word[i] == rpn[k]) {
							dp[k][i][i + 1] = true;
						}
					}
			}
			stack.push_back(k);
		}
	} catch (out_of_range e) {
		throw logic_error("string '" + rpn + "' is not a valid regular expression: there are LESS operands than required");
	}
	assert(!stack.empty());
	if (stack.size() > 1) {
		throw logic_error("string '" + rpn + "' is not a valid regular expression: there are MORE operands than required");
	}
	for (int maximum_prefix_length = word_length; maximum_prefix_length >= 0; --maximum_prefix_length) {
		if (dp[rpn_length - 1][0][maximum_prefix_length]) {
			return maximum_prefix_length;
		}
	}
	return 0;
}

int main() {
	string rpn, word;
	cin >> rpn >> word;
	cout << get_longest_prefix_length(rpn, word) << endl;
	return 0;
}
