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
#ifndef QT_GUI_H
#define QT_GUI_H
//----------------------------------------------------------------------
#include <QApplication>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QDir>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QFrame>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QWidgetAction>
#include <QScreen>
#include <QElapsedTimer>
#include <libintl.h>
#include "defines.h"
#include "structs.h"
#include "ArduinoIO.h"
//----------------------------------------------------------------------

class QtGui : public QObject
{
    Q_OBJECT
    
private:
    //without this the qt application will be uninitialized
    int pArgc;
    char** pArgv;
    // serial port
    QSerialPort *serialPort = nullptr;
    ArduinoIO *arduinoIO = nullptr;
    EPROM_TYPE selectedEprom = EPROM_TYPE::NONE;
    bool epromRead = false;
    bool epromWritten = false;
    bool fileLoaded = false;
    bool writeOptimization = false;
    bool isVerifyRead = false;
    QElapsedTimer operationTimer;
    
    //widgets
    QApplication *application;
    QWidget *mainWindow;
    QTabWidget *mainTabs;
    QToolBar *mainToolbar;
    QStatusBar *mainStatusBar;
    QProgressBar *mainProgressBar;
    
    QWidget *dataTab;
    QTextEdit *logBox;
    
    QWidget *deviceTab;
    QComboBox *epromTypeCombo;
    QLabel *epromInfoLabel;
    QComboBox *progAlgoCombo;
    QLineEdit *entryVpp;
    QLineEdit *entryProgVcc;
    
    QWidget *connectWindow;
    QListWidget *connectionList;
    
    QWidget *voltageWindow;
    QLabel *readVccVoltageLabel;
    QLabel *readVppVoltageLabel;
    
    QWidget *aboutWindow;
    QWidget *licenseWindow;
    
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *readDeviceAction;
    QAction *writeDeviceAction;
    QAction *verifyDataAction;
    //QAction *stopRWDeviceAction;
    QAction *connectProgrammerAction;
    QAction *hardwareTestAction;
    QAction *readVoltageAction;
    QAction *showHelpAction;
    
    QMetaObject::Connection serialOperationStartConnection;
    QMetaObject::Connection serialOperationErrorConnection;
    QMetaObject::Connection serialOperationCompleteConnection;
    QMetaObject::Connection updateVoltageValueConnection;
    QMetaObject::Connection hardwareTestConnection;
    QMetaObject::Connection checkEpromClearConnection;
    QMetaObject::Connection progressBarConnection;
    QMetaObject::Connection updateReadDataConnection;
    QMetaObject::Connection writeCompleteConnection;
    
    void CloseMainWindow(void);
    QToolBar *CreateToolbar(void);
    QIcon* GetMainIcon(void);
    void CreateDataTab(QTabWidget *mainTabs);
    void CreateDeviceTab(QTabWidget *mainTabs);
    void OpenSerialPort(QString path);
    void CloseSerialPort(void);
    void LogMessage(const char *data);
    void LogMessage(QString data);
    void UpdateButtonsStatus(void);
    void DisableButtons(void);
    void DisplayEpromData(uint8_t *buffer, int maxSize, int baseAddr, int currentSize);
    void ResetVariables(void);
    bool WriteOptimizationAvailable(EPROM eprom);
    void DisplayDifferences(uint8_t* inBuffer, uint8_t* outBuffer, int offset, int size);
    
private slots:
    void ShowHelpSlot(void);
    void CloseAboutWindowSlot(void);
    void ShowLicenseWindowSlot(void);
    void CloseLicenseWindowSlot(void);
    void ShowConnectSlot(void);
    void CloseConnectWindowSlot(void);
    void ConnectToSerialPortSlot(void);
    void RefreshSerialPortSlot(void);
    void UpdateCursorOnSerialOperationStartSlot(void);
    void ShowMessageOnSerialOperationErrorSlot(const char *message);
    void UpdateCursorOnSerialOperationCompleteSlot(void);
    void ChangeEpromTypeSlot(int _index);
    void ChangeProgrammingAlgorithmSlot(int _index);
    void EpromOperationProgressBarSlot(int value);
    void ShowVoltageSlot(void);
    void CloseVoltageWindowSlot(void);
    void RefresVoltageSlot(void);
    void UpdateVoltageValueSlot(double vccValue, double vppValue);
    void HardwareTestSlot(void);
    void HardwareTestStepSlot(const char *message);
    void ReadEpromSlot(void);
    void CheckEpromClearSlot(void);
    void ReadEpromDataSlot(int address, int size, uint8_t *data);
    void FileSaveSlot(void);
    void FileOpenSlot(void);
    void WriteEpromSlot(void);
    void WriteCompleteSlot(bool successful, const char *message);
    void VerifyDataSlot(void);
    void ReReadEpromSlot(void);
    void ContinueVerifyDataSlot(void);
    
protected:
    
public:
    QtGui(int argc, char** argv);
    ~QtGui();
    void Run(void);
    
};
//----------------------------------------------------------------------
#endif
