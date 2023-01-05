# Signals

This is a small side project to practice real time programming by simulating hardware communication signals in C. Plots are generated with Plotly and Pandas.

## Timing Inconsistencies
### Set Up
One device is set up as an "easy clock" (called "clk") that oscillates with a specified period.
Another device is set up to measure and record the clock signal (called "writer") at "uniform" time intervals.

### Results
Initially, delays were implemented with `usleep` and reading time was implemented with `clock`. This resulted in a sampling time that varied with a standard deviation of $\pm1.89$ ms when set up to sample every $5$ ms. The clock was also quite inconsistent. An improved implementation uses `nanosleep` for delays and `gettimeofday` for reading time. This improved implementation yielded a much more uniform sampling time with a standard deviation of $\pm0.3740$ ms when set up to sample every $5$ ms.

