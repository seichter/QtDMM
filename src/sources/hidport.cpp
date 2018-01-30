
#include "hidport.h"

#include <QDebug>

HIDPort::HIDPort(QObject *parent, const QString &path)
    : QObject( parent)
    , m_handle(nullptr)
{
    if (!path.isNull()) {
        QString prefix = "HID 0x1a86:0xe008 ";

        fprintf(stderr, "connection open with path %s\n",path.toLocal8Bit().data());

        if (path.startsWith(prefix)) {
            QString s = path;
            QByteArray ba = s.remove(0, prefix.size()).toUtf8();

            m_handle = hid_open_path(ba.data());

            fprintf(stderr, "connecting to %s result 0x%x\n", ba.data(), m_handle);
        }
    }

    // Open the device using the VID, PID,
    // and optionally the Serial number (NULL for the hoitek chip).
    if (m_handle == Q_NULLPTR) {

        m_handle = hid_open(0x1a86, 0xe008, NULL);

        fprintf(stderr, "connection open with VID:PID result 0x%x\n", m_handle);


    }

    if (m_handle == Q_NULLPTR) {

        m_handle = hid_open_path("/dev/hidraw1");

        fprintf(stderr, "connecting to raw device 0x%x\n", m_handle);

    }

    if (m_handle != Q_NULLPTR) {

        fprintf(stderr, "connection started\n");

        m_isOpen = true;
        QThread* thread = new QThread;
        this->moveToThread(thread);
        connect(thread, SIGNAL( started() ), this, SLOT( run() ));
        connect(this, SIGNAL( finished() ), thread, SLOT( quit() ));
        // connect(this, SIGNAL( finished() ), this, SLOT( deleteLater() ));
        connect(thread, SIGNAL( finished() ), thread, SLOT( deleteLater() ));
        thread->start();
    }
}

HIDPort::~HIDPort() {
    if (m_isOpen) {
        close();
    }
}

QStringList HIDPort::availablePorts() {

    QStringList portlist;
    int dev_cnt;
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x1a86, 0xe008); // all chips this SW belongs to...
    for (dev_cnt = 0, cur_dev = devs; cur_dev != Q_NULLPTR; cur_dev = cur_dev->next) {
        dev_cnt++;
    }

    fprintf(stderr, "[!] found %i devices:\n", dev_cnt);

    for (cur_dev = devs; cur_dev != Q_NULLPTR; cur_dev = cur_dev->next) {

        qDebug() << "HID path " << cur_dev->path;

        fprintf(stderr, "[!] found %s %x:%x %S %S %S %d %d\n",
                cur_dev->path,
                cur_dev->vendor_id,cur_dev->product_id,
                cur_dev->manufacturer_string,
                cur_dev->product_string,
                cur_dev->serial_number,
                cur_dev->usage,
                cur_dev->usage_page
                );


        portlist << QString("HID 0x1a86:0xe008 ").append(cur_dev->path);
    }

    hid_free_enumeration(devs);

    return portlist;
}

void HIDPort::close() {
    if (m_isOpen) {
        Q_EMIT aboutToClose();
        m_isOpen = false;

        if (m_handle != Q_NULLPTR) {
            hid_close(m_handle);
            m_handle = Q_NULLPTR;
        }
    }
}

void HIDPort::run() {
    if (m_isOpen) {
        memset(m_buffer, 0, m_buflen);

        unsigned int bps = 19200;
        // Send a Feature Report to the device
        m_buffer[0] = 0x0; // report ID
        m_buffer[1] = bps;
        m_buffer[2] = bps >> 8;
        m_buffer[3] = bps >> 16;
        m_buffer[4] = bps >> 24;
        m_buffer[5] = 0x03; // 3 = enable?
        int res = hid_send_feature_report(m_handle, m_buffer, 6); // 6 bytes

        if (res < 0) {
            fprintf(stderr, "Unable to send a feature report.\n");
            close();
        }
        else {
            memset(m_buffer, 0, m_buflen);
            QThread::usleep(1000);

            do {
                unsigned char buf[32];

                res = 0;
                while (res == 0) {

                    res = hid_read(m_handle, buf, sizeof(buf));
                    if (res < 0) {
                        close();
                    }
                }

                if (res > 0) {
                    // format data
                    int len = buf[0] & 0x07; // the first byte contains the length in the lower 3 bits ( 111 = 7 )
                    for (int i = 1; i <= len; i++) {
                        m_buffer[m_buffer_w] = buf[i] & 0x7f; // bitwise and with 0111 1111, mask the upper bit which is always 1
                        // printf("HIDPort::run() got 0x%02x @ %d\n", m_buffer[m_buffer_w], m_buffer_w);
                        m_buffer_w = (m_buffer_w + 1) % m_buflen;
                    }

                    if (len > 0) {
                        Q_EMIT readyRead();
                    }
                }
            }
            while (m_isOpen && res >= 0);
        }
    }

    Q_EMIT finished();
}
