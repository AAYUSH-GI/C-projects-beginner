# Beginner C Projects Collection

This repository is a curated set of beginner-friendly C projects. Each project lives in its own folder under `projects/` with a focused README describing the goal, sample usage, and how to build it with `gcc`.

## Projects

| Project | Description |
| --- | --- |
| hello_world | Minimal program that prints a greeting. |
| basic_calculator | Performs addition, subtraction, multiplication, and division of two numbers. |
| temperature_converter | Converts Celsius to Fahrenheit (and vice versa). |
| number_guessing_game | Randomly picks a number for the user to guess. |
| factorial_calculator | Computes factorials using iteration. |
| fibonacci_generator | Prints the Fibonacci sequence up to a limit. |
| prime_checker | Checks whether a number is prime. |
| palindrome_checker | Tests if a string is a palindrome. |
| array_stats | Calculates the sum and average of an integer array. |
| linear_search | Finds an element using linear search. |
| binary_search | Demonstrates binary search on sorted data. |
| bubble_sort | Sorts integers using bubble sort. |
| selection_sort | Sorts integers using selection sort. |
| insertion_sort | Sorts integers using insertion sort. |
| string_reverse | Reverses a string in-place. |
| character_counter | Counts characters, words, and lines from stdin. |
| file_copy | Copies one file to another. |
| tic_tac_toe | Console tic-tac-toe against another player. |
| todo_list_cli | Simple in-memory to-do list manager. |
| simple_timer | Counts down from a user-provided number of seconds. |
| grade_average | Averages a set of student grades. |

## How to build a project

Each project can be compiled with a single `gcc` command. From the repository root, run for example:

```bash
cd projects/hello_world
gcc -std=c11 -Wall -Wextra -Wpedantic -o hello_world main.c
./hello_world
```

The per-project README files provide tailored compile and usage examples.
