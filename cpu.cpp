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
    //cpu.run();

    Word a = 0x13;
    RegVal b = 0xffffffa1;
    printf("%d\n", b);

    b = a + (SignedRegVal)b;
    
    printf("%d\n", a);
    printf("%d\n", b);

    rf.close();

    return 0;
}
