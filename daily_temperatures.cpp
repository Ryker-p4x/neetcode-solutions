#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Given an array of integers `temperatures` where temperatures[i]
// represents the daily temperature on the ith day, return an array
// `result` where result[i] is the number of days you have to wait
// after the ith day before a warmer temperature appears. If there is
// no future day with a warmer temperature for the ith day, set
// result[i] to 0 instead.
//
// Constraints: 1 <= temperatures.length <= 100,000, 1 <= temperatures[i] <= 100
// ============================================================

struct TestCase {
    std::string name;
    std::vector<int> temperatures; // input
    std::vector<int> expected;     // expected output, one entry per day
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE
// ------------------------------------------------------------
class Solution {
  public:
    vector<int> dailyTemperatures(vector<int> &temperatures) {
        int n = temperatures.size();
        stack<int> stack;
        vector<int> result(n);

        for (int i = 0; i < n; i++) {
            while (!stack.empty() &&
                   temperatures[i] > temperatures[stack.top()]) {
                int popped = stack.top();
                stack.pop();
                int distance = i - popped;
                result[popped] = distance;
            }
            stack.push(i);
        }

        return result;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_nums(const std::vector<int> &nums) {
    if (nums.size() <= 20) {
        std::string s = "[";
        for (std::size_t i = 0; i < nums.size(); ++i) {
            if (i)
                s += ", ";
            s += std::to_string(nums[i]);
        }
        s += "]";
        return s;
    }
    std::string s = "[";
    for (std::size_t i = 0; i < 5; ++i) {
        if (i)
            s += ", ";
        s += std::to_string(nums[i]);
    }
    s += ", ... , ";
    for (std::size_t i = nums.size() - 5; i < nums.size(); ++i) {
        if (i != nums.size() - 5)
            s += ", ";
        s += std::to_string(nums[i]);
    }
    s += "] (n=" + std::to_string(nums.size()) + ")";
    return s;
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    // --- samples from the problem statement ---
    cases.push_back(
        {"sample 1", {30, 38, 30, 36, 35, 40, 28}, {1, 4, 1, 2, 1, 0, 0}});
    cases.push_back({"sample 2", {22, 21, 20}, {0, 0, 0}});

    // --- boundary lengths ---
    cases.push_back({"single day, min temp", {1}, {0}});
    cases.push_back({"single day, max temp", {100}, {0}});
    cases.push_back({"two days increasing", {30, 40}, {1, 0}});
    cases.push_back({"two days equal", {30, 30}, {0, 0}});
    cases.push_back({"two days decreasing", {40, 30}, {0, 0}});
    cases.push_back({"min and max temp only", {1, 100}, {1, 0}});

    // --- duplicates / equal runs ---
    cases.push_back(
        {"all equal mid temp", {50, 50, 50, 50, 50}, {0, 0, 0, 0, 0}});
    cases.push_back({"all equal min temp", {1, 1, 1, 1}, {0, 0, 0, 0}});
    cases.push_back({"all equal max temp", {100, 100, 100, 100}, {0, 0, 0, 0}});
    cases.push_back(
        {"alternating 30/20", {30, 20, 30, 20, 30, 20}, {0, 1, 0, 1, 0, 0}});
    cases.push_back({"equal runs with warmer tail",
                     {70, 69, 70, 71, 70, 72},
                     {3, 1, 1, 2, 1, 0}});

    // --- monotonic / sorted shapes ---
    cases.push_back({"strictly increasing", {1, 2, 3, 4, 5}, {1, 1, 1, 1, 0}});
    cases.push_back({"strictly decreasing", {5, 4, 3, 2, 1}, {0, 0, 0, 0, 0}});
    cases.push_back({"non-strictly increasing",
                     {1, 1, 2, 2, 3, 3, 3},
                     {2, 1, 2, 1, 0, 0, 0}});
    cases.push_back(
        {"one warmer after cold run", {50, 60, 55, 65}, {1, 2, 1, 0}});
    cases.push_back(
        {"warp after colder days", {10, 20, 30, 10, 40}, {1, 1, 2, 1, 0}});

    // --- traps: warmer day far away, mixed highs and lows ---
    cases.push_back({"classic mixed pattern",
                     {73, 74, 75, 71, 69, 72, 76, 73},
                     {1, 1, 4, 2, 1, 1, 0, 0}});
    cases.push_back({"decreasing, warm day at the end",
                     {60, 55, 50, 45, 40, 41},
                     {0, 0, 0, 0, 1, 0}});
    cases.push_back({"new max found at the very end",
                     {50, 51, 49, 48, 47, 46, 45, 60},
                     {1, 6, 5, 4, 3, 2, 1, 0}});
    cases.push_back({"cold dip between highs",
                     {80, 81, 79, 78, 77, 76, 82, 81},
                     {1, 5, 4, 3, 2, 1, 0, 0}});

    // --- stress: max size, structured (not garbage) ---
    // 100,000 days cycling 1..100: every value below 100 sees its immediate
    // successor one day later, value 100 is the max temp so it never warms up.
    {
        TestCase tc;
        tc.name = "max size, repeating 1..100 cycle";
        tc.temperatures.reserve(100000);
        tc.expected.reserve(100000);
        for (int i = 0; i < 100000; ++i) {
            tc.temperatures.push_back(1 + (i % 100));
            tc.expected.push_back((i % 100 == 99) ? 0 : 1);
        }
        cases.push_back(tc);
    }
    // 100,000 days as 100 blocks of 1000 equal days, block value rising 1..100:
    // a day in block b warms up exactly when block b+1 starts, so the wait is
    // the distance to the next block; days in the final (max-temp) block are 0.
    {
        TestCase tc;
        tc.name = "max size, 100 flat blocks warming 1..100";
        tc.temperatures.reserve(100000);
        tc.expected.reserve(100000);
        for (int i = 0; i < 100000; ++i) {
            int block = i / 1000;
            tc.temperatures.push_back(1 + (block % 100));
            tc.expected.push_back((block % 100 == 99) ? 0 : 1000 - (i % 1000));
        }
        cases.push_back(tc);
    }

    // cases.push_back({"", {}, {}});

    return cases;
}

void run_tests() {
    int passed = 0, failed = 0;

    for (const auto &tc : make_test_cases()) {
        std::vector<int> temps = tc.temperatures;
        Solution sol;
        std::vector<int> result = sol.dailyTemperatures(temps);

        bool ok = (result.size() == tc.expected.size());
        if (ok) {
            for (std::size_t i = 0; i < result.size(); ++i) {
                if (result[i] != tc.expected[i]) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            ++passed;
            std::cout << "[PASS] " << tc.name << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL] " << tc.name << "\n";
            std::cout << "  input:    " << format_nums(tc.temperatures) << "\n";
            std::cout << "  expected: " << format_nums(tc.expected) << "\n";
            std::cout << "  actual:   " << format_nums(result) << "\n";
        }
    }

    std::cout << "\n" << passed << "/" << (passed + failed) << " passed";
    if (failed)
        std::cout << ", " << failed << " FAILED";
    std::cout << std::endl;
}

int main() {
    run_tests();
    return 0;
}
