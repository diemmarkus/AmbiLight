/*******************************************************************************************************
 AmbiLight is the cheapjack version of Philips TV lighting system.
  
 Copyright (C) 2016 Markus Diem <diem@caa.tuwien.ac.at>
 Copyright (C) 2016 Stefan Fiel <fiel@caa.tuwien.ac.at>
 Copyright (C) 2016 Florian Kleber <kleber@caa.tuwien.ac.at>

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

#include "GrabScreen.h"

#include <cassert>

#pragma warning(push, 0)	// no warnings from includes
#include <QPixmap>
#include <QDebug>
#include <QWindow>
#include <QPainter>
#include <QElapsedTimer>
#pragma warning(pop)

namespace al {
	
ScreenRecorder::ScreenRecorder(QScreen * screen) {
	mScreen = screen;
}

QPixmap ScreenRecorder::pixmap() const {

	if (!mScreen) {
		qWarning() << "could not find a screen to shoot...";
		return QPixmap();
	}

	QPixmap pm = mScreen->grabWindow(0);
	
	return pm;
}

QColor ScreenRecorder::screenColor() const {
	
	QPixmap pm = pixmap();
	QColor col = findDominantColor(pm);

	return col;
}

QPixmap ScreenRecorder::debugPixmap() const {

	QPixmap pm = pixmap();
	QSize maxSize(100, 100);
	pm = pm.scaled(maxSize, Qt::KeepAspectRatio);

	QColor col = findDominantColor(pm);

	QPainter p(&pm);
	p.setPen(Qt::NoPen);
	p.setBrush(col);
	p.drawRect(QRect(QPoint(), QSize(10, 10)));

	return pm;
}

QColor ScreenRecorder::findDominantColor(const QPixmap & pm) const {

	// function parameters
	QSize maxSize(100, 100);

	QPixmap pms = pm;
	pms = pms.scaled(maxSize, Qt::KeepAspectRatio, Qt::FastTransformation);

	QVector<QColor> cols = saturatedColors(pms.toImage());
	
	// cluster
	kmeans::KMeans km(toPoints(cols));
	km.cluster(2);

	//qDebug() << "dominant center" << km.dominantCenter().toString();

	return km.dominantCenter().color();
}

QVector<QColor> ScreenRecorder::saturatedColors(const QImage & img) const {


	// params
	int minSaturation = 100;
	int minValue = 100;	// is needed for black pixels might have a high saturation

	QVector<QColor> colors;

	for (int rIdx = 0; rIdx < img.height(); rIdx++) {
		const QRgb* bits = (const QRgb*)img.scanLine(rIdx);

		for (int cIdx = 0; cIdx < img.width(); cIdx++) {
		
			// add only 'colors' (no graytones)
			QColor c = bits[cIdx];
			if (c.saturation() > minSaturation && c.value() > minValue)
				colors << c;
		}
	}

	//qDebug() << "initial/saturated colors:" << (img.width() * img.height()) << "/" << colors.size();

	return colors;
}

QVector<kmeans::Point> ScreenRecorder::toPoints(const QVector<QColor>& cols) const {
	
	QVector<kmeans::Point> pts;
	pts.reserve(cols.size());

	for (auto c : cols) {
		kmeans::Point p;
		p.setColor(c);
		pts << p;
	}
	
	return pts;
}

}