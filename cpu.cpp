#include"includes.h"
#include"Hart.h"
#include"Memory.h"

int main()
{
    ifstream rf("test.bin", ios::out | ios::binary);
    if(!rf)
        cout << "Cannot open file!" << endl;

    Memory mem(&rf);
    Instr32raw buf = 0;

    Hart cpu(&mem);
    cpu.run();

    rf.close();

    return 0;
}
