#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QTcpSocket socket;
    socket.connectToHost("example.com", 80);

    if (socket.waitForConnected()) {
        socket.write("GET / HTTP/1.0\r\n\r\n");
        socket.waitForBytesWritten();
        socket.waitForReadyRead();
        QByteArray response = socket.readAll();
        qDebug() << response;
        socket.close();
        return 0;
    } else {
        qWarning() << "Failed to connect to host!";
        return 1;
    }
}