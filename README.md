# termBreaker

[![ci](https://github.com/ArthurSonzogni/termBreaker/actions/workflows/ci.yml/badge.svg)](https://github.com/ArthurSonzogni/termBreaker/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/ArthurSonzogni/termBreaker/branch/main/graph/badge.svg)](https://codecov.io/gh/ArthurSonzogni/termBreaker)
[![CodeQL](https://github.com/ArthurSonzogni/termBreaker/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/ArthurSonzogni/termBreaker/actions/workflows/codeql-analysis.yml)
[![Language grade: C++](https://img.shields.io/lgtm/grade/cpp/github/ArthurSonzogni/termBreaker)](https://lgtm.com/projects/g/ArthurSonzogni/termBreaker/context:cpp)


Check the WebAssembly version online:
[Play](https://arthursonzogni.com/TermBreaker/)

## About termBreaker

This is a game made for the C++ Best Practices Game Jam.

Use your mouse to throw balls toward the blocks before they reach you.

Earn coins by completing levels. Buy additional balls to grow and complete even
hardder levels.


## How to build.

```bash
mkdir build;
cd build;
cmake ..
make -j
./termBreaker
```

## How to test

```bash
mkdir build;
cd build;
cmake ..
make -j
./test/termBreakerTests
```

## What warning have been removed?

MSVC with DEVELOPER_MODE seems to fail. I had to remove them to pass the CI.
