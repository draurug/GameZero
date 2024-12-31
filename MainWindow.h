#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include "TcpClient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
    TcpClient* m_client;
    boost::asio::io_context io_context;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void dbgStartServer();
private slots:
    void onSendButtonClicked(); // Обработчик кнопки Send
    void onMessageReceived(const std::string& message); // Обработчик полученных сообщений
};
