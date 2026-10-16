# 50 small C exercises

I made this repo as a small beginner playground, for [Gabe Sampson](https://www.youtube.com/@gabe_sampson/featured) to try it out. \
It comes with prepared exercises and a testing suite for ease running and testing. Feel free to clone this repo and try the exercises yourself, if you feel like.

## Requirements

- A C Compiler
- Make
- Python 3

## How to work with this repo

Exercises are located in [./exercises/](./exercises/). Each exercise is accompanied by an `.md`, which explains the problem in more detail, and even gives you some test cases for you to try it. Your job is to implement the corresponding function in each `.c` file.
Each `.c` file also contains a `main` function that you can use to play around and test your code before running the real tests.

> [!TIP]
> If you're feeling bold, try doing each exercise without reading the `.md` first.

Here are the commands you need:

```bash
make run N=1        # Run your `main` function in exercise 1
make test N=1       # Run the tests for exercise 1
```

You can append `V=1` to the above commands if you wanna see the full compile command from Make. \
You can also append `SANITIZE=1` to run the `main`/tests with sanitizers on. It can be useful to help you debug your programs.

In root of the repo, there is a `lib.{c,h}` file. You can put your function declarations/definitions on them for global reuse on all tests. After you finish an exercise, if you find the function you implemented useful and want to reuse it, **move** the definition from the exercise to the `lib.c` and write it's declaration on `lib.h`.

You can view the full index of exercises in [./exercises/README.md](./exercises/README.md).

## AI Disclosure

The idea of this playground, structure of the exercises, amount of information given and how to run the tests was made all by myself. \
I used AI to write all of the tests and exercises tho. I do not respect the C language enough to want to do all of this by myself, sorry </3
