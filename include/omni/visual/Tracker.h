/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_VISUAL_TRACKER_H_
#define OMNI_VISUAL_TRACKER_H_ 

#include <omni/PolarVec.h>

namespace omni
{
  namespace visual
  {
    struct Tracker
    {
      Tracker();
      Tracker(const QVector3D& _center, const PolarVec& _direction);
      
      void track( float _longitude, float _latitude, float _radius );
      
      /// get tracker position
      QVector3D eye() const;

      void setEye( const QVector3D& _pos );

      void setCenter(QVector3D const&);
      QVector3D& center();
      QVector3D const& center() const;
      
      void setDirection(PolarVec const&);
      
      /// Set direction with a certain distance
      void setDistance(float _t); 

      PolarVec& direction();
      PolarVec const& direction() const;

      /// Keep distance in limits
      void limitDistance(float _minDist, float _maxDist);

    private:
      /// target position
      QVector3D center_;
      
      /// tracker's postion relatively to target
      PolarVec direction_; 
    };
  }
}

#endif /* OMNI_VISUAL_TRACKER_H_ */
