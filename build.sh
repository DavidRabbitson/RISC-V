if [[ $PATH != *":/opt/riscv/bin"* ]]; then
    export PATH=$PATH:/opt/riscv/bin
fi

if [ $# -eq 0 ]; then
    echo "riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib test.c -o test"
    riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib test.c -o test
    echo "riscv32-unknown-elf-objcopy --dump-section .text=test.bin test"
    riscv32-unknown-elf-objcopy --dump-section .text=test.bin test
    echo "g++ cpu.cpp -o cpu"
    g++ cpu.cpp -o cpu
    exit 0
fi

case "$1" in
    test)
    echo "riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib test.c -o test"
    riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib test.c -o test
    echo "riscv32-unknown-elf-objcopy --dump-section .text=test.bin test"
    riscv32-unknown-elf-objcopy --dump-section .text=test.bin test
    exit 0
    ;;
    queens)
    echo "riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 8-queens.c -o queens"
    riscv32-unknown-elf-gcc -Wl,-Ttext=0x0 8-queens.c -o queens
    echo "riscv32-unknown-elf-objcopy --dump-section .text=queens.bin queens"
    riscv32-unknown-elf-objcopy --dump-section .text=queens.bin queens
    exit 0
    ;;
    cpu)
    echo "g++ cpu.cpp -o cpu"
    g++ cpu.cpp -o cpu
    exit 0
    ;;
    clean)
    echo "rm test.bin"
    rm test.bin
    echo "rm test"
    rm test
    echo "rm queens.bin"
    rm queens.bin
    echo "rm queens"
    rm queens
    echo "rm cpu"
    rm cpu
    exit 0
    ;;
    -h|--help)
    echo "test:  Build test."
    echo "cpu:   Build cpu."
    echo "clean: Remove output files."
    echo "-h|--help: Show this message."
    exit 0
    ;;
    *)
    echo "Invalid argument. Try running \"./build.sh -h\""
    exit 1
    ;;
esac
