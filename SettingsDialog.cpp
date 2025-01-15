#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "Settings.h"
#include <QIntValidator>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Создаем объект Settings и загружаем текущие настройки
    Settings settings;
    settings.load();

    // Устанавливаем значения полей в диалоге
    ui->m_address->setText(settings.getAddress());
    ui->m_port->setText(QString::number(settings.getPort()));
    ui->m_username->setText(settings.getUsername());

    // Валидация для поля порта
    QIntValidator *intValidator = new QIntValidator(0, 65535, this);
    ui->m_port->setValidator(intValidator);

    // Деактивируем кнопку OK, если поле username пустое
    connect(ui->m_username, &QLineEdit::textChanged, this, [this](const QString &text){
        ui->dialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    });
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    if (ui->m_username->text().isEmpty())
    {
        return;
    }

    // Создаем объект Settings и сохраняем введенные пользователем значения
    Settings settings;

    settings.setAddress(ui->m_address->text());
    settings.setPort(ui->m_port->text().toInt());
    settings.setUsername(ui->m_username->text());
    settings.save();

    QDialog::accept();
}
