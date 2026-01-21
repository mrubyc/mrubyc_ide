/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "writerwidget.h"
#include "setting.h"
#include "buildsetting.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGridLayout>
#include <QComboBox>
#include <QSerialPortInfo>
#include <QDebug>

const char * const WriterWidget::TBL_BAUDRATE[] = {"", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};

WriterWidget::WriterWidget(BuildSetting *buildSetting, QWidget *parent)
    : QWidget(parent)
    ,m_buildSetting(buildSetting)
{
    // all of parts for QWidget.
    QLabel *mrbWriteLabel = new QLabel(tr("mrbwrite"));
    m_mrbWriterLineEdit = new QLineEdit;
    QPushButton *buttonSelectMrbWriter = new QPushButton(tr("..."));

    QLabel *mrbWriteOptionsLabel = new QLabel(tr("Options"));
    m_mrbWriterOptionsLineEdit = new QLineEdit;

    QLabel *portLabel = new QLabel(tr("Port"));
    m_portComboBox = new QComboBox;
    QPushButton *updatingPortButton = new QPushButton(tr("Update"));

    QLabel *baudRateLabel = new QLabel(tr("Baud rate"));
    m_baudRate = new QComboBox;
    for( unsigned int i = 0; i < sizeof(TBL_BAUDRATE)/sizeof(TBL_BAUDRATE[0]); i++ ) {
	m_baudRate->addItem( TBL_BAUDRATE[i] );
    }

    // layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(mrbWriteLabel, 0, 0);
    gridLayout->addWidget(m_mrbWriterLineEdit, 0, 1);
    gridLayout->addWidget(buttonSelectMrbWriter, 0, 2);
    gridLayout->addWidget(mrbWriteOptionsLabel, 1, 0);
    gridLayout->addWidget(m_mrbWriterOptionsLineEdit, 1, 1, 1, 2);
    gridLayout->addWidget(portLabel, 2, 0);
    gridLayout->addWidget(m_portComboBox, 2, 1);
    gridLayout->addWidget(updatingPortButton, 2, 2 );
    gridLayout->addWidget(baudRateLabel, 3, 0);
    gridLayout->addWidget(m_baudRate, 3, 1 );

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(gridLayout);
    verticalLayout->addStretch();

    setLayout(verticalLayout);

    // set a value on an each element.
    m_mrbWriterLineEdit->setText(m_buildSetting->mrbwCommand());
    m_mrbWriterOptionsLineEdit->setText(m_buildSetting->mrbwCommandOptions());
    m_baudRate->setCurrentText( m_buildSetting->baudRate() );

    // update serial port....
    updateSirialPortList();

    // set a button on a relative slot.
    connect(buttonSelectMrbWriter, &QPushButton::clicked, this, &WriterWidget::setupWriterCommand);
    // The value for a checkbox and a textfield is changed.
    connect(m_mrbWriterLineEdit, &QLineEdit::editingFinished, this, &WriterWidget::changeMrbwComand);
    connect(m_mrbWriterOptionsLineEdit, &QLineEdit::editingFinished, this, &WriterWidget::changeMrbwComandOptions);

    // Update the list of a combobox with a current port list.
    connect(m_portComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &WriterWidget::changeSerialPort);
    connect(updatingPortButton, &QPushButton::clicked, this, &WriterWidget::updateSirialPortList);

    connect(m_baudRate, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &WriterWidget::changeBaudRate);
}

WriterWidget::~WriterWidget()
{

}

void WriterWidget::setupWriterCommand()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Choose Writer"), "", "" );
    if (fileName.isEmpty())
        return;

    m_mrbWriterLineEdit->setText(fileName);

    // send a signal.
    changeMrbwComand();
}

void WriterWidget::changeMrbwComand()
{
    m_mrbWriterLineEdit->setText( m_mrbWriterLineEdit->text().trimmed() );
    m_buildSetting->setMrbwCommand(m_mrbWriterLineEdit->text());
}

void WriterWidget::changeMrbwComandOptions()
{
    m_mrbWriterOptionsLineEdit->setText( m_mrbWriterOptionsLineEdit->text().trimmed() );
    m_buildSetting->setMrbwCommandOptions(m_mrbWriterOptionsLineEdit->text());
}

void WriterWidget::changeSerialPort(int index)
{
    if (index < 0) {
//        m_buildSetting->setPortName(Q_NULLPTR);
        return;
    }

    QVariant data = m_portComboBox->itemData(index);
    QString portName = data.toString();

    m_buildSetting->setPortName(portName);
}

void WriterWidget::updateSirialPortList()
{
    // Update Serial Port List
    m_portComboBox->clear();
    int index = -1;
    QList<QSerialPortInfo> serialPortList = QSerialPortInfo::availablePorts();
    for (int i=0; i<serialPortList.size(); i++) {
        const QSerialPortInfo &info = serialPortList.at(i);
        QString newPortName = info.portName();
        QVariant param(newPortName);
        m_portComboBox->addItem(info.description() + " (" + info.portName() + ")", param);

        if (newPortName == m_buildSetting->portName()) {
            qDebug() << "Port:" << newPortName;
            index = i;
        }
    }

    // if a port name is not found, setting is null....
    int portCount = m_portComboBox->count();
    if (portCount == 0) {
        // no port.
        m_buildSetting->setPortName(Q_NULLPTR);
        return;
    } else if (index < 0) {
        // The some ports exist, but previous selected port does not find.
        // select first port.
        index = 0;
    }

    m_portComboBox->setCurrentIndex(index);
    changeSerialPort(index);
}

void WriterWidget::changeBaudRate()
{
    QString baud = m_baudRate->currentText();
    m_buildSetting->setBaudRate( baud );
}
