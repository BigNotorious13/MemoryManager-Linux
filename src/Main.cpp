#include "Memory.hpp"
#include <cstdint>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    //check if it's running as root
    if(getuid() != 0) {
        printf("You should run this as root\n");
        return -1;
    }

    //How to get pid and open process
    Memory::Handle process;
    while(true) {
        if(Memory::FindProcessByName("process", &process)) {
            break;
        }
        usleep(500);
    }
    printf("Process found!\n");
    printf("Process location [%s]", process.GetPath().c_str());

    //How to get module (.so)
    Memory::MapModuleMemoryRegion module;
    module.start = 0;
    while(module.start == 0) {
        if(process.IsRunning()){
            process.ParseMaps();
            for (auto region : process.regions) {
            if(region.filename.compare("moduleName.so") == 0 && region.executable) {
                printf(".so: [%li][%li][%s]\n", region.start, region.end, region.pathname.c_str());
                module = region;
                break;
            }
        }
        usleep(500);
        }
    }
    //How to read multilevel pointers
    const std::vector<unsigned int> offsets = {
        0x0,
        0x28,
        0x38,
        0x98,
        0x170,
        0x0,
        0x1F4
    };
    uintptr_t base = 0xDEADBEEF;
    uintptr_t pointer = Memory::FindDMAAddy(process, base, offsets);

    //Run main thread
    while(process.IsRunning()) {
        //code 

        
        usleep(500);
    }
}