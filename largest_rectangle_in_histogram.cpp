#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Largest Rectangle In Histogram
//
// You are given an array of integers heights where heights[i] represents the
// height of a bar. The width of each bar is 1.
//
// Return the area of the largest rectangle that can be formed among the bars.
//
// Constraints:
//   1 <= heights.length <= 100,000
//   0 <= heights[i] <= 10,000
//
// The answer is a single integer, so each TestCase carries one expected value.

struct TestCase {
    std::string name;
    vector<int> heights;
    int expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE
// ------------------------------------------------------------
class Solution {
  public:
    int largestRectangleArea(vector<int> &heights) {
        int maxArea = 0;
        int n = heights.size();
        stack<int> stack;

        for (int i = 0; i <= n; i++) {
            int currHeight = 0;
            if (i != n) {
                currHeight = heights[i];
            }

            while (!stack.empty() && currHeight < heights[stack.top()]) {
                int popped = stack.top();
                stack.pop();
                int top = -1;
                if (!stack.empty()) {
                    top = stack.top();
                }

                int leftBoundary = top, rightBoundary = i;
                int width = rightBoundary - leftBoundary - 1;
                int height = heights[popped];

                int area = width * height;
                maxArea = max(area, maxArea);
            }
            stack.push(i);
        }

        return maxArea;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_nums(const vector<int> &nums) {
    std::string out = "[";
    size_t limit = nums.size() <= 12 ? nums.size() : 12;
    for (size_t i = 0; i < limit; ++i) {
        if (i > 0)
            out += ", ";
        out += to_string(nums[i]);
    }
    if (limit < nums.size())
        out += ", ... (" + to_string(nums.size() - limit) + " more)";
    out += "]";
    return out;
}

vector<int> build_triangular() {
    vector<int> heights;
    heights.reserve(100000);
    for (int i = 0; i < 100000; ++i) {
        heights.push_back(min(min(i + 1, 100000 - i), 10000));
    }
    return heights;
}

vector<int> build_alternating() {
    vector<int> heights;
    heights.reserve(100000);
    for (int i = 0; i < 100000; ++i)
        heights.push_back(i % 2 == 0 ? 1 : 2);
    return heights;
}

vector<int> build_constant(int value) { return vector<int>(100000, value); }

vector<TestCase> make_test_cases() {
    vector<TestCase> cases;
    // cases.add(TestCase{/* name */, /* heights */, /* expected */});

    // --- samples from the problem statement ---
    cases.push_back(TestCase{"sample 1", {7, 1, 7, 2, 2, 4}, 8});
    cases.push_back(TestCase{"sample 2", {1, 3, 7}, 7});

    // --- boundary lengths: minimum length 1, value extremes ---
    cases.push_back(TestCase{"single bar, min height 0", {0}, 0});
    cases.push_back(TestCase{"single bar, mid height", {5}, 5});
    cases.push_back(TestCase{"single bar, max height 10000", {10000}, 10000});

    // --- length 2 ---
    cases.push_back(TestCase{"two bars increasing", {2, 3}, 4});
    cases.push_back(TestCase{"two bars equal", {3, 3}, 6});

    // --- monotonic ramps: widest span must be found, not tallest bar ---
    cases.push_back(TestCase{"strictly increasing", {1, 2, 3, 4, 5}, 9});
    cases.push_back(TestCase{"strictly decreasing", {5, 4, 3, 2, 1}, 9});
    cases.push_back(TestCase{"descending plateaus", {3, 3, 2, 2, 1}, 8});
    cases.push_back(TestCase{"mixed classic", {2, 1, 5, 6, 2, 3}, 10});

    // --- zeros: zero-height bars break every rectangle ---
    cases.push_back(TestCase{"all zeros, small", {0, 0, 0, 0, 0}, 0});
    cases.push_back(TestCase{"zeros split equal peaks", {0, 3, 0, 3, 0}, 3});

    // --- equal heights: full width usable ---
    cases.push_back(TestCase{"all equal small", {4, 4, 4, 4}, 16});
    cases.push_back(
        TestCase{"max height, two adjacent", {10000, 10000, 0, 10000}, 20000});

    // --- stress: max length 100,000 (int stays within 2^31) ---
    cases.push_back(TestCase{"all zeros, max length", build_constant(0), 0});
    cases.push_back(TestCase{"all max height, max length",
                             build_constant(10000), 1000000000});
    cases.push_back(
        TestCase{"alternating 1,2, max length", build_alternating(), 100000});
    cases.push_back(
        TestCase{"triangular ramp, max length", build_triangular(), 800020000});

    return cases;
}

void run_tests() {
    int passed = 0, failed = 0;
    vector<TestCase> cases = make_test_cases();
    for (const auto &tc : cases) {
        vector<int> heights = tc.heights;
        Solution sol;
        int actual = sol.largestRectangleArea(heights);
        if (actual == tc.expected) {
            passed++;
            std::cout << "[PASS] " << tc.name << std::endl;
        } else {
            failed++;
            std::cout << "[FAIL] " << tc.name
                      << "  input=" << format_nums(tc.heights)
                      << "  expected=" << tc.expected << "  actual=" << actual
                      << std::endl;
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
