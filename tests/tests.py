# -*- coding: utf-8 -*-

# parsing arguments
from argparse import ArgumentParser

# run command as a subprocess
import subprocess as sp

# Colors for pretty print
from colored import fg, bg, attr, stylize

# Timer for timeout
from  threading import Timer
import time

# Managing files path
import glob
import os

# create dir
from pathlib import Path

# random number generation / shuffle
import random

# Var (10 sec for timeout)
TEST_TIMEOUT = 10

# =======================
# ======= DISPLAY =======
# =======================

def display_OK():
    '''
    Pretty Display of OK
    '''
    ok = fg("#000000") + bg("#46ef38")
    print(stylize("OK", ok), end='')

def display_KO():
    """
    Pretty Display of KO
    """
    ko = fg("#000000") + bg("#ef3838") + attr("bold")
    print(stylize("KO", ko), end='')


def display_tests(tests_path):
    '''
    Display all the tests name
    '''
    for elm in tests_path:
        test_name = elm.split('/')[-1]
        display_one_test(test_name)
        print()

def display_one_test(test_name):
    '''
    Display one test name
    '''
    test_style = fg("#8b38ef") + attr("bold")
    print(stylize(test_name, test_style))

# =====================
# ======= TESTS =======
# =====================

def run_command_with_timeout(cmd, timeout):
    proc = sp.Popen(cmd)
    timer = Timer(timeout, proc.kill)
    timer.start()
    start_time = time.time()
    proc.communicate()

    if timer.is_alive():
        # Process execution done
        timer.cancel()
        return True, time.time() - start_time
    return False, timeout

def list_tests(path):
    return glob.glob(os.path.join(path, '*'))

def launch_one_test(test_path):
    display_one_test(test_path)
    is_ok, duration = run_command_with_timeout(['./a.out', '{}'.format(test_path)], TEST_TIMEOUT)
    print()
    if is_ok:
        display_OK()
        print(" Test passed in {} seconds".format(round(duration, 4)))
    else:
        display_KO()
        print(" Test timeout {} seconds".format(round(duration, 4)))
    print()

def launch_all_tests(tests_path):
    for test_path in tests_path:
        launch_one_test(test_path)

def generate_coords(grid, idx, grid_size):
    '''
    Generate top / left / right / bottom coordinates
    '''
    # if corner top (generate top random)
    # if corner left (generate left random)
    # generate random all other
    i_coord = idx // grid_size
    j_coord = idx % grid_size

    # TOP case
    # if not corner top
    if i_coord - 1 >= 0:
        top_idx = (i_coord - 1) * grid_size + j_coord
        top = grid[top_idx][3]
    else: # random generation
        top = random.randint(0, 9)

    # LEFT case
    # if not corner left
    if j_coord - 1 >= 0:
        left_idx = i_coord * grid_size + j_coord - 1
        left = grid[left_idx][2]
    else: # random generation
        left  = random.randint(0, 9)
        
    # BOTTOM / RIGHT cases
    # generate bottom and right randomly
    bottom = random.randint(0, 9)
    right = random.randint(0, 9)

    return [top, left, right, bottom]

def generate_grid(grid_size):
    '''
    Generate a random grid of 'grid_size'
    '''
    new_grid = []
    for i in range(grid_size * grid_size):
        new_grid.append(generate_coords(new_grid, i, grid_size))

    return new_grid

def test_fuzzing(nb_tests, launch=True):
    grid_sizes = [3,4,4,4,5,5,5,6,6,6]
    Path("tests/generated_tests/").mkdir(parents=True, exist_ok=True)

    for i in range(nb_tests):
        # Generate the grid
        grid_size = grid_sizes[random.randint(0, len(grid_sizes)-1)]
        grid = generate_grid(grid_size)

        # shuffle the grid
        random.shuffle(grid)

        # save the grid to file
        test_path = 'tests/generated_tests/{}_{}x{}'.format(i, grid_size, grid_size)
        grid_to_file(grid, test_path)

        # Launch the test
        if launch:
            launch_one_test(test_path)

# ====================
# ======= SAVE =======
# ====================

def grid_to_file(grid, path):
    f = open(path, 'w+')

    res = ''
    for tile in grid:
        res += "{}{}{}{}\n".format(tile[0], tile[1], tile[2], tile[3])

    f.write(res)
    f.close()

# ====================
# ======= MAIN =======
# ====================

def build_arg_list():
    '''
    Build the list of arguments
    '''
    arg_parser = ArgumentParser(description='Tetravex Solver tests module')
    arg_parser.add_argument('binary', help='Path to the tetravex Binary')
    arg_parser.add_argument('-l', '--list', action="store_true", help='List all the possible tests')
    arg_parser.add_argument('-n', '--name', help='Launch only one test according to its name')
    arg_parser.add_argument('-a', '--all', action="store_true", help='Launch all tests (set by default)')
    arg_parser.add_argument('-f', '--fuzzing', help='Launch test fuzzing, if not precised the module launch 10 random fuzzing tests')
    arg_parser.add_argument('-g', '--gen', help='Generate some tests')

    return arg_parser.parse_args()

def main():
    arg_list = build_arg_list()

    base_tests_path = 'tests/base_cases'

    if arg_list.list:
        tests_path = list_tests(base_tests_path)
        display_tests(tests_path)
    elif arg_list.name:
        launch_one_test(arg_list.name)
    elif arg_list.fuzzing:
        test_fuzzing(int(arg_list.fuzzing))
    elif arg_list.gen:
        test_fuzzing(int(arg_list.gen), False)
    else: # all case (default case)
        tests_path = list_tests(base_tests_path)
        launch_all_tests(tests_path)

if __name__ == "__main__":
    main()