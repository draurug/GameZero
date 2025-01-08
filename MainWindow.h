#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include "ChatClient.h"

namespace Ui
{ class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    ChatClient* m_client;
    std::thread io_thread;
    boost::asio::io_context io_context;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dbgStartServer();

signals:
    void onMessageReceived(const QString &message);

private slots:
    void onSendButtonClicked();
};
