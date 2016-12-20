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

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QThread>
#include <QColor>
#pragma warning(pop)		// no warnings from includes - end

#include <windows.h>	// needed to read from serial

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

namespace al {

class DllExport ArduinoController {

public:
	ArduinoController();
	~ArduinoController();

	void setComPort(const QString& cP) { comPort = cP; };
	void setColor(const QColor & col);

	void initComPort();
	
protected:
	HANDLE hCOM;

	QColor mColor;
	QString comPort;

	void printComParams(const DCB& dcb) const;

	void readSettings();
	void writeSettings() const;

};

};