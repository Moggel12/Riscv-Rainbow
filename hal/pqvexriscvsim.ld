OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY( _start )

__ram_size = 1024K;

MEMORY
{
  rom (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 512K
  ram (wxa!ri) : ORIGIN = 0x80080000, LENGTH = 512K
}

INCLUDE nonvolatile.ld
