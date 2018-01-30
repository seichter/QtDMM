#ifndef PORTHANDLE_HH
#define PORTHANDLE_HH

#include <QtCore>
#include <QtSerialPort>
#include <QThread>
#include <QString>

class HIDPort;

class PortHandle: public QObject {
    Q_OBJECT
public:
    enum class Type { Serial, HID };

    PortHandle(QObject *parent, QSerialPort *sp);
    PortHandle(QObject *parent, HIDPort *hp);
    ~PortHandle();

    qint64 bytesAvailable();

    void close();

    bool isOpen();

    qint64 read(char *data, qint64 maxSize);

    qint64 write(const char *data, qint64 maxSize);

Q_SIGNALS:
    void readyRead();
    void aboutToClose();

public Q_SLOTS:
    void handleReadyRead() {
        // printf("PortHandler got handleReadyRead()\n");
        Q_EMIT readyRead();
    }

    void handleAboutToClose() {
        Q_EMIT aboutToClose();
    }

private:
    Type m_type;
    QSerialPort *m_serialPort;
    HIDPort *m_HIDPort;
};

#endif // PORTHANDLE_HH
