#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM: Container With Most Water
// ============================================================
// You are given an integer array `heights` where heights[i] is the
// height of the i-th bar. You may choose any two bars to form a
// container. Return the maximum amount of water a container can
// store (width = index distance, height = min of the two bars).
//
// Example 1:
//   Input:  height = [1,7,2,5,4,7,3,6]
//   Output: 36
//   (bars at indices 1 and 7: width 6 * min(7,6) = 36)
//
// Example 2:
//   Input:  height = [2,2,2]
//   Output: 4
//
// Constraints:
//   2 <= height.length <= 100,000
//   0 <= height[i] <= 10,000
// ============================================================

struct TestCase {
    std::string name;
    vector<int> heights;
    int expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    int maxArea(vector<int> &heights) {
        // your code here
        int left = 0, right = heights.size() - 1;
        int maxArea = 0;

        while (left < right) {
            int currWidth = right - left;
            int currHeight = min(heights[left], heights[right]);
            int currArea = currHeight * currWidth;

            maxArea = max(currArea, maxArea);

            if (heights[left] < heights[right]) {
                left++;
            } else {
                right--;
            }
        }

        return maxArea;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_nums(const vector<int> &nums) {
    const size_t max_shown = 12;
    std::string s = "[";
    for (size_t i = 0; i < nums.size() && i < max_shown; ++i) {
        if (i)
            s += ",";
        s += std::to_string(nums[i]);
    }
    if (nums.size() > max_shown) {
        s += ",... (" + std::to_string(nums.size()) + " elements)";
    }
    s += "]";
    return s;
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    // --- GENERATED CASES START ---
    cases.push_back({"sample 1", {1, 7, 2, 5, 4, 7, 3, 6}, 36});
    cases.push_back({"sample 2 (all equal, 3 bars)", {2, 2, 2}, 4});
    cases.push_back({"min length (2 bars)", {1, 1}, 1});
    cases.push_back({"min length, both zero", {0, 0}, 0});
    cases.push_back({"min length, one zero", {0, 5}, 0});
    cases.push_back({"all zeros", {0, 0, 0, 0, 0}, 0});
    cases.push_back({"classic", {1, 8, 6, 2, 5, 4, 8, 3, 7}, 49});
    cases.push_back({"strictly increasing", {1, 2, 3, 4, 5}, 6});
    cases.push_back({"strictly decreasing", {5, 4, 3, 2, 1}, 6});
    cases.push_back({"tall ends, empty middle", {10, 0, 0, 0, 10}, 40});
    cases.push_back(
        {"tall adjacent middle beats wide short ends", {1, 100, 100, 1}, 100});
    cases.push_back({"symmetric valley", {4, 3, 2, 1, 4}, 16});
    {
        // max size, all max height: 10000 * 99999 = 999,990,000
        cases.push_back({"max size (100000), all 10000",
                         vector<int>(100000, 10000), 999990000});
    }
    {
        // max size, only the two ends are tall, middle all zeros.
        vector<int> h(100000, 0);
        h[0] = 10000;
        h[99999] = 10000;
        cases.push_back({"max size (100000), tall ends only", h, 999990000});
    }
    {
        // max size, background of 1s with two 10000 bars at 25000 and 75000.
        // best = 50000 * 10000 = 500,000,000 (any pair with a 1-bar <= 99999)
        vector<int> h(100000, 1);
        h[25000] = 10000;
        h[75000] = 10000;
        cases.push_back(
            {"max size (100000), two tall bars mid-array", h, 500000000});
    }
    // --- GENERATED CASES END ---

    // cases.push_back({"name", {...}, expected});

    return cases;
}

void run_tests() {
    std::vector<TestCase> cases = make_test_cases();
    int passed = 0;
    for (const TestCase &tc : cases) {
        Solution sol;
        vector<int> input = tc.heights;
        int actual = sol.maxArea(input);
        bool ok = (actual == tc.expected);
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << tc.name << "\n";
        } else {
            std::cout << "[FAIL] " << tc.name << "\n"
                      << "       input:    " << format_nums(tc.heights) << "\n"
                      << "       expected: " << tc.expected << "\n"
                      << "       actual:   " << actual << "\n";
        }
    }
    int total = (int)cases.size();
    std::cout << "\n" << passed << "/" << total << " passed";
    if (passed != total)
        std::cout << ", " << (total - passed) << " FAILED";
    std::cout << "\n";
}

int main() {
    run_tests();
    return 0;
}
