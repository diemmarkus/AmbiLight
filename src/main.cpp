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

#include <QCoreApplication>
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QElapsedTimer>
#include <QCommandLineParser>

#include "AmbiLight.h"

// TODO: remove
#include "GrabScreen.h"
#include "DkArduinoController.h"

int main(int argc, char *argv[]) {

	QCoreApplication::setApplicationName("AmbiLight");
	QCoreApplication::setOrganizationName("TU Wien");




	QApplication app(argc, (char**)argv);

	// CMD parser --------------------------------------------------------------------
	QCommandLineParser parser;

	//parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();

	// set com port
	QCommandLineOption comOpt(QStringList() << "c" << "comport",
		QObject::tr("Open <com> com port."),
		QObject::tr("com"));
	parser.addOption(comOpt);

	parser.process(app);
	// CMD parser --------------------------------------------------------------------


	// start
	al::AmbientLight* ambiLight = new al::AmbientLight();


	// set COM port
	if (!parser.value(comOpt).isEmpty()) {
		al::ArduinoController* controller = ambiLight->controller();
		controller->setComPort(parser.value(comOpt));
	}

	ambiLight->start();
	app.exec();
	delete ambiLight;

	// debug ------------------------------
	al::ScreenRecorder sr;
	
	QElapsedTimer et;
	et.start();
	QColor col = sr.screenColor();
	qDebug() << "color taken in" << et.elapsed() << "ms";

	QPixmap pm = sr.debugPixmap();
	pm.save("C:/temp/josef.png");
	
	return 0;
}
