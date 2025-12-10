# CMake Targets

## Table of Contents

- [honesty](#honesty)
- [honesty::entry](#entry)

---

## `honesty` {#honesty}

A static target that links to every other target in the project.

## `honesty::entry` {#entry}

A static target that provides the same functionality as [honesty](#honesty) but also includes a main function that will run all tests discoverable in the executable.
