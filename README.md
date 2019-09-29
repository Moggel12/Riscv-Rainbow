# PQRiscv-VexRiscv Project Template

Use this repository as a template for your application which you want to
run on the pqriscv-vexriscv processor family. Adapt the `Makefile` to
use your sources.

This template requires the `pqriscv` repository to be checked out
somewhere. Point the `PQRISCV_DIR` variable to the right path (default
is `pqriscv` checked out beside this dir).

Change the `VEXRISCV_PLATFORM` variable to the desired platform (default
is the simulator). Remember to rebuild your project if you change the
target platform (the build system will remind you).
