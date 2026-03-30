#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <fstream>

// Function to price European Call using Control Variate (Geometric Mean)
void priceWithControlVariate(double S0, double K, double T, double r, double sigma, int N, int steps) {
    std::mt19937 generator(42); // Fixed seed for reproducibility
    std::normal_distribution<double> dist(0.0, 1.0);

    double dt = T / steps;
    std::vector<double> payoff_Y(N);   // Standard Option Payoffs - Y
    std::vector<double> control_X(N);  // Geometric Average Values - X

    // Pre-calculate E[X] for GBM (Continuous approximation)
    double expected_X = S0 * exp(0.5 * (r - (sigma * sigma / 6.0)) * T);

    for (int i = 0; i < N; ++i) { // Monte Carlo simulation done N times
        double St = S0;
        double log_sum = 0;

        for (int j = 0; j < steps; ++j) { //GBM path to calculate final S_T
            St *= exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * dist(generator));
            log_sum += log(St);
        }
        
        payoff_Y[i] = std::max(St - K, 0.0) * exp(-r * T); // Discounted Payoff
        control_X[i] = exp(log_sum / steps);         // Geometric Mean
    }

    // Calculate Sample Means and Covariance for 'c'
    double sum_Y = accumulate(payoff_Y.begin(), payoff_Y.end(), 0.0);
    double sum_X = accumulate(control_X.begin(), control_X.end(), 0.0);
    double mean_Y = sum_Y / N;
    double mean_X = sum_X / N;

    double cov_YX = 0, var_X = 0;
    for (int i = 0; i < N; ++i) {
        cov_YX += (payoff_Y[i] - mean_Y) * (control_X[i] - mean_X);
        var_X += pow(control_X[i] - mean_X, 2);
    }
    double c_star = cov_YX / var_X;

    std::vector<double> adjusted_payoffs(N); //Adjusted Payoffs - Y_CV
    for (int i = 0; i < N; i++){
        adjusted_payoffs[i] = payoff_Y[i] - c_star*(control_X[i] - expected_X);
    }

    // Adjusted Estimator
    double price_CV = mean_Y - c_star * (mean_X - expected_X);

    std::cout << "Standard MC Price: " << mean_Y << std::endl;
    std::cout << "Control Variate Price: " << price_CV << std::endl;
    std::cout << "Variance Reduction Factor: " << (var_X / (var_X - pow(c_star, 2) * var_X)) << "x" << std::endl;

    //Export CSV for plotting
    std::ofstream outFile("simulation_results.csv");
    outFile << "Original,Adjusted\n";
    for (int i = 0; i < N; ++i){
        outFile << payoff_Y[i] << "," << adjusted_payoffs[i] << "\n";
    }
    outFile.close();

    std::cout << "Results exported to simulation_results.csv" << std::endl;

}

int main() {
    priceWithControlVariate(100.0, 100.0, 1.0, 0.05, 0.2, 10000, 100);
    return 0;
}