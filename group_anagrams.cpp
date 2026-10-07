#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Given an array of strings strs, group all anagrams together
// into sublists. You may return the output in any order.
//
// An anagram is a string that contains the exact same characters
// as another string, but the order of the characters can differ.
//
// Constraints:
//   1 <= strs.length <= 10000
//   0 <= strs[i].length <= 100
//   strs[i] is made up of lowercase English letters.
//
// Output may be returned in any order; the order of strings inside
// each group is also unconstrained.
// ============================================================

struct TestCase {
  std::string name;
  vector<string> strs;
  vector<vector<string>> expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
public:
  vector<vector<string>> groupAnagrams(vector<string> &strs) {
    // your code here
    unordered_map<string, vector<string>> hashmap;
    vector<vector<string>> result;

    for (string s : strs) {
      vector<int> count(26, 0);
      for (char c : s) {
        count[c - 'a']++;
      }

      string key = "";
      for (int val : count) {
        key += to_string(val) + ",";
      }

      if (!hashmap.contains(key)) {
        hashmap.insert({key, {}});
      }
      hashmap[key].push_back(s);
    }

    for (const auto &[key, value] : hashmap) {
      result.push_back(value);
    }
    return result;
  }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_short(const std::string &s) {
  if (s.size() <= 16)
    return s;
  return s.substr(0, 16) + "...";
}

std::string format_groups(const vector<vector<string>> &groups) {
  std::string out = "[";
  size_t shown = 0;
  for (const auto &g : groups) {
    if (shown >= 6) {
      out += ", ... (" + std::to_string(groups.size() - shown) + " more)";
      break;
    }
    if (shown > 0)
      out += ", ";
    out += "[";
    for (size_t i = 0; i < g.size(); ++i) {
      if (i > 0)
        out += ", ";
      out += "\"" + format_short(g[i]) + "\"";
    }
    out += "]";
    ++shown;
  }
  out += "]";
  return out;
}

// Order of the outer groups and of strings inside each group is
// unspecified by the problem, so compare canonicalized forms.
vector<vector<string>> canonicalize(vector<vector<string>> groups) {
  for (auto &g : groups)
    sort(g.begin(), g.end());
  sort(groups.begin(), groups.end());
  return groups;
}

std::vector<TestCase> make_test_cases() {
  std::vector<TestCase> cases;
  // <-- GENERATED CASES LIVE HERE -->

  cases.push_back({"sample 1",
                   {"act", "pots", "tops", "cat", "stop", "hat"},
                   {{"hat"}, {"act", "cat"}, {"stop", "pots", "tops"}}});

  cases.push_back({"sample 2 (single)", {"x"}, {{"x"}}});

  cases.push_back({"sample 3 (single empty string)", {""}, {{""}}});

  {
    std::string big_a(100, 'a');
    cases.push_back({"longest single string (100 chars)", {big_a}, {{big_a}}});
  }

  cases.push_back({"same letters, different order",
                   {"eat", "tea", "ate"},
                   {{"eat", "tea", "ate"}}});

  cases.push_back({"shared chars, different counts",
                   {"aab", "aba", "baa", "abb"},
                   {{"aab", "aba", "baa"}, {"abb"}}});

  cases.push_back({"no anagrams at all",
                   {"abc", "def", "ghi"},
                   {{"abc"}, {"def"}, {"ghi"}}});

  cases.push_back(
      {"exact duplicate strings", {"a", "a", "b"}, {{"a", "a"}, {"b"}}});

  cases.push_back(
      {"all strings identical", {"ab", "ab", "ab"}, {{"ab", "ab", "ab"}}});

  cases.push_back({"empty strings mixed in", {"", "b", ""}, {{"", ""}, {"b"}}});

  cases.push_back({"different lengths are never anagrams",
                   {"ab", "abc", "abcd"},
                   {{"ab"}, {"abc"}, {"abcd"}}});

  {
    std::string s1 = std::string(99, 'a') + "b";
    std::string s2 = "b" + std::string(99, 'a');
    cases.push_back(
        {"max-length anagram pair (100 chars)", {s1, s2}, {{s1, s2}}});
  }

  {
    std::vector<string> strs(10000, "abc");
    std::vector<string> expected_group(10000, "abc");
    cases.push_back(
        {"max size (10000 identical strings)", strs, {expected_group}});
  }

  cases.push_back({"boundary chars with duplicates (a/z only)",
                   {"az", "za", "zz", "aa", "aa", "az"},
                   {{"az", "za", "az"}, {"zz"}, {"aa", "aa"}}});

  return cases;
}

void run_tests() {
  int passed = 0;
  int total = 0;
  for (const TestCase &tc : make_test_cases()) {
    Solution sol;
    vector<string> input = tc.strs;
    vector<vector<string>> actual = sol.groupAnagrams(input);
    ++total;

    vector<vector<string>> expected = canonicalize(tc.expected);
    vector<vector<string>> canonicalActual = canonicalize(actual);
    if (canonicalActual == expected) {
      ++passed;
      cout << "[PASS] " << tc.name << "\n";
    } else {
      cout << "[FAIL] " << tc.name << "\n";
      vector<vector<string>> inputGroups;
      for (const auto &s : tc.strs)
        inputGroups.push_back({s});
      cout << "       input:    " << format_groups(inputGroups) << "\n";
      cout << "       expected: " << format_groups(expected) << "\n";
      cout << "       actual:   " << format_groups(canonicalActual) << "\n";
    }
  }
  cout << passed << "/" << total << " passed, " << (total - passed)
       << " FAILED\n";
}

int main() {
  run_tests();
  return 0;
}
