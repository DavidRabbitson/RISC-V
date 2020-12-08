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

    rf.close();

    return 0;
}
