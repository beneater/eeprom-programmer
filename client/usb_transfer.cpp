#ifndef USB_TRANSFER_H
#define USB_TRANSFER_H

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <termios.h>
#include <getopt.h>

class USBTransfer {
public:
    USBTransfer(const char* device_path);
    ~USBTransfer();

    bool open_device();
    bool send_command(const char* command);
    bool send_file(const char* file_path);
    void close_device();

private:
    const char* device_path;
    int usb_fd;
    struct termios tty;

    bool configure_device();
};

#endif // USB_TRANSFER_H
