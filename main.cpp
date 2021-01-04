#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::string;

int getOperand(vector<int> &stack) {
  if (stack.empty()) {
    throw std::out_of_range("stack is empty!");
  }
  int operand = stack.back();
  stack.pop_back();
  return operand;
}


int getLongestPrefixLength(string regular_expression, string word) {
  if (regular_expression.empty()) {
    std::cout << "INF" << std::endl;
    return -1;
  }
  regular_expression.erase(remove_if(regular_expression.begin(), regular_expression.end(), ::isspace), regular_expression.end());
  vector<vector<vector<int >>> dp(regular_expression.length(), vector<vector<int >>(word.length() + 1, vector<int>(word.length() + 1, false)));

  vector<int> stack;
  try {
    for (int k = 0; k < regular_expression.length(); ++k) {
      switch (regular_expression[k]) {
        case '1':
          for (int i = 0; i <= word.length(); ++i) {
            dp[k][i][i] = true;
          }
          break;
        case '+': {
          int operand2 = getOperand(stack);
          int operand1 = getOperand(stack);
          for (int i = 0; i <= word.length(); ++i) {
            for (int j = i; j <= word.length(); ++j) {
              dp[k][i][j] = dp[operand1][i][j] || dp[operand2][i][j];
            }
          }
          break;
        }
        case '.': {
          int operand2 = getOperand(stack);
          int operand1 = getOperand(stack);
          for (int i = 0; i <= word.length(); ++i) {
            for (int j = i; j <= word.length(); ++j) {
              for (int delimeter = i; delimeter <= j; ++delimeter) {
                dp[k][i][j] |= dp[operand1][i][delimeter] && dp[operand2][delimeter][j];
              }
            }
          }
          break;
        }
        case '*': {
          int operand = getOperand(stack);
          for (int i = 0; i <= word.length(); ++i) {
            dp[k][i][i] = true;
          }
          for (int i = 0; i <= word.length(); ++i) {
            for (int j = i + 1; j <= word.length(); ++j) {
              for (int delimeter = i; delimeter <= j; ++delimeter) {
                dp[k][i][j] |= dp[k][i][delimeter] && dp[operand][delimeter][j];
              }
            }
          }
          break;
        }
        default:
          if (!('a' <= regular_expression[k] && regular_expression[k] <= 'c')) {
            std::cout << "INF" << std:: endl;
            return -1;
          }
          for (int i = 0; i < word.length(); ++i) {
            if (word[i] == regular_expression[k]) {
              dp[k][i][i + 1] = true;
            }
          }
      }
      stack.push_back(k);
    }
  } catch (std::out_of_range e) {
    std::cout << "ERROR" << std::endl;
    return -1;
  }
  assert(!stack.empty());
  if (stack.size() > 1) {
    std::cout << "ERROR" << std::endl;
    return -1;
  }
  for (int maxPrefLength = word.length(); maxPrefLength >= 0; --maxPrefLength) {
    if (dp[regular_expression.length() - 1][0][maxPrefLength]) {
      return maxPrefLength;
    }
  }
  return 0;
}

int main() {
  string regular_expression;
  std::cin >> regular_expression;
  string word;
  std::cin >> word;
  std::cout << getLongestPrefixLength(regular_expression, word) << std::endl;
  return 0;
}
