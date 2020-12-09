#include"includes.h"
#include"./headers/Hart.h"
#include"./headers/Memory.h"
#include"./headers/Instruction.h"
#include"./headers/Executor.h"

int main()
{
    ifstream rf("queens.bin", ios::out | ios::binary);
    if(!rf)
        cout << "Cannot open file!" << endl;

    Memory mem(&rf);

    Hart cpu(&mem);
    cpu.run();
    
    Word test_w = 256 * 256 + 256 + 1;
    Word test_r = 0;
    
    mem.dram_write(&test_w, 100, sizeof(Byte));
    mem.dram_write(&test_w, 200, sizeof(HalfWord));
    mem.dram_write(&test_w, 300, sizeof(Word));

    mem.dram_read(&test_r, 100, sizeof(Byte));
    printf("B from B = %d\n", test_r);
    mem.dram_read(&test_r, 200, sizeof(Byte));
    printf("B from HW = %d\n", test_r);
    mem.dram_read(&test_r, 300, sizeof(Byte));
    printf("B from W = %d\n", test_r);

    mem.dram_read(&test_r, 100, sizeof(HalfWord));
    printf("HW from B = %d\n", test_r);
    mem.dram_read(&test_r, 200, sizeof(HalfWord));
    printf("HW from HW = %d\n", test_r);
    mem.dram_read(&test_r, 300, sizeof(HalfWord));
    printf("HW from W = %d\n", test_r);

    mem.dram_read(&test_r, 100, sizeof(Word));
    printf("W from B = %d\n", test_r);
    mem.dram_read(&test_r, 200, sizeof(Word));
    printf("W from HW = %d\n", test_r);
    mem.dram_read(&test_r, 300, sizeof(Word));
    printf("W from W = %d\n", test_r);
    
    
    rf.close();

    return 0;
}
