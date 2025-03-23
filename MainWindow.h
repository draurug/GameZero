#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include "ChatClient.h"
#include "Settings.h"

namespace Ui
{ class MainWindow; }

void dbgStartSecondClient();

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    ChatClient* m_client;
    std::thread io_thread;
    boost::asio::io_context io_context;
    QStringList *m_clientListModel;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initClient(const Settings& settings);
    void displayMessage(const QString& message);
    static void dbgStartServer();

signals:
    void onUserListSignal(const QStringList& users);
    void onMessageSignal(const QString& message);
private slots:
    void onSendButtonClicked();
    void disconnectClient();
    void onUserListSlot(const QStringList& userList);
    void onMessageSlot(const QString& message);
};
