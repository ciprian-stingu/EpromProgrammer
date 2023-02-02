/**
 * 
 * Eprom Programmer
 * 
 * Copyright (C) 2021-2023 Ciprian Stingu
 * ciprian stingu at gmail dot com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 * 
 */
//----------------------------------------------------------------------
#include "QtGui.h"
#include <libintl.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <math.h>
#include "QtIcons.h"
//----------------------------------------------------------------------

QtGui::QtGui(int argc, char **argv)
{
    pArgc = argc;
    pArgv = argv;
    application = new QApplication(pArgc, pArgv, true);
    
    mainWindow = new QWidget();
    
    QIcon *MainIcon = GetMainIcon();
    mainWindow->setWindowIcon(*MainIcon);
    delete MainIcon;
    
    mainWindow->resize(700, 640);
    //set title
    QString MainWindowTitle = MAKE_STR(APP_NAME);
    MainWindowTitle.append(" v");
    MainWindowTitle.append(APP_VERSION);
    mainWindow->setWindowTitle(MainWindowTitle);
    //move to screen center
    QRect screenGeometry = QGuiApplication::screens().first()->geometry();
    int x = (screenGeometry.width() - mainWindow->width()) / 2;
    int y = (screenGeometry.height() - mainWindow->height()) / 2;
    mainWindow->move(x, y);
    
    mainToolbar = CreateToolbar();
    
    mainTabs = new QTabWidget();
	CreateDataTab(mainTabs);
	CreateDeviceTab(mainTabs);
    
    mainStatusBar = new QStatusBar();
    
    
    QVBoxLayout *MainLayout = new QVBoxLayout();
    MainLayout->addWidget(mainToolbar);
    MainLayout->addWidget(mainTabs);
    MainLayout->addWidget(mainStatusBar);
    mainWindow->setLayout(MainLayout);

    serialPort = new QSerialPort();
    
    selectedEprom = EPROM_TYPE::NONE;
    arduinoIO = nullptr;
    
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

QtGui::~QtGui()
{	
    CloseMainWindow();
}
//----------------------------------------------------------------------

void QtGui::Run(void)
{
    mainWindow->show();
    application->exec();
}
//----------------------------------------------------------------------

void QtGui::CloseMainWindow(void)
{
    CloseSerialPort();
}
//----------------------------------------------------------------------

QToolBar *QtGui::CreateToolbar(void)
{    
    QToolBar *mainToobar = new QToolBar();
    
    mainToobar->setStyleSheet("QToolButton { padding-left: 5px; padding-right: 5px; }");
    
    connectProgrammerAction = new QAction(QIcon::fromTheme("view-refresh"), "Connect", NULL);
    connect(connectProgrammerAction, SIGNAL(triggered()), this, SLOT(ShowConnectSlot()));
    mainToobar->addAction(connectProgrammerAction);
    
    mainToobar->addSeparator();
    
    openFileAction = new QAction(QIcon::fromTheme("document-open"), "Open File", NULL);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(FileOpenSlot()));
    openFileAction->setEnabled(false);
    mainToobar->addAction(openFileAction);
    
    saveFileAction = new QAction(QIcon::fromTheme("document-save"), "Save File", NULL);
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(FileSaveSlot()));
    saveFileAction->setEnabled(false);
    mainToobar->addAction(saveFileAction);
    
    mainToobar->addSeparator();
    
    readDeviceAction = new QAction(QIcon::fromTheme("go-up"), "Read device", NULL);
    connect(readDeviceAction, SIGNAL(triggered()), this, SLOT(ReadEpromSlot()));
    readDeviceAction->setEnabled(false);
    mainToobar->addAction(readDeviceAction);
    
    writeDeviceAction = new QAction(QIcon::fromTheme("go-down"), "Write device", NULL);
    connect(writeDeviceAction, SIGNAL(triggered()), this, SLOT(WriteEpromSlot()));
    writeDeviceAction->setEnabled(false);
    mainToobar->addAction(writeDeviceAction);
    
    verifyDataAction = new QAction(QIcon::fromTheme("dialog-ok"), "Verify", NULL);
    connect(verifyDataAction, SIGNAL(triggered()), this, SLOT(VerifyDataSlot()));
    verifyDataAction->setEnabled(false);
    mainToobar->addAction(verifyDataAction);
    
    //stopRWDeviceAction = new QAction(QIcon::fromTheme("process-stop"), "Stop", NULL);
    //connect(stopRWDeviceAction, SIGNAL(triggered()), this, SLOT(StopRWDevice()));
    //stopRWDeviceAction->setEnabled(false);
    //mainToobar->addAction(stopRWDeviceAction);
    
    mainToobar->addSeparator();
    
    hardwareTestAction = new QAction(QIcon::fromTheme("input-tablet"), "Hardware Test", NULL);
    connect(hardwareTestAction, SIGNAL(triggered()), this, SLOT(HardwareTestSlot()));
    hardwareTestAction->setEnabled(false);
    mainToobar->addAction(hardwareTestAction);
    
    readVoltageAction = new QAction(QIcon::fromTheme("battery"), "Read Voltage", NULL);
    connect(readVoltageAction, SIGNAL(triggered()), this, SLOT(ShowVoltageSlot()));
    readVoltageAction->setEnabled(false);
    mainToobar->addAction(readVoltageAction);
    
    mainToobar->addSeparator();
    
    showHelpAction = new QAction(QIcon::fromTheme("help-about"), "Info", NULL);
    connect(showHelpAction, SIGNAL(triggered()), this, SLOT(ShowHelpSlot()));
    mainToobar->addAction(showHelpAction);
    
    mainToobar->addSeparator();
    
    QWidgetAction* progressBarAct = new QWidgetAction(this);
    mainProgressBar = new QProgressBar();
    progressBarAct->setDefaultWidget(mainProgressBar);
    mainToobar->addAction(progressBarAct);
    
    return mainToobar;
}
//----------------------------------------------------------------------

void QtGui::CreateDataTab(QTabWidget *mainTabs)
{
    //log box
    logBox = new QTextEdit();
    logBox->setReadOnly(true);
    logBox->setLineWrapMode(QTextEdit::NoWrap);
    logBox->setAcceptRichText(false);
    logBox->setFontFamily("monospace");
    logBox->setFontPointSize(10);
    
    QVBoxLayout *dataTabLayout = new QVBoxLayout();
    dataTabLayout->addWidget(logBox);
    
    dataTab = new QWidget;
    dataTab->setLayout(dataTabLayout);
    
    mainTabs->addTab(dataTab, "Data");
}
//----------------------------------------------------------------------

void QtGui::CreateDeviceTab(QTabWidget *mainTabs)
{
    extern EPROM EpromsList[];
	extern size_t EpromsListSize;
    char tmpStr[1024];
    
    QLabel *deviceTypeComboLabel = new QLabel("Eprom Type");
    epromTypeCombo = new QComboBox();
    for(size_t i = 0; i < EpromsListSize; i++)
    {
        if(EpromsList[i].EpromType == EPROM_TYPE::LAST) {
            epromTypeCombo->insertSeparator(0xFFFF);
        }
        else
        {
            strncpy(tmpStr, EpromsList[i].EpromName, 1023);
            epromTypeCombo->addItem(tmpStr);
        }
    }
    epromTypeCombo->setCurrentIndex(selectedEprom);
    connect(epromTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeEpromTypeSlot(int)));
    epromTypeCombo->setEnabled(false);
    epromTypeCombo->setFixedWidth(200);
    
    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->setAlignment(Qt::AlignLeft);
    firstRowLayout->addWidget(deviceTypeComboLabel);
    firstRowLayout->addSpacing(10);
    firstRowLayout->addWidget(epromTypeCombo);
    
    QLabel *deviceInfoLabelLabel = new QLabel("Info: ");
    epromInfoLabel = new QLabel();
    sprintf(tmpStr, "%s, VCC: %.2fV, VPP: %.2fV, %srite optimization available", 
            EpromsList[selectedEprom].EpromDescription, EpromsList[selectedEprom].ProgrammingVccVoltage[0], EpromsList[selectedEprom].VppVoltage[0], 
            WriteOptimizationAvailable(EpromsList[selectedEprom]) ? "W" : "No w");
    epromInfoLabel->setText(tmpStr);
    QHBoxLayout *secondRowLayout = new QHBoxLayout();
    secondRowLayout->setAlignment(Qt::AlignLeft);
    secondRowLayout->addWidget(deviceInfoLabelLabel);
    secondRowLayout->addSpacing(10);
    secondRowLayout->addWidget(epromInfoLabel);
    
    QLabel *progAlgoComboLabel = new QLabel("Programming algorithm");
    progAlgoCombo = new QComboBox();
    progAlgoCombo->setCurrentIndex(0);
    connect(progAlgoCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeProgrammingAlgorithmSlot(int)));
    progAlgoCombo->setEnabled(false);
    progAlgoCombo->setFixedWidth(200);
    
    QHBoxLayout *thirdRowLayout = new QHBoxLayout();
    thirdRowLayout->setAlignment(Qt::AlignLeft);
    thirdRowLayout->addWidget(progAlgoComboLabel);
    thirdRowLayout->addSpacing(10);
    thirdRowLayout->addWidget(progAlgoCombo);
    
    QLabel *labelVpp = new QLabel("VPP");
    entryVpp = new QLineEdit();
    entryVpp->setMaxLength(5);
    entryVpp->setFixedWidth(50);
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].VppVoltage[0]);
    entryVpp->setText(tmpStr);
    entryVpp->setEnabled(false);
    QLabel *labelVppVolts = new QLabel("V - Please check the data sheet before programming!");
    QHBoxLayout *layoutVpp = new QHBoxLayout();
    layoutVpp->setAlignment(Qt::AlignRight);
    layoutVpp->addWidget(labelVpp);
    layoutVpp->addSpacing(45);
    layoutVpp->addWidget(entryVpp);
    layoutVpp->addSpacing(2);
    layoutVpp->addWidget(labelVppVolts);
    QFrame *frameVpp = new QFrame();
    frameVpp->setFrameShadow(QFrame::Plain);
    frameVpp->setFrameShape(QFrame::NoFrame);
    frameVpp->setFixedWidth(510);
    frameVpp->setLayout(layoutVpp);
    
    QLabel *labelProgVcc = new QLabel("Programming VCC");
    entryProgVcc = new QLineEdit();
    entryProgVcc->setMaxLength(4);
    entryProgVcc->setFixedWidth(50);
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].ProgrammingVccVoltage[0]);
    entryProgVcc->setText(tmpStr);
    entryProgVcc->setEnabled(false);
    QLabel *labelProgVccVolts = new QLabel("V - Please check the data sheet before programming!");
    QHBoxLayout *layoutProgVcc = new QHBoxLayout();
    layoutProgVcc->setAlignment(Qt::AlignRight);
    layoutProgVcc->addWidget(labelProgVcc);
    layoutProgVcc->addSpacing(5);
    layoutProgVcc->addWidget(entryProgVcc);
    layoutProgVcc->addSpacing(2);
    layoutProgVcc->addWidget(labelProgVccVolts);
    QFrame *frameProgVcc = new QFrame();
    frameProgVcc->setFrameShadow(QFrame::Plain);
    frameProgVcc->setFrameShape(QFrame::NoFrame);
    frameProgVcc->setFixedWidth(510);
    frameProgVcc->setLayout(layoutProgVcc);

    QGridLayout *optionsLayout = new QGridLayout();
    optionsLayout->setAlignment(Qt::AlignTop);
    optionsLayout->addWidget(frameVpp, 0, 0, 1, 2);
    optionsLayout->addWidget(frameProgVcc, 1, 0, 1, 2);
    
    QVBoxLayout *deviceTabLayout = new QVBoxLayout();
    deviceTabLayout->setContentsMargins(25, 25, 25, 25);
    deviceTabLayout->setAlignment(Qt::AlignTop);
    deviceTabLayout->addLayout(firstRowLayout);
    deviceTabLayout->addLayout(secondRowLayout);
    deviceTabLayout->addLayout(thirdRowLayout);
    deviceTabLayout->addLayout(optionsLayout);

    deviceTab = new QWidget;
    deviceTab->setLayout(deviceTabLayout);
    
    mainTabs->addTab(deviceTab, "Device");
}
//----------------------------------------------------------------------

void QtGui::ShowConnectSlot(void)
{
    if(serialPort->isOpen())
    {
        CloseSerialPort();
        connectProgrammerAction->setText("Connect");
        return;
    }
    
    QLabel *serialPortLabel = new QLabel("Available serial ports");
    serialPortLabel->setAlignment(Qt::AlignHCenter);
    
    connectionList = new QListWidget();
    RefreshSerialPortSlot();
    
    QVBoxLayout *connectLayout = new QVBoxLayout();
    connectLayout->setAlignment(Qt::AlignTop);
    connectLayout->setContentsMargins(25, 5, 25, 20);
    connectLayout->addSpacing(5);
    connectLayout->addWidget(serialPortLabel);
    connectLayout->addSpacing(5);
    connectLayout->addWidget(connectionList);
    
    QPushButton *connectButton = new QPushButton();
    connectButton->setText("Connect");
    connect(connectButton, SIGNAL(clicked()), this, SLOT(ConnectToSerialPortSlot()));
    
    QPushButton *refreshButton = new QPushButton();
    refreshButton->setText("Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(RefreshSerialPortSlot()));
    
    QPushButton *closeButton = new QPushButton();
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(CloseConnectWindowSlot()));
    
    QHBoxLayout *connectButtonsLayout = new QHBoxLayout();
    connectButtonsLayout->setContentsMargins(25, 15, 25, 0);
    connectButtonsLayout->addWidget(connectButton);
    connectButtonsLayout->addSpacing(20);
    connectButtonsLayout->addWidget(refreshButton);
    connectButtonsLayout->addSpacing(20);
    connectButtonsLayout->addWidget(closeButton);
    
    connectLayout->addLayout(connectButtonsLayout);
    
    connectWindow = new QWidget();
    connectWindow->resize(250, 200);
    connectWindow->setWindowTitle(gettext("Connect"));
    
    QIcon *icon = GetMainIcon();
    connectWindow->setWindowIcon(*icon);
    delete icon;
    
    connectWindow->setLayout(connectLayout);
    
    //move to screen center
    QRect screenGeometry = QGuiApplication::screens().first()->geometry();
    int x = (screenGeometry.width() - connectWindow->width()) / 2;
    int y = (screenGeometry.height() - connectWindow->height()) / 2;
    connectWindow->move(x, y);
    
    DisableButtons();
    
    connectWindow->show();
}
//----------------------------------------------------------------------

void QtGui::RefreshSerialPortSlot(void)
{
    connectionList->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QListWidgetItem *item = new QListWidgetItem(info.portName(), connectionList);
        item->setData(Qt::UserRole, info.systemLocation());
        if(info.isBusy())
        {
            item->setText(info.portName() + " (Busy)");
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        }
    }
}
//----------------------------------------------------------------------

void QtGui::ConnectToSerialPortSlot(void)
{
    QListWidgetItem* item = connectionList->currentItem();

    if(item == nullptr)
    {
        QMessageBox::critical(mainWindow, MAKE_STR(APP_NAME), "Select a serial port!");
        return;
    }

    if(!(item->flags() & Qt::ItemIsSelectable))
    {
        QMessageBox::critical(mainWindow, MAKE_STR(APP_NAME), "Port is busy!");
        return;
    }
    
    CloseSerialPort();
    LogMessage(QString("Connecting to %1...\n").arg(item->data(Qt::UserRole).toString()));
    OpenSerialPort(item->data(Qt::UserRole).toString());
}
//----------------------------------------------------------------------

void QtGui::OpenSerialPort(QString path)
{
    serialPort->setPortName(path);
    serialPort->setBaudRate(QSerialPort::Baud115200);

    if(serialPort->open(QIODevice::ReadWrite))
    {
        int counter = 0;
        while(!serialPort->waitForReadyRead(1) && ++counter < 5000);
        if(serialPort->bytesAvailable())
        {
            QByteArray readData;
            
            while(!serialPort->atEnd()) {
                readData += serialPort->readAll();
            }
            
            serialPort->clear();
            
            if(readData.indexOf(PROGRAMMER_NAME, 0) != -1)
            {
                LogMessage("Connected to eprom programmer.\n");

                arduinoIO = new ArduinoIO(serialPort);

                serialOperationStartConnection = QObject::connect(arduinoIO, SIGNAL(SerialOperationStartSignal()), this, SLOT(UpdateCursorOnSerialOperationStartSlot()));
                serialOperationErrorConnection = QObject::connect(arduinoIO, SIGNAL(SerialOperationErrorSignal(const char *)), this, SLOT(ShowMessageOnSerialOperationErrorSlot(const char *)));
                serialOperationCompleteConnection = QObject::connect(arduinoIO, SIGNAL(SerialOperationCompleteSignal()), this, SLOT(UpdateCursorOnSerialOperationCompleteSlot()));

                ResetVariables();
                
                CloseConnectWindowSlot();
                
                connectProgrammerAction->setText("Disconnect");
                UpdateButtonsStatus();
                return;
            }
        }
        LogMessage("Eprom programmer not found.\n");
        CloseSerialPort();
    } 
    else {
        QMessageBox::critical(mainWindow, "Error", serialPort->errorString());
    }
}
//----------------------------------------------------------------------

void QtGui::UpdateButtonsStatus(void)
{
    bool enabled = serialPort->isOpen();
    
    connectProgrammerAction->setEnabled(true);
    openFileAction->setEnabled(enabled && selectedEprom != EPROM_TYPE::NONE);
    saveFileAction->setEnabled(enabled && epromRead && selectedEprom != EPROM_TYPE::NONE);
    readDeviceAction->setEnabled(enabled && selectedEprom != EPROM_TYPE::NONE);
    writeDeviceAction->setEnabled(enabled && fileLoaded && selectedEprom != EPROM_TYPE::NONE);
    verifyDataAction->setEnabled(enabled && (epromWritten || fileLoaded) && selectedEprom != EPROM_TYPE::NONE);
    //stopRWDeviceAction->setEnabled(enabled);
    hardwareTestAction->setEnabled(enabled);
    readVoltageAction->setEnabled(enabled);
    epromTypeCombo->setEnabled(enabled);
    progAlgoCombo->setEnabled(enabled && selectedEprom != EPROM_TYPE::NONE);
    entryVpp->setEnabled(enabled && selectedEprom != EPROM_TYPE::NONE);
    entryProgVcc->setEnabled(enabled && selectedEprom != EPROM_TYPE::NONE);
    showHelpAction->setEnabled(true);
}
//----------------------------------------------------------------------

void QtGui::DisableButtons(void)
{
    connectProgrammerAction->setEnabled(false);
    openFileAction->setEnabled(false);
    saveFileAction->setEnabled(false);
    readDeviceAction->setEnabled(false);
    writeDeviceAction->setEnabled(false);
    verifyDataAction->setEnabled(false);
    //stopRWDeviceAction->setEnabled(enabled);
    hardwareTestAction->setEnabled(false);
    readVoltageAction->setEnabled(false);
    epromTypeCombo->setEnabled(false);
    progAlgoCombo->setEnabled(false);
    entryVpp->setEnabled(false);
    entryProgVcc->setEnabled(false);
    showHelpAction->setEnabled(false);
}
//----------------------------------------------------------------------

void QtGui::CloseSerialPort(void)
{
    if(serialPort->isOpen())
    {
        QObject::disconnect(serialOperationStartConnection);
        QObject::disconnect(serialOperationErrorConnection);
        QObject::disconnect(serialOperationCompleteConnection);

        if(arduinoIO != nullptr)
        {
            delete arduinoIO;
            arduinoIO = nullptr;
        }
        
        serialPort->close();
        LogMessage("Disconnected from eprom programmer.\n");
        epromTypeCombo->setCurrentIndex(0);
        ResetVariables();
        UpdateButtonsStatus();
    }
}
//----------------------------------------------------------------------

void QtGui::CloseConnectWindowSlot(void)
{
    connectWindow->close();
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

void QtGui::ChangeEpromTypeSlot(int _index)
{
    extern EPROM EpromsList[];
    char tmpStr[1024];
    
    ResetVariables();
    
    selectedEprom = (EPROM_TYPE)_index;
    
    progAlgoCombo->clear();
    for(size_t i = 0; i < PROGRAMMING_ALGORITHMS; i++)
    {
        if(EpromsList[selectedEprom].ProgrammingAlgorithm[i] != PRG_ALGO::UNDEFINED) {
            progAlgoCombo->addItem(PRG_ALGO_NAMES[EpromsList[selectedEprom].ProgrammingAlgorithm[i]]);
        }
    }
    progAlgoCombo->setCurrentIndex(0);
    
    sprintf(tmpStr, "%s, VCC: %.2fV, VPP: %.2fV, %srite optimization available", 
            EpromsList[selectedEprom].EpromDescription, EpromsList[selectedEprom].ProgrammingVccVoltage[0], EpromsList[selectedEprom].VppVoltage[0], 
            WriteOptimizationAvailable(EpromsList[selectedEprom]) ? "W" : "No w");
    epromInfoLabel->setText(tmpStr);
    
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].VppVoltage[0]);
    entryVpp->setText(tmpStr);
    
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].ProgrammingVccVoltage[0]);
    entryProgVcc->setText(tmpStr);
    
    sprintf(tmpStr, "Setting %s eprom.\n", EpromsList[selectedEprom].EpromName);
    LogMessage(tmpStr);
    
    if(serialPort->isOpen()) {
        arduinoIO->SelectEprom(selectedEprom);
    }
    
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

void QtGui::ChangeProgrammingAlgorithmSlot(int _index)
{
    extern EPROM EpromsList[];
    char tmpStr[1024];
    
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].VppVoltage[_index]);
    entryVpp->setText(tmpStr);
    
    snprintf(tmpStr, 100, "%.2f", EpromsList[selectedEprom].ProgrammingVccVoltage[_index]);
    entryProgVcc->setText(tmpStr);
}
//----------------------------------------------------------------------

void QtGui::ShowHelpSlot(void)
{
    QLabel *titleLabel = new QLabel(MAKE_STR(APP_NAME) " " APP_VERSION);
    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setFont(QFont("Arial", 14, QFont::Bold));
    
    QLabel *commentLabel = new QLabel(gettext("A graphical interface for Eprom Programmer"));
    commentLabel->setAlignment(Qt::AlignHCenter);
    
    QLabel *infoLabel = new QLabel();
    infoLabel->setFont(QFont("Courier", 8));
    infoLabel->setText(
        "Copyright (C) 2021 Ciprian Stingu\n"
        "ciprian stingu at gmail dot com\n"
        "This program is free software; you can\n"
		"redistribute it and/or modify it under\n"
		"the terms of the GNU General Public License\n"
		"as published by the Free Software Foundation;\n"
		"either version 2 of the License, or\n"
		"(at your option) any later version."
    );
    infoLabel->setAlignment(Qt::AlignHCenter);
    
    QVBoxLayout *aboutLayout = new QVBoxLayout();
    aboutLayout->setAlignment(Qt::AlignTop);
    aboutLayout->addWidget(titleLabel);
    aboutLayout->addSpacing(10);
    aboutLayout->addWidget(commentLabel);
    aboutLayout->addSpacing(10);
    aboutLayout->addWidget(infoLabel);
    aboutLayout->addSpacing(25);
    
    QPushButton *licenseButton = new QPushButton();
    licenseButton->setText("License");
    connect(licenseButton, SIGNAL(clicked()), this, SLOT(ShowLicenseWindowSlot()));
    
    QPushButton *closeButton = new QPushButton();
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(CloseAboutWindowSlot()));
    
    QHBoxLayout *aboutButtonsLayout = new QHBoxLayout();
    aboutButtonsLayout->setContentsMargins(50, 0, 50, 0);
    aboutButtonsLayout->addWidget(licenseButton);
    aboutButtonsLayout->addSpacing(50);
    aboutButtonsLayout->addWidget(closeButton);
    
    aboutLayout->addLayout(aboutButtonsLayout);
    
    aboutWindow = new QWidget();
    aboutWindow->resize(350, 280);
    aboutWindow->setWindowTitle(gettext("About"));
    
    QIcon *icon = GetMainIcon();
    aboutWindow->setWindowIcon(*icon);
    delete icon;
    
    aboutWindow->setLayout(aboutLayout);
    
    //move to screen center
    QRect screenGeometry = QGuiApplication::screens().first()->geometry();
    int x = (screenGeometry.width() - aboutWindow->width()) / 2;
    int y = (screenGeometry.height() - aboutWindow->height()) / 2;
    aboutWindow->move(x, y);
    
    DisableButtons();
    
    aboutWindow->show();
}
//----------------------------------------------------------------------

void QtGui::CloseAboutWindowSlot(void)
{
    aboutWindow->close();
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

void QtGui::ShowLicenseWindowSlot(void)
{    
    QTextEdit *licenseBox = new QTextEdit();
    licenseBox->setReadOnly(true);
    licenseBox->setLineWrapMode(QTextEdit::NoWrap);
    licenseBox->setAcceptRichText(false);
    licenseBox->setFontFamily("monospace");
    licenseBox->setFontPointSize(8);
    licenseBox->insertPlainText(
        "This program is free software; you can redistribute it and/or\n"
        "modify it under the terms of the GNU Library General Public License as\n"
        "published by the Free Software Foundation; either version 2 of the\n"
        "License, or (at your option) any later version.\n"
        "\n"
        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
        "Library General Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU Library General Public\n"
        "License along with the Gnome Library; see the file COPYING.LIB.  If not,\n"
        "write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n"
        "Boston, MA 02111-1307, USA.\n"
    );
    licenseBox-> moveCursor(QTextCursor::Start);
    
    QPushButton *closeButton = new QPushButton();
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(CloseLicenseWindowSlot()));
    
    QHBoxLayout *licenseButtonLayout = new QHBoxLayout();
    licenseButtonLayout->setAlignment(Qt::AlignRight);
    licenseButtonLayout->addWidget(closeButton);
    
    QVBoxLayout *licenseLayout = new QVBoxLayout();
    licenseLayout->addWidget(licenseBox);
    licenseLayout->addLayout(licenseButtonLayout);
    
    licenseWindow = new QWidget();
    licenseWindow->resize(400, 250);
    licenseWindow->setWindowTitle(gettext("License"));
    
    QIcon *icon = GetMainIcon();
    licenseWindow->setWindowIcon(*icon);
    delete icon;
    
    licenseWindow->setLayout(licenseLayout);
    
    //move to screen center
    QRect screenGeometry = QGuiApplication::screens().first()->geometry();
    int x = (screenGeometry.width() - licenseWindow->width()) / 2;
    int y = (screenGeometry.height() - licenseWindow->height()) / 2;
    licenseWindow->move(x, y);
    
    licenseWindow->show();
}
//----------------------------------------------------------------------

void QtGui::CloseLicenseWindowSlot(void)
{
    licenseWindow->close();
}
//----------------------------------------------------------------------

QIcon* QtGui::GetMainIcon(void)
{
	extern const char *guiIcon[];
	
	QPixmap *pixData = new QPixmap(guiIcon);
	QIcon *icon = new QIcon(*pixData);
    delete pixData;
    
    return icon;
}
//----------------------------------------------------------------------

void QtGui::LogMessage(const char *data)
{
    logBox->insertPlainText(data);
    logBox->ensureCursorVisible();
}
//----------------------------------------------------------------------

void QtGui::LogMessage(QString data)
{
    logBox->insertPlainText(data);
    logBox->ensureCursorVisible();
}
//----------------------------------------------------------------------

void QtGui::UpdateCursorOnSerialOperationStartSlot(void)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}
//----------------------------------------------------------------------

void QtGui::ShowMessageOnSerialOperationErrorSlot(const char *message)
{
    QMessageBox::critical(mainWindow, "Serial port error", message);
}
//----------------------------------------------------------------------

void QtGui::UpdateCursorOnSerialOperationCompleteSlot(void)
{
    QApplication::restoreOverrideCursor();
}
//----------------------------------------------------------------------

void QtGui::EpromOperationProgressBarSlot(int value)
{
    mainProgressBar->setValue(value);
    QCoreApplication::processEvents();
}
//----------------------------------------------------------------------

void QtGui::UpdateVoltageValueSlot(double vccValue, double vppValue)
{
    QObject::disconnect(updateVoltageValueConnection);
    
    QString text;
    text.sprintf("VCC: %02.2fV", vccValue);
    readVccVoltageLabel->setText(text);
    
    text.sprintf("VPP: %02.2fV", vppValue);
    readVppVoltageLabel->setText(text);
    
    text.sprintf("[VCC] %02.2fV, [VPP] %02.2fV\n", vccValue, vppValue);
    LogMessage(text);
}
//----------------------------------------------------------------------

void QtGui::ShowVoltageSlot(void)
{
    QLabel *readVoltageLabel = new QLabel("Current voltages");
    readVoltageLabel->setAlignment(Qt::AlignHCenter);
    
    readVccVoltageLabel = new QLabel("VCC: Reading...");
    readVppVoltageLabel = new QLabel("VPP: Reading...");
    
    QVBoxLayout *readVoltageLayout = new QVBoxLayout();
    readVoltageLayout->setAlignment(Qt::AlignTop);
    readVoltageLayout->setContentsMargins(50, 10, 50, 25);
    readVoltageLayout->addWidget(readVoltageLabel);
    readVoltageLayout->addSpacing(5);
    readVoltageLayout->addWidget(readVccVoltageLabel);
    readVoltageLayout->addSpacing(5);
    readVoltageLayout->addWidget(readVppVoltageLabel);
    
    QPushButton *refreshButton = new QPushButton();
    refreshButton->setText("Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(RefresVoltageSlot()));
    
    QPushButton *closeButton = new QPushButton();
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(CloseVoltageWindowSlot()));
    
    QHBoxLayout *readVoltageButtonsLayout = new QHBoxLayout();
    readVoltageButtonsLayout->setContentsMargins(25, 25, 25, 0);
    readVoltageButtonsLayout->addWidget(refreshButton);
    readVoltageButtonsLayout->addSpacing(40);
    readVoltageButtonsLayout->addWidget(closeButton);
    
    readVoltageLayout->addLayout(readVoltageButtonsLayout);
    
    voltageWindow = new QWidget();
    voltageWindow->resize(180, 160);
    voltageWindow->setWindowTitle(gettext("Voltage"));
    
    QIcon *icon = GetMainIcon();
    voltageWindow->setWindowIcon(*icon);
    delete icon;
    
    voltageWindow->setLayout(readVoltageLayout);
    
    //move to screen center
    QRect screenGeometry = QGuiApplication::screens().first()->geometry();
    int x = (screenGeometry.width() - voltageWindow->width()) / 2;
    int y = (screenGeometry.height() - voltageWindow->height()) / 2;
    voltageWindow->move(x, y);
    
    RefresVoltageSlot();
    
    DisableButtons();
    
    voltageWindow->show();
}
//----------------------------------------------------------------------

void QtGui::CloseVoltageWindowSlot(void)
{
    voltageWindow->close();
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

void QtGui::RefresVoltageSlot(void)
{
    readVccVoltageLabel->setText("VCC: Reading...");
    readVppVoltageLabel->setText("VPP: Reading...");
    updateVoltageValueConnection = QObject::connect(arduinoIO, SIGNAL(VoltageUpdatedSignal(double, double)), this, SLOT(UpdateVoltageValueSlot(double, double)));
    arduinoIO->ReadVoltage();
}
//----------------------------------------------------------------------

void QtGui::HardwareTestSlot(void)
{
    if(selectedEprom != EPROM_TYPE::NONE)
    {
        QMessageBox::critical(mainWindow, "Hardware test", "Please remove any EPROM from socket\nand set the Eprom type to NONE.");
        return;
    }
    
    DisableButtons();
    
    LogMessage("Starting hardware test...\n");
    hardwareTestConnection = QObject::connect(arduinoIO, SIGNAL(HardwareTestStepSignal(const char *)), this, SLOT(HardwareTestStepSlot(const char *)));
    arduinoIO->HardwareTest();
}
//----------------------------------------------------------------------

void QtGui::HardwareTestStepSlot(const char *message)
{
    LogMessage(message);
    
    if(strcmp(arduinoIO->HW_TEST_END, message) == 0) 
    {
        QObject::disconnect(hardwareTestConnection);
        UpdateButtonsStatus();
    }
    
    QCoreApplication::processEvents();
}
//----------------------------------------------------------------------

void QtGui::ReadEpromSlot(void)
{
    extern EPROM EpromsList[];
    
    /*if(!serialPort->isOpen())
    {
        QMessageBox::critical(mainWindow, "Serial port error", "Connect eprom programmer!");
        return;
    }*/
    
    mainProgressBar->setValue(0);
    mainProgressBar->setMaximum(arduinoIO->GetEpromSize());
    LogMessage(QString("Reading %1 bytes from eprom...\n").arg(arduinoIO->GetEpromSize()));
    progressBarConnection = QObject::connect(arduinoIO, SIGNAL(ReadBlockSignal(int)), this, SLOT(EpromOperationProgressBarSlot(int)));
    updateReadDataConnection = QObject::connect(arduinoIO, SIGNAL(ReadEpromDataSignal(int, int, uint8_t *)), this, SLOT(ReadEpromDataSlot(int, int, uint8_t *)));
    checkEpromClearConnection = QObject::connect(arduinoIO, SIGNAL(ReadCompleteSignal()), this, SLOT(CheckEpromClearSlot()));
    epromRead = false;

    DisableButtons();
    
    operationTimer.start();
    
    arduinoIO->ReadEprom();
}
//----------------------------------------------------------------------

void QtGui::CheckEpromClearSlot(void)
{
    QObject::disconnect(checkEpromClearConnection);
    QObject::disconnect(progressBarConnection);
    QObject::disconnect(updateReadDataConnection);
    
    LogMessage(QString("Read in %1 seconds.\n").arg(operationTimer.elapsed() / 1000));
    epromRead = true;
    UpdateButtonsStatus();
    
    uint8_t *dataRead = reinterpret_cast<uint8_t *>(arduinoIO->GetReadBuffer()->data());
    for(int i = 0, j = arduinoIO->GetReadBuffer()->length(); i < j; i++)
    {
        if(dataRead[i] != 0xFF)
        {
            LogMessage("Eprom not clear.\n");
            return;
        }
    }
    LogMessage("Eprom clear.\n");
}
//----------------------------------------------------------------------

void QtGui::DisplayEpromData(uint8_t *buffer, int maxSize, int baseAddr, int currentSize)
{
    char msgStr[1024];
    QString message;
    
	for(int i = baseAddr; i < baseAddr + currentSize && i < maxSize; i += DISPLAY_COLUMNS)
	{
		bool lineIsEmpty = true;
		for(int j = i; j < i + DISPLAY_COLUMNS && j < baseAddr + currentSize && i < maxSize; j++) 
		{
			if(buffer[j] < 0xFF) 
			{
				lineIsEmpty = false;
				break;
			}	
		}
		if(!lineIsEmpty) 
		{
            sprintf(msgStr, "%05X: ", i);
            message.append(msgStr);
			for(int j = i; j < i + DISPLAY_COLUMNS && j < baseAddr + currentSize && i < maxSize; j++) 
            {
				sprintf(msgStr, "%02X%s", buffer[j], j < i + DISPLAY_COLUMNS - 1 ? " " : "");
                message.append(msgStr);
			}
			message.append("\t");
			for(int j = i; j < i + DISPLAY_COLUMNS && j < baseAddr + currentSize && i < maxSize; j++) 
            {
				sprintf(msgStr, "%c", isprint(buffer[j]) && buffer[j] < 0xFF ? buffer[j] : '.');
                message.append(msgStr);
			}
            message.append("\n");
		}
	}
	
	LogMessage(message);
}
//----------------------------------------------------------------------

void QtGui::ReadEpromDataSlot(int address, int size, uint8_t *data)
{
    DisplayEpromData(data, arduinoIO->GetEpromSize(), address, size);
}
//----------------------------------------------------------------------

void QtGui::ResetVariables(void)
{
    epromRead = false;
    epromWritten = false;
    fileLoaded = false;
    mainProgressBar->setValue(0);
    mainProgressBar->setMaximum(100);
}
//----------------------------------------------------------------------

void QtGui::FileSaveSlot(void)
{
    QString fileName = QFileDialog::getSaveFileName(mainWindow, "Save EPROM file", "", "Binary (*.bin);;Rom (*.rom);;All Files (*)");

    if(fileName.isEmpty()) {
        return;
    }
    else
    {
        if(fileName.right(4).indexOf(".bin", 0) == -1 && fileName.right(4).indexOf(".rom", 0) == -1) {
            fileName.append(QString(".bin"));
        }

        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(mainWindow, "Unable to open file", file.errorString());
            return;
        }

        file.write(*arduinoIO->GetReadBuffer());
        file.close();
        LogMessage(QString("Saving to %1...\n").arg(fileName));
        LogMessage(QString("Written %1 bytes.\n").arg(arduinoIO->GetReadBuffer()->count()));
    }
}
//----------------------------------------------------------------------

void QtGui::FileOpenSlot(void)
{
    QString fileName = QFileDialog::getOpenFileName(mainWindow, "Load EPROM file", "", "Binary (*.bin);;Rom (*.rom);;All Files (*)");

    if(fileName.isEmpty()) {
        return;
    }
    else
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(mainWindow, "Unable to open file", file.errorString());
            return;
        }

        arduinoIO->GetWriteBuffer()->clear();
        arduinoIO->GetWriteBuffer()->append(file.readAll());

        LogMessage(QString("Loading from %1...\n").arg(fileName));
        LogMessage(QString("Read %1 bytes.\n").arg(arduinoIO->GetWriteBuffer()->count()));
        if (arduinoIO->GetWriteBuffer()->count() < arduinoIO->GetEpromSize())
        {
            LogMessage(QString("Padding by %1 bytes.\n").arg(arduinoIO->GetEpromSize() - arduinoIO->GetWriteBuffer()->count()));
            arduinoIO->GetWriteBuffer()->append((arduinoIO->GetEpromSize() - arduinoIO->GetWriteBuffer()->count()), static_cast<char>(0xFF));
        }
        else if(arduinoIO->GetWriteBuffer()->count() > arduinoIO->GetEpromSize())
        {
            LogMessage(QString("Deleted %1 bytes.\n").arg(arduinoIO->GetWriteBuffer()->count() - arduinoIO->GetEpromSize()));
            arduinoIO->GetWriteBuffer()->resize(arduinoIO->GetEpromSize());
        }
        
        mainProgressBar->setValue(0);
        mainProgressBar->setMaximum(arduinoIO->GetEpromSize());
        uint8_t *fileData = reinterpret_cast<uint8_t *>(arduinoIO->GetWriteBuffer()->data());
        for(int i = 0; i < arduinoIO->GetEpromSize(); i += DISPLAY_COLUMNS * 32) 
        {
            DisplayEpromData(fileData, arduinoIO->GetEpromSize(), i, DISPLAY_COLUMNS * 32);
            EpromOperationProgressBarSlot(i + DISPLAY_COLUMNS * 32);
        }
        
        fileLoaded = true;

        UpdateButtonsStatus();
    }
}
//----------------------------------------------------------------------

void QtGui::WriteEpromSlot(void)
{
    /*if(!fileLoaded) 
    {
        QMessageBox::critical(mainWindow, "Write EPROM", "No file loaded!");
        return;
    }*/
    
    extern EPROM EpromsList[];

    mainProgressBar->setValue(0);
    mainProgressBar->setMaximum(arduinoIO->GetEpromSize());
    LogMessage(QString("Writing %1 bytes...").arg(arduinoIO->GetEpromSize()));
    progressBarConnection = QObject::connect(arduinoIO, SIGNAL(WriteBlockSignal(int)), this, SLOT(EpromOperationProgressBarSlot(int)));
    writeCompleteConnection = QObject::connect(arduinoIO, SIGNAL(WriteCompleteSignal(bool, const char *)), this, SLOT(WriteCompleteSlot(bool, const char *)));
    epromRead = false;
    epromWritten = false;
    
    DisableButtons();
    
    operationTimer.start();
    
    arduinoIO->WriteEprom(
        entryProgVcc->text().toDouble(), 
        entryVpp->text().toDouble(), 
        EpromsList[selectedEprom].ProgrammingAlgorithm[progAlgoCombo->currentIndex()]
    );
}
//----------------------------------------------------------------------

void QtGui::WriteCompleteSlot(bool successful, const char *message)
{
    QObject::disconnect(writeCompleteConnection);
    QObject::disconnect(progressBarConnection);
    epromWritten = successful;
    
    QString infoMessage = epromWritten ? " complete." : " error.\nMessage: ";
    if(!epromWritten) 
    {
        infoMessage.append(message);
        infoMessage.append("\n");
    }
    else 
    {
        infoMessage.append(QString(" Duration: %1 seconds.\n").arg(operationTimer.elapsed() / 1000));
        infoMessage.append(message);
        infoMessage.append("\n");
    }
    LogMessage(infoMessage);
    
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------

bool QtGui::WriteOptimizationAvailable(EPROM eprom)
{
    for(int i = 0; i < PROGRAMMING_ALGORITHMS; i++) 
    {
        if(eprom.ProgrammingAlgorithm[i] != PRG_ALGO::UNDEFINED && eprom.ProgrammingAlgorithm[i] != PRG_ALGO::STANDARD) {
            return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------

void QtGui::VerifyDataSlot(void)
{
    DisableButtons();
    
    if(arduinoIO->GetReadBuffer()->count())
    {
        int result = QMessageBox::question(NULL, "Verify", "Re-read eprom?", QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes) {
            ReReadEpromSlot();
        }
        else {
            ContinueVerifyDataSlot();
        }
    }
    else {
        ReReadEpromSlot();
    }
}
//----------------------------------------------------------------------

void QtGui::DisplayDifferences(uint8_t* inBuffer, uint8_t* outBuffer, int offset, int size)
{
    char msgStr[1024];
    QString message;
    
    sprintf(msgStr, "OFFSET: 0x%05X\nFile:  ", offset);
    message.append(msgStr);
	for(int i = offset; i < offset + size; i++) 
    {
		sprintf(msgStr, "%02X%s", inBuffer[i], i == offset + size - 1 ? "" : " ");
        message.append(msgStr);
	}
	message.append("\nEprom: ");
	for(int i = offset; i < offset + size; i++) 
    {
		sprintf(msgStr, "%02X%s", outBuffer[i], i == offset + size - 1 ? "" : " ");
        message.append(msgStr);
	}
	message.append("\n\n");
    LogMessage(message);
}
//----------------------------------------------------------------------

void QtGui::ReReadEpromSlot(void)
{
    extern EPROM EpromsList[];
    
    mainProgressBar->setValue(0);
    mainProgressBar->setMaximum(arduinoIO->GetEpromSize());
    progressBarConnection = QObject::connect(arduinoIO, SIGNAL(ReadBlockSignal(int)), this, SLOT(EpromOperationProgressBarSlot(int)));
    updateReadDataConnection = QObject::connect(arduinoIO, SIGNAL(ReadEpromDataSignal(int, int, uint8_t *)), this, SLOT(ReadEpromDataSlot(int, int, uint8_t *)));
    checkEpromClearConnection = QObject::connect(arduinoIO, SIGNAL(ReadCompleteSignal()), this, SLOT(ContinueVerifyDataSlot()));
    
    arduinoIO->ReadEprom();
}
//----------------------------------------------------------------------

void QtGui::ContinueVerifyDataSlot(void)
{
    QObject::disconnect(checkEpromClearConnection);
    QObject::disconnect(progressBarConnection);
    QObject::disconnect(updateReadDataConnection);
    
    mainProgressBar->setValue(0);
    mainProgressBar->setMaximum(arduinoIO->GetEpromSize());
    uint8_t *fileData = reinterpret_cast<uint8_t *>(arduinoIO->GetWriteBuffer()->data());
    uint8_t *epromData = reinterpret_cast<uint8_t *>(arduinoIO->GetReadBuffer()->data());
    int totalCodeDifferences = 0;
    
    LogMessage("\nCODE COMPARISION RESULT\n\n");
    for(int i = 0; i < arduinoIO->GetEpromSize(); i += DISPLAY_COLUMNS) 
    {
        int differences = 0;
        
        for(int j = 0; j < DISPLAY_COLUMNS; j++)
		{
			if(fileData[i + j] != epromData[i + j]) {
				differences++;
			}
		}
		if(differences > 0) 
		{
			totalCodeDifferences += differences;
			DisplayDifferences(fileData, epromData, i, DISPLAY_COLUMNS);
		}

        EpromOperationProgressBarSlot(i + DISPLAY_COLUMNS);
    }
    
    QString message;
    message.append(QString::number(totalCodeDifferences));
    message.append(" differences found.\n");
    LogMessage(message);
    
    UpdateButtonsStatus();
}
//----------------------------------------------------------------------
