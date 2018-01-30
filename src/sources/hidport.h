#ifndef QTDMM_HIDPORT_H
#define QTDMM_HIDPORT_H

#include <hidapi/hidapi.h>
#include <QObject>
#include <QThread>


class HIDPort: public QObject {
    Q_OBJECT
public:

    HIDPort(QObject *parent = Q_NULLPTR, const QString & path = QString());
    ~HIDPort();

    qint64 bytesAvailable() {
        int ret = (m_buffer_w + m_buflen - m_buffer_r) % m_buflen;
        // printf("HIDPort::bytesAvailable() -> %d\n", ret);

        return ret;
    }

    static QStringList availablePorts();

    void close();
    bool isOpen() {
        return m_isOpen;
    }

    qint64 read(char *data, qint64 maxSize) {
        // printf("HIDPort::read(buf, %d)\n", maxSize);
        if (m_isOpen) {
            unsigned int len = 0;

            while (maxSize > 0 && bytesAvailable() > 0) {
                data[len] = m_buffer[m_buffer_r];
                len++;
                maxSize--;
                m_buffer_r = (m_buffer_r + 1) % m_buflen;
            }

            return len;
        }
        else {
            return -1;
        }
    }

Q_SIGNALS:
    void readyRead();
    void aboutToClose();
    void finished();

public Q_SLOTS:
    void run();

private:
    static const unsigned int m_buflen = 1024;
    volatile bool m_isOpen = false;
    hid_device *m_handle = Q_NULLPTR;
    unsigned int m_buffer_r = 0;
    unsigned int m_buffer_w = 0;
    unsigned char m_buffer[m_buflen];
};

#endif
