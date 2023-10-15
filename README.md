# Emos

Project **emos** is an **e**mulator of **MOS**6502 microprocessor developed in 1975 by MOS Technology. It was used in famous machines, such as Apple I or Atari. This project is an attempt to emulate its behaviour along with known bugs and quirks on a modern x86 processor using C++ language.

## Resources
- [Wikipedia article with historical context](https://en.wikipedia.org/wiki/MOS_Technology_6502)
- [Architecture](http://www.6502.org/users/obelisk/6502/index.html)
- [Video series on the same project](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37)

## Building and testing
```
git submodule init
git submodule update
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config=Debug
ctest -C Debug --verbose
```
