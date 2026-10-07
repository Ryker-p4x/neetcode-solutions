#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// ============================================================
// PROBLEM
// ============================================================
// Given a vector of integers and a target, return the indices
// (i, j) with i < j such that nums[i] + nums[j] == target.
// Assume exactly one such pair exists.
//
// Constraints:
//   2 <= nums.size() <= 1000
//   -10,000,000 <= nums[i] <= 10,000,000
//   -10,000,000 <= target <= 10,000,000
// ============================================================

struct TestCase {
  std::string name;
  std::vector<int> nums;
  int target;
  int first;
  int second;
};

using namespace std;

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE
// ------------------------------------------------------------
std::pair<int, int> solve(const std::vector<int> &nums, int target) {
  // your code here
  std::unordered_map<int, int> hashmap;

  for (size_t i = 0; i < nums.size(); i++) {
    int number_required = target - nums[i];
    if (hashmap.contains(number_required)) {
      return {hashmap[number_required], i};
    }
    hashmap[nums[i]] = i;
  }
  return {-1, -1}; // provisional: makes all tests fail until implemented
}

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

  cases.push_back({"basic ascending", {3, 4, 5, 6}, 7, 0, 1});
  cases.push_back({"skip middle element", {4, 5, 6}, 10, 0, 2});
  cases.push_back({"duplicate values", {5, 5}, 10, 0, 1});
  cases.push_back({"negative and positive", {-7, 3}, -4, 0, 1});
  cases.push_back(
      {"min/max cancel with extra element", {-10000000, 10000000, 5}, 0, 0, 1});
  cases.push_back(
      {"min value boundary", {-10000000, 0, 10000000}, -10000000, 0, 1});
  cases.push_back(
      {"zero plus max value", {-5, -3, 0, 10000000}, 10000000, 2, 3});
  cases.push_back(
      {"duplicate low value, correct pair later", {7, 1, 3, 4, 1}, 11, 0, 3});
  cases.push_back(
      {"duplicate pair skipped for correct sum", {3, 3, 5, 10}, 15, 2, 3});
  cases.push_back({"zero midpoint symmetric cancel", {-5, 0, 5}, 0, 0, 2});
  cases.push_back(
      {"negative pair amid positives", {-3, 4, -2, 3, 90}, -5, 0, 2});
  cases.push_back({"descending order pair", {9, 6, 5, 3, 0}, 8, 2, 3});
  cases.push_back(
      {"sequential ascending, last pair", {2, 7, 11, 15}, 26, 2, 3});

  // max size (1000 elements): unique pair is the two largest values,
  // 998 + 999 = 1997, since no smaller pair can reach the max sum.
  {
    std::vector<int> big;
    big.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
      big.push_back(i);
    }
    cases.push_back(
        {"max size (1000 elements)", std::move(big), 1997, 998, 999});
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
    std::pair<int, int> result = solve(tc.nums, tc.target);
    bool ok = (result.first == tc.first && result.second == tc.second);

    std::cout << "[" << (ok ? "PASS" : "FAIL") << "] Test " << t + 1 << "/"
              << cases.size() << ": " << tc.name;

    if (ok) {
      ++passed;
      std::cout << std::endl;
    } else {
      ++failed;
      std::cout << "\n       nums:     " << format_nums(tc.nums)
                << "\n       target:   " << tc.target << "\n       expected: ("
                << tc.first << ", " << tc.second << ")"
                << "\n       actual:   (" << result.first << ", "
                << result.second << ")" << std::endl;
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
