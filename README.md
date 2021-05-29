# RISCV-Rainbow 
A Risc-V implementation of the Rainbow MPKC, made for the BADM500 course at University of Southern Denmark. The project is basically a fork of https://github.com/fast-crypto-lab/rainbow-submission-round2 with some extras
  
## Information   
Other than having a ported version of the reference implementation from
https://github.com/fast-crypto-lab/rainbow-submission-round2 
the project files also include a high-level bitsliced implementation written in C and a smaller lookup table tweak for the original code. The bitsliced implementation is quite slow, though lies as a basis on which it is possible to optimize as wanted.


This project is built on [PQVexRiscV](https://github.com/mupq/pqriscv-vexriscv).

The implementation also allows for continous calls to the verification/signing/key-generation without rebooting the CPU, assuming no jobs currently communicating with the CPU and executable are prematurely killed.

All Python scripts associated with this project uses the libraries in `requirements.txt`. To run any Python script mentioned in the subsequent sections, make sure to have these installed in your environment.

## Compilation
Before compiling, an implementation should be chosen. The different implementations can be chosen by commenting out some of the lines in `src/Ia_Classic_Reference/impl.h`
1. If `#define SLICE_IMPL` is *not* commented out, then the bitsliced implementation is compiled.
2. If above is not the case and `#define LOOKUP_INC_ZERO` is *not* commented out (this is the only uncommented line), then the lookup table variant is compiled.
3. If all lines are commented out, then the standard (ported) implementation is compiled.

Compiling for a simulated PQVexRiscV CPU can be done using
```bash
make PLATFORM=pqvexriscvsim
```

## How to run
**BENCHMARKS** are only supported for verification currently (though is easily implemented in the others).

In the `test/` folder two small scripts can be used to communicate with a target device. Both scripts use pseudo-terminal (pts) devices to communicate with the CPU though should also work with other ways of communicating through `UART`. Running the simulated CPU using verilator (only tested with verilator 4.102) can be done by
```bash
sbt "runMain mupq.PQVexRiscvSim --ram 512,512 --init <path/to/bin-file> --pts"
```
The `--pts` flag makes sure to open a pts device for communication. The bin-file needed for `--init` can be found in `build/` once compiled (using the provided Makefile).

Once the CPU is running, the Rainbow executables each expect the following protocol for obtaining keys, signatures and messages:
* Key generation:
  * Send a random seed for key generation (48 bytes)
* Message signing
  * Send the secret key first
  * Send the length of the message (using the protocol below) followed by the message
* Signature verification
  * Send the public key first
  * Same as the second step for message signing
  * Send the signature in the same way that messages were sent
(NOTE: One executable runs on the CPU at a time)

The protocol for sending a message or signature along with its length is
* Send a 0 if the length is more than 256
* Continously send lengths of at most 256. E.g. len(message) = 520:
  * Send 256
  * Send 256
  * Send 8
* Send an ending 0
If a nonzero value arrives at the device in the first step, then all subsequent bytes sent are assumed to be part of the message or signature currently being communicated. (NOTE: This protocol is not designed to be greatly secure, just one way of communicating lengths in single bytes at a time)

All the above is handled by the `test/kat.py` script. A typical call for this script could be
```bash
./kat.py -u /dev/pts/2
```
 for a CPU being hooked up to  the `/dev/pts/2 device`. Running the script like this will create a `KAT_<some numbers>` folder with the *Known-Answer Test* files just used. If the `-b` flag is used, the script will also benchmark the cycle count and instruction count for the implementation being tested. If you already have some `KAT_<some number>`, one such folder can tested again using `-c <some number>` where *<some number>* is the number after the underscore. By default, the *verification* scheme is used by the `kat.py` script (as this was one focus for the project) though the other functions can be tested by using `-f <genkey|verify|sign>`
  
For more flags for the `kat.py` script, use `./kat.py --help`
  
The `test/multitest.sh` can also be called to either do bulk-testing or bulk-benchmarking. It will create a `KAT_<some number>/<implementation chosen>/bencmarks` file if used for benchmarking. This file states the cycles and instructions used for the known-answer test that it resides in. 

## Report  
A report documenting the efforts of this project can be found in the `report/` directory. The latex files can be compiled using the makefile situated in that folder (assuming latexmk is installed on your system).
```bash 
cd report 
make
```
  
## Bar plots
Using the `test/grapher.py` script allows for creating bar plots easily, assuming the `multitest.sh` script has been run prior to execution (to obtain the needed becnhmark files).
