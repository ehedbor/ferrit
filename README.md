# Ferrit
Ferrit is a statically-typed object-oriented programming language designed to compile to LLVM bytecode. 
This is a work in progress and doesn't do anything useful yet. :(

## Building
Take the following steps to compile the project:
1. Install CMake and Git if you haven't already.
2. Clone the repository.
3. Download and build both [LLVM](https://releases.llvm.org/12.0.1/docs/GettingStarted.html) 
   and [Boost](https://www.boost.org/doc/libs/1_77_0/more/getting_started/windows.html). 
   Install them to your operating system's program root, or specify their locations via the 
   environment variables `LLVM_DIR` and `BOOST_ROOT`. 
4. Compile the project with CMake.

This project was developed with MSVC. Other compilers should work, though if you're a Windows user 
I would highly recommend sticking with MSVC, as I had issues with other toolchains.

## License
This project is licensed under the MIT license. See LICENSE.txt for details.