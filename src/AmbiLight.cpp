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

#include "AmbiLight.h"
#include "ArduinoController.h"

#include <cassert>

#pragma warning(push, 0)	// no warnings from includes
#include <QObject>
#include <QDebug>
#pragma warning(pop)

namespace al {

//  --------------------------------------------------------------------
AmbientLight::AmbientLight(QObject* parent) : QObject(parent) {

	mController = new ArduinoController();

	mUpdateTimer.setInterval(250);
	mUpdateTimer.setSingleShot(false);
	connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

}

ArduinoController * AmbientLight::controller() const {
	return mController;
}

void AmbientLight::start() {
	mUpdateTimer.start();

	if (mController)
		mController->initComPort();
}

void AmbientLight::update() {

	// get current screen color
	QColor col = mRecorder.screenColor();

	//// update AmbiColor
	if (col != mLastColor) {

		qDebug() << "new color: " << col.name();
		mController->setColor(col);
		mLastColor = col;
	}
}

}