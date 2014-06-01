/*  These are just offsets, thanks to iH8sn0w and winocm*/


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

   
