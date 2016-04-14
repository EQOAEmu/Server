/**
 * These are the bytes we have identified from the packet analysis work
 *
 */

#ifndef OPCODES_H
#define OPCODES_H_

#define OPCODE 5
#define OP_OPTION 6

#define E_CODE  224               // [E0]
#define SESSION_REQUEST 33        // [E0 21]
#define SESSION_CLOSE   20        // [E0 14]
#define SERVER_COMPLEX_REQUEST 1  // [E0 01]
#define CHARACTER_REQUEST 112     // [70]

#endif
