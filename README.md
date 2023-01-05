# Signals

This is a small side project to practice real time programming by simulating hardware communication signals in C. Plots are generated with Plotly and Pandas.

## Demos
`signals.c` implements and demonstrates the usage of several basic devices. Compile with `make signals` and run with `./a.out <demoname>`.

## Devices

### Easy Clock
Clock with 50% duty cycle and configurable clock period. Oscillates all its connections between 1 and 0. Clock signal(s) will start at 1.

### Duty Clock
Clock with configurable duty cycle and clock period. Oscillates all its connections between 1 and 0. Clock signal(s) will start at 1.

### Writer
Records signal values at a configurable sampling period.

## Dev Notes

### Timing Inconsistencies

#### Set Up
One device is set up as an "easy clock" (called "clk") that oscillates with a specified period.
Another device is set up to measure and record the clock signal (called "writer") at "uniform" time intervals.

#### Results
Initially, delays were implemented with `usleep` and reading time was implemented with `clock`. This resulted in a sampling time that varied with a standard deviation of $\pm1.89$ ms when set up to sample every $5$ ms. The clock was also quite inconsistent. An improved implementation uses `nanosleep` for delays and `gettimeofday` for reading time. This improved implementation yielded a much more uniform sampling time with a standard deviation of $\pm0.3740$ ms when set up to sample every $5$ ms.
