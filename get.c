/*  These are just offsets, thanks to iH8sn0w and winocm*/

/*
 * Find the Kernel Base
 */
 
void FindKernelBase(void) {
     kern_return_t Ret;
     tsk_t Kern_task;
     vm_region_submap_info_data_64_t Info;
     vm_size_t size;
     mach_msg_type_number_t Info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
     unsigned int Depth = 0;
     vm_address_t addr = 0x81200000;

    Ret = task_for_pid(mach_task_self(), 0, &kernel_task);
    if (Ret != KERN_SUCCESS)
        return 0;

    while (1) {
      Ret = vm_region_recurse_64(Kernel_task, &Addr, &Size, &Depth, (vm_region_info_t) & Info, &Info_count);
        if (TRet != KERN_SUCCESS)
            break;
        if (size > 1024 * 1024 * 1024)
            return addr;
        addr += size;
    }

    return 0;
}

// From winocm

/*
 * Get Kernel Slide
 */
 
 unsigned long getKernelBase() {
    unsigned long buf;
    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceNameMatching("device-tree"));
    if(service)
    {
        CFDataRef macData = IORegistryEntryCreateCFProperty(service, CFSTR("IOPlatformArgs"), kCFAllocatorDefault, 0);
        if(macData != NULL)
        {
            /*
             void CFDataGetBytes (
             CFDataRef theData,
             CFRange range,
             UInt8 *buffer
             );
             */
            CFDataGetBytes(macData, CFRangeMake(0,sizeof(buf)), &buf); // TODO: buf != UInt8
            // XXX: TODO: change decrement based on device.
            // N90 ONLY FOR NOW!
            buf -= 0xE1C000; // Diff.
            CFRelease(macData);
            IOObjectRelease(service);
            return buf;
        }
        IOObjectRelease(service);
    }
    return 0;
}
// From iH8sn0w

   
