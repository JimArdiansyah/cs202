#include <iostream>
#include <iomanip>
#include <vector>
#include "fraction.hpp"

using namespace std;

int main() {
    double bet;
    int ballsPicked;
    vector<pair<int, double>> payoutTable;

    // Read the amount to bet and the number of balls picked
    cin >> bet >> ballsPicked;

    // Read the payout table
    int catchCount;
    double payout;
    while (cin >> catchCount >> payout) {
        payoutTable.push_back(make_pair(catchCount, payout));
    }

    // Print the bet and balls picked
    printf("Bet: %.2f\n", bet);
    cout << "Balls Picked: " << ballsPicked << endl;

    // Total ways to choose 20 balls from 80
    Fraction totalWays;
    totalWays.Multiply_Binom(80, 20);

    double totalExpectedReturn = 0.0;

    // Calculate probabilities and expected returns
    for (const auto& entry : payoutTable) {
        int catchBalls = entry.first;
        double payoutAmount = entry.second;

        // Number of ways to match exactly `catchBalls` of the picked balls
        Fraction matchWays;
        matchWays.Multiply_Binom(80 - ballsPicked, 20 - catchBalls);
        matchWays.Multiply_Binom(ballsPicked, catchBalls);

        // Calculate probability as a double
        matchWays.Divide_Binom(80, 20);
        double probability = matchWays.Calculate_Product();

        // Expected return for this payout
        double expectedReturn = probability * payoutAmount;

        // Print probability and expected return
        cout << "  Probability of catching " << catchBalls << " of " << ballsPicked << ": " 
             << fixed << setprecision(7) << probability 
             << " -- Expected return: " << expectedReturn << endl;

        // Accumulate total expected return
        totalExpectedReturn += expectedReturn;
    }

    // Calculate expected return per bet
    double returnPerBet = totalExpectedReturn - bet;
    printf("Your return per bet: %.2f\n", returnPerBet);

    // Calculate normalized return
    double normalizedReturn = returnPerBet / bet;
    printf("Normalized: %.2f\n", normalizedReturn);

    return 0;
}