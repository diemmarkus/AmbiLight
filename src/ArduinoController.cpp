/*******************************************************************************************************
 AmbiLight is the cheapjack version of Philips TV lighting system.
  
 Copyright (C) 2016 Markus Diem <diem@caa.tuwien.ac.at>

 This file is part of AmbiLight.

 AmbiLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AmbiLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 related links:
 [1] http://nomacs.org
 *******************************************************************************************************/

#include "ArduinoController.h"

#include "Utils.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QSettings>
#include <QDebug>
#include <QWidget>
#include <QColor>
#pragma warning(pop)		// no warnings from includes - end

namespace al {

// ArduinoController --------------------------------------------------------------------
ArduinoController::ArduinoController() {
	comPort = "COM5";
	readSettings();
}

ArduinoController::~ArduinoController() {
	writeSettings();
}

void ArduinoController::readSettings() {
	
	QSettings& settings = Settings::instance().getSettings();
	
	settings.beginGroup("ArduinoController");
	comPort = settings.value("comPort", comPort).toString();
	settings.endGroup();
	
}

void ArduinoController::writeSettings() const {

	QSettings& settings = Settings::instance().getSettings();
	
	settings.beginGroup("ArduinoController");
	settings.setValue("comPort", comPort);
	settings.endGroup();

	qDebug() << "[ArduinoController] settings written...";
}

void ArduinoController::initComPort() {

	std::wstring comPortStd = DkUtils::qStringToStdWString(comPort);
	hCOM = CreateFileW((LPCWSTR)comPortStd.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hCOM == INVALID_HANDLE_VALUE) {
		qDebug() << "hCOM " << comPort << " is NULL....";
		return;
	}
	else {
		qDebug() << comPort << "is up and running...\n";
	}

	DCB dcb;

	FillMemory(&dcb, sizeof(dcb), 0);
	if (!GetCommState(hCOM, &dcb))     // get current DCB
		qDebug() << "error in get COM state...";

	//qDebug() << "dcb defaults ------------------------";
	//printComParams(dcb);

	// set params for serial port communication
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	//dcb.EofChar = 0xFE;		// þ - should be that... (small letter thorn)
	//dcb.ErrorChar = 0x01;		// SOH (start of heading)
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fTXContinueOnXoff = 1;
	dcb.XoffLim = 512;
	dcb.XonLim = 2048;

	// Set new state.
	if (!SetCommState(hCOM, &dcb))
		qDebug() << "cannot set com state!";

	if (!GetCommState(hCOM, &dcb))     // get current DCB
		qDebug() << "error in get COM state...";

	//qDebug() << "\n\ndcb our params ------------------------";
	//printComParams(dcb);

	// clear all operations that were performed _before_ we started...
	PurgeComm(hCOM, PURGE_RXCLEAR);
}

void ArduinoController::setColor(const QColor & col) {
	
	mColor = col;

	DWORD written;
	byte magicByteW = 11;
	WriteFile(hCOM, &magicByteW, sizeof(magicByteW), &written, NULL);
	byte red = col.red();
	WriteFile(hCOM, &red, sizeof(red), &written, NULL);
	
	byte green = col.green();
	WriteFile(hCOM, &magicByteW, sizeof(magicByteW), &written, NULL);
	WriteFile(hCOM, &green, sizeof(green), &written, NULL);
	
	byte blue = col.blue();
	WriteFile(hCOM, &magicByteW, sizeof(magicByteW), &written, NULL);
	WriteFile(hCOM, &blue, sizeof(blue), &written, NULL);

	qDebug() << "new color set";
}

void ArduinoController::printComParams(const DCB& dcb) const {

	qDebug() << "BaudRate" << dcb.BaudRate;
	qDebug() << "ByteSize"<< dcb.ByteSize;
	qDebug() << "DCBlength" << dcb.DCBlength;
	qDebug() << "EofChar" << dcb.EofChar;
	qDebug() << "ErrorChar" << dcb.ErrorChar;
	qDebug() << "EvtChar" << dcb.EvtChar;
	qDebug() << "fAbortOnError" << dcb.fAbortOnError;
	qDebug() << "fBinary" << dcb.fBinary;
	qDebug() << "fDsrSensitivity" << dcb.fDsrSensitivity;
	qDebug() << "fDtrControl" << dcb.fDtrControl;
	qDebug() << "fDummy2" << dcb.fDummy2;
	qDebug() << "fErrorChar" << dcb.fErrorChar;
	qDebug() << "fInX" << dcb.fInX;
	qDebug() << "fNull" << dcb.fNull;
	qDebug() << "fOutX" << dcb.fOutX;
	qDebug() << "fOutxCtsFlow" << dcb.fOutxCtsFlow;
	qDebug() << "fOutxDsrFlow" << dcb.fOutxDsrFlow;
	qDebug() << "fParity" << dcb.fParity;
	qDebug() << "fRtsControl" << dcb.fRtsControl;
	qDebug() << "fTXContinueOnXoff" << dcb.fTXContinueOnXoff;
	qDebug() << "Parity" << dcb.Parity;
	qDebug() << "StopBits" << dcb.StopBits;
	qDebug() << "wReserved" << dcb.wReserved;
	qDebug() << "wReserved1" << dcb.wReserved1;
	qDebug() << "XoffChar" << dcb.XoffChar;
	qDebug() << "XoffLim" << dcb.XoffLim;
	qDebug() << "XonChar" << dcb.XonChar;
	qDebug() << "XonLim" << dcb.XonLim;
}

}