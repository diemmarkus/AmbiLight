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

#pragma once

#pragma warning(push, 0)	// no warnings from includes
#include <QColor>
#include <QVector>
#pragma warning(pop)

// Qt defines

// read defines
namespace kmeans {

	class Center;

	class Point {

	public:
		Point(int nDims = 3);

		void operator+=(const Point& p);

		void setColor(const QColor& col);
		QColor color() const;

		void setGroup(int g);
		int group() const;

		double dist(const Point& pt) const;
		bool update(const QVector<Center>& centers);

		QString toString() const;

		enum ColorIndex {
			red,
			green,
			blue
		};

	protected:
		QVector<double> mVals;
		int mGroup = 0;
	};

	class Center : public Point {

	public:
		Center();

		void addPoint(const Point& p);
		int count() const;

		void normalize();

	};

	typedef QVector<Point> Points;
	typedef QVector<Center> Centers;

	class KMeans {
	
	public:
		KMeans(const Points& pts = Points());

		void cluster(int numClusters = 10);

		Points points() const;
		Centers centers() const;
		Center dominantCenter() const;

	private:
		Points mPts;
		Centers mCenters;

		Centers cluster(Points& pts, int numClusters) const;
	};

};