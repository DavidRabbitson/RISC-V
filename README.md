# RISC-V
Functional simulator of RISC-V processor. This model is based on RV32I base instruction set.

#### Curently implemented:
* Build script;
* Fetch from binary file;
* Decode stage;
* Arithmetical functions.

#### To do:
* Branches, system calls and CSR commands.

## Prerequisites:
* Installed [RISC-V toolchain](https://github.com/riscv/riscv-gnu-toolchain). Make sure to perform **Installation (Newlib)** steps with
```sh
./configure --prefix=/opt/riscv --with-arch=rv32g --with-abi=ilp32d
make
```
to install uncompressed 32bit RISC-V instruction set toolchain with `riscv32-unknown-elf-*` commands.
* Installed `g++-multilib` to build cpu with `-m32` flag.
