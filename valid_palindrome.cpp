#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// Valid Palindrome (Easy)
// Given a string s, return true if it is a palindrome, otherwise return false.
//
// A palindrome is a string that reads the same forward and backward. It is
// also case-insensitive and ignores all non-alphanumeric characters.
// Alphanumeric characters consist of letters (A-Z, a-z) and numbers (0-9).
//
// Example 1: s = "Was it a car or a cat I saw?"  -> true
// ("wasitacaroracatisaw") Example 2: s = "tab a cat"                     ->
// false  ("tabacat")
//
// Constraints: 1 <= s.length <= 1000, printable ASCII characters only.

struct TestCase {
    std::string name;
    std::string s;
    bool expected;
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    bool isPalindrome(string s) {
        std::erase(s, ' ');
        int n = s.size();

        int left = 0, right = n - 1;

        while (left < right) {
            while (!isalnum(s[left]) && left < right)
                left++;
            while (!isalnum(s[right]) && left < right)
                right--;
            if (tolower(s[left]) != tolower(s[right]))
                return false;
            left++;
            right--;
        }

        return true;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_s(const std::string &s) {
    if (s.size() <= 48)
        return "\"" + s + "\"";
    return "\"" + s.substr(0, 20) + "...\"...(len " + std::to_string(s.size()) +
           ")";
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;
    // --- samples from the problem statement ---
    cases.push_back({"sample 1 (sentence palindrome)",
                     "Was it a car or a cat I saw?", true});
    cases.push_back(
        {"sample 2 (sentence not a palindrome)", "tab a cat", false});

    // --- boundary lengths: min length 1 ---
    cases.push_back({"min length 1: single letter", "a", true});
    cases.push_back({"min length 1: single digit", "7", true});
    cases.push_back({"min length 1: single space", " ", true});
    cases.push_back({"min length 1: single punctuation", "#", true});

    // --- nothing left after filtering -> vacuously a palindrome ---
    cases.push_back({"all punctuation, nothing alphanumeric", ".,!?", true});

    // --- two characters ---
    cases.push_back({"two different letters", "ab", false});
    cases.push_back({"two identical letters", "aa", true});
    cases.push_back({"same letter, different case", "Aa", true});
    cases.push_back(
        {"punctuation between mirrored chars breaks it", "a,b", false});
    cases.push_back(
        {"punctuation between mirrored chars is ignored", "a b a", true});

    // --- digits vs letters ---
    cases.push_back({"digit vs letter mismatch", "0P", false});
    cases.push_back({"digit palindrome with letter center", "0P0", true});
    cases.push_back({"digit palindrome", "12321", true});
    cases.push_back({"digit non-palindrome", "12345", false});

    // --- case-insensitive + punctuation-heavy ---
    cases.push_back({"canonical palindrome with punctuation",
                     "A man, a plan, a canal: Panama", true});
    cases.push_back({"canonical non-palindrome", "race a car", false});
    cases.push_back({"surrounding whitespace ignored", "  racecar  ", true});
    cases.push_back(
        {"junk between every mirrored char", "b o o k s t o r y", false});

    // --- stress cases at max length (1000 chars) ---
    cases.push_back(
        {"max length: 1000 identical letters", std::string(1000, 'a'), true});
    cases.push_back(
        {"max length: 999 a's then one b", std::string(999, 'a') + "b", false});
    cases.push_back({"max length: junk-wrapped \"abba\"",
                     std::string(996, ' ') + "abba", true});
    cases.push_back({"max length: junk-wrapped \"abc\"",
                     std::string(997, ' ') + "abc", false});

    return cases;
}

void run_tests() {
    std::vector<TestCase> cases = make_test_cases();
    int passed = 0, failed = 0;
    for (const auto &tc : cases) {
        Solution sol;
        bool actual = sol.isPalindrome(tc.s);
        bool ok = (actual == tc.expected);
        if (ok) {
            passed++;
            std::cout << "[PASS] " << tc.name << "\n";
        } else {
            failed++;
            std::cout << "[FAIL] " << tc.name << "  input=" << format_s(tc.s)
                      << "  expected=" << (tc.expected ? "true" : "false")
                      << "  got=" << (actual ? "true" : "false") << "\n";
        }
    }
    std::cout << "\n" << passed << "/" << cases.size() << " passed";
    if (failed > 0)
        std::cout << ", " << failed << " FAILED";
    std::cout << std::endl;
}

int main() {
    run_tests();
    return 0;
}
