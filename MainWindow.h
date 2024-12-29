#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include "TcpClient.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    TcpClient* m_client; // Клиент для работы с сервером
    //    std::unique_ptr<TcpClient> m_client; // предложение гпт (вызывает конфликт в срр)
    boost::asio::io_context io_context; // Контекст для работы Boost.Asio
    std::thread io_thread;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSendButtonClicked(); // Обработчик кнопки Send
    void onMessageReceived(const std::string& message); // Обработчик полученных сообщений
};
