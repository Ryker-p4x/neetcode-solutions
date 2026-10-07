#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Given an array of integers numbers that is sorted in non-decreasing
// order, return the indices (1-indexed) of two numbers [index1, index2]
// such that numbers[index1 - 1] + numbers[index2 - 1] == target and
// index1 < index2. The same element may not be used twice.
//
// Constraints:
//   2 <= numbers.length <= 30000
//   -1000 <= numbers[i] <= 1000
//   -1000 <= target <= 1000
//   There is always exactly one valid solution.
//   Must use O(1) additional space.
//
// Example 1:
//   Input: numbers = [1,2,3,4], target = 3
//   Output: [1,2]

struct TestCase {
    std::string name;
    vector<int> nums;
    int target;
    int expectedFirst;  // 1-indexed index of the smaller element
    int expectedSecond; // 1-indexed index of the larger element
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    vector<int> twoSum(vector<int> &numbers, int target) {
        // your code here
        int n = numbers.size();
        int left = 0, right = n - 1;

        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum < target) {
                left++;
            } else if (sum > target) {
                right--;
            } else {
                return {left + 1, right + 1};
            }
        }

        return {0, 0};
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_nums(const vector<int> &nums) {
    std::string out = "[";
    const size_t limit = 12;
    for (size_t i = 0; i < nums.size(); i++) {
        if (i == limit) {
            out += ", ..., " + std::to_string(nums.size() - limit) + " more]";
            break;
        }
        if (i > 0)
            out += ",";
        out += std::to_string(nums[i]);
    }
    if (nums.empty())
        out += "]";
    else if (nums.size() <= limit)
        out += "]";
    return out;
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;
    cases.push_back({"sample 1", {1, 2, 3, 4}, 3, 1, 2});

    // --- edges at the limits ---
    cases.push_back({"min length (2), both positive", {5, 7}, 12, 1, 2});
    cases.push_back({"min length (2), both negative", {-3, -4}, -7, 1, 2});
    cases.push_back({"min value bound (-1000)", {-1000, -999}, -1999, 1, 2});
    cases.push_back({"max value bound (1000)", {999, 1000}, 1999, 1, 2});

    // --- pointer placement: head / middle / tail ---
    cases.push_back(
        {"answer at head (target == first element)", {0, 1, 2, 3, 4}, 1, 1, 2});
    cases.push_back(
        {"pointers converge mid-array", {1, 2, 3, 4, 5, 6, 7}, 12, 5, 7});
    cases.push_back(
        {"answer at tail", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 19, 9, 10});
    cases.push_back({"target equals an element value", {1, 2, 3, 4}, 4, 1, 3});

    // --- duplicates and zeros ---
    // Duplicated values present but not part of the pair: only 1 + 9 = 10.
    cases.push_back(
        {"duplicate values not in the pair", {1, 2, 2, 9}, 10, 1, 4});
    // The pair itself is two equal values, and the only such occurrence.
    cases.push_back({"pair is two equal maximums", {1, 1, 2, 2}, 4, 3, 4});
    cases.push_back({"all zeros, min length (2)", {0, 0}, 0, 1, 2});
    cases.push_back({"zero target, mixed signs", {-3, 0, 3, 7}, 0, 1, 3});

    // --- negative targets / all-negative arrays ---
    cases.push_back({"negative target", {-8, -4, -1, 2, 5}, -9, 1, 3});

    // --- large structured cases ---
    // -1000..1000 ramp: 999 + 1000 = 1999 is the only pair reaching 1999.
    vector<int> ramp;
    for (int v = -1000; v <= 1000; v++)
        ramp.push_back(v);
    cases.push_back({"full value range (-1000..1000), 2001 elements", ramp,
                     1999, 2000, 2001});

    // 29998 copies of -1000 (indices 1..29998), then 999, then 1000.
    // Only 999 + 1000 = 1999 exists, so the pair is forced to the last two.
    vector<int> big(30000, -1000);
    big[29998] = 999;
    big[29999] = 1000;
    cases.push_back({"max length (30000)", big, 1999, 29999, 30000});

    // stress-case / placeholder slots:
    // {"large random valid case", {...}, target, i, j},

    return cases;
}

void run_tests() {
    int passed = 0;
    int total = 0;
    for (const auto &tc : make_test_cases()) {
        total++;
        vector<int> nums = tc.nums;
        Solution sol;
        vector<int> result = sol.twoSum(nums, tc.target);

        bool ok = result.size() == 2 && result[0] == tc.expectedFirst &&
                  result[1] == tc.expectedSecond;
        if (ok) {
            passed++;
            std::cout << "[PASS] " << tc.name << std::endl;
        } else {
            std::cout << "[FAIL] " << tc.name << std::endl;
            std::cout << "       input:    numbers=" << format_nums(tc.nums)
                      << ", target=" << tc.target << std::endl;
            std::cout << "       expected: [" << tc.expectedFirst << ", "
                      << tc.expectedSecond << "]" << std::endl;
            std::cout << "       actual:   [";
            for (size_t i = 0; i < result.size(); i++) {
                if (i > 0)
                    std::cout << ", ";
                std::cout << result[i];
            }
            std::cout << "] (size " << result.size() << ", want size 2 ["
                      << tc.expectedFirst << ", " << tc.expectedSecond << "])"
                      << std::endl;
        }
    }

    std::cout << "\n" << passed << "/" << total << " passed";
    if (passed < total)
        std::cout << ", " << (total - passed) << " FAILED";
    std::cout << std::endl;
}

int main() {
    run_tests();
    return 0;
}
