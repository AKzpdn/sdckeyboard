#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "hidapi.h"
#include <linux/uinput.h>

#include "sdckeyboard.h"

#define MAX_STR 1024  // for manufacturer, product strings
#define MAX_BUF 1024  // for buf reads & writes


// normally this is obtained from git tags and filled out by the Makefile
#ifndef SDCKEYBOARD_VERSION
#define SDCKEYBOARD_VERSION "v0.0"
#endif

bool msg_quiet = false;
bool msg_verbose = true;

int print_base = 16; // 16 or 10, hex or decimal
int print_width = 32; // how many characters per line

int uinput_dev;

uint64_t but_modifiers=BUT_MODIFIERS;
uint64_t but_significant=BUT_SIGNIFICANT;

/**
 * printf that can be shut up
 */
void msg(char* fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    if(!msg_quiet) { vprintf(fmt,args); }
    va_end(args);
}
/**
 * printf that is wordy
 */
void msginfo(char* fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    if(msg_verbose) { vprintf(fmt,args); }
    va_end(args);
}

/**
 * print out a buffer of len bufsize in decimal or hex form
 */
void printbuf(uint8_t* buf, int bufsize, int base, int width)
{
    for( int i=0 ; i<bufsize; i++) {
        if( base==10 ) {
            printf(" %3d", buf[i]);
        } else if( base==16 ) {
            printf(" %02X", buf[i] );
        }
       // if (i % 16 == 15 && i < bufsize-1) printf("\n");
       //if (i % width == width-1 && i < bufsize-1) printf("\n");
    }
    printf("\r");
}


volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}


void key_emit(int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(uinput_dev, &ie, sizeof(ie));
}

void key_setup(){
    struct uinput_setup usetup;

    uinput_dev = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    /*
    * The ioctls below will enable the device that is about to be
    * created, to pass key events, in this case the space key.
    */
    ioctl(uinput_dev, UI_SET_EVBIT, EV_KEY);
    for (uint8_t i=0;i<CHORDS_COUNT;i++){
        ioctl(uinput_dev, UI_SET_KEYBIT, chords[i].keycode);
    }
    for (uint8_t i=0;i<MODS_COUNT;i++){
        ioctl(uinput_dev, UI_SET_KEYBIT, mods[i].keycode);
    }

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0xDEAD; /* sample vendor */
    usetup.id.product = 0xBEEF; /* sample product */
    strcpy(usetup.name, "Chorded Virtual Keyboard");

    ioctl(uinput_dev, UI_DEV_SETUP, &usetup);
    ioctl(uinput_dev, UI_DEV_CREATE);

    /*
        * On UI_DEV_CREATE the kernel will create the device node for this
        * device. We are inserting a pause here so that userspace has time
        * to detect, initialize the new device, and can start listening to
        * the event, otherwise it will not notice the event we are about
        * to send. This pause is only needed in our example code!
        */
    //sleep(1);
}

void key_unsetup(){
    ioctl(uinput_dev, UI_DEV_DESTROY);
    close(uinput_dev);
}

void key_event(int code, int val){
    key_emit(EV_KEY, code, val);
    key_emit(EV_SYN, SYN_REPORT, 0);
}

void find_and_send_chord(uint64_t buttons, int state){
    if (buttons==0)
        return;
    for (uint8_t i=0;i<CHORDS_COUNT;i++){
        if (chords[i].chord == buttons){
            key_event(chords[i].keycode,state);
            break;
        }
    }
}


void find_and_send_mod(uint64_t buttons, uint64_t old_buttons){
    for (uint8_t i=0;i<MODS_COUNT;i++){
        if ((mods[i].chord & buttons)>0 && (mods[i].chord & old_buttons)==0){
            //printf("%016llX:1\n",mods[i].chord);
            key_event(mods[i].keycode,1);
        }else if ((mods[i].chord & buttons)==0 && (mods[i].chord & old_buttons)>0){
            //printf("%016llX:0\n",mods[i].chord);
            key_event(mods[i].keycode,0);
        }
    }
}

/**
 *
 */
int main(int argc, char* argv[])
{
    signal(SIGINT, inthand);
    uint8_t buf[MAX_BUF];   // data buffer for send/recv
    hid_device *dev = NULL; // HIDAPI device we will open
    int res;
    int i;
    int buflen = 64;        // length of buf in use
    int timeout_millis = 50;

    uint16_t vid = VID;        // vendorId
    uint16_t pid = PID;        // productId
    uint16_t usage_page = USGP; // usagePage to search for, if any
    uint16_t usage = USG;      // usage to search for, if any
    char devpath[MAX_STR];   // path to open, if filter by usage

    key_setup();

    setbuf(stdout, NULL);  // turn off buffering of stdout

    msg("Opening device, vid/pid:0x%04X/0x%04X, usagePage/usage: %X/%X\n",
        vid,pid,usage_page,usage);

    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(vid, pid); // 0,0 = find all devices
    if (!devs) {
        msg("Error: no HID devices found for given vid/pid\n");
        hid_exit();
        return 1;
    }
    cur_dev = devs;
    while (cur_dev) {
        msginfo("Device found: path: %s, vid: 0x%04X, pid: 0x%04X, usage_page: 0x%04X, usage: 0x%04X\n",
            cur_dev->path,
            cur_dev->vendor_id,
            cur_dev->product_id,
            cur_dev->usage_page,
            cur_dev->usage);

        if( (!vid || cur_dev->vendor_id == vid) &&
            (!pid || cur_dev->product_id == pid) &&
            (!usage_page || cur_dev->usage_page == usage_page) &&
            (!usage || cur_dev->usage == usage) ) {
                strncpy(devpath, cur_dev->path, MAX_STR); // save it!
        }
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    if( devpath[0] ) {
        msginfo("Opening device by path: %s\n",devpath);
        hid_device *handle = hid_open_path(devpath);
        if (!handle) {
            msg("Error: could not open device at path: %s\n",devpath);
            msg("Error: %ls\n", hid_error(handle));
            hid_exit();
            return 1;
        }
        dev = handle;
        msg("Device opened\n");
    } else {
        msg("Error: no matching devices\n");
    }



    if( !dev ) {
        msg("Error on read: no device opened.\n"); return -1;
    }
    if( !buflen) {
        msg("Error on read: buffer length is 0. Use --len to specify.\n"); return -2;
    }
    msg("Reading up to %d-byte input report, %d msec timeout...\n",
            buflen, timeout_millis);
    uint8_t but_pressed=0, press_sent=0;
    uint16_t skip_count=0;
    uint64_t last_btn=0, last_mod=0;
    do {
        res = hid_read_timeout(dev, buf, buflen, timeout_millis);
        if( res > 0 ) {
            uint64_t btn = *((uint64_t *)&(buf[7]));
            uint64_t mod = btn & BUT_MODIFIERS;
            if (mod!=last_mod){
                find_and_send_mod(mod,last_mod);
                last_mod=mod;
            }
            btn = btn & BUT_SIGNIFICANT;
            if (btn==0 && but_pressed){
                but_pressed=0;
                if(last_btn!=0 && !press_sent){
                    press_sent=1;
                    find_and_send_chord(last_btn, 1);
                }
                if (press_sent){
                    //printf("\nCLEAR\n");
                    find_and_send_chord(last_btn, 0);
                }
                press_sent=0;
                last_btn=0;
            }else if (btn!=last_btn && !press_sent){
                if (last_btn<btn){
                    but_pressed=1;
                    printf("%016llX\n",btn);
                    last_btn=btn;
                    skip_count=0;
                }
            }else if (++skip_count>=SKIP_COUNT && btn !=0 && last_btn==btn){
                press_sent=1;
                find_and_send_chord(btn, 1);
            }
            memset(buf,0,buflen);  // clear it out
            usleep(TIMEOUT);
        }
        else if( res == -1 )  { // removed device

            break;
        }
    } while( !stop );

    printf("\n");
    msg("Closing device\n");
    if(dev) {
        hid_close(dev);
        dev = NULL;
    }
    key_unsetup();
    return 0;
} // main
