/*******************************************************************************************************
AmbiLight is the cheapjack version of Philips TV lighting system. 

Copyright (C) 2016 Markus Diem <diem@caa.tuwien.ac.at>

This file is part of AmbiLight.

AmbiLight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

AmbiLight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

related links:
[1] http://nomacs.org
*******************************************************************************************************/

#include <QCoreApplication>
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QElapsedTimer>

#include "GrabScreen.h"

int main(int argc, char *argv[]) {

	QCoreApplication::setApplicationName("AmbiLight");
	QCoreApplication::setOrganizationName("TU Wien");

	QApplication a(argc, (char**)argv);
	//a.exec();

	al::ScreenRecorder sr;
	
	QElapsedTimer et;
	et.start();
	QColor col = sr.screenColor();
	qDebug() << "color taken in" << et.elapsed() << "ms";

	QPixmap pm = sr.debugPixmap();
	pm.save("C:/temp/josef.png");
	
	return 0;
}
