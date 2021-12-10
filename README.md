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

Paper about Simulated Annealing:

https://hal-enac.archives-ouvertes.fr/hal-01887543/document

http://rbanchs.com/documents/THFEL_PR15.pdf

https://www.researchgate.net/publication/227061666_Computing_the_Initial_Temperature_of_Simulated_Annealing
