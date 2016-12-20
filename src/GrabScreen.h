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
#pragma once

#pragma warning(push, 0)	// no warnings from includes
#include <QScreen>
#include <QGuiApplication>
#include <QColor>
#pragma warning(pop)

#include "KMeans.h"

// Qt defines

namespace al {

class ScreenRecorder {

public:
	ScreenRecorder(QScreen* screen = QGuiApplication::primaryScreen());

	QPixmap pixmap() const;
	QColor screenColor() const;
	QPixmap debugPixmap() const;

private:
	QScreen* mScreen = 0;

	QColor findDominantColor(const QPixmap& pm) const;
	QVector<QColor> saturatedColors(const QImage& pm) const;
	QVector<kmeans::Point> toPoints(const QVector<QColor>& cols) const;
};

};