#ifndef NDSERIAL_H
#define NDSERIAL_H

#include <sys/ioctl.h>
#include <linux/serial.h>
#include <termios.h>

#include <QObject>

class ndSerial : public QObject
{
    Q_OBJECT

public:
    explicit ndSerial(std::string name, QObject *parent = nullptr);

    int Open();
    int ConfigureTermios(int speed = 115200, int parity = 0);

    int Write(const char* buf, int len);
    int Read(unsigned char* buf, unsigned int len, unsigned int timeout = 500/*milliseconds*/);

    void FlushCache();
    void SetLowLatency();
    void Close();

    int SetBlockMode(bool is_block);

signals:


private:
    int _fd;
    bool _is_opened;
    bool _is_blocked_mode;

    std::string _name;

};

#endif // NDSERIAL_H
