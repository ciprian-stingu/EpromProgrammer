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
#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H
//----------------------------------------------------------------------
#include <QObject>
#include <QSerialPort>
#include <QElapsedTimer>
#include "structs.h"
//----------------------------------------------------------------------

class ArduinoIO : public QObject
{
    Q_OBJECT
    
private:
    const char *COMMAND_FLAG           = "@CMD";
    const char *COMMAND_SET_EPROM_TYPE = "@CMDTYPE%2d";
    const char *COMMAND_VOLTAGE_INFO   = "@CMDVINF";
    const char *COMMAND_READ_EPROM     = "@CMDREAD";
    const char *COMMAND_HW_TEST        = "@CMDTEST";
    const char *COMMAND_WRITE_EPROM    = "@CMDWRIT%5.2f%5.2f%2d";
    const char *RESPONSE_OK            = "@RSPOK  ";
    const char *RESPONSE_VOLTAGE_INFO  = "@RSPVINF";
    const char *RESPONSE_READ_EPROM    = "@RSPREAD";
    const char *RESPONSE_HW_TEST_ON    = "@RSPTEST ON";
    const char *RESPONSE_HW_TEST_OFF   = "@RSPTEST OFF";
    const char *RESPONSE_ERROR         = "@RSPERR ";
    const char *RESPONSE_WRITE_EPROM   = "@RSPWRIT";
    const char *RESPONSE_WRITE_BLOCK   = "@RSPBLCK";
    
    EPROM_TYPE selectedEpromType;
    QSerialPort *serialPort = nullptr;
    QMetaObject::Connection serialDataConnection;
    QByteArray readBuffer;
    QByteArray writeBuffer;
    int maxBufferSize;
    PRG_ALGO programmingAlghoritm;
    
    void Send(const QByteArray &data);
    void ResetVariables(void);
    int WaitForReadyRead(int timeout);
    QString CleanErrorMessage(QByteArray errorMessage);
    
private slots:    
    void SelectEpromSlot(void);
    void ReadVoltageSlot(void);
    void ReadEpromSlot(void);
    void HarwareTestSlot(void);
    void WriteEpromSlot(void);
    
public:
    const char *HW_TEST_END = "Hardware test end.\n";
    
    ArduinoIO(QSerialPort *port);
    ~ArduinoIO();
    int GetEpromSize(void);
    QByteArray *GetReadBuffer(void);
    QByteArray *GetWriteBuffer(void);
    void SelectEprom(EPROM_TYPE type);
    void ReadVoltage(void);
    void ReadEprom(void);
    void HardwareTest(void);
    void WriteEprom(double vcc, double vpp, PRG_ALGO algo);
    
signals:
    void SerialOperationStartSignal(void);
    void SerialOperationErrorSignal(const char *);
    void SerialOperationCompleteSignal(void);
    void VoltageUpdatedSignal(double, double);
    void ReadBlockSignal(int);
    void ReadCompleteSignal(void);
    void HardwareTestStepSignal(const char *);
    void ReadEpromDataSignal(int, int, uint8_t *);
    void WriteBlockSignal(int);
    void WriteCompleteSignal(bool, const char *);
    
};
//----------------------------------------------------------------------
#endif

