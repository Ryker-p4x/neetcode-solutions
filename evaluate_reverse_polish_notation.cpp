#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Evaluate Reverse Polish Notation (Medium)
//
// You are given an array of strings `tokens` that represents a valid
// arithmetic expression in Reverse Polish Notation. Return the integer
// that represents the evaluation of the expression.
//
// - Operands may be integers or the results of other operations.
// - The operators include '+', '-', '*', and '/'.
// - Division between integers always truncates toward zero.
//
// Constraints:
//   1 <= tokens.length <= 10000
//   tokens[i] is "+", "-", "*", or "/", or a string representing an
//   integer in the range [-200, 200].
//
// The expression is guaranteed valid and has a single unique integer
// result, so every case below has exactly one correct answer.
// ============================================================

struct TestCase {
    std::string name;
    vector<string> tokens;
    int expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    int evalRPN(vector<string> &tokens) {
        static const std::unordered_map<std::string,
                                        std::function<int(int, int)>>
            operations = {{{"+", [](int a, int b) { return a + b; }},
                           {"-", [](int a, int b) { return a - b; }},
                           {"*", [](int a, int b) { return a * b; }},
                           {"/", [](int a, int b) { return a / b; }}}};

        stack<int> stack;
        for (const std::string &token : tokens) {
            auto it = operations.find(token);

            if (it != operations.end()) {
                int r = stack.top();
                stack.pop();
                int l = stack.top();
                stack.pop();

                stack.push(it->second(l, r));
            } else {
                stack.push(stoi(token));
            }
        }

        return stack.top();
    };
};
// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_tokens(const vector<string> &tokens) {
    const size_t max_shown = 8;
    if (tokens.size() <= max_shown) {
        std::string out = "[";
        for (size_t i = 0; i < tokens.size(); i++) {
            if (i)
                out += ", ";
            out += tokens[i];
        }
        return out + "]";
    }
    std::string out = "[";
    for (size_t i = 0; i < 4; i++) {
        if (i)
            out += ", ";
        out += tokens[i];
    }
    out += ", ... (" + std::to_string(tokens.size() - 8) + " more) ..., ";
    for (size_t i = tokens.size() - 4; i < tokens.size(); i++) {
        out += tokens[i];
        if (i + 1 < tokens.size())
            out += ", ";
    }
    return out + "]";
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    cases.push_back({"sample 1", {"1", "2", "+", "3", "*", "4", "-"}, 5});
    cases.push_back({"single operand", {"42"}, 42});
    cases.push_back({"single operand (min value)", {"-200"}, -200});
    cases.push_back({"subtraction reaches min value", {"0", "200", "-"}, -200});
    cases.push_back({"multiplication near int max",
                     {"200", "200", "*", "200", "*", "200", "*"},
                     1600000000});
    cases.push_back({"division truncates toward zero", {"7", "-3", "/"}, -2});
    cases.push_back({"negative / positive truncates", {"-7", "2", "/"}, -3});
    cases.push_back({"negative / negative truncates", {"-7", "-2", "/"}, 3});
    cases.push_back(
        {"subexpression yields zero", {"6", "7", "*", "200", "/"}, 0});
    cases.push_back({"chained division", {"8", "2", "/", "2", "/"}, 2});
    cases.push_back({"subtracting a negative", {"-6", "-9", "-"}, 3});
    cases.push_back(
        {"operator precedence chain", {"2", "3", "*", "4", "5", "*", "+"}, 26});
    cases.push_back(
        {"classic leetcode example",
         {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"},
         22});
    cases.push_back(
        {"negative chain with division", {"13", "-5", "/", "-4", "+"}, -6});

    // Max-length chain: 5000 operands + 4999 operators = 9999 tokens.
    {
        TestCase big;
        big.name = "max length chain (9999 tokens)";
        big.tokens.reserve(9999);
        for (int i = 0; i < 5000; i++)
            big.tokens.push_back("1");
        for (int i = 0; i < 4999; i++)
            big.tokens.push_back("+");
        big.expected = 5000;
        cases.push_back(big);
    }

    // Keep the commented-out placeholder style:
    // cases.push_back({"name", {...}, expected});

    return cases;
}

void run_tests() {
    std::vector<TestCase> cases = make_test_cases();
    int passed = 0;
    int failed = 0;

    for (const TestCase &tc : cases) {
        Solution sol;
        vector<string> input = tc.tokens; // evalRPN takes a non-const ref
        int result = sol.evalRPN(input);

        if (result == tc.expected) {
            std::cout << "[PASS] " << tc.name << "\n";
            passed++;
        } else {
            std::cout << "[FAIL] " << tc.name << "\n"
                      << "       tokens:   " << format_tokens(tc.tokens) << "\n"
                      << "       expected: " << tc.expected << "\n"
                      << "       actual:   " << result << "\n";
            failed++;
        }
    }

    std::cout << "\n"
              << passed << "/" << cases.size() << " passed, " << failed
              << " FAILED" << std::endl;
}

int main() {
    run_tests();
    return 0;
}
