SO_BROADCAST
Set or get the broadcast flag. When enabled, datagram sockets are allowed to send packets to a broadcast address. This op‐
tion has no effect on stream-oriented sockets.

Enable a socket to send broadcasts to the subnet by setting the SO_BROADCAST flag.

```c
#include <sys/socket.h>
       int setsockopt(int socket, int level, int option_name,
           const void *option_value, socklen_t option_len);
/*The level argument specifies the protocol level at which the option resides. To set options at the socket level, specify  the  level
  argument as SOL_SOCKET. To set options at other levels, supply the appropriate level identifier for the protocol controlling the op‐
  tion.  For  example,  to indicate that an option is interpreted by the TCP (Transport Control Protocol), set level to IPPROTO_TCP as
  defined in the <netinet/in.h> header. */

```
