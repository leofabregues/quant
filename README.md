# Quant Projects

Personal projects built while preparing for quantitative finance roles, covering derivatives pricing and interest rate risk. Both are ongoing — see each notebook's own "Known limitations" section at the end for what's simplified or still to be extended.

## `pricing_library.ipynb`

An object-oriented derivatives pricing library covering:
- Closed-form Black-Scholes pricing and Greeks (Delta, Gamma, Vega, Theta, Rho) for European vanilla and cash-or-nothing digital options, with full derivations in markdown.
- A Cox-Ross-Rubinstein binomial tree for European and American exercise, cross-validated for convergence against the Black-Scholes price.
- Monte Carlo pricing with two variance-reduction techniques (antithetic variates, control variates), including standard error / confidence interval reporting.
- Vectorised Monte Carlo simulation for path-dependent barrier options (up-and-in call/put).
- 10 inline validation checks (put-call parity, Monte Carlo vs. closed-form agreement, variance reduction, binomial tree convergence, delta cross-check via common random numbers).

## `rates_swap_dv01.ipynb`

A small interest rate toolkit covering:
- A discount curve bootstrapped from money-market deposit rates and par swap rates, with a markdown primer on the instruments used.
- A vanilla fixed-for-floating interest rate swap pricer (NPV, par rate).
- DV01 under a parallel curve shock and a non-parallel steepener/flattener stress scenario, with plots of the curve and of DV01 by maturity.
- 16 inline validation checks, including full bootstrap self-consistency (the par rate recovered from the curve matches the original market input at every maturity used to build it).

## Stack

Python, NumPy, SciPy, Matplotlib. Built and tested as Jupyter notebooks.
