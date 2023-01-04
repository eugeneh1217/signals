# Signals

This is a small side project to practice real time programming by simulating hardware communication signals in C. Plots are generated with Plotly and Pandas.

## Timing Inconsistencies
### Set Up
One device is set up as an "easy clock" (called "clk") that oscillates with a specified period.
Another device is set up to measure and record the clock signal (called "writer") at "uniform" time intervals.

### Results
A naive approach of sampling signal, computing elapsed time, formatting output, and writing to the file system every sample resulted in a sampling time that varied with a standard deviation of $\pm1.89$ ms when set up to sample every $5$ ms. The clock was also quite inconsistent. 

### Improvements
A potential approach to improving this inconsistency could be to delegate IO to other threads.
