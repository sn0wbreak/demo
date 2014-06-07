#include <boot.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct boot_args {
    uint16_t        Revision;
    uint16_t        Version;
    uint32_t        virtBase;
    uint32_t        physBase;
    uint32_t        memSize;
    uint32_t        topOfKernelData;
    Boot_Video      Video;
    uint32_t        machineType;
    void            *deviceTreeP;
    uint32_t        deviceTreeLength;
    char            CommandLine[BOOT_LINE_LENGTH];
} boot_args;


#define PAGE_SIZE_64 (0x4000)
#define PAGE_SIZE_32 4096

int main() {

char *top_kdata->topofKernelData;
char *tte_base_32 = top_kdata + 4 * 2 * PAGE_SIZE_32;
char *tte_base_64 = top_kdata + 4 * 2 * PAGE_SIZE_64;

printf("tte base for arm32: 0x8%x",
       "tte base for arm64; 0x8%x",
        tte_base_32, tte_base_64   );
        disable_irq();
__asm( "mcr    p15, 0, r0, c7, c5, 0",
       "mcr    p15, 0, r0, c7, c5, 6",    
       "mcr    p15, 0, r0, c8, c7, 0", );

return 0;

}
