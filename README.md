# CMKV

Simulated Annealing with Metropolis-Hastings algorithm for Tetravex Solver

## Launch

Input file format (Each tile on one row in Top/Left/Right/Bottom format):
```
0986 @ 
6524
5283
3805
```

@ means that the tile is at the proper position


Build:
```
make all
```

Launch with input file:
```
./a.out <input_file> <output_file>
```

Launch on only one file (example):
```
./a.out tests/base_cases/test_input1
+-----+-----+
|  0  |  3  |
| 9 8 | 8 0 |
|  6  |  5  |
+-----+-----+
|  6  |  5  |
| 5 2 | 2 8 |
|  4  |  3  |
+-----+-----+
```

## Resources

Initial temperature of simulated annealing:
https://www.researchgate.net/publication/227061666_Computing_the_Initial_Temperature_of_Simulated_Annealing

Cooling schedule:
http://what-when-how.com/artificial-intelligence/a-comparison-of-cooling-schedules-for-simulated-annealing-artificial-intelligence/