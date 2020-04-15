# PQRiscv-VexRiscv Project Template

Use this repository as a template for your application which you want to
run on the pqriscv-vexriscv processor family. Adapt the `Makefile` to
use your sources.

Change the `PLATFORM` variable to the desired platform (default is the
simulator).

# Build System

## Out-of-tree build

The build system uses an out-of-tree build approach using the `VPATH`
mechanism of GNU make. All generated files will land in the directory
pointed at by the `OBJDIR` variable (default is `./build`). Note, that
`make clean` will simply delete the output directory!

## Configuration

The build process is configurable with the following variables.

  * `OBJDIR` (default `build`): The output directory
  * `Q` (default `@`): Silences the build, set to empty value (i.e.,
    build with `make Q=`) to enable printing of build of commands.
  * `DEBUG` (default empty): Set to `1` to compile without optimization
    (debug symbols are always on).
  * `LTO` (default empty): Set to `1` to enable link time optimization.
  * `OPT_SIZE` (default empty): Set to `1` to compile with size
    optimization (i.e., `-Os`). The `DEBUG` option takes precedence.
  * `PLATFORM` (default `pqvexriscvsim`): Set to any valid platform,
    c.f. `hal/hal.mk` for a list.

Most configuration variables should not be changed between subsequent
builds without running `make clean`. The build system actually remembers
and checks these values between successive rebuilds.

```SH
make clean
make PLATFORM=pqvexriscvup5k DEBUG=1
make # This call will use the PLATFORM/DEBUG values set above!
make PLATFORM=pqvexriscvsim # This call will then lead to an error!
```

# Hardware Abstraction Layer

The project template includes a HAL akin to that of the
`mupq`/`pqm4`/`pqriscv` projects. You should call the `hal_setup`
function at the start of your `main` function. After this, the HAL
functions (c.f. `hal/hal.h`). The HAL also includes a printing utility
for printing simple python dictionaries (c.f. `hal/sendfn.h`). See
`src/demo.c` for an example. These functions avoid `sprintf` calls to
conserve code size. Each `send_X` function takes an identifying string
and a value. The integer values may also be either binary, decimal of
hexadecimal formated. Example:

```C
unsigned char bytes[] = {0xDEu, 0xADu, 0xBEu, 0xEFu};
send_start();
send_unsigned("u_bin", 0xDEADBEEFu, 2);
send_unsigned("u_dec", 0xDEADBEEFu, 10);
send_unsigned("u_hex", 0xDEADBEEFu, 16);
send_unsignedll("llu_dec", 0xDEADBEEFDEADBEEFllu, 10);
send_string("strings", "\"Escaped\"\n string!");
send_bytes("bytes", bytes, sizeof(bytes));
send_stop();
```

Produces the output:
```Python
{
"u_bin": 0b11011110101011011011111011101111,
"u_dec": 3735928559,
"u_hex": 0xDEADBEEF,
"llu_dec": 16045690984833335023,
"strings": "\"Escaped\"\n string!",
"bytes": bytes.fromhex("DEADBEEF"),
}
```

The build system will also generate a `hal_name` string, which you can
use to identify the name of your executable. For example, the in the
`demo.elf` executable, the `hal_name` string will contain the value
`"demo.elf"`. The string is automatically generated and linked into your
executable so you don't need to bother with it. You can then use, for
example, use the following to send an identifiable string via UART:

```C
send_string("targetname", hal_name)
```

