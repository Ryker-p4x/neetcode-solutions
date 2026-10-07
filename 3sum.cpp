#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Given an integer array nums, return all the triplets
// [nums[i], nums[j], nums[k]] where nums[i] + nums[j] + nums[k] == 0,
// and the indices i, j and k are all distinct. The output must not
// contain duplicate triplets; triplets may be returned in any order.
//
// Constraints:
//   3 <= nums.size() <= 3000
//   -100000 <= nums[i] <= 100000
// ============================================================

struct TestCase {
    std::string name;
    vector<int> nums;
    vector<vector<int>> expected; // distinct triplets, order-insensitive
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE
// ------------------------------------------------------------
class Solution {
  public:
    vector<vector<int>> threeSum(vector<int> &nums) {
        // your code here
        vector<vector<int>> triplets;
        int n = nums.size();

        sort(nums.begin(), nums.end());

        for (int i = 0; i < n; i++) {
            if (i != 0) {
                if (nums[i] == nums[i - 1])
                    continue;
            }
            int expected_sum = 0 - nums[i];
            int left = i + 1, right = n - 1;

            while (left < right) {
                int sum = nums[left] + nums[right];
                if (sum > expected_sum)
                    right--;
                else if (sum < expected_sum)
                    left++;
                else {
                    triplets.push_back({nums[i], nums[left], nums[right]});
                    left++;
                    right--;
                    while (left < right && nums[left] == nums[left - 1])
                        left++;
                }
            }
        }

        return triplets;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

// Prints a vector compactly, truncating long ones so output stays readable.
std::string format_nums(const vector<int> &nums) {
    const size_t max_shown = 10;
    std::string out = "[";
    size_t shown = std::min(nums.size(), max_shown);
    for (size_t i = 0; i < shown; ++i) {
        out += std::to_string(nums[i]);
        if (i + 1 < shown)
            out += ", ";
    }
    if (nums.size() > max_shown) {
        out += ", ... (" + std::to_string(nums.size()) + " total)";
    }
    out += "]";
    return out;
}

// Prints a triplet list compactly, truncating long ones.
std::string format_triplets(const vector<vector<int>> &triplets) {
    const size_t max_shown = 8;
    std::string out = "[";
    size_t shown = std::min(triplets.size(), max_shown);
    for (size_t i = 0; i < shown; ++i) {
        out += "[";
        for (size_t j = 0; j < triplets[i].size(); ++j) {
            out += std::to_string(triplets[i][j]);
            if (j + 1 < triplets[i].size())
                out += ", ";
        }
        out += "]";
        if (i + 1 < shown)
            out += ", ";
    }
    if (triplets.size() > max_shown) {
        out += ", ... (" + std::to_string(triplets.size()) + " total)";
    }
    out += "]";
    return out;
}

// Canonical form for order-insensitive comparison: sort each triplet, then
// sort the list of triplets.
vector<vector<int>> normalize(vector<vector<int>> triplets) {
    for (auto &t : triplets)
        std::sort(t.begin(), t.end());
    std::sort(triplets.begin(), triplets.end());
    return triplets;
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    // Samples from the problem statement.
    cases.push_back(
        {"sample 1", {-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}}});
    cases.push_back({"sample 2", {0, 1, 1}, {}});
    cases.push_back({"sample 3", {0, 0, 0}, {{0, 0, 0}}});

    // Length boundary: the minimum allowed size of 3.
    cases.push_back({"min length (3), no solution", {1, 2, 3}, {}});
    cases.push_back(
        {"min length (3), single solution", {-1, -1, 2}, {{-1, -1, 2}}});

    // Sign-only inputs: no triplet of three equal signs can reach zero.
    cases.push_back({"all positive, no solution", {1, 2, 3, 4, 5}, {}});
    cases.push_back({"all negative, no solution", {-5, -4, -3, -2, -1}, {}});

    // Duplicate traps.
    cases.push_back(
        {"all zeros collapse to one triplet", {0, 0, 0, 0, 0}, {{0, 0, 0}}});
    cases.push_back({"repeated values, dedup required",
                     {-1, 0, 1, -1, 0, 1},
                     {{-1, 0, 1}}});
    cases.push_back(
        {"only two distinct values, no solution", {-1, -1, 1, 1}, {}});
    cases.push_back({"two zeros are not enough for [0,0,0]", {0, 0, 1, 1}, {}});
    cases.push_back({"two-value groups", {2, 2, 2, -4, -4, -4}, {{-4, 2, 2}}});

    // Value boundaries at +/-100000.
    cases.push_back({"max magnitude extremes cancel",
                     {-100000, 0, 100000},
                     {{-100000, 0, 100000}}});
    cases.push_back({"max magnitude with duplicate pair",
                     {-100000, 50000, 50000},
                     {{-100000, 50000, 50000}}});
    cases.push_back({"all values at max magnitude, no solution",
                     {100000, 100000, -100000},
                     {}});
    cases.push_back(
        {"min/max magnitude mix", {-100000, -100000, 100000, 100000}, {}});

    // Ordering.
    cases.push_back({"unsorted with three solutions",
                     {3, 0, -2, -1, 1, 2},
                     {{-2, -1, 3}, {-2, 0, 2}, {-1, 0, 1}}});
    cases.push_back({"already sorted input",
                     {-4, -2, -2, 0, 1, 2, 3},
                     {{-4, 1, 3}, {-2, 0, 2}}});
    cases.push_back({"reverse sorted input",
                     {3, 2, 1, 0, -1, -2, -4},
                     {{-4, 1, 3}, {-2, -1, 3}, {-2, 0, 2}, {-1, 0, 1}}});
    cases.push_back(
        {"duplicate-heavy many solutions",
         {-2, -2, -1, -1, -1, 0, 0, 0, 1, 1, 2, 2, 2},
         {{-2, 0, 2}, {-2, 1, 1}, {-1, -1, 2}, {-1, 0, 1}, {0, 0, 0}}});

    // Stress cases at the maximum length (3000 elements).
    {
        // 1500 of -1 and 1500 of 1: every triple of these values sums to an odd
        // or +-3 value, never zero.
        std::vector<int> big;
        big.reserve(3000);
        for (int i = 0; i < 1500; ++i)
            big.push_back(-1);
        for (int i = 0; i < 1500; ++i)
            big.push_back(1);
        cases.push_back(
            {"max size (3000) all +/-1, no solution", std::move(big), {}});
    }
    {
        // 600 copies each of -2, -1, 0, 1, 2: an enormous number of index
        // combinations collapse to just 5 distinct triplets.
        std::vector<int> big;
        big.reserve(3000);
        for (int v = -2; v <= 2; ++v)
            for (int i = 0; i < 600; ++i)
                big.push_back(v);
        cases.push_back(
            {"max size (3000) five repeated values",
             std::move(big),
             {{-2, 0, 2}, {-2, 1, 1}, {-1, -1, 2}, {-1, 0, 1}, {0, 0, 0}}});
    }
    {
        // Same duplicate soup pinned to the value limits.
        std::vector<int> big;
        big.reserve(3000);
        for (int i = 0; i < 1000; ++i)
            big.push_back(-100000);
        for (int i = 0; i < 1000; ++i)
            big.push_back(0);
        for (int i = 0; i < 1000; ++i)
            big.push_back(100000);
        cases.push_back({"max size (3000) extreme values",
                         std::move(big),
                         {{-100000, 0, 100000}, {0, 0, 0}}});
    }

    return cases;
}

void run_tests() {
    std::vector<TestCase> cases = make_test_cases();

    int passed = 0;
    int failed = 0;

    std::cout << "Running " << cases.size() << " test(s)...\n" << std::endl;

    for (size_t t = 0; t < cases.size(); ++t) {
        const TestCase &tc = cases[t];

        Solution sol;
        vector<int> input = tc.nums; // copy: the solution may sort in place
        vector<vector<int>> result = sol.threeSum(input);

        bool ok = true;
        std::string reason;

        // Shape checks before comparing contents.
        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i].size() != 3) {
                ok = false;
                reason = "triplet " + std::to_string(i + 1) + " has " +
                         std::to_string(result[i].size()) +
                         " element(s), expected 3";
                break;
            }
        }

        if (ok && result.size() != tc.expected.size()) {
            ok = false;
            reason = "wrong number of triplets: expected " +
                     std::to_string(tc.expected.size()) + ", got " +
                     std::to_string(result.size());
        }

        if (ok && normalize(result) != normalize(tc.expected)) {
            ok = false;
            reason = "triplet sets differ";
        }

        std::cout << "[" << (ok ? "PASS" : "FAIL") << "] Test " << t + 1 << "/"
                  << cases.size() << ": " << tc.name;

        if (ok) {
            ++passed;
            std::cout << std::endl;
        } else {
            ++failed;
            std::cout << "\n       nums:     " << format_nums(tc.nums)
                      << "\n       expected: " << format_triplets(tc.expected)
                      << "\n       actual:   " << format_triplets(result)
                      << "\n       reason:   " << reason << std::endl;
        }
    }

    std::cout << "\n--------------------------------------\n";
    std::cout << passed << "/" << cases.size() << " passed";
    if (failed > 0) {
        std::cout << ", " << failed << " FAILED";
    }
    std::cout << "\n--------------------------------------" << std::endl;
}

int main() {
    run_tests();
    return 0;
}
