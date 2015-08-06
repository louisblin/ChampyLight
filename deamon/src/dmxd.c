// ==========================================================================
// Velleman K8062 DMX controller Deamon for VM116/K8062
// ==========================================================================
//
// Modified from code from Denis Moreaux 2008 ( <vapula@endor.be> )
//
//
// This program should be run as a background process and continously updates
// a shared memory segment created by the application to control DMX channels
// sent through the DMX controller. The DMX channels can be accessed through
// a shared memory block that is allocated as:
//
// 0     = max # of channels to send  ( 0 - 512 )
// 1     = exit deamon control flag   ( 0 = run, 1 = exit )
// 2-514 = dmx channel data
//
// ==========================================================================
//
// Prerequisites ( USB lib ):
//   sudo apt-get install libusb-dev
//
//
// ==========================================================================
//

#include <usb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "../../values_handler/src/constants.h"

// dmx data and control registers
typedef unsigned char ubyte;

uint16_t  * maxChanAddr; // control register for # of channels to update
ubyte     * exitAddr;    // control register to exit deamon
ubyte     * chanData;    // 512 byte array of channel data


ubyte     *shm;          // shared memory segment containing data & ctrl regs
int       shmid;         // handel to shared memory segment


// constants and defs

#define ProgName "dmxd"  // name of this program
#define VendorID 0x10cf  // K8062 USB vendor ID
#define ProdID   0x8062  // K8062 USB product ID

#define UpdateInt 100000 // update interval ( microseconds )
#define DefMaxChans   16 // default number of maximum channels

// internal structures

struct usb_bus *bus;    // pointer to the USB bus
struct usb_device *dev; // pointer to the K8062 USB device
usb_dev_handle *udev;   // access handle to the K8062 device


// function delcarations

int  main();

int sendDMX();


int  initUSB();
int  writeUSB ( ubyte *data , int numBytes );
void exitUSB();

int  initSHM();
void exitSHM();

void timediff ( struct timeval *res, struct timeval *a, struct timeval *b );
void timeadd  ( struct timeval *res, struct timeval *a, struct timeval *b );


// ==========================================================================
// main -- dmx deamon
// ==========================================================================

int main() {
    
    struct timeval now,next,diff,delay;
    int success;
    
    printf("%s: Starting dmx deamon\n", ProgName);

    // intialize USB device
    
    success = initUSB();
    
    if ( !success ) {
        printf ( "%s: Error initializing USB interface\n" , ProgName );
        (*exitAddr)++;
        return ( -1 );
    }
    
    // initialize shared memory segment
    
    success = initSHM();
    
    if ( !success  ) {
        printf ( "%s: Error initializing shared memory\n" , ProgName );
        (*exitAddr)++;
        return ( -2 );
    }
    
    
    // start timer
    
    delay.tv_sec = 0;
    delay.tv_usec= UpdateInt;
    
    gettimeofday ( &next , NULL );
    
    printf("%s: Init done. Entering loop cycle...\n", ProgName);
    
    // loop until commanded to shutdown
    
    while( !*exitAddr ) {
        
        
        // send DMX data
       
        success = sendDMX();
        
        if ( !success ) {
            printf  ( "%s: DMX send error\n" , ProgName );
            (*exitAddr)++;
        }
        
        // wait for update interval

        timeadd ( &next , &next , &delay );
        gettimeofday ( &now , NULL );
        timediff ( &diff, &next , &now );
        
        while (diff.tv_sec || diff.tv_usec) {
            
            select ( 0, NULL, NULL, NULL, &diff );
            gettimeofday ( &now, NULL );
            timediff ( &diff, &next, &now );
        };
        
    }
    
    printf ( "%s: dmx deamon is shutting down\n" , ProgName );
    
    // on shutdown reset all DMX channels
    
    memset ( chanData , 0, 512 * sizeof (ubyte) );
    
    sendDMX();
    
    
    // exit the system
    
    exitUSB();
    exitSHM();
    
    return ( 0 );
}


// ==========================================================================
// sendDMX -- send current DMX data
// ==========================================================================

int sendDMX ()
{
    ubyte data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int numChans = *maxChanAddr;
    
    #ifdef VERBOSE
    printf("\n\nnumChans is %d", numChans); 
    #endif
    

    #if 1
    
    // |=== START PACKET
    // find out how many consecutive zeroes are in the data - the start
    // packet can indicate this to avoid sending a bunch of leading
    // zeroes
    
    int curChanIdx = 0;
    
    for ( curChanIdx = 0; curChanIdx < numChans; curChanIdx++ ) {
        if ( chanData[ curChanIdx ] != 0 ) break;
    }
    
    // build starting packet. this packet specifies how many channels have
    // zero data from the start and then contains the next 6 channels of
    // data
    
    data[0] = 4;                          // start packet header (4)
    data[1] = curChanIdx + 1;                 // number of zeroes ( not sent )
    
    data[2] = chanData [ curChanIdx++ ];  // first ( non-zero ) chan data
    data[3] = chanData [ curChanIdx++ ];  // next chan data
    data[4] = chanData [ curChanIdx++ ];  // next chan data
    data[5] = chanData [ curChanIdx++ ];  // next chan data
    data[6] = chanData [ curChanIdx++ ];  // next chan data
    data[7] = chanData [ curChanIdx++ ];  // next chan data
    
    #ifdef VERBOSE    
    printf("\nWriting start packet... data[code.no.1..6] = ");
    int i = 0;
    while (i < 8) {
       printf("-%d", data[i++]);
    }
    #endif

    int success = writeUSB ( data , 8 );
    
    if ( !success ) {
        printf ( "%s: Error sending DMX start packet\n" , ProgName );
        return ( 0 );
    }
    
    // Exit if only all data is already in starting header
    if ( curChanIdx >= numChans ) return ( 1 );
   
    
    // |=== BULK PACKETS
    // after the first packet additional packets are sent that contain seven
    // channels each up to 512.
    
    while ( curChanIdx < ( numChans - 6 ) ) {
    //while ( curChanIdx < numChans ) {
    
        data[0] = 2;                          // start packet header (2)
        data[1] = chanData [ curChanIdx++ ];  // next chan data
        data[2] = chanData [ curChanIdx++ ];  // next chan data
        data[3] = chanData [ curChanIdx++ ];  // next chan data
        data[4] = chanData [ curChanIdx++ ];  // next chan data
        data[5] = chanData [ curChanIdx++ ];  // next chan data
        data[6] = chanData [ curChanIdx++ ];  // next chan data
        data[7] = chanData [ curChanIdx++ ];  // next chan data ADDED
        
    //}
    
        #ifdef VERBOSE    
        printf("\nWriting bulk packet... data[code-%d..%d] = ", curChanIdx - 6, curChanIdx);
        i = 0;
        while (i < 8) {
           printf("-%d", data[i++]);
        }
        #endif
    
        success = writeUSB ( data , 8 );
        
        if ( !success ) {
            printf ( "%s: Error sending DMX bulk packet\n" , ProgName );
            return ( 0 );
        }
    
    }
    // If sent packets out of range, exit failure
    //if ( curChanIdx >= numChans ) return ( 0 );
    
    // |=== END PACKET
    data[0] = 3;                               // start packet header (3)
    
    int idx = 1;
    while ( curChanIdx < numChans ) {
    
        data[++idx] = chanData[curChanIdx++];  // next chan data    
    }
    data[1] = --idx;                             // Number of remaining packets
    
    #ifdef VERBOSE    
    printf("\nWriting end packet... data[code-no-%d..%d] = ", curChanIdx - idx + 1, curChanIdx);
    i = 0;
    while (i < 8) {
       printf("-%d", data[i++]);
    }
    printf("\n");
    #endif
    
    success = writeUSB ( data , 8 );
    
    if ( !success ) {
        printf ( "%s: Error sending DMX bulk packet\n" , ProgName );
        return ( 0 );
    }
    
    #else
    
    data[0] = 5;   // packet header for single channeld data
    
    #ifdef VERBOSE
    printf ( "Sending %d channels\n" , numChans );
    #endif 
    
    for ( int chIdx = 0; chIdx < numChans; chIdx++ )
    {
        data[1] = chanData [ chIdx ];
        
        int success = writeUSB ( data , 8 );
        
        if ( !success ) {
            printf ( "%s: Error sending DMX data packet\n" , ProgName );
            return ( 0 );
        }
    }
    
    #endif
    
    
    return ( 1 );
    
}

// ==========================================================================
// initUSB -- intialize the USB interface for the device
// ==========================================================================

int initUSB()
{
    int success;
    
    
    // open the usb library
    
    usb_init();
    
    
    // find the usb device for DMX controller
    
    usb_find_busses();
    usb_find_devices();
    
    struct usb_device_descriptor *descr = 0x0;
    
    for ( bus = usb_busses; bus; bus = bus -> next ) {
        
        for ( dev = bus->devices; dev; dev = dev -> next ) {
            
            printf ( "%s: Checking device [%s]\n" , ProgName , dev -> filename );

            descr = & dev->descriptor;
            
            if (      ( descr -> idVendor == VendorID )
            && ( descr -> idProduct == ProdID  ) ) break;
        }
    }
    
    if ( !dev ) {
        printf ( "%s: DMX device not found on USB\n" , ProgName );
        return ( 0 );
    }
    
    
    // open the device
    
    printf ( "%s: Opening device [%s] ... " , ProgName , dev -> filename );

    udev = usb_open ( dev );
    
    if ( udev == 0x0 ) {
        printf ( "%s: Error opening device\n" , ProgName );
        return ( 0 );
    }
    else {
        printf ( "Ok\n" );
    }
    
    
    // claim the interface
    
    
    #if defined(LIBUSB_HAS_GET_DRIVER_NP) \
        && defined(LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP)
    
    usb_detach_kernel_driver_np( udev, 0);
    
    #endif
    
    
    // set configuration
    
    
    usb_set_debug(4);
    
    success = usb_set_configuration ( udev, 1 );
    
    if ( success != 0 ) {
        printf ( "%s: Configuration error [%d]\n" , ProgName , success );
        return ( 0 );
        
    }
    
    
    // claim the interface
    
    success = usb_claim_interface ( udev, 0 );
    
    if ( success != 0 ) {
        
        printf ("Error claiming interface [%d]\n" , success );
        return ( 0 );
    }
    
    return ( 1 );
}

// ==========================================================================
// writeUSB -- write a command to the USB interface
// ==========================================================================

int writeUSB ( ubyte *data , int numBytes )
{
    int nSent;
    
    //  printf ( "%s: writing [%d] bytes " , ProgName , numBytes );
    //  for ( int b = 0; b < numBytes; b++ ) printf ( "[%d]" , data[b] );
    //  printf ( "n" );
    
    // write the data
    
    nSent = usb_interrupt_write (udev, 1, (char *) data, numBytes, 200);
    
    if ( nSent != numBytes ) {
        
        printf ( "Error writing [%d] bytes [%d]\n" , numBytes , nSent );
        return ( 0 );
    }
    
    return ( 1 );
    
}


// ==========================================================================
// exitUSB -- terminate USB connection
// ==========================================================================

void exitUSB()
{
    usb_close(udev);
}

// ==========================================================================
// initSHM -- initialize shared memory segment
// ==========================================================================

int initSHM()
{
    
    printf ( "%s: Creating shared memory segment ... " , ProgName );
    
    // create the shared memory segment
    
    shmid = shmget ( 0x56444D58 , sizeof ( ubyte ) * 515 , IPC_CREAT | 0666 );
    
    if ( shmid == -1 ) {
        printf ( "Error creating shared memory segment [%d]\n" , errno );
        return ( 0 );
    }
    else {
        printf ( "Ok\n" );
    }
    
    
    // attach to segment and initialize
    
    printf ( "%s: Initializing segment [0x%x] ... " , ProgName , shmid );

    shm = ( ubyte * ) shmat ( shmid , NULL , 0 );
    
    if ( shm == 0x0 ) {
        printf ( "Error connecting to segment [%d]\n" , errno );
        return ( 0 );
    }
    else {
        printf ( "Ok\n" );
    }
    
    memset ( shm , 0 , sizeof ( ubyte ) * 515 );
    
    // set up command & data registers
    
    maxChanAddr  = ( uint16_t * ) shm;
    *maxChanAddr = DefMaxChans;
    
    exitAddr     = ( ubyte * ) maxChanAddr + 2;
    chanData     = ( ubyte * ) maxChanAddr + 3;
    
    return ( 1 );
}

// ==========================================================================
// exitSHM -- terminate shared memory segment
// ==========================================================================

void exitSHM()
{
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL);
}

// ==========================================================================
// timediff | timeadd -- timing functions
// ==========================================================================

void timediff ( struct timeval *res, struct timeval *a, struct timeval *b)
{
    long sec,usec;
    sec=a->tv_sec-b->tv_sec;
    usec=a->tv_usec-b->tv_usec;
    
    while (usec<0) {
        usec+=1000000;
        sec--;
    }
    if (sec<0) {
        res->tv_sec=0;
        res->tv_usec=0;
        } else {
        res->tv_sec=sec;
        res->tv_usec=usec;
    }
}

void timeadd(struct timeval *res, struct timeval *a, struct timeval *b)
{
    res->tv_usec=a->tv_usec+b->tv_usec;
    res->tv_sec=a->tv_sec+b->tv_sec;
    while (res->tv_usec >= 1000000) {
        res->tv_usec-=1000000;
        res->tv_sec++;
    }
}
