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

def test_fuzzing():
    pass

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
        print(arg_list.fuzzing)
        test_fuzzing()
    else: # all case (default case)
        tests_path = list_tests(base_tests_path)
        launch_all_tests(tests_path)

if __name__ == "__main__":
    main()