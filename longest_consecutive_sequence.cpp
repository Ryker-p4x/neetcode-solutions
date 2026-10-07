#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// ============================================================
// PROBLEM
// ============================================================
// Given an array of integers nums, return the length of the
// longest consecutive sequence of elements that can be formed.
//
// A consecutive sequence is a sequence of elements in which each
// element is exactly 1 greater than the previous element. The
// elements do not have to be adjacent in the original array.
//
// Constraints:
//   0 <= nums.length <= 100,000
//   -10^9 <= nums[i] <= 10^9
//
// Required: O(n) time.
// ============================================================

struct TestCase {
    std::string name;
    std::vector<int> nums;
    int expected;
};

using namespace std;

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE
// ------------------------------------------------------------
class Solution {
  public:
    int longestConsecutive(vector<int> &nums) {
        unordered_set<int> set(nums.begin(), nums.end());
        int longestSequence = 0;

        for (int n : set) {
            int currSequence = 1;
            if (set.contains(n - 1)) {
                continue;
            }

            currSequence = 0;
            while (set.contains(n + currSequence))
                currSequence++;
            if (currSequence > longestSequence) {
                longestSequence = currSequence;
            }
        }

        return longestSequence;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

// Prints a vector compactly, truncating long ones so output stays readable.
std::string format_nums(const std::vector<int> &nums) {
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

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    // samples from the problem statement
    cases.push_back({"sample 1", {2, 20, 4, 10, 3, 4, 5}, 4});
    cases.push_back({"sample 2", {0, 3, 2, 5, 4, 6, 1, 1}, 7});

    // degenerate sizes
    cases.push_back({"empty array", {}, 0});
    cases.push_back({"single element", {7}, 1});

    // small shapes
    cases.push_back({"two consecutive", {1, 2}, 2});
    cases.push_back({"two non-consecutive", {1, 5}, 1});
    cases.push_back(
        {"no consecutive pairs (all gaps of 2)", {1, 3, 5, 7, 9}, 1});

    // duplicates must not inflate the count
    cases.push_back({"all identical elements", {5, 5, 5, 5}, 1});
    cases.push_back({"duplicates inside a long run", {1, 2, 2, 3, 3, 4}, 4});
    cases.push_back(
        {"duplicate-heavy interleaved run", {4, 2, 3, 1, 5, 6, 4, 3}, 6});

    // ordering
    cases.push_back(
        {"reverse sorted full run", {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}, 10});
    cases.push_back(
        {"already sorted full run", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10});

    // negatives and zero
    cases.push_back({"negative run crossing zero", {-3, -2, -1, 0, 1}, 5});
    cases.push_back({"negative run below zero", {-1000, -999, -998, 5, 6}, 3});

    // value boundaries
    cases.push_back({"min value boundary", {-1000000000, -999999999}, 2});
    cases.push_back({"max value boundary", {999999999, 1000000000}, 2});
    cases.push_back(
        {"min and max value together", {-1000000000, 1000000000}, 1});

    // traps: several runs, longest is not the first or the last
    cases.push_back(
        {"three equal-length runs", {1, 2, 3, 10, 11, 12, 20, 21}, 3});
    cases.push_back({"longest run is the last one", {1, 2, 50, 4, 5, 6, 7}, 4});
    cases.push_back({"leading outlier before the run", {9, 4, 5, 6, 7}, 4});
    cases.push_back({"single gap splits the run", {1, 2, 3, 5, 6, 7, 8}, 4});

    // max size (100,000 elements): one contiguous run covering the whole
    // array, so the answer is the full length.
    {
        std::vector<int> big;
        big.reserve(100000);
        for (int i = 0; i < 100000; ++i) {
            big.push_back(i);
        }
        cases.push_back({"max size, single full run", std::move(big), 100000});
    }

    // max size (100,000 elements): two well-separated runs, the first is
    // 0..49999 (length 50000) and the second 1000000..1049999 (length 50000);
    // they are tied, and the shorter-looking second block must still be 50000.
    {
        std::vector<int> big;
        big.reserve(100000);
        for (int i = 0; i < 50000; ++i) {
            big.push_back(i);
        }
        for (int i = 1000000; i < 1050000; ++i) {
            big.push_back(i);
        }
        cases.push_back({"max size, two tied runs", std::move(big), 50000});
    }

    // max size (100,000 elements): every value isolated (spacing 10000), so
    // the answer is 1 despite the large input; largest value is 999,990,000,
    // still inside the +/-10^9 bound.
    {
        std::vector<int> big;
        big.reserve(100000);
        for (int i = 0; i < 100000; ++i) {
            big.push_back(i * 10000);
        }
        cases.push_back({"max size, all isolated", std::move(big), 1});
    }

    // max size (100,000 elements): a negative run of -110000..-50001 (length
    // 60000) plus a positive run 0..39999 (length 40000). They are separated by
    // a gap, so the longer negative run wins.
    {
        std::vector<int> big;
        big.reserve(100000);
        for (int i = 0; i < 60000; ++i) {
            big.push_back(-50001 - i);
        }
        for (int i = 0; i < 40000; ++i) {
            big.push_back(i);
        }
        cases.push_back(
            {"max size, longer negative run", std::move(big), 60000});
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
        std::vector<int> nums = tc.nums;
        Solution sol;
        int result = sol.longestConsecutive(nums);
        bool ok = (result == tc.expected);

        std::cout << "[" << (ok ? "PASS" : "FAIL") << "] Test " << t + 1 << "/"
                  << cases.size() << ": " << tc.name;

        if (ok) {
            ++passed;
            std::cout << std::endl;
        } else {
            ++failed;
            std::cout << "\n       nums:     " << format_nums(tc.nums)
                      << "\n       expected: " << tc.expected
                      << "\n       actual:   " << result << std::endl;
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
