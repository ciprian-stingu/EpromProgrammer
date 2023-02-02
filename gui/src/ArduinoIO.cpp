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
#include "ArduinoIO.h"
//----------------------------------------------------------------------

ArduinoIO::ArduinoIO(QSerialPort *port)
{
    serialPort = port;
    ResetVariables();
}
//----------------------------------------------------------------------

ArduinoIO::~ArduinoIO()
{	
    
}
//----------------------------------------------------------------------

void ArduinoIO::ResetVariables(void)
{
    selectedEpromType = EPROM_TYPE::NONE;
    maxBufferSize = 0;
    readBuffer.clear();
    writeBuffer.clear();
    programmingAlghoritm = PRG_ALGO::STANDARD;
}
//----------------------------------------------------------------------

int ArduinoIO::GetEpromSize(void)
{
    return maxBufferSize;
}
//----------------------------------------------------------------------

QByteArray *ArduinoIO::GetReadBuffer(void)
{
    return &readBuffer;
}
//----------------------------------------------------------------------

QByteArray *ArduinoIO::GetWriteBuffer(void)
{
    return &writeBuffer;
}
//---------------------------------------------------------------------

void ArduinoIO::SelectEprom(EPROM_TYPE type)
{    
    ResetVariables();
    
    extern EPROM EpromsList[];
    
    serialDataConnection = QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(SelectEpromSlot()));

    selectedEpromType = type;
    maxBufferSize = EpromsList[type].MemorySize;
    
    char command[1024];
    sprintf(command, COMMAND_SET_EPROM_TYPE, EpromsList[type].EpromType);
    Send(command);
}
//----------------------------------------------------------------------

void ArduinoIO::Send(const QByteArray &data)
{
    emit SerialOperationStartSignal();
    serialPort->write(data);
    serialPort->flush();
}
//----------------------------------------------------------------------

void ArduinoIO::SelectEpromSlot(void)
{
    QObject::disconnect(serialDataConnection);
    
    QByteArray readData;

    WaitForReadyRead(500);
    while(!serialPort->atEnd()) {
        readData += serialPort->readAll();
    }
    
    if(readData.indexOf(RESPONSE_OK, 0) == -1) {
        emit SerialOperationErrorSignal("Fail to set eprom type!\nPlease retry selecting the eprom...");
    }
    
    emit SerialOperationCompleteSignal();
}
//----------------------------------------------------------------------

void ArduinoIO::ReadVoltage(void)
{
    serialDataConnection = QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(ReadVoltageSlot()));
    Send(COMMAND_VOLTAGE_INFO);
}
//----------------------------------------------------------------------

void ArduinoIO::ReadVoltageSlot(void)
{
    QObject::disconnect(serialDataConnection);
    
    QByteArray readData;

    WaitForReadyRead(1000);
    while(!serialPort->atEnd()) {
        readData += serialPort->readAll();
    }
    
    QString str = RESPONSE_OK;
    str.append("\r\n");

    int index = 0;
    if((index = readData.indexOf(str, 0)) != -1)
    {
        //emit SerialOperationErrorSignal(readData);
        
        readData.remove(0, index + str.length());
        
        str = RESPONSE_VOLTAGE_INFO;
        if((index = readData.indexOf(str, 0)) != -1)
        {
            readData.remove(0, index + str.length());
            
            QStringList voltageList = QString(readData).split(QLatin1Char('|'));
            if(voltageList.size() == 2)
            {
                QString vccVoltage = voltageList[0].replace(QLatin1String("[VCC] "), QLatin1String(""), Qt::CaseSensitive);
                QString vppVoltage = voltageList[1].replace(QLatin1String("[VPP] "), QLatin1String(""), Qt::CaseSensitive);
                emit VoltageUpdatedSignal(vccVoltage.simplified().toDouble(), vppVoltage.simplified().toDouble());
            }
            else {
                emit SerialOperationErrorSignal("Invalid data received!");
            }
        }
        else {
            emit SerialOperationErrorSignal("Invalid response!");
        }
    }
    else {
        emit SerialOperationErrorSignal("Fail to read voltage!");
    }
    
    emit SerialOperationCompleteSignal();
}
//----------------------------------------------------------------------

void ArduinoIO::ReadEprom(void)
{
    readBuffer.clear();
    serialDataConnection = QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(ReadEpromSlot()));
    
    Send(COMMAND_READ_EPROM);
}
//----------------------------------------------------------------------

void ArduinoIO::ReadEpromSlot(void)
{
    QObject::disconnect(serialDataConnection);
    
    QByteArray readData;

    WaitForReadyRead(100);
    while(!serialPort->atEnd()) {
        readData += serialPort->readAll();
    }
    
    QString str = RESPONSE_OK;
    str.append("\r\n");

    int index = 0;
    if((index = readData.indexOf(str, 0)) != -1)
    {
        readData.remove(0, index + str.length());
        
        str = RESPONSE_READ_EPROM;
        str.append("\r\n");
        
        index = 0;
        if((index = readData.indexOf(str, 0)) != -1)
        {
            readData.remove(0, index + str.length());
            
            readBuffer.append(readData);
            
            //uint8_t *dataRead = reinterpret_cast<uint8_t *>(readBuffer.data());
            if(readBuffer.length() > 0) {
                emit ReadEpromDataSignal(0, readBuffer.length(), reinterpret_cast<uint8_t *>(readBuffer.data()));
            }
            
            int lastReadBufferLength = readBuffer.length();
            
            WaitForReadyRead(100);
            
            while(!serialPort->atEnd()) 
            {
                readData = serialPort->readAll();
                
                readBuffer.append(readData);
                emit ReadBlockSignal(readBuffer.length());
                if(readBuffer.length() - lastReadBufferLength > 0) {
                    emit ReadEpromDataSignal(lastReadBufferLength, readBuffer.length() - lastReadBufferLength, reinterpret_cast<uint8_t *>(readBuffer.data()));
                }
                lastReadBufferLength = readBuffer.length();
                
                WaitForReadyRead(100);
            }
        }
        else 
        {
            QString errorMessage = "Read fail!\nMessage: ";
            errorMessage.append(CleanErrorMessage(readData));
            emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
        }
    }
    else {
        emit SerialOperationErrorSignal("Communication error!");
    }
    
    if(readBuffer.length() >= maxBufferSize) {
        readBuffer.resize(maxBufferSize);
    }
    
    emit ReadCompleteSignal();
    emit SerialOperationCompleteSignal();
}
//----------------------------------------------------------------------

void ArduinoIO::HardwareTest(void)
{
    serialDataConnection = QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(HarwareTestSlot()));
    Send(COMMAND_HW_TEST);
}
//----------------------------------------------------------------------

void ArduinoIO::HarwareTestSlot(void)
{
    QObject::disconnect(serialDataConnection);
    
    QByteArray readData;

    WaitForReadyRead(250);
    while(!serialPort->atEnd()) {
        readData += serialPort->readAll();
    }
    
    QString str = RESPONSE_OK;
    str.append("\r\n");

    int index = 0;
    if((index = readData.indexOf(str, 0)) != -1)
    {
        readData.remove(0, index + str.length());
        
        str = RESPONSE_HW_TEST_ON;
        str.append("\r\n");
        
        index = 0;
        if((index = readData.indexOf(str, 0)) != -1)
        {
            readData.remove(0, index + str.length());
            
            WaitForReadyRead(1000);
            
            while(!serialPort->atEnd()) 
            {
                readData += serialPort->readAll();
                
                str = RESPONSE_HW_TEST_OFF;
                str.append("\r\n");
                
                index = 0;
                if((index = readData.indexOf(str, 0)) != -1) 
                {
                    readData.remove(readBuffer.length() ? index : 0, (readBuffer.length() ? 0 : index) + str.length());
                    emit HardwareTestStepSignal(readData);
                }
                else 
                {
                    //check for reset
                    str = PROGRAMMER_NAME;
                    index = 0;
                    if((index = readData.indexOf(str, 0)) != -1) {
                        break;
                    }
                    
                    emit HardwareTestStepSignal(readData);
                    
                    readData.clear();
                
                    WaitForReadyRead(5000);
                }
            }
        }
        else 
        {
            QString errorMessage = "Hardware test fail!\nMessage: ";
            errorMessage.append(CleanErrorMessage(readData));
            emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
        }
    }
    else {
        emit SerialOperationErrorSignal("Communication error!");
    }
    
    emit HardwareTestStepSignal(HW_TEST_END);
    emit SerialOperationCompleteSignal();
}
//----------------------------------------------------------------------

int ArduinoIO::WaitForReadyRead(int timeout)
{
    int counter = 0;
    while(!serialPort->waitForReadyRead(1) && ++counter < timeout);
    
    return serialPort->bytesAvailable();
}
//----------------------------------------------------------------------

void ArduinoIO::WriteEprom(double vcc, double vpp, PRG_ALGO algo)
{
    if(writeBuffer.length() != maxBufferSize)
    {
        QString errorMessage = "Invalid data length of ";
        errorMessage.append(QString::number(writeBuffer.length()));
        emit WriteCompleteSignal(false, errorMessage.toStdString().c_str());
        return;
    }

    programmingAlghoritm = algo;
    
    serialDataConnection = QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(WriteEpromSlot()));
    
    char command[1024];
    sprintf(command, COMMAND_WRITE_EPROM, vcc, vpp, algo);
    Send(command);
}
//----------------------------------------------------------------------

void ArduinoIO::WriteEpromSlot(void)
{
    QObject::disconnect(serialDataConnection);
    
    QByteArray readData;
    
    WaitForReadyRead(1000);
    while(!serialPort->atEnd()) {
        readData += serialPort->readAll();
    }
    
    QString str = RESPONSE_OK;
    str.append("\r\n");
    
    int index = 0;
    if((index = readData.indexOf(str, 0)) != -1)
    {
        readData.remove(0, index + str.length());
    
        str = RESPONSE_WRITE_EPROM;
        str.append("\r\n");
        
        WaitForReadyRead(250);
        readData.append(serialPort->readAll());
                
        index = 0;
        if((index = readData.indexOf(str, 0)) != -1)
        {
            readData.remove(0, index + str.length());
            
            bool writeCompleted = true;
            for(int i = 0; i < maxBufferSize; i += BLOCK_SIZE)
            {
                readData.append(serialPort->readAll());
                
                QString str = RESPONSE_WRITE_BLOCK;
                int index = 0, blockIndex = 0;
                if((index = readData.indexOf(str, 0)) != -1)
                {
                    readData.remove(0, index + str.length());
                    blockIndex = QString(readData).simplified().toInt();
                }
                else
                {
                    QString errorMessage = "Invalid write acknowledge!\nMessage: ";
                    errorMessage.append(CleanErrorMessage(readData));
                    emit WriteCompleteSignal(false, errorMessage.toStdString().c_str());
                    emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
                    writeCompleted = false;
                    break;
                }
                
                if(i != blockIndex)
                {
                    QString errorMessage = "Invalid block ";
                    errorMessage.append(QString::number(blockIndex, 16));
                    errorMessage.append(" received, expected ");
                    errorMessage.append(QString::number(i, 16));
                    emit WriteCompleteSignal(false, errorMessage.toStdString().c_str());
                    emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
                    writeCompleted = false;
                    break;
                }
                
                char data[BLOCK_SIZE];
                memcpy(data, &writeBuffer.data()[i], BLOCK_SIZE);
                serialPort->write(data, BLOCK_SIZE);
                
                WaitForReadyRead(5000);
                
                readData.clear();
                readData.append(serialPort->readAll());
                
                str = RESPONSE_ERROR;

                if((index = readData.indexOf(str, 0)) != -1)
                {
                    WaitForReadyRead(250);
                    readData.append(serialPort->readAll());
                    emit WriteCompleteSignal(false, readData.data());
                    emit SerialOperationErrorSignal(readData.data());
                    writeCompleted = false;
                    break;
                }

                str = RESPONSE_OK;
                str.append("\r\n");

                if((index = readData.indexOf(str, 0)) == -1)
                {
                    QString errorMessage = "Can't acknowledge block ";
                    errorMessage.append(QString::number(i, 16));
                    emit WriteCompleteSignal(false, readData.data());
                    emit SerialOperationErrorSignal(readData.data());
                    writeCompleted = false;
                    break;
                }
                readData.remove(0, index + str.length());

                emit WriteBlockSignal(i);
            }
            
            if(writeCompleted) 
            {
                WaitForReadyRead(1000);
                readData.append(serialPort->readAll());

                emit WriteBlockSignal(maxBufferSize);
                emit WriteCompleteSignal(true, nullptr);
            }
        }
        else 
        {
            QString errorMessage = "Write fail!\nMessage: ";
            errorMessage.append(CleanErrorMessage(readData));
            emit WriteCompleteSignal(false, errorMessage.toStdString().c_str());
            emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
        }
    }
    else 
    {
        QString errorMessage = "Write error!\nMessage: ";
        errorMessage.append(CleanErrorMessage(readData));
        emit WriteCompleteSignal(false, errorMessage.toStdString().c_str());
        emit SerialOperationErrorSignal(errorMessage.toStdString().c_str());
    }
    
    emit SerialOperationCompleteSignal();
}
//----------------------------------------------------------------------

QString ArduinoIO::CleanErrorMessage(QByteArray errorMessage)
{
    QString message = QString(errorMessage.data());
    message.replace(RESPONSE_ERROR, "");
    message.replace(RESPONSE_WRITE_EPROM, "");
    return message;
}
//----------------------------------------------------------------------
