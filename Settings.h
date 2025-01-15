#pragma once

#include <QSettings>
#include <QString>

class Settings
{
public:
    Settings();
    bool load();
    void save();

    QString getAddress() const;
    void setAddress(const QString &address);

    int getPort() const;
    void setPort(int port);

    QString getUsername() const;
    void setUsername(const QString &username);

private:
    QString m_address = "127.0.0.1";
    int m_port        = 15001;
    QString m_username;
    QSettings m_qSettings;
};

inline Settings gSettings;
