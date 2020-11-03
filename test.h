#include <string>
#include <cassert>
using namespace std;

int get_longest_prefix_length(string rpn, string word);

void test(string rpn, string word, int answer_right = -1) {
	int answer = get_longest_prefix_length(rpn, word);
	if (answer != answer_right) {
		dbg(rpn, word, answer, answer_right);
	}
	assert(answer == answer_right);
}

void test() {
	test("ab + c.aba. * .bac. + . + *", "abacb", 4);
	test("acb..bab.c. * .ab.ba. + . + *a.", "cb", 0);
	test("aca..", "ackba", 0);
	test("ac1..", "ackba", 2);
	test("ab1+.c.", "", 0);
	test("ab1+.c.", "acqwerfsd", 2);
}

int main() {
	test();
	return 0;
}
