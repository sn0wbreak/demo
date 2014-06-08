#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <mach/mach_types.h>
#include <mach-o/loader.h>

// WEEEEE I DON"T KNOW WHAT I"M DOING

static inline void patch_with_range(const char *name, addr_t addr, prange_t pr) {
    uint32_t len = strlen(name);
    write(patchfd, &len, sizeof(len));
    write(patchfd, name, len);
    write(patchfd, &addr, sizeof(addr));
    uint32_t size = pr.size; // size_t no good
    write(patchfd, &size, sizeof(size));
    write(patchfd, pr.start, pr.size);
}

#define patch(name, addr, typeof_to, to...) \
    ({ typeof_to to_[] = to; \
       patch_with_range(name, addr, (prange_t) {&to_[0], sizeof(to_)}); })

char vm_map_enter_text[] = {0x06, 0x28, 0x04, 0xbf, 0x19, 0x98, 0x00, 0x28};
char AMFI_text[] = {0xd0, 0x47, 0x01, 0x21, 0x40, 0xb1, 0x13, 0x35};
char PE_I_CAN_HAS_DEBUGGER_enabled[] = {0x09, 0x68, 0x01, 0x60, 0x01, 0x48, 0x00, 0x68, 0x70, 0x47};
char task_for_pid_0_text[] = {{0x01, 0x91, 0x06, 0xd1, 0x02, 0xa8, 0x41, 0x46};
char cs_enforcement_disable_text[] = {0xa2, 0x6a, 0x1b, 0x68, 0x00, 0x2b, 0x04, 0xbf};
char SandboxPatch_text[] = {0x00, 0x78, 0x10, 0xf0, 0x04, 0x0f, 0x04, 0xd0};

typedef struct {
    const char* name;
    const uint8_t* patch;
    uint32_t size;
} patches;

pathes kpatch[] = {
{"K_VM_MAP_ENTER, vm_map_enter_text, sizeof(vm_map_enter_text)},
{"K_AMFI", AMFI_textm sizeof(AMFI_text)},
{"K_PE_I_CAN_HAS_DEBUGGER", PE_I_CAN_HAS_DEBUGGER_enabled, sizeof(PE_I_CAN_HAS_DEBUGGER_enabled)},
{"K_TFP0", task_for_pid_0_text, sizeof(task_for_pid_0_text)},
{"K_CS_ENFORC_DISABLE", cs_enforcement_disable_text, sizeof(cs_enforcement_disable_text)},
{"K_SANDBOX_PATCHES", SandboxPatch_text, sizeof(SandboxPatch_text)},
{NULL, NULL},
                  };

#define VM_ADDR 0x80001000   /* Kernel ASLR is added in iOS 6 and higher, non of these patches will work, 
                                If KASLR is still present. I am just pointing that out. */

void patch_all_kernel_protections(void) {

/* vm_map_enter */
char *vme = patch(&kpatch.name[0], &kpatch.patch[0], &kpatch.size[0]);
/* AppleMobileFileIntegrity */
char *amfi = patch(&kpatch.name[1], &kpatch.patch[1], &kpatch.size[1]);
/* PE_i_can_has_debugger */
char *pe_debug = patch(&kpatch.name[2], &kpatch.patch[2], &kpatch.size[2]);
/* Task For Pid 0 */
char *tfp0 = patch(&kpatch.name[3], &kpatch.patch[3], &kpatch.size[3]);
/* CodeSigning Enforcement Disable */
char *csd = patch(&kpatch.name[4], &kpatch.patch[4], &kpatch.size[4]);
/* Sandbox Patches */
char *sdp = patch(&kpatch.name[5], &kpatch.patch[5], &kpatch.size[5]);
/* if the patch doesn't exist */
if(patch(&kpatch.name[6], &kpatch.patch[6], &kpatch.size[6]))
   syslog("error: invalid kernel patch\n");

/* print patches */

syslog("vm_map_enter",
       "AppleMobileFileIntegrity: %s"
       "PE_i_can_has_debugger: %s"
       "Task For Pid 0: %s"
       "CodeSigning Enforcement Disable: %s"
       "Sandbox Patches: %s", vme, amfi, pe_debug, tfp0, csd, sdp);
}


int main(int argc, char **argv) {
     syslog("Printing Patches");  
     sleep(10.0);
     patch_all_kernel_protections();
     sleep(10.0);
     syslog("Done.");

     return 0;
}
