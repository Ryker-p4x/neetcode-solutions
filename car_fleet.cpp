#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PROBLEM
// ============================================================
// There are n cars traveling to the same destination on a one-lane highway.
// Given position[] and speed[] (both length n), the destination is at `target`
// miles. A car cannot pass another car ahead of it: it can only catch up and
// then drive at the same speed as the car ahead. A car fleet is a non-empty set
// of cars driving at the same position and same speed; a single car counts as
// a fleet. If a car catches up to a fleet exactly when the fleet reaches the
// destination, that car is part of the fleet. Return the number of fleets.
//
// Constraints:
//   n == position.length == speed.length, 1 <= n <= 100000
//   0 < target <= 1000000
//   1 <= speed[i] <= 1000000
//   0 <= position[i] < target, all position values unique
//
// Note: the fleet count is unique for a given (target, position, speed).
// A float-exact comparison of arrival times is required: a car whose arrival
// time EQUALS the fleet ahead's is part of that fleet (not a new one).

struct TestCase {
    std::string name;
    int target;
    vector<int> position;
    vector<int> speed;
    int expected; // number of car fleets
};

// ------------------------------------------------------------
// YOUR SOLUTION GOES HERE (pasted from NeetCode / LeetCode)
// ------------------------------------------------------------
class Solution {
  public:
    int carFleet(int target, vector<int> &position, vector<int> &speed) {
        // your code here
        // Initial variables
        int res = 0;
        double lastTime = 0;

        // Make a cars pair
        int posSize = position.size();
        std::vector<std::pair<int, int>> cars(posSize);

        for (int i = 0; i < posSize; i++) {
            cars[i] = {position[i], speed[i]};
        }

        // Sort by pos
        sort(cars.begin(), cars.end(), std::greater<std::pair<int, int>>());
        // Do a loop that goes through each car and if its time declared by
        // (target - position) / speed is lower or equal to the most recent one
        for (const auto &[position, speed] : cars) {
            double currTime = (double)(target - position) / speed;
            if (currTime > lastTime) {
                res++;
                lastTime = currTime;
            }
        }
        return res;
    }
};

// ------------------------------------------------------------
// TEST HARNESS (generally no need to edit below this line)
// ------------------------------------------------------------

std::string format_nums(const std::vector<int> &nums) {
    std::string out = "[";
    for (size_t i = 0; i < nums.size(); ++i) {
        if (i)
            out += ", ";
        if (i == 8 && nums.size() > 10) {
            out += "... (" + std::to_string(nums.size()) + " elements)";
            break;
        }
        out += std::to_string(nums[i]);
    }
    return out + "]";
}

std::vector<TestCase> make_test_cases() {
    std::vector<TestCase> cases;

    // ---- samples ----
    cases.push_back(
        {"sample 1 (two cars meet at the destination)", 10, {1, 4}, {3, 2}, 1});
    cases.push_back({"sample 2 (unsorted positions, 3 fleets)",
                     10,
                     {4, 1, 0, 7},
                     {2, 2, 1, 1},
                     3});

    // ---- edges at the limits ----
    cases.push_back({"single car (min n = 1)", 10, {0}, {1}, 1});
    cases.push_back(
        {"min target = 1 (only position 0 allowed)", 1, {0}, {1000000}, 1});
    cases.push_back({"max target, front car 1 mile away at min speed",
                     1000000,
                     {0, 999999},
                     {1000000, 1},
                     1}); // tie at the destination
    cases.push_back({"max values, back car much slower",
                     1000000,
                     {0, 999999},
                     {1, 1000000},
                     2});

    // ---- special shapes ----
    cases.push_back(
        {"two cars, neither catches the other", 10, {0, 5}, {1, 2}, 2});
    cases.push_back(
        {"two cars, same speed (gap never closes)", 10, {0, 2}, {2, 2}, 2});
    cases.push_back(
        {"two cars, rear faster but arrives later", 10, {2, 5}, {1, 2}, 2});
    cases.push_back({"two cars, faster car behind merges early",
                     10,
                     {0, 4},
                     {5, 2},
                     1}); // catch-up at mile 6.67, before the target
    cases.push_back(
        {"two cars, tie exactly at the target", 10, {0, 4}, {5, 3}, 1});
    cases.push_back({"three-car merge chain (all arrive at t = 2)",
                     10,
                     {0, 4, 8},
                     {5, 3, 1},
                     1});
    cases.push_back(
        {"three cars, three separate fleets", 10, {1, 4, 8}, {1, 2, 4}, 3});
    cases.push_back({"descending positions and speeds (one fleet)",
                     10,
                     {9, 5, 0},
                     {1, 5, 10},
                     1});
    cases.push_back({"every car arrives at t = 1 (max speed pattern)",
                     1000000,
                     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                     {1000000, 999999, 999998, 999997, 999996, 999995, 999994,
                      999993, 999992, 999991},
                     1});

    // ---- stress cases (max n = 100000) ----
    // all cars at speed 1 at distinct positions: never merge, one fleet each
    {
        std::vector<int> pos(100000), spd(100000, 1);
        for (int i = 0; i < 100000; ++i)
            pos[i] = i;
        cases.push_back({"stress: n = 100000, all speed 1 (100000 fleets)",
                         1000000, pos, spd, 100000});
    }
    // speed[i] = target - position[i] => every car takes exactly 1 hour
    {
        std::vector<int> pos(100000), spd(100000);
        for (int i = 0; i < 100000; ++i) {
            pos[i] = i;
            spd[i] = 1000000 - i; // arrival time is exactly 1 for every car
        }
        cases.push_back({"stress: n = 100000, all arrive at t = 1 (1 fleet)",
                         1000000, pos, spd, 1});
    }
    // alternating slow/fast cars: each slow car forms its own fleet
    {
        std::vector<int> pos(100000), spd(100000);
        for (int i = 0; i < 100000; ++i) {
            pos[i] = i;
            spd[i] = (i % 2 == 0) ? 1000000 : 1;
        }
        cases.push_back(
            {"stress: n = 100000, alternating speeds (50000 fleets)", 1000000,
             pos, spd, 50000});
    }

    return cases;
}

void run_tests() {
    std::vector<TestCase> cases = make_test_cases();
    int passed = 0, failed = 0;

    for (const TestCase &tc : cases) {
        vector<int> position = tc.position;
        vector<int> speed = tc.speed;
        Solution sol;
        int actual = sol.carFleet(tc.target, position, speed);

        if (actual == tc.expected) {
            ++passed;
            std::cout << "[PASS] " << tc.name << std::endl;
        } else {
            ++failed;
            std::cout << "[FAIL] " << tc.name << std::endl;
            std::cout << "       target=" << tc.target
                      << " position=" << format_nums(tc.position)
                      << " speed=" << format_nums(tc.speed) << std::endl;
            std::cout << "       expected: " << tc.expected
                      << " fleets, got: " << actual << std::endl;
        }
    }

    std::cout << "\n" << passed << "/" << cases.size() << " passed";
    if (failed)
        std::cout << ", " << failed << " FAILED";
    std::cout << std::endl;
}

int main() {
    run_tests();
    return 0;
}
