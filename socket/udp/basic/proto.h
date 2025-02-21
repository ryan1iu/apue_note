#ifndef PROTO_H__
#define PROTO_H__
#include <stdint.h>
#define NAMEMAXSIZE (512 - 8 - 8) // Set the memory size pointed to by the name pointer to the recommended size of a
                                  // UDP packet - the size of other data members in the structure.

// Define the communication message format between the sender and the receiver.
struct msg_st{
    uint32_t chinese; // Beacuse of the byte size of `int` may vary on different machines, it's best to define variables using explicitly
                      // sized data type.
    uint32_t math;
    char name[]; // variable-length struct
}__attribute__((packed));

#endif
