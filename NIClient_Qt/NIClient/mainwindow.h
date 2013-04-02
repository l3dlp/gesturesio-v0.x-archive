#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
	QTcpSocket *tcpSocket;
	bool isConnected;

private slots:
	void onConnect();
	void onDisconnect();
	void onRead();

	void onConnected();
	void onDisconnected();
	void readData();
};

#endif // MAINWINDOW_H
