*Este proyecto ha sido creado como parte del currículo de 42 por abarba-m, gecasas.*

# push_swap

> Sorting a stack of integers with two stacks and a restricted set of operations, using four strategies of different complexity and an adaptive selector driven by a disorder index.

<details>
<summary><strong>English</strong></summary>

## Description

`push_swap` is a group project from the 42 curriculum. The program receives a list of integers, which form stack `a`, and must print the sequence of operations that sorts them in ascending order (smallest number on top), using only a second, initially empty stack `b` and eleven allowed operations.

The goal of the project is to understand **algorithmic complexity** in a very concrete way: the cost of an algorithm is not measured in comparisons or CPU time, but in the **number of push_swap operations it prints**. Sorting a random list and sorting an almost-sorted list are different problems, and choosing the right strategy for each case makes a big difference in the number of operations.

Our implementation includes:

* **Four sorting strategies**, selectable at runtime:
  * `--simple` — adapted bubble sort, `O(n²)`.
  * `--medium` — chunk-based sort with `√n` chunks, `O(n√n)`.
  * `--complex` — binary LSD radix sort on normalized indices, `O(n log n)`.
  * `--adaptive` — computes a disorder index and picks one of the above (default behaviour).
* **A `--bench` mode** that reports the disorder index, the strategy used, the total number of operations and the count of each operation type.
* **Robust argument parsing**: several numbers per argument, signs, non-numeric input and duplicates.
* **Bonus: our own `checker` program**, which reads operations from standard input, executes them and prints `OK` or `KO`.

---

## Table of Contents

* [Description](#description)
* [The Rules](#the-rules)
* [Algorithms](#algorithms)
* [Data Structures](#data-structures)
* [Technical Decisions](#technical-decisions)
* [Project Structure](#project-structure)
* [Instructions](#instructions)
* [Usage Examples](#usage-examples)
* [Bonus: checker](#bonus-checker)
* [Benchmarks](#benchmarks)
* [Quality and Validation](#quality-and-validation)
* [Memory Management](#memory-management)
* [Team and Contributions](#team-and-contributions)
* [What We Learned](#what-we-learned)
* [AI Usage](#ai-usage)
* [Resources](#resources)
* [Authors](#authors)

---

## The Rules

At the start, stack `a` contains the input numbers (the first argument on top) and stack `b` is empty. The only allowed operations are:

| Operation | Name                 | Effect                                                              |
| --------- | -------------------- | ------------------------------------------------------------------- |
| `sa`      | swap a               | Swaps the first two elements of `a`. Does nothing with fewer than 2. |
| `sb`      | swap b               | Swaps the first two elements of `b`. Does nothing with fewer than 2. |
| `ss`      | swap both            | `sa` and `sb` at the same time.                                     |
| `pa`      | push a               | Moves the top of `b` to the top of `a`. Does nothing if `b` is empty. |
| `pb`      | push b               | Moves the top of `a` to the top of `b`. Does nothing if `a` is empty. |
| `ra`      | rotate a             | Shifts `a` up by one: the first element becomes the last.           |
| `rb`      | rotate b             | Shifts `b` up by one: the first element becomes the last.           |
| `rr`      | rotate both          | `ra` and `rb` at the same time.                                     |
| `rra`     | reverse rotate a     | Shifts `a` down by one: the last element becomes the first.         |
| `rrb`     | reverse rotate b     | Shifts `b` down by one: the last element becomes the first.         |
| `rrr`     | reverse rotate both  | `rra` and `rrb` at the same time.                                   |

The program prints one operation per line, separated only by `\n`.

---

## Algorithms

All complexities in this section are expressed in **number of push_swap operations**, which is the cost model required by the subject. `n` is the number of elements.

### Index normalization

Before sorting, every node receives an `index`: its rank in the final sorted order (`0` for the smallest value, `n - 1` for the largest). This is done once in `assign_indices()` by counting, for each node, how many values are smaller than it.

Working with ranks instead of raw values has two advantages:

* Values can be any `int`, including negatives and very large numbers, but ranks are always in `[0, n - 1]`. This is what makes radix sort possible without dealing with signs.
* The chunk sort can define chunks as simple index ranges (`[0, √n)`, `[√n, 2√n)`, ...) and always knows which element must go back to `a` next (the one with the highest index left in `b`).

### Disorder index

The adaptive strategy needs a number between `0` and `1` describing how far stack `a` is from being sorted. It is computed in `compute_disorder()` **before any operation is performed**.

Our implementation walks the stack once and counts the **adjacent pairs that are out of order** (a value greater than the next one), and divides that count by the number of adjacent pairs (`n - 1`):

```text
disorder = (adjacent pairs where a[i] > a[i + 1]) / (n - 1)
```

* A sorted stack gives `0`.
* A stack in strictly decreasing order gives `1`.
* For a random permutation, the expected value is around `0.5`, because each adjacent pair has roughly the same probability of being in order or not.

We chose to look at adjacent pairs because it is computed in a single `O(n)` pass and it directly reflects what our low-disorder strategy cares about: how many "breaks" there are between already-sorted runs.

### 1. Simple — adapted bubble sort, `O(n²)` (`--simple`)

File: `algorithms.c` — `bubble_sort()`, `bubble_operations()`.

Bubble sort is adapted to the stack model by using the top of `a` as the "comparison window":

1. Compare the first two elements of `a`. If they are out of order, `sa`.
2. `ra` to move the window one position forward.
3. Repeat for the unsorted part of the stack (`size - 1 - pass` comparisons).
4. Undo the rotations with the same number of `rra`, so the stack returns to its original alignment with the largest element of this pass at its final position.
5. If a full pass did not perform any swap, the stack is sorted and the algorithm stops early.

Stack `b` is never used.

* **Time (operations):** each pass costs at most one `sa` per comparison plus the rotations forward and back, so `O(n)` per pass. There are at most `n - 1` passes, giving **`O(n²)`** in the worst case.
* **Space:** `O(1)` extra; only stack `a` is used.

It is by far the most expensive strategy on random input (around 12 000 operations for 100 numbers), but it has an important property: **early termination**. When the input is almost sorted and the out-of-order elements only need to move a short distance, very few passes are needed and the total cost is `O(n · p)`, where `p` is the number of passes.

### 2. Medium — chunk sort, `O(n√n)` (`--medium`)

File: `sort_chunks.c` — `sort_chunks()`, `push_chunk()`, `push_back()`.

The indices are divided into chunks of size `⌊√n⌋`. The algorithm works in two phases:

**Phase 1 — push chunks to `b` (`push_chunk`)**

For each chunk `[end - √n, end)`, in increasing order:

1. If the top of `a` belongs to the current chunk or an earlier one (`index < end`), `pb`.
2. Otherwise, `ra` and keep looking.
3. Each chunk has a midpoint. If the element just pushed is in the **lower half** of the chunk, it is sent to the bottom of `b` with `rb`. This splits each chunk around its midpoint, so larger values stay near the top of `b` and smaller ones near the bottom, which shortens the searches in phase 2.

**Phase 2 — push back to `a` (`push_back`)**

While `b` is not empty:

1. Look for the position of the largest remaining index (`b->size - 1`).
2. Bring it to the top using the shortest direction: `rb` if it is in the upper half of `b`, `rrb` otherwise.
3. `pa`.

Because elements come back from largest to smallest, each `pa` puts the new element on top of a sorted stack, so `a` ends sorted.

* **Time (operations):** in phase 1, each of the `√n` chunks may need to rotate through the whole stack `a`, so `√n · n` rotations plus `n` pushes. In phase 2, thanks to the chunk ordering, the maximum of `b` is always inside the most recent chunk, close to the top or to the bottom of `b`, so finding it costs `O(√n)` rotations per element. Total: **`O(n√n)`**.
* **Space:** `O(1)` extra; the `n` nodes are only moved between `a` and `b`.

### 3. Complex — binary LSD radix sort, `O(n log n)` (`--complex`)

File: `sort_radix.c` — `sort_radix()`, `radix_pass()`, `get_max_bits()`.

Radix sort fits the two-stack model naturally, because each pass is a stable partition into two groups, and two stacks are exactly two groups.

1. Compute the number of bits needed to represent the largest index: `bits = ⌈log₂ n⌉` (`get_max_bits(n - 1)`).
2. For each bit, from the least significant to the most significant (`radix_pass`):
   * Go through all `n` elements of `a` once. If the current bit of the top element's index is `1`, `ra` (it stays in `a`); if it is `0`, `pb`.
   * Push everything back with `pa`.
3. After each pass, if `a` is already sorted, the algorithm stops early.

After processing all bits, the indices — and therefore the values — are sorted.

* **Time (operations):** each pass costs `n` operations (`ra` or `pb`) plus one `pa` for each element sent to `b`, so at most `2n`. With `⌈log₂ n⌉` passes, the total is **`O(n log n)`**.
* **Space:** `O(1)` extra.

A very useful property: since indices are always a permutation of `0..n-1`, the number of operations **does not depend on the input order**. For 100 elements it always uses 1 084 operations, and for 500 elements it always uses 6 784. That makes it the most predictable strategy for highly disordered inputs.

### 4. Adaptive — disorder-based selector (`--adaptive`, default)

File: `algorithm_parser.c` — `parser()`, `compute_disorder()`.

The adaptive strategy computes the disorder index and chooses the internal method according to the thresholds defined in the subject:

| Disorder               | Regime | Method used            | Target complexity |
| ---------------------- | ------ | ---------------------- | ----------------- |
| `disorder < 0.2`       | Low    | Bubble sort (early exit) | `O(n)`          |
| `0.2 ≤ disorder < 0.5` | Medium | Chunk sort             | `O(n√n)`          |
| `disorder ≥ 0.5`       | High   | Radix sort             | `O(n log n)`      |

**Justification of the thresholds:**

* **Below `0.2`**, fewer than one in five adjacent pairs is broken: the stack is made of long runs that are already in order. In this situation the strongest point of bubble sort is its early exit — when only a few elements are out of place and they are close to their final position, it finishes after very few passes, and each pass is linear. Moving every element to `b` and back (as the other two strategies do) would cost at least `2n` operations even for a nearly sorted input.
* **Between `0.2` and `0.5`**, the input is partially ordered. Chunk sort benefits from this, because elements of the same chunk tend to be close together in `a`, which reduces the rotations needed in phase 1.
* **From `0.5`**, the input behaves like a random permutation or worse. Radix sort is the best choice here because its cost is guaranteed and independent of the order of the input.

### Complexity summary

| Strategy | Technique                | Operations (time) | Extra space | 100 random numbers | 500 random numbers |
| -------- | ------------------------ | ----------------- | ----------- | ------------------ | ------------------ |
| Simple   | Bubble sort on the stack | `O(n²)`           | `O(1)`      | ~12 000            | not practical      |
| Medium   | `√n` chunks              | `O(n√n)`          | `O(1)`      | ~790               | ~7 540             |
| Complex  | Binary LSD radix         | `O(n log n)`      | `O(1)`      | 1 084              | 6 784              |
| Adaptive | Selector by disorder     | depends on regime | `O(1)`      | ~950               | ~7 240             |

The space column refers to extra memory used by each strategy beyond the `n` nodes shared by both stacks. The program also stores every generated operation in a list before printing it, which uses `O(k)` memory for `k` operations (see [Technical Decisions](#technical-decisions)).

---

## Data Structures

### Stacks: circular doubly linked lists

```c
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;
```

Each stack is a **circular doubly linked list** with a pointer to its top and its size. This choice makes the most frequent operations constant time:

* `ra` / `rb` only move `top` to `top->next`.
* `rra` / `rrb` only move `top` to `top->prev`.
* `pa` / `pb` unlink one node from one list and link it into the other, with no memory allocation.
* The bottom of the stack is always `top->prev`, so it is never necessary to walk the list to reach it.

### Execution context

```c
typedef struct s_context
{
	t_stack		*a;
	t_stack		*b;
	t_move		*move;
	t_list		*instructions;
	t_list		*instructions_tail;
	t_algorithm	algorithm;
	int			has_algorithm;
	int			total_ops;
	int			bench_mode;
	float		original_disorder;
}	t_context;
```

Global variables are forbidden by the subject, so all the program state is grouped in a single `t_context` structure that is passed to every operation and algorithm: both stacks, the operation counters (`t_move`), the list of generated instructions, the selected strategy and the benchmark data.

### Strategy selection

```c
typedef enum e_algorithm
{
	ALGO_SIMPLE,
	ALGO_MEDIUM,
	ALGO_COMPLEX,
	ALGO_ADAPTIVE
}	t_algorithm;
```

The strategy is stored as an enum, and `run_algorithm()` dispatches to the corresponding function.

---

## Technical Decisions

* **Operations record themselves.** Each operation function (`sa`, `pb`, `rra`, ...) checks whether it can be applied, appends its name to the instruction list and modifies the stacks. The algorithms never print anything directly; they only call operations. This keeps the sorting logic independent from the output.
* **Instructions are buffered and printed at the end.** Generated operations are stored in a `t_list` (from our Libft) and printed in `print_instructions()` once the algorithm finishes. This allows the program to know the total number of operations for `--bench` and keeps the output in a single place.
* **Operations do nothing when they cannot be applied** (for example `sa` with fewer than two elements or `pa` with `b` empty), and in that case they are not recorded, so the output never contains useless instructions.
* **Ranks instead of values.** All three sorting algorithms work on `index`, not on `value`, so negative numbers and the full `int` range need no special handling.
* **Early termination** in bubble sort (no swaps in a pass) and in radix sort (stack already sorted after a pass) avoids printing operations that do not change anything.
* **Flags can be combined.** `--bench` and a strategy selector can be used together (for example `--bench --complex`). Options are recognized by `is_option()` and skipped while numbers are parsed.
* **Several numbers per argument.** `"3 2 1"` and `3 2 1` are equivalent: each argument is split on spaces in place, without extra allocations.
* **Separate data structures for the checker.** The bonus program has its own header and its own simpler node structure (without `index`), so it can be compiled and evaluated independently from `push_swap`, as required by the subject.

---

## Project Structure

```text
push_swap/
├── Makefile
├── README.md
├── push_swap.h
├── push_swap.c
├── argparser.c
├── argparser_utils.c
├── algorithm_parser.c
├── algorithms.c
├── sort_chunks.c
├── sort_radix.c
├── flags.c
├── stack.c
├── stack_operations.c
├── push_operations.c
├── rotate_operations.c
├── reverse_rotate_operations.c
├── checker_bonus.h
├── checker_bonus.c
├── parse_bonus.c
├── read_bonus.c
├── stack_bonus.c
├── operations_bonus.c
└── libft/
    ├── Makefile
    ├── libft.h
    ├── ft_*.c
    └── printf/
        ├── Makefile
        ├── ft_printf.h
        └── ft_*.c
```

### Mandatory part

| File                          | Contents                                                                                     |
| ----------------------------- | -------------------------------------------------------------------------------------------- |
| `push_swap.c`                 | `main`, context initialization, flag reading, `--bench` report and final cleanup.            |
| `argparser.c`                 | Argument splitting, stack construction and index normalization (`assign_indices`).          |
| `argparser_utils.c`           | Number validation, duplicate detection and option detection.                                 |
| `algorithm_parser.c`          | Disorder index, adaptive selector, flag-to-strategy parsing and strategy dispatcher.         |
| `algorithms.c`                | Simple strategy: adapted bubble sort.                                                        |
| `sort_chunks.c`               | Medium strategy: chunk sort.                                                                 |
| `sort_radix.c`                | Complex strategy: binary radix sort, and `is_sorted`.                                        |
| `flags.c`                     | Instruction list: append, print and free.                                                    |
| `stack.c`                     | Node creation, stack/counter initialization, insertion at the bottom and stack cleanup.      |
| `stack_operations.c`          | `sa`, `sb`, `ss` and the internal swap.                                                      |
| `push_operations.c`           | `pa`, `pb` and the internal pop/push.                                                        |
| `rotate_operations.c`         | `ra`, `rb`, `rr`.                                                                            |
| `reverse_rotate_operations.c` | `rra`, `rrb`, `rrr`.                                                                         |
| `push_swap.h`                 | Structures, enum and prototypes.                                                             |

### Bonus part

| File                 | Contents                                                                         |
| -------------------- | -------------------------------------------------------------------------------- |
| `checker_bonus.c`    | `main`, initialization, `is_sorted`, error exit and cleanup.                     |
| `parse_bonus.c`      | Argument parsing with overflow, duplicate and empty-argument detection.          |
| `read_bonus.c`       | Reading and validating instructions from standard input.                         |
| `operations_bonus.c` | Execution of the eleven operations.                                              |
| `stack_bonus.c`      | Stack primitives: push, pop, add to bottom and clear.                            |
| `checker_bonus.h`    | Structures, operation enum and prototypes.                                       |

The project uses our Libft (with our `ft_printf` inside `libft/printf/`), each with its own Makefile. The main Makefile builds them before building the program.

---

## Instructions

### Requirements

* A Unix-like system (developed and tested on Linux).
* `cc` and `make`.

### Compilation

```bash
make
```

This builds `libft/libft.a`, `libft/printf/libftprintf.a` and then the `push_swap` executable. The Makefile uses:

```text
cc -Wall -Wextra -Werror
```

Running `make` a second time does not recompile or relink anything.

To build the bonus `checker`:

```bash
make bonus
```

### Makefile rules

| Rule     | Description                                                       |
| -------- | ----------------------------------------------------------------- |
| `all`    | Builds Libft, ft_printf and `push_swap`.                          |
| `bonus`  | Builds the `checker` program.                                     |
| `clean`  | Removes object files (including the libraries' objects).          |
| `fclean` | Removes object files, both executables and the compiled libraries. |
| `re`     | Cleans the project and recompiles it.                             |

### Execution

```bash
./push_swap [--simple | --medium | --complex | --adaptive] [--bench] <numbers...>
```

| Flag         | Effect                                                                 |
| ------------ | ---------------------------------------------------------------------- |
| `--simple`   | Forces the `O(n²)` strategy (bubble sort).                             |
| `--medium`   | Forces the `O(n√n)` strategy (chunk sort).                             |
| `--complex`  | Forces the `O(n log n)` strategy (radix sort).                         |
| `--adaptive` | Uses the disorder-based selector. This is the default without a flag.  |
| `--bench`    | After sorting, prints a report with the metrics of the execution.      |

Flags go before the numbers. Numbers can be passed as separate arguments or inside a single quoted argument.

### Error handling

The program writes `Error` followed by `\n` to the standard error and exits when:

* An argument is not a valid integer (for example `one`, `-`, `1a`).
* A number is duplicated.
* There are no numbers to sort.

If the program is run without any argument, it prints nothing and returns control to the user.

---

## Usage Examples

Sorting a small list:

```bash
$ ./push_swap 2 1 3 6 5 8 | wc -l
```

Forcing each strategy on the same input and checking it with our checker:

```bash
$ ARG="4 67 3 87 23"
$ ./push_swap --simple   $ARG | ./checker $ARG
OK
$ ./push_swap --medium   $ARG | ./checker $ARG
OK
$ ./push_swap --complex  $ARG | ./checker $ARG
OK
$ ./push_swap --adaptive $ARG | ./checker $ARG
OK
```

Counting operations on a large random input:

```bash
$ shuf -i 0-9999 -n 500 > args.txt
$ ./push_swap $(cat args.txt) | wc -l
```

Benchmark mode:

```bash
$ ./push_swap --bench --medium 4 67 3 87 23
...
[Bench] strategy: Medium / O(n√n)
[Bench] total_ops: 15
[Bench] sb: 0 ss: 0  pa: 5  pb: 5
[Bench] ra: 2 rb: 2  rr: 0  rra: 0  rrb: 1  rrr: 0
```

Error handling:

```bash
$ ./push_swap --adaptive 0 one 2 3
Error
$ ./push_swap --simple 3 2 3
Error
```

---

## Bonus: checker

`checker` takes stack `a` as arguments, reads a list of operations from standard input (one per line), executes them all and prints:

* `OK` if `a` is sorted and `b` is empty.
* `KO` otherwise.
* `Error` (on standard error) if an argument is invalid, a number overflows `int`, a number is duplicated, an argument is empty, or an instruction does not exist or is badly formatted.

If no argument is given, it prints nothing.

```bash
$ ./checker 3 2 1 0
rra
pb
sa
rra
pa
OK
$ ./checker 3 2 one 0
Error
$ ./checker "" 1
Error
```

### How it works

* **Parsing (`parse_bonus.c`).** Each argument is scanned for numbers separated by any whitespace. The accumulated value is checked against the `int` limits **while it is being read**, so overflows are detected before they can wrap around. Duplicates are rejected, and an argument that contains no number at all (such as `""`) is an error.
* **Reading (`read_bonus.c`).** Standard input is read with `read()` in blocks of 4096 bytes and processed character by character. Only the letters `a`, `b`, `p`, `r` and `s` are accepted, with at most three characters per line. On every `\n`, the line is compared against a compact table of the eleven operation names stored in 3-character slots (`"sa sb ss pa pb ra rb rr rrarrbrrr"`). Any unknown instruction, or a last line not terminated by `\n`, is an error.
* **Storage.** Each valid instruction is stored as a single byte (its `t_op` value) in a dynamic array that grows when it is full. **All instructions are read before executing any**, as the subject requires.
* **Execution (`operations_bonus.c`).** `apply_op()` executes each operation. Combined operations (`ss`, `rr`, `rrr`) reuse the single-stack ones. Swaps exchange the two values instead of relinking nodes, which is simpler and equivalent.
* **Errors.** `error_exit()` frees both stacks and the instruction array before printing `Error` and exiting, so there are no leaks on error paths either.

---

## Benchmarks

Measured on Linux, 20 random inputs per row (values in `0..99999`). Every output was verified with the checker (`OK` in all cases).

| Input size | Strategy   | Average ops | Max ops |
| ---------- | ---------- | ----------- | ------- |
| 100        | `--simple` | 11 993      | 12 551  |
| 100        | `--medium` | 789         | 815     |
| 100        | `--complex`| 1 084       | 1 084   |
| 100        | default    | ~940        | 1 084   |
| 500        | `--medium` | 7 543       | 7 653   |
| 500        | `--complex`| 6 784       | 6 784   |
| 500        | default    | 7 235       | 7 720   |

Subject thresholds for reference: 100 numbers → minimum `< 2000`, good `< 1500`, excellent `< 700`; 500 numbers → minimum `< 12000`, good `< 8000`, excellent `< 5500`.

**Observations:**

* With the default selector, both input sizes stay within the "good" range.
* For 100 numbers, chunk sort beats radix sort (≈789 vs 1 084). For 500 numbers it is the opposite (≈7 543 vs 6 784): the `n√n` term grows faster than `n log n`, and the crossover happens between these two sizes.
* For random inputs the disorder index is close to `0.5`, which is exactly the boundary between the medium and high regimes. This explains why the adaptive results vary between the chunk sort and radix sort figures.

---

## Quality and Validation

* Compilation with `-Wall -Wextra -Werror` and no relink on a second `make`.
* Every strategy checked with our `checker` on random inputs of 3, 5, 100 and 500 numbers.
* Error cases tested: non-numeric arguments, lone signs, duplicates, empty arguments and flags combined with numbers.
* Checker tested with valid sequences, invalid instructions, unterminated lines, overflows and empty arguments.
* Memory leaks and invalid accesses checked with `valgrind --leak-check=full` on `push_swap` (all four strategies) and on `checker`, including error paths.

---

## Memory Management

* **`push_swap`:** each number is a node allocated once in `new_node()`. Operations only relink existing nodes, so no allocation happens while sorting except for the instruction list. At the end — and also when the parser finds an error — `global_clear()` frees both stacks and the instruction list (`ft_lstclear` with `free`).
* **`checker`:** nodes are freed with `stack_clear()`, and the instruction array with `free_checker()`. Every error path goes through `error_exit()`, which frees everything before exiting.

---

## Team and Contributions

This project was developed by two students, as required by the subject. Both of us reviewed the whole code base and are able to explain any part of it.

| Area                                                                                  | Responsible |
| ------------------------------------------------------------------------------------- | ----------- |
| Stack data structure (circular doubly linked list) and its primitives                 | abarba-m    |
| The eleven operations (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`) | abarba-m |
| Argument parsing, validation, error handling and index normalization                  | abarba-m    |
| Instruction list, output and `main` / context                                         | abarba-m    |
| Simple strategy (bubble sort), disorder index and adaptive selector                   | abarba-m    |
| `--bench` mode and Makefile                                                           | abarba-m/gecasas    |
| Medium strategy (chunk sort)                                                          | gecasas     |
| Complex strategy (radix sort)                                                         | gecasas     |
| Bonus `checker` program                                                               | gecasas     |

* **abarba-m** focused on how the operations work and on parsing: the stack structure, all operations, input validation and the general architecture of the program.
* **gecasas** focused on the medium and complex algorithms and on the bonus checker.

Estimated share of the total work: **abarba-m ≈ 65%**, **gecasas ≈ 35%**.

---

## What We Learned

The main lesson of this project was that **complexity depends on the cost model**. A sorting algorithm that is optimal on an array is not necessarily good here: what matters is how many stack operations it produces, and that changes which algorithm wins.

We also improved our ability to:

* Adapt classic algorithms (bubble sort, bucket/chunk sort, radix sort) to a very restricted set of operations.
* Analyse and justify the complexity of each strategy in a specific model, and check the analysis with real measurements.
* Choose data structures based on the operations that will be used most (circular doubly linked lists for constant-time rotations).
* Design programs without global variables, passing a shared context.
* Validate input robustly and handle errors without leaking memory.
* Split a project between two people, agree on common interfaces (`push_swap.h`) and integrate each other's work.

The most challenging part was the medium strategy and the adaptive selector: finding a chunk scheme that really stays in `O(n√n)` and choosing a disorder measure that makes the selector useful.

---

## AI Usage

AI was used only as a support tool to help **debug**, **test**, **fix specific errors** and **understand specific sorting concepts**.

---

## Resources

### Documentation and references

* 42 `push_swap` subject — Version 1.0.
* Donald E. Knuth, *The Art of Computer Programming, Vol. 3: Sorting and Searching*.
* Thomas H. Cormen et al., *Introduction to Algorithms* — chapters on sorting and asymptotic notation.
* [Big O notation — Wikipedia](https://en.wikipedia.org/wiki/Big_O_notation)
* [Radix sort — Wikipedia](https://en.wikipedia.org/wiki/Radix_sort)
* [Bubble sort — Wikipedia](https://en.wikipedia.org/wiki/Bubble_sort)
* [Bucket sort — Wikipedia](https://en.wikipedia.org/wiki/Bucket_sort)
* [Doubly linked list — Wikipedia](https://en.wikipedia.org/wiki/Doubly_linked_list)
* `valgrind(1)` — Linux manual page.
* Our previous Libft and ft_printf implementations.

### AI

Used only as described in [AI Usage](#ai-usage).

---

## Authors

**abarba-m** — 42 Málaga student.

**gecasas** — 42 Málaga student. GitHub: https://github.com/gecasas

</details>

<details>
<summary><strong>Español</strong></summary>

## Descripción

`push_swap` es un proyecto en grupo del currículo de 42. El programa recibe una lista de enteros, que forman el stack `a`, y debe imprimir la secuencia de operaciones que los ordena de forma ascendente (el número más pequeño en la cima), usando únicamente un segundo stack `b`, inicialmente vacío, y once operaciones permitidas.

El objetivo del proyecto es entender la **complejidad algorítmica** de una forma muy concreta: el coste de un algoritmo no se mide en comparaciones ni en tiempo de CPU, sino en el **número de operaciones de push_swap que imprime**. Ordenar una lista aleatoria y ordenar una lista casi ordenada son problemas distintos, y elegir la estrategia adecuada para cada caso cambia mucho el número de operaciones.

Nuestra implementación incluye:

* **Cuatro estrategias de ordenación**, seleccionables en tiempo de ejecución:
  * `--simple` — bubble sort adaptado, `O(n²)`.
  * `--medium` — ordenación por chunks con `√n` chunks, `O(n√n)`.
  * `--complex` — radix sort binario LSD sobre índices normalizados, `O(n log n)`.
  * `--adaptive` — calcula un índice de desorden y elige una de las anteriores (comportamiento por defecto).
* **Un modo `--bench`** que muestra el índice de desorden, la estrategia usada, el número total de operaciones y el número de operaciones de cada tipo.
* **Un parseo de argumentos robusto**: varios números por argumento, signos, entradas no numéricas y duplicados.
* **Bonus: nuestro propio programa `checker`**, que lee operaciones de la entrada estándar, las ejecuta e imprime `OK` o `KO`.

---

## Índice

* [Descripción](#descripción)
* [Las reglas](#las-reglas)
* [Algoritmos](#algoritmos)
* [Estructuras de datos](#estructuras-de-datos)
* [Decisiones técnicas](#decisiones-técnicas)
* [Estructura del proyecto](#estructura-del-proyecto)
* [Instrucciones](#instrucciones)
* [Ejemplos de uso](#ejemplos-de-uso)
* [Bonus: checker](#bonus-checker-1)
* [Benchmarks](#benchmarks-1)
* [Calidad y validación](#calidad-y-validación)
* [Gestión de memoria](#gestión-de-memoria)
* [Equipo y contribuciones](#equipo-y-contribuciones)
* [Qué hemos aprendido](#qué-hemos-aprendido)
* [Uso de IA](#uso-de-ia)
* [Recursos](#recursos)
* [Autores](#autores)

---

## Las reglas

Al empezar, el stack `a` contiene los números de entrada (el primer argumento en la cima) y el stack `b` está vacío. Las únicas operaciones permitidas son:

| Operación | Nombre               | Efecto                                                                        |
| --------- | -------------------- | ----------------------------------------------------------------------------- |
| `sa`      | swap a               | Intercambia los dos primeros elementos de `a`. No hace nada con menos de 2.   |
| `sb`      | swap b               | Intercambia los dos primeros elementos de `b`. No hace nada con menos de 2.   |
| `ss`      | swap ambos           | `sa` y `sb` a la vez.                                                         |
| `pa`      | push a               | Pasa la cima de `b` a la cima de `a`. No hace nada si `b` está vacío.         |
| `pb`      | push b               | Pasa la cima de `a` a la cima de `b`. No hace nada si `a` está vacío.         |
| `ra`      | rotate a             | Desplaza `a` una posición hacia arriba: el primero pasa a ser el último.      |
| `rb`      | rotate b             | Desplaza `b` una posición hacia arriba: el primero pasa a ser el último.      |
| `rr`      | rotate ambos         | `ra` y `rb` a la vez.                                                         |
| `rra`     | reverse rotate a     | Desplaza `a` una posición hacia abajo: el último pasa a ser el primero.       |
| `rrb`     | reverse rotate b     | Desplaza `b` una posición hacia abajo: el último pasa a ser el primero.       |
| `rrr`     | reverse rotate ambos | `rra` y `rrb` a la vez.                                                       |

El programa imprime una operación por línea, separadas únicamente por `\n`.

---

## Algoritmos

Todas las complejidades de esta sección se expresan en **número de operaciones de push_swap**, que es el modelo de coste que exige el subject. `n` es el número de elementos.

### Normalización de índices

Antes de ordenar, cada nodo recibe un `index`: su posición en el orden final (`0` para el valor más pequeño, `n - 1` para el más grande). Se calcula una sola vez en `assign_indices()`, contando para cada nodo cuántos valores son menores que él.

Trabajar con posiciones en lugar de valores tiene dos ventajas:

* Los valores pueden ser cualquier `int`, incluidos negativos y números muy grandes, pero los índices siempre están en `[0, n - 1]`. Esto es lo que permite usar radix sort sin tener que tratar los signos.
* La ordenación por chunks puede definir los chunks como simples rangos de índices (`[0, √n)`, `[√n, 2√n)`, ...) y sabe siempre qué elemento debe volver a `a` a continuación (el de mayor índice que quede en `b`).

### Índice de desorden

La estrategia adaptativa necesita un número entre `0` y `1` que indique lo lejos que está el stack `a` de estar ordenado. Se calcula en `compute_disorder()` **antes de realizar ninguna operación**.

Nuestra implementación recorre el stack una vez, cuenta los **pares adyacentes desordenados** (un valor mayor que el siguiente) y divide ese número entre el número de pares adyacentes (`n - 1`):

```text
desorden = (pares adyacentes con a[i] > a[i + 1]) / (n - 1)
```

* Un stack ordenado da `0`.
* Un stack en orden estrictamente decreciente da `1`.
* Para una permutación aleatoria, el valor esperado ronda `0.5`, porque cada par adyacente tiene aproximadamente la misma probabilidad de estar en orden o no.

Elegimos mirar los pares adyacentes porque se calcula en una sola pasada `O(n)` y refleja directamente lo que le importa a nuestra estrategia de desorden bajo: cuántos "cortes" hay entre los tramos que ya están ordenados.

### 1. Simple — bubble sort adaptado, `O(n²)` (`--simple`)

Archivo: `algorithms.c` — `bubble_sort()`, `bubble_operations()`.

El bubble sort se adapta al modelo de stacks usando la cima de `a` como "ventana de comparación":

1. Se comparan los dos primeros elementos de `a`. Si están desordenados, `sa`.
2. `ra` para avanzar la ventana una posición.
3. Se repite sobre la parte no ordenada del stack (`size - 1 - pass` comparaciones).
4. Se deshacen las rotaciones con el mismo número de `rra`, de modo que el stack vuelve a su alineación original con el mayor elemento de esa pasada en su posición final.
5. Si en una pasada completa no se ha hecho ningún intercambio, el stack está ordenado y el algoritmo termina antes.

El stack `b` no se utiliza.

* **Tiempo (operaciones):** cada pasada cuesta como mucho un `sa` por comparación más las rotaciones de ida y vuelta, es decir, `O(n)` por pasada. Hay como máximo `n - 1` pasadas, lo que da **`O(n²)`** en el peor caso.
* **Espacio:** `O(1)` adicional; solo se usa el stack `a`.

Es con diferencia la estrategia más costosa con entradas aleatorias (unas 12 000 operaciones para 100 números), pero tiene una propiedad importante: la **terminación anticipada**. Cuando la entrada está casi ordenada y los elementos desordenados solo tienen que moverse poca distancia, se necesitan muy pocas pasadas y el coste total es `O(n · p)`, siendo `p` el número de pasadas.

### 2. Medium — ordenación por chunks, `O(n√n)` (`--medium`)

Archivo: `sort_chunks.c` — `sort_chunks()`, `push_chunk()`, `push_back()`.

Los índices se dividen en chunks de tamaño `⌊√n⌋`. El algoritmo trabaja en dos fases:

**Fase 1 — enviar los chunks a `b` (`push_chunk`)**

Para cada chunk `[end - √n, end)`, en orden creciente:

1. Si la cima de `a` pertenece al chunk actual o a uno anterior (`index < end`), `pb`.
2. Si no, `ra` y se sigue buscando.
3. Cada chunk tiene un punto medio. Si el elemento recién enviado está en la **mitad inferior** del chunk, se manda al fondo de `b` con `rb`. Así cada chunk queda dividido en torno a su punto medio: los valores mayores quedan cerca de la cima de `b` y los menores cerca del fondo, lo que acorta las búsquedas de la fase 2.

**Fase 2 — devolver a `a` (`push_back`)**

Mientras `b` no esté vacío:

1. Se busca la posición del mayor índice restante (`b->size - 1`).
2. Se lleva a la cima por el camino más corto: `rb` si está en la mitad superior de `b`, `rrb` si no.
3. `pa`.

Como los elementos vuelven de mayor a menor, cada `pa` coloca el nuevo elemento encima de un stack ya ordenado, así que `a` termina ordenado.

* **Tiempo (operaciones):** en la fase 1, cada uno de los `√n` chunks puede necesitar rotar todo el stack `a`, es decir, `√n · n` rotaciones más `n` pushes. En la fase 2, gracias al orden de los chunks, el máximo de `b` siempre está dentro del chunk más reciente, cerca de la cima o del fondo de `b`, por lo que encontrarlo cuesta `O(√n)` rotaciones por elemento. Total: **`O(n√n)`**.
* **Espacio:** `O(1)` adicional; los `n` nodos solo se mueven entre `a` y `b`.

### 3. Complex — radix sort binario LSD, `O(n log n)` (`--complex`)

Archivo: `sort_radix.c` — `sort_radix()`, `radix_pass()`, `get_max_bits()`.

El radix sort encaja de forma natural en el modelo de dos stacks, porque cada pasada es una partición estable en dos grupos, y dos stacks son exactamente dos grupos.

1. Se calcula el número de bits necesario para representar el mayor índice: `bits = ⌈log₂ n⌉` (`get_max_bits(n - 1)`).
2. Para cada bit, del menos significativo al más significativo (`radix_pass`):
   * Se recorren una vez los `n` elementos de `a`. Si el bit actual del índice de la cima es `1`, `ra` (se queda en `a`); si es `0`, `pb`.
   * Se devuelve todo con `pa`.
3. Después de cada pasada, si `a` ya está ordenado, el algoritmo termina antes.

Tras procesar todos los bits, los índices —y por tanto los valores— quedan ordenados.

* **Tiempo (operaciones):** cada pasada cuesta `n` operaciones (`ra` o `pb`) más un `pa` por cada elemento enviado a `b`, es decir, como mucho `2n`. Con `⌈log₂ n⌉` pasadas, el total es **`O(n log n)`**.
* **Espacio:** `O(1)` adicional.

Una propiedad muy útil: como los índices siempre son una permutación de `0..n-1`, el número de operaciones **no depende del orden de la entrada**. Para 100 elementos siempre usa 1 084 operaciones, y para 500 elementos siempre usa 6 784. Eso la convierte en la estrategia más predecible para entradas muy desordenadas.

### 4. Adaptativa — selector según el desorden (`--adaptive`, por defecto)

Archivo: `algorithm_parser.c` — `parser()`, `compute_disorder()`.

La estrategia adaptativa calcula el índice de desorden y elige el método interno según los umbrales definidos en el subject:

| Desorden               | Régimen | Método usado                    | Complejidad objetivo |
| ---------------------- | ------- | ------------------------------- | -------------------- |
| `desorden < 0.2`       | Bajo    | Bubble sort (salida anticipada) | `O(n)`               |
| `0.2 ≤ desorden < 0.5` | Medio   | Ordenación por chunks           | `O(n√n)`             |
| `desorden ≥ 0.5`       | Alto    | Radix sort                      | `O(n log n)`         |

**Justificación de los umbrales:**

* **Por debajo de `0.2`**, menos de uno de cada cinco pares adyacentes está roto: el stack está formado por tramos largos que ya están en orden. En esta situación, el punto fuerte del bubble sort es su salida anticipada: cuando solo unos pocos elementos están fuera de sitio y cerca de su posición final, termina tras muy pocas pasadas, y cada pasada es lineal. Mover todos los elementos a `b` y de vuelta (como hacen las otras dos estrategias) costaría al menos `2n` operaciones incluso con una entrada casi ordenada.
* **Entre `0.2` y `0.5`**, la entrada está parcialmente ordenada. La ordenación por chunks se beneficia de ello, porque los elementos de un mismo chunk tienden a estar cerca en `a`, lo que reduce las rotaciones necesarias en la fase 1.
* **A partir de `0.5`**, la entrada se comporta como una permutación aleatoria o peor. Aquí el radix sort es la mejor opción, porque su coste está garantizado y no depende del orden de la entrada.

### Resumen de complejidades

| Estrategia | Técnica                       | Operaciones (tiempo) | Espacio extra | 100 números aleatorios | 500 números aleatorios |
| ---------- | ----------------------------- | -------------------- | ------------- | ---------------------- | ---------------------- |
| Simple     | Bubble sort sobre el stack    | `O(n²)`              | `O(1)`        | ~12 000                | no práctico            |
| Medium     | `√n` chunks                   | `O(n√n)`             | `O(1)`        | ~790                   | ~7 540                 |
| Complex    | Radix binario LSD             | `O(n log n)`         | `O(1)`        | 1 084                  | 6 784                  |
| Adaptativa | Selector según el desorden    | según el régimen     | `O(1)`        | ~950                   | ~7 240                 |

La columna de espacio se refiere a la memoria adicional que usa cada estrategia aparte de los `n` nodos que comparten los dos stacks. El programa además guarda cada operación generada en una lista antes de imprimirla, lo que ocupa `O(k)` memoria para `k` operaciones (ver [Decisiones técnicas](#decisiones-técnicas)).

---

## Estructuras de datos

### Stacks: listas doblemente enlazadas circulares

```c
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;
```

Cada stack es una **lista doblemente enlazada circular** con un puntero a su cima y su tamaño. Esta elección hace que las operaciones más frecuentes sean de tiempo constante:

* `ra` / `rb` solo mueven `top` a `top->next`.
* `rra` / `rrb` solo mueven `top` a `top->prev`.
* `pa` / `pb` desenlazan un nodo de una lista y lo enlazan en la otra, sin reservar memoria.
* El fondo del stack es siempre `top->prev`, así que nunca hace falta recorrer la lista para llegar a él.

### Contexto de ejecución

```c
typedef struct s_context
{
	t_stack		*a;
	t_stack		*b;
	t_move		*move;
	t_list		*instructions;
	t_list		*instructions_tail;
	t_algorithm	algorithm;
	int			has_algorithm;
	int			total_ops;
	int			bench_mode;
	float		original_disorder;
}	t_context;
```

El subject prohíbe las variables globales, así que todo el estado del programa se agrupa en una única estructura `t_context` que se pasa a cada operación y a cada algoritmo: los dos stacks, los contadores de operaciones (`t_move`), la lista de instrucciones generadas, la estrategia seleccionada y los datos del benchmark.

### Selección de estrategia

```c
typedef enum e_algorithm
{
	ALGO_SIMPLE,
	ALGO_MEDIUM,
	ALGO_COMPLEX,
	ALGO_ADAPTIVE
}	t_algorithm;
```

La estrategia se guarda como un enum, y `run_algorithm()` llama a la función correspondiente.

---

## Decisiones técnicas

* **Las operaciones se registran a sí mismas.** Cada función de operación (`sa`, `pb`, `rra`, ...) comprueba si se puede aplicar, añade su nombre a la lista de instrucciones y modifica los stacks. Los algoritmos nunca imprimen nada directamente; solo llaman a operaciones. Así la lógica de ordenación es independiente de la salida.
* **Las instrucciones se guardan y se imprimen al final.** Las operaciones generadas se almacenan en una `t_list` (de nuestra Libft) y se imprimen en `print_instructions()` cuando termina el algoritmo. Esto permite conocer el número total de operaciones para `--bench` y mantiene la salida en un único sitio.
* **Las operaciones no hacen nada cuando no se pueden aplicar** (por ejemplo `sa` con menos de dos elementos o `pa` con `b` vacío) y, en ese caso, no se registran, así que la salida nunca contiene instrucciones inútiles.
* **Índices en lugar de valores.** Los tres algoritmos de ordenación trabajan con `index`, no con `value`, así que los números negativos y todo el rango de `int` no necesitan ningún tratamiento especial.
* **Terminación anticipada** en el bubble sort (ningún intercambio en una pasada) y en el radix sort (stack ya ordenado tras una pasada), para no imprimir operaciones que no cambian nada.
* **Los flags se pueden combinar.** `--bench` y un selector de estrategia se pueden usar juntos (por ejemplo `--bench --complex`). Las opciones las reconoce `is_option()` y se ignoran al parsear los números.
* **Varios números por argumento.** `"3 2 1"` y `3 2 1` son equivalentes: cada argumento se divide por espacios sobre la propia cadena, sin reservas de memoria adicionales.
* **Estructuras de datos propias para el checker.** El programa bonus tiene su propio header y su propia estructura de nodo, más sencilla (sin `index`), para que pueda compilarse y evaluarse de forma independiente de `push_swap`, como pide el subject.

---

## Estructura del proyecto

```text
push_swap/
├── Makefile
├── README.md
├── push_swap.h
├── push_swap.c
├── argparser.c
├── argparser_utils.c
├── algorithm_parser.c
├── algorithms.c
├── sort_chunks.c
├── sort_radix.c
├── flags.c
├── stack.c
├── stack_operations.c
├── push_operations.c
├── rotate_operations.c
├── reverse_rotate_operations.c
├── checker_bonus.h
├── checker_bonus.c
├── parse_bonus.c
├── read_bonus.c
├── stack_bonus.c
├── operations_bonus.c
└── libft/
    ├── Makefile
    ├── libft.h
    ├── ft_*.c
    └── printf/
        ├── Makefile
        ├── ft_printf.h
        └── ft_*.c
```

### Parte obligatoria

| Archivo                       | Contenido                                                                                       |
| ----------------------------- | ----------------------------------------------------------------------------------------------- |
| `push_swap.c`                 | `main`, inicialización del contexto, lectura de flags, informe de `--bench` y limpieza final.   |
| `argparser.c`                 | División de argumentos, construcción del stack y normalización de índices (`assign_indices`).  |
| `argparser_utils.c`           | Validación de números, detección de duplicados y detección de opciones.                         |
| `algorithm_parser.c`          | Índice de desorden, selector adaptativo, traducción de flags a estrategia y dispatcher.         |
| `algorithms.c`                | Estrategia simple: bubble sort adaptado.                                                        |
| `sort_chunks.c`               | Estrategia media: ordenación por chunks.                                                        |
| `sort_radix.c`                | Estrategia compleja: radix sort binario, e `is_sorted`.                                         |
| `flags.c`                     | Lista de instrucciones: añadir, imprimir y liberar.                                             |
| `stack.c`                     | Creación de nodos, inicialización de stacks/contadores, inserción por el fondo y liberación.    |
| `stack_operations.c`          | `sa`, `sb`, `ss` y el swap interno.                                                             |
| `push_operations.c`           | `pa`, `pb` y el pop/push interno.                                                               |
| `rotate_operations.c`         | `ra`, `rb`, `rr`.                                                                               |
| `reverse_rotate_operations.c` | `rra`, `rrb`, `rrr`.                                                                            |
| `push_swap.h`                 | Estructuras, enum y prototipos.                                                                 |

### Parte bonus

| Archivo              | Contenido                                                                              |
| -------------------- | -------------------------------------------------------------------------------------- |
| `checker_bonus.c`    | `main`, inicialización, `is_sorted`, salida por error y limpieza.                      |
| `parse_bonus.c`      | Parseo de argumentos con detección de overflow, duplicados y argumentos vacíos.        |
| `read_bonus.c`       | Lectura y validación de instrucciones desde la entrada estándar.                       |
| `operations_bonus.c` | Ejecución de las once operaciones.                                                     |
| `stack_bonus.c`      | Primitivas del stack: push, pop, añadir al fondo y liberar.                            |
| `checker_bonus.h`    | Estructuras, enum de operaciones y prototipos.                                         |

El proyecto utiliza nuestra Libft (con nuestro `ft_printf` dentro de `libft/printf/`), cada una con su propio Makefile. El Makefile principal las compila antes de compilar el programa.

---

## Instrucciones

### Requisitos

* Un sistema tipo Unix (desarrollado y probado en Linux).
* `cc` y `make`.

### Compilación

```bash
make
```

Esto compila `libft/libft.a`, `libft/printf/libftprintf.a` y después el ejecutable `push_swap`. El Makefile utiliza:

```text
cc -Wall -Wextra -Werror
```

Ejecutar `make` por segunda vez no recompila ni reenlaza nada.

Para compilar el `checker` del bonus:

```bash
make bonus
```

### Reglas del Makefile

| Regla    | Descripción                                                                   |
| -------- | ----------------------------------------------------------------------------- |
| `all`    | Compila Libft, ft_printf y `push_swap`.                                       |
| `bonus`  | Compila el programa `checker`.                                                |
| `clean`  | Elimina los archivos objeto (incluidos los de las librerías).                 |
| `fclean` | Elimina los archivos objeto, los dos ejecutables y las librerías compiladas.  |
| `re`     | Limpia el proyecto y vuelve a compilarlo.                                     |

### Ejecución

```bash
./push_swap [--simple | --medium | --complex | --adaptive] [--bench] <números...>
```

| Flag         | Efecto                                                                  |
| ------------ | ----------------------------------------------------------------------- |
| `--simple`   | Fuerza la estrategia `O(n²)` (bubble sort).                             |
| `--medium`   | Fuerza la estrategia `O(n√n)` (chunks).                                 |
| `--complex`  | Fuerza la estrategia `O(n log n)` (radix sort).                         |
| `--adaptive` | Usa el selector basado en el desorden. Es el comportamiento por defecto. |
| `--bench`    | Tras ordenar, muestra un informe con las métricas de la ejecución.      |

Los flags van antes de los números. Los números pueden pasarse como argumentos separados o dentro de un único argumento entre comillas.

### Gestión de errores

El programa escribe `Error` seguido de `\n` en la salida de error estándar y termina cuando:

* Un argumento no es un entero válido (por ejemplo `one`, `-`, `1a`).
* Hay un número duplicado.
* No hay números que ordenar.

Si se ejecuta sin ningún argumento, no muestra nada y devuelve el control al usuario.

---

## Ejemplos de uso

Ordenar una lista pequeña:

```bash
$ ./push_swap 2 1 3 6 5 8 | wc -l
```

Forzar cada estrategia sobre la misma entrada y comprobarla con nuestro checker:

```bash
$ ARG="4 67 3 87 23"
$ ./push_swap --simple   $ARG | ./checker $ARG
OK
$ ./push_swap --medium   $ARG | ./checker $ARG
OK
$ ./push_swap --complex  $ARG | ./checker $ARG
OK
$ ./push_swap --adaptive $ARG | ./checker $ARG
OK
```

Contar las operaciones con una entrada grande y aleatoria:

```bash
$ shuf -i 0-9999 -n 500 > args.txt
$ ./push_swap $(cat args.txt) | wc -l
```

Modo benchmark:

```bash
$ ./push_swap --bench --medium 4 67 3 87 23
...
[Bench] strategy: Medium / O(n√n)
[Bench] total_ops: 15
[Bench] sb: 0 ss: 0  pa: 5  pb: 5
[Bench] ra: 2 rb: 2  rr: 0  rra: 0  rrb: 1  rrr: 0
```

Gestión de errores:

```bash
$ ./push_swap --adaptive 0 one 2 3
Error
$ ./push_swap --simple 3 2 3
Error
```

---

## Bonus: checker

`checker` recibe el stack `a` como argumentos, lee una lista de operaciones de la entrada estándar (una por línea), las ejecuta todas e imprime:

* `OK` si `a` está ordenado y `b` está vacío.
* `KO` en cualquier otro caso.
* `Error` (en la salida de error) si un argumento no es válido, un número desborda `int`, hay un número duplicado, un argumento está vacío o una instrucción no existe o tiene un formato incorrecto.

Si no recibe ningún argumento, no muestra nada.

```bash
$ ./checker 3 2 1 0
rra
pb
sa
rra
pa
OK
$ ./checker 3 2 one 0
Error
$ ./checker "" 1
Error
```

### Cómo funciona

* **Parseo (`parse_bonus.c`).** Se buscan números en cada argumento, separados por cualquier tipo de espacio en blanco. El valor acumulado se compara con los límites de `int` **mientras se va leyendo**, de modo que los overflows se detectan antes de que el número pueda desbordarse. Los duplicados se rechazan, y un argumento que no contiene ningún número (como `""`) es un error.
* **Lectura (`read_bonus.c`).** La entrada estándar se lee con `read()` en bloques de 4096 bytes y se procesa carácter a carácter. Solo se aceptan las letras `a`, `b`, `p`, `r` y `s`, con un máximo de tres caracteres por línea. En cada `\n`, la línea se compara con una tabla compacta de los once nombres de operación guardados en huecos de 3 caracteres (`"sa sb ss pa pb ra rb rr rrarrbrrr"`). Cualquier instrucción desconocida, o una última línea sin `\n`, es un error.
* **Almacenamiento.** Cada instrucción válida se guarda como un único byte (su valor `t_op`) en un array dinámico que crece cuando se llena. **Se leen todas las instrucciones antes de ejecutar ninguna**, como pide el subject.
* **Ejecución (`operations_bonus.c`).** `apply_op()` ejecuta cada operación. Las operaciones combinadas (`ss`, `rr`, `rrr`) reutilizan las de un solo stack. Los swaps intercambian los dos valores en lugar de reenlazar nodos, lo cual es más sencillo y equivalente.
* **Errores.** `error_exit()` libera los dos stacks y el array de instrucciones antes de imprimir `Error` y salir, así que tampoco hay leaks en los caminos de error.

---

## Benchmarks

Medido en Linux, 20 entradas aleatorias por fila (valores en `0..99999`). Todas las salidas se verificaron con el checker (`OK` en todos los casos).

| Tamaño | Estrategia  | Media de ops | Máximo de ops |
| ------ | ----------- | ------------ | ------------- |
| 100    | `--simple`  | 11 993       | 12 551        |
| 100    | `--medium`  | 789          | 815           |
| 100    | `--complex` | 1 084        | 1 084         |
| 100    | por defecto | ~940         | 1 084         |
| 500    | `--medium`  | 7 543        | 7 653         |
| 500    | `--complex` | 6 784        | 6 784         |
| 500    | por defecto | 7 235        | 7 720         |

Umbrales del subject como referencia: 100 números → mínimo `< 2000`, bueno `< 1500`, excelente `< 700`; 500 números → mínimo `< 12000`, bueno `< 8000`, excelente `< 5500`.

**Observaciones:**

* Con el selector por defecto, los dos tamaños de entrada quedan dentro del rango "bueno".
* Con 100 números, la ordenación por chunks supera al radix sort (≈789 frente a 1 084). Con 500 números ocurre lo contrario (≈7 543 frente a 6 784): el término `n√n` crece más rápido que `n log n`, y el punto de cruce está entre estos dos tamaños.
* Con entradas aleatorias, el índice de desorden ronda `0.5`, que es justo la frontera entre los regímenes medio y alto. Esto explica que los resultados de la estrategia adaptativa oscilen entre las cifras de los chunks y las del radix sort.

---

## Calidad y validación

* Compilación con `-Wall -Wextra -Werror` y sin relink en un segundo `make`.
* Todas las estrategias comprobadas con nuestro `checker` sobre entradas aleatorias de 3, 5, 100 y 500 números.
* Casos de error probados: argumentos no numéricos, signos sueltos, duplicados, argumentos vacíos y flags combinados con números.
* Checker probado con secuencias válidas, instrucciones inválidas, líneas sin terminar, overflows y argumentos vacíos.
* Memory leaks y accesos inválidos comprobados con `valgrind --leak-check=full` en `push_swap` (las cuatro estrategias) y en `checker`, incluidos los caminos de error.

---

## Gestión de memoria

* **`push_swap`:** cada número es un nodo reservado una sola vez en `new_node()`. Las operaciones solo reenlazan nodos existentes, así que durante la ordenación no se reserva memoria salvo para la lista de instrucciones. Al final —y también cuando el parser encuentra un error— `global_clear()` libera los dos stacks y la lista de instrucciones (`ft_lstclear` con `free`).
* **`checker`:** los nodos se liberan con `stack_clear()` y el array de instrucciones con `free_checker()`. Todos los caminos de error pasan por `error_exit()`, que libera todo antes de salir.

---

## Equipo y contribuciones

Este proyecto lo han desarrollado dos estudiantes, como exige el subject. Los dos hemos revisado todo el código y somos capaces de explicar cualquier parte.

| Área                                                                                         | Responsable |
| -------------------------------------------------------------------------------------------- | ----------- |
| Estructura del stack (lista doblemente enlazada circular) y sus primitivas                   | abarba-m    |
| Las once operaciones (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`)   | abarba-m    |
| Parseo de argumentos, validación, gestión de errores y normalización de índices              | abarba-m    |
| Lista de instrucciones, salida y `main` / contexto                                           | abarba-m    |
| Estrategia simple (bubble sort), índice de desorden y selector adaptativo                    | abarba-m    |
| Modo `--bench` y Makefile                                                                    | abarba-m/gecasas    |
| Estrategia media (chunks)                                                                    | gecasas     |
| Estrategia compleja (radix sort)                                                             | gecasas     |
| Programa bonus `checker`                                                                     | gecasas     |

* **abarba-m** se ha centrado en el funcionamiento de las operaciones y en el parseo: la estructura del stack, todas las operaciones, la validación de la entrada y la arquitectura general del programa.
* **gecasas** se ha centrado en los algoritmos medium y complex y en el checker del bonus.

Reparto estimado del trabajo total: **abarba-m ≈ 65 %**, **gecasas ≈ 35 %**.

---

## Qué hemos aprendido

La lección principal de este proyecto ha sido que **la complejidad depende del modelo de coste**. Un algoritmo de ordenación óptimo sobre un array no tiene por qué ser bueno aquí: lo que importa es cuántas operaciones de stack produce, y eso cambia qué algoritmo gana.

Además, hemos mejorado en:

* Adaptar algoritmos clásicos (bubble sort, bucket/chunk sort, radix sort) a un conjunto de operaciones muy limitado.
* Analizar y justificar la complejidad de cada estrategia en un modelo concreto, y comprobar ese análisis con mediciones reales.
* Elegir estructuras de datos en función de las operaciones que más se van a usar (listas doblemente enlazadas circulares para rotaciones en tiempo constante).
* Diseñar programas sin variables globales, pasando un contexto compartido.
* Validar la entrada de forma robusta y gestionar los errores sin perder memoria.
* Repartir un proyecto entre dos personas, acordar interfaces comunes (`push_swap.h`) e integrar el trabajo del otro.

La parte más complicada fue la estrategia media y el selector adaptativo: encontrar un esquema de chunks que se mantenga realmente en `O(n√n)` y elegir una medida de desorden que haga útil al selector.

---

## Uso de IA

La IA se ha utilizado únicamente como herramienta de apoyo para **depurar**, **testear**, **solucionar errores puntuales** y **entender conceptos puntuales de ordenación**.

---

## Recursos

### Documentación y referencias

* Subject de `push_swap` de 42 — versión 1.0.
* Donald E. Knuth, *The Art of Computer Programming, Vol. 3: Sorting and Searching*.
* Thomas H. Cormen et al., *Introduction to Algorithms* — capítulos sobre ordenación y notación asintótica.
* [Notación Big O — Wikipedia](https://es.wikipedia.org/wiki/Cota_superior_asint%C3%B3tica)
* [Radix sort — Wikipedia](https://en.wikipedia.org/wiki/Radix_sort)
* [Bubble sort — Wikipedia](https://en.wikipedia.org/wiki/Bubble_sort)
* [Bucket sort — Wikipedia](https://en.wikipedia.org/wiki/Bucket_sort)
* [Lista doblemente enlazada — Wikipedia](https://en.wikipedia.org/wiki/Doubly_linked_list)
* `valgrind(1)` — manual de Linux.
* Nuestras implementaciones anteriores de Libft y ft_printf.

### IA

Utilizada únicamente tal y como se describe en [Uso de IA](#uso-de-ia).

---

## Autores

**abarba-m** — Estudiante de 42 Málaga.

**gecasas** — Estudiante de 42 Málaga. GitHub: https://github.com/gecasas

</details>