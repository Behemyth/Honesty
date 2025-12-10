# Modules

## Table of Contents

- [Root Modules](#root-modules)
- [Test Modules](#test-modules)
- [Metric Modules](#metric-modules)

---

## Root Modules

Convenience modules that provide access to a selection of other modules.

### `synodic.honesty`

Exports all other modules.

---

## Test Modules

Library for unit testing and other testing utilities.

### Public Modules

There are a variety of ways to interact with the testing library.

#### `synodic.honesty.test`

For defining and implementing tests.

#### `synodic.honesty.test.api`

Direct access to the test API.

### Private Modules

Modules that are used in multiple other, public modules that are not intended to be used directly by the user.

#### `synodic.honesty.test.backend`

The private infrastructure of the API module.

#### `synodic.honesty.test.entry`

The entrypoint for the library that defines the main function. Linking to the target generated with this module will automatically collect and run tests according to the default configuration.

---

## Metric Modules

Library for benchmarking and performance measurement.

### `synodic.honesty.metric`

The main metric module providing benchmarking contexts, timing utilities, and statistical result types.

#### Partitions

- `:types` - Clock and duration type aliases
- `:timer` - RAII-based timer for duration measurement
- `:results` - Statistical results with accumulator integration
- `:context` - Measurement contexts (`TimedContext`, `RegressionContext`)
- `:sink.otel` - OpenTelemetry metrics export

---

## Module Relationships

```
synodic.honesty
├── synodic.honesty.test
│   ├── synodic.honesty.test.api
│   ├── synodic.honesty.test.backend
│   └── synodic.honesty.test.entry
├── synodic.honesty.metric
├── synodic.honesty.log
├── synodic.honesty.trace
└── synodic.honesty.utility
```
