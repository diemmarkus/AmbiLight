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

#include "KMeans.h"

#include <cassert>

#pragma warning(push, 0)	// no warnings from includes
#include <QDebug>
#include <QElapsedTimer>
#pragma warning(pop)

// k-means --------------------------------------------------------------------
namespace kmeans {

	// A data point --------------------------------------------------------------------
	Point::Point(int nDims) {
		mVals.resize(nDims);

		for (auto v : mVals)
			v = 0;
	}

	void Point::operator+=(const Point & p) {

		for (int idx = 0; idx < mVals.size(); idx++)
			mVals[idx] += p.mVals[idx];

	}

	void Point::setColor(const QColor& col) {
		
		// not legal
		if (mVals.size() < 3)
			return;
		
		mVals[red] = col.red();
		mVals[green] = col.green();
		mVals[blue] = col.blue();
	}

	QColor Point::color() const {

		if (mVals.size() < 3)
			return QColor();

		QColor col;
		col.setRed(qRound(mVals[0]));
		col.setGreen(qRound(mVals[1]));
		col.setBlue(qRound(mVals[2]));

		return col;
	}

	void Point::setGroup(int g) {
		mGroup = g;
	}

	int Point::group() const {
		return mGroup;
	}

	double Point::dist(const Point & pt) const {
		
		// squared euclidean distance
		double d = 0.0;
		for (int idx = 0; idx < mVals.size(); idx++) {
			double diff = mVals[idx] - pt.mVals[idx];
			d += diff * diff;
		}
		
		//qDebug() << "dist^2:" << std::sqrt(d);
		return std::sqrt(d);
	}

	bool Point::update(const QVector<Center> & centers) {

		double minD = DBL_MAX;
		int cIdx = -1;

		for (int idx = 0; idx < centers.size(); idx++) {
			
			double d = dist(centers[idx]);
			if (d < minD) {
				minD = d;
				cIdx = idx;
			}
		}

		if (cIdx != -1 && mGroup != cIdx) {
			mGroup = cIdx;
			return true;
		}

		return false;
	}

	QString Point::toString() const {
		QString msg = "group: " + QString::number(mGroup) + " vals: ";
		for (auto v : mVals)
			msg += QString::number(v) + ", ";

		return msg;
	}

	// Center --------------------------------------------------------------------
	Center::Center() {
	}

	void Center::addPoint(const Point & p) {
		mGroup++;	// group is used as counter
		(*this) += p;
	}

	int Center::count() const {
		return group();
	}

	void Center::normalize() {
		
		int s = count();

		if (s == 0)
			return;

		for (double& v : mVals)
			v /= s;
	}

	// KMeans clustering --------------------------------------------------------------------
	KMeans::KMeans(const Points & pts) {
		mPts = pts;
	}

	Points KMeans::points() const {
		return mPts;
	}

	Centers KMeans::centers() const {
		return mCenters;
	}

	Center KMeans::dominantCenter() const {
		
		double maxElements = 0;
		Center dCenter;

		for (Center c : mCenters) {

			if (c.count() > maxElements) {
				dCenter = c;
				maxElements = c.count();
			}
		}
		
		return dCenter;
	}

	void KMeans::cluster(int numClusters) {
		mCenters = cluster(mPts, numClusters);
	}

	// k-means clustering --------------------------------------------------------------------
	Centers KMeans::cluster(Points& pts, int numClusters) const {

		// parameters
		double kEps = pts.size() *0.01;

		// init
		for (int idx = 0; idx < pts.size(); idx++) {
			pts[idx].setGroup(idx % numClusters);
		}

		int numChanged = 0;
		Centers fCenters;

		do {
			// update centers
			Centers centers;
			centers.resize(numClusters);

			for (Point& p : pts) {
				assert(p.group() >= 0 && p.group() < centers.size());
				centers[p.group()].addPoint(p);
			}

			// normalize centers
			for (Center& c : centers)
				c.normalize();

			// update points
			numChanged = 0;
			for (Point& p : pts) {
				bool changed = p.update(centers);

				if (changed) {
					numChanged++;
				}
			}

			//qDebug() << "#changed/# pts: " << numChanged << "/" << pts.size();
			fCenters = centers;

		} while (numChanged > kEps);

		return fCenters;
	}

}
