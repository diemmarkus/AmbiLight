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

class DllExport DkArduinoController : public QThread {
	Q_OBJECT

public:
	DkArduinoController(QObject* widget = 0);
	~DkArduinoController();

	void setComPort(const QString& cP) { comPort = cP; };
	void setColor(const QColor & col);

	void initComPort();
	void run();
	void quit() {
		stop = true;
	};
	
signals:
	void controllerSignal(int controller, int value) const;

protected:
	HANDLE hCOM;

	QColor mColor;
	bool mWriteColor = false;
	QString comPort;
	bool stop;

	void init();
	void serialValue(unsigned short val) const;
	void printComParams(const DCB& dcb) const;

	void readSettings();
	void writeSettings() const;

};

};