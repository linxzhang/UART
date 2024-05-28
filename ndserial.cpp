#include "ndserial.h"

#include <iostream>
#include <unistd.h>    // write(), read(), close()
#include  <iomanip>
#include <chrono>
#include <termios.h>   // Contains POSIX terminal control definitions
#include <errno.h>     // Error integer and strerror() function
#include <fcntl.h>     // Contains file controls like O_RDWR
#include <time.h>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/serial.h>


ndSerial::ndSerial(std::string name, QObject *parent) : QObject(parent)
{
    _name = name;
    _fd              = -1;
    _is_opened       = false;
    _is_blocked_mode = false;

}

int ndSerial::Open()
{
    int err = 0;

    if (_is_opened) {
        return -1;
    }

    if (_name.empty()) {
        return -1;
    }

    try {
        _fd = open(_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NDELAY);
        if (_fd < 0)
        {
            std::cout << "error " << errno << " opening " << _name << " " <<  strerror(errno) << std::endl;
            return -1;
        }
        _is_opened = true;
    }
    catch(std::exception &e) {
        std::cout << "Open UART " << _name << " failed!" << std::endl;
        err = -1;
    }

    return err;
}

int ndSerial::ConfigureTermios(int speed, int parity)
{
    struct termios tty;
    if (tcgetattr (_fd, &tty) != 0) // Read in existing settings, and handle any error
    {
        std::cout << "error " << errno << " from tcgetattr" << std::endl;
        return -1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;

    cfmakeraw(&tty); // Set raw mode

    /**
     * TODO: add code here to set the custom baud rate
     */
#if 0
    tty.c_cflag &= ~CBAUD; // Remove current baud rate setting
    tty.c_cflag |= BOTHER; // Specify Other Baud Rate
    tty.c_ispeed = speed; // Input speed
    tty.c_ospeed = speed; // Output speed

    // Set the rest of your configuration as before...

    if (ioctl(_fd, TCSETS2, &tty) != 0)
    {
        std::cout << "error " << errno << " from ioctl(TCSETS2)" << std::endl;
        return -1;
    }
#endif
    if (0 != speed) {  // speed = 0 is custom baud rate, config by stty.
        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);
    }

    if (tcsetattr(_fd, TCSANOW, &tty) != 0)
    {
        std::cout << "error %d from tcsetattr" << errno << std::endl;
        return -1;
    }

    return 0;
}

void ndSerial::SetLowLatency()
{
    struct serial_struct serial;

    ioctl(_fd, TIOCGSERIAL, &serial);
    serial.flags |= ASYNC_LOW_LATENCY; // (0x2000)
    ioctl(_fd, TIOCSSERIAL, &serial);
}


int ndSerial::SetBlockMode(bool is_block)
{
    int err = 0;
    if (!_is_opened) {
        std::cout << "please call open first" << std::endl;
        return -1;
    }

    struct termios tty;
    std::memset (&tty, 0, sizeof(tty));
    if (tcgetattr (_fd, &tty) != 0)
    {
        std::cout << "error %d from tggetattr"  << errno << std::endl;
        return -1;
    }

    tty.c_cc[VMIN]  = is_block ? 1 : 0;
    if (tcsetattr (_fd, TCSANOW, &tty) != 0) {
        std::cout << "error %d setting term attributes" << errno << std::endl;
        err = -1;
    }

    _is_blocked_mode = is_block;

    return err;
}

int ndSerial::Write(const char* buf, int len)
{
    if (!_is_opened) {
        std::cout << "please call open first" << std::endl;
        return 0;
    }

    return write(_fd, buf, len);
}

int ndSerial::Read(unsigned char* buf, unsigned int len, unsigned int timeout)
{
    unsigned int n = 0;
    unsigned int millseconds = 0;
    auto start = std::chrono::high_resolution_clock::now();    // just for initialize

    /**
     * First, check UART device was opened or not.
     */
    if (!_is_opened) {
        std::cout << "please call open first" << std::endl;
        return 0;
    }

    if (timeout > 0) {
        start = std::chrono::high_resolution_clock::now();
    }
    unsigned int readbytes = 0;
    do {
        readbytes = read(_fd, &buf[n], len);
        if (readbytes > 0) {
            n += readbytes;
        }

        //
        // read time out
        //
        if (timeout > 0) {
            usleep(1); // wait 1 microseconds here.
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            // seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
            millseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

        }
    } while(n < len && millseconds < timeout);

    return n;
}

void ndSerial::FlushCache()
{
    if (!_is_opened) {
        std::cout << "please call open first" << std::endl;
        return ;
    }

    tcflush(_fd, TCIOFLUSH);

    return;
}

void ndSerial::Close()
{
    if (!_is_opened) {
        std::cout << "please call open first" << std::endl;
        return ;
    }

    close(_fd);
    _is_opened = false;
}


