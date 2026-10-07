#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM: Trapping Rain Water
// ============================================================
// You are given an array of non-negative integers `height` which
// represent an elevation map. Each value height[i] is the height of
// a bar of width 1. Return the total amount of water that can be
// trapped between the bars.
//
// Example 1:
//   Input:  height = [0,2,0,3,1,0,1,3,2,1]
//   Output: 9
//
// Constraints:
//   1 <= height.length <= 20,000
//   0 <= height[i] <= 100,000
// ============================================================

struct TestCase {
    std::string name;
    vector<int> height;
    int expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    int trap(vector<int> &height) {
        // your code here
        int res = 0;
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    res += leftMax - height[left];
                }
                left++;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    res += rightMax - height[right];
                }
                right--;
            }
        }
        return res;
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
    cases.push_back({"sample 1", {0, 2, 0, 3, 1, 0, 1, 3, 2, 1}, 9});
    cases.push_back({"min length (1 element)", {5}, 0});
    cases.push_back({"two elements, no basin possible", {3, 0}, 0});
    cases.push_back({"all zeros", {0, 0, 0, 0}, 0});
    cases.push_back({"flat equal bars", {4, 4, 4, 4}, 0});
    cases.push_back({"strictly increasing", {1, 2, 3, 4, 5}, 0});
    cases.push_back({"strictly decreasing", {5, 4, 3, 2, 1}, 0});
    cases.push_back({"peak with zero walls only", {0, 5, 0}, 0});
    cases.push_back({"single valley", {3, 0, 3}, 3});
    cases.push_back({"classic uneven walls", {4, 2, 0, 3, 2, 5}, 9});
    cases.push_back(
        {"classic multi-basin", {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6});
    cases.push_back({"wide flat basin", {5, 1, 1, 1, 5}, 12});
    cases.push_back({"asymmetric walls (short wall limits)", {5, 0, 0, 2}, 4});
    cases.push_back({"nested basins", {3, 0, 2, 0, 4}, 7});
    cases.push_back({"alternating 2,0 pattern", {2, 0, 2, 0, 2}, 4});
    {
        // max size, deep V: two 100000 walls, 19998 zeros between.
        // 19998 * 100000 = 1,999,800,000 (fits in int, near the limit)
        vector<int> h(20000, 0);
        h[0] = 100000;
        h[19999] = 100000;
        cases.push_back(
            {"max size (20000), deep basin near int limit", h, 1999800000});
    }
    {
        // max size, all max height: flat, nothing trapped
        cases.push_back(
            {"max size (20000), all 100000", vector<int>(20000, 100000), 0});
    }
    {
        // max size zigzag: even idx = 100000, odd idx = 0.
        // 10000 peaks, 9999 single-bar gaps (last odd index has no right wall)
        vector<int> h(20000, 0);
        for (int i = 0; i < 20000; i += 2)
            h[i] = 100000;
        cases.push_back({"max size (20000), zigzag peaks/zeros", h, 999900000});
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
        vector<int> input = tc.height;
        int actual = sol.trap(input);
        bool ok = (actual == tc.expected);
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << tc.name << "\n";
        } else {
            std::cout << "[FAIL] " << tc.name << "\n"
                      << "       input:    " << format_nums(tc.height) << "\n"
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
