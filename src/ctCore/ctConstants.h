#ifndef  CTCONSTANTS_H
#define  CTCONSTANTS_H

const int CT_MAX_PACKET_SIZE = 255;

const int CT_N_SERVER_THREADS = 1;
const int CT_N_CLIENT_THREADS = 1;

//Defines protocol numbers
#define CT_START 'b'
#define CT_CHECK 'c'
#define CT_STATE 's'
#define CT_PAUSE 1
#define CT_END 2

#endif   // CTCONSTANTS_H
