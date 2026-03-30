# Variance Reduction in Option Pricing: The Control Variate Method

This project demonstrates the implementation of the **Control Variate** technique to optimize Monte Carlo simulations for pricing European Call options. By leveraging the known analytical properties of the **Geometric Mean** under **Geometric Brownian Motion (GBM)**, this approach significantly reduces statistical noise and computational expense while maintaining an unbiased estimate.

## Project Overview
Monte Carlo (MC) methods are essential for pricing complex derivatives but suffer from a slow convergence rate of $O(1/\sqrt{N})$. To achieve high precision, a standard simulation requires a vast number of paths, making it computationally expensive. 

This project implements a **Control Variate** estimator to "soak up" simulation noise by introducing an auxiliary variable $X$ (the Geometric Mean) that is highly correlated with the target payoff $Y$ 

## Theoretical Framework
The adjusted estimator is defined as:
$$Y_{CV} = Y - c(X - \mathbb{E}[X])$$

Key features of this implementation:
* **Unbiasedness**: The expected value $\mathbb{E}[Y_{CV}]$ remains equal to the true price $\mathbb{E}[Y]$
* **Optimal Coefficient ($c^*$)**: Calculated as $c = \frac{Cov(Y, X)}{Var(X)}$ to minimize total variance.
* **Asset Dynamics**: Underlying prices follow GBM: $S_t = S_0 \exp((\mu - \frac{1}{2}\sigma^2)t + \sigma W_t)$
* **Geometric Mean Expectation**: We use the analytical continuous average expectation:
$$\mathbb{E}[X] = S_0 \exp\left( \frac{1}{2}(\mu - \frac{\sigma^2}{6})T \right)$$

## Results
The implementation successfully demonstrates **Variance Reduction**. As seen in the generated histograms:
1. **Vanilla MC (Blue)**: Exhibits a high-frequency spike at zero (Out-of-the-Money paths) and a long, volatile tail.
2. **Control Variate (Orange)**: Results in a significantly narrower, more stable distribution centered around the mean, representing a much higher "precision-to-computation" ratio[cite: 135, 136].
   g++ -O3 src/main.cpp -o mc_engine
   ./mc_engine
