#ifndef PORTHANDLE_HH
#define PORTHANDLE_HH

#include <hidapi.h>
#include <QtCore>
#include <QtSerialPort>
#include <QThread>
#include <QString>

class HIDPort: public QObject {
  Q_OBJECT
  public:

  HIDPort(QObject *parent = Q_NULLPTR, const char *path = Q_NULLPTR);
  ~HIDPort();

  qint64 bytesAvailable() {
    int ret = (m_buffer_w + m_buflen - m_buffer_r) % m_buflen;
    // printf("HIDPort::bytesAvailable() -> %d\n", ret);

    return ret;
  }

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

class PortHandle: public QObject {
  Q_OBJECT
  public:
  enum class Type { Serial, HID };

  PortHandle(QObject *parent, QSerialPort *sp);
  PortHandle(QObject *parent, HIDPort *hp);
  ~PortHandle();

  qint64 bytesAvailable() {
    switch (m_type) {
    case Type::Serial:
      return m_serialPort->bytesAvailable();
    case Type::HID:
      return m_HIDPort->bytesAvailable();
    }

    return 0;
  }

  void close() {
    switch (m_type) {
    case Type::Serial:
      return m_serialPort->close();
    case Type::HID:
      return m_HIDPort->close();
    }
  }

  bool isOpen() {
    switch (m_type) {
    case Type::Serial:
      return m_serialPort->isOpen();
    case Type::HID:
      return m_HIDPort->isOpen();
    }

    return false;
  }

  qint64 read(char *data, qint64 maxSize) {
    switch (m_type) {
    case Type::Serial:
      return m_serialPort->read(data, maxSize);
    case Type::HID:
      return m_HIDPort->read(data, maxSize);
    }

    return -1;
  }

  qint64 write(char *data, qint64 maxSize) {
    switch (m_type) {
    case Type::Serial:
      return m_serialPort->write(data, maxSize);
    }

    return -1;
  }

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
