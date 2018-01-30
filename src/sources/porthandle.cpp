#include "porthandle.h"

#include "hidport.h"

PortHandle::PortHandle(QObject *parent, QSerialPort *sp): QObject( parent) {
  m_type = Type::Serial;
  m_serialPort = sp;
  m_HIDPort = Q_NULLPTR;

  connect(m_serialPort, SIGNAL( readyRead() ), this, SLOT( handleReadyRead() ));
  connect(m_serialPort, SIGNAL( aboutToClose() ), this, SLOT( handleAboutToClose() ));
}

PortHandle::PortHandle(QObject *parent, HIDPort *hp): QObject( parent) {
  m_type = Type::HID;
  m_serialPort = Q_NULLPTR;
  m_HIDPort = hp;

  connect(m_HIDPort, SIGNAL( readyRead() ), this, SLOT( handleReadyRead() ));
  connect(m_HIDPort, SIGNAL( aboutToClose() ), this, SLOT( handleAboutToClose() ));
}

PortHandle::~PortHandle() {
  if (m_serialPort != Q_NULLPTR) {
    delete m_serialPort;
    m_serialPort = Q_NULLPTR;
  }
  if (m_HIDPort != Q_NULLPTR) {
    delete m_HIDPort;
    m_HIDPort = Q_NULLPTR;
  }
}

qint64 PortHandle::bytesAvailable() {
    switch (m_type) {
    case Type::Serial:
        return m_serialPort->bytesAvailable();
    case Type::HID:
        return m_HIDPort->bytesAvailable();
    }

    return 0;
}

void PortHandle::close() {
    switch (m_type) {
    case Type::Serial:
        return m_serialPort->close();
    case Type::HID:
        return m_HIDPort->close();
    }
}

bool PortHandle::isOpen() {
    switch (m_type) {
    case Type::Serial:
        return m_serialPort->isOpen();
    case Type::HID:
        return m_HIDPort->isOpen();
    }

    return false;
}

qint64 PortHandle::read(char *data, qint64 maxSize) {
    switch (m_type) {
    case Type::Serial:
        return m_serialPort->read(data, maxSize);
    case Type::HID:
        return m_HIDPort->read(data, maxSize);
    }

    return -1;
}

qint64 PortHandle::write(const char *data, qint64 maxSize) {
    switch (m_type) {
    case Type::Serial:
        return m_serialPort->write(data, maxSize);
    }

    return -1;
}
