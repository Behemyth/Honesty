# Loggers

## Table of Contents

- [root](#root)
- [root.main](#main)
- [root.honesty](#honesty)
- [root.honesty.\<thread id\>](#thread)

---

## `root` {#root}

## `root.main` {#main}

The main tagged logger is separated from the logger used by `honesty` itself. In our bundled console application we use `root.main` to separate our logging concerns.

## `root.honesty` {#honesty}

## `root.honesty.<thread id>` {#thread}

This is the logger used for each suite, and ultimately each test. It is associated with the thread that is running the test.
