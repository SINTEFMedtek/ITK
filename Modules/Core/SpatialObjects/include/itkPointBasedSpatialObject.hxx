/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkPointBasedSpatialObject_hxx
#define itkPointBasedSpatialObject_hxx


#include "itkPointBasedSpatialObject.h"

namespace itk
{
/** Constructor */
template< unsigned int TDimension, class TSpatialObjectPointType >
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::PointBasedSpatialObject():
  SpatialObject< TDimension >()
{
  this->SetTypeName("PointBasedSpatialObject");
  m_Points.clear();
}

/** Print the object */
template< unsigned int TDimension, class TSpatialObjectPointType >
void
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::SetPoints( SpatialObjectPointListType & newPoints )
{
  m_Points.clear();

  auto it = newPoints.begin();
  while( it != newPoints.end() )
    {
    it->SetSpatialObject( this );
    m_Points.push_back( *it );
    ++it;
    }

  this->Modified();
}

/** Determine closest point in world space */
template< unsigned int TDimension, class TSpatialObjectPointType >
SpatialObjectPointType *
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::ClosestPoint( const PointType & point )
{
  auto it = m_Points.begin();
  auto itend = m_Points.end();

  if ( it == itend )
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription(
      "SpatialObject: ClosestPoint called using an empty point list");
    throw exception;
    }

  SpatialObjectPointType closestPoint = 0;
  double closestPointDistance = NumericTraits< double >::max();
  while ( it != itend )
    {
    typename SpatialObjectPoint< TDimension >::PointType curpos =
      this->GetObjectToWorldTransform()->TransformPoint( ( *it ).GetPosition() );
    double curdistance = curpos.EuclideanDistanceTo(curPoint);
    if ( curdistance < closestPointDistance )
      {
      closestPoint = &(*it);
      closestPointDistance = curdistance;
      }
    it++;
    }

  return closestPoint;
}

/** Compute bounding box of just this object in world space */
template< unsigned int TDimension, class TSpatialObjectPointType >
void
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::ComputeMyBoundingBox()
{
  itkDebugMacro("Computing blob bounding box");

  auto it  = m_Points.begin();
  auto end = m_Points.end();

  if ( it == end )
    {
    return false;
    }

  PointType pt = ( *it ).GetPosition();

  // Compute a bounding box in object space
  typename BoundingBoxType::Pointer bb = BoundingBoxType::New();

  bb->SetMinimum(pt);
  bb->SetMaximum(pt);
  it++;
  while ( it != end )
    {
    bb->ConsiderPoint( ( *it ).GetPosition() );
    it++;
    }
  bb->ComputeBoundingBox();

  // Next Transform the corners of the bounding box into world space
  using PointsContainer = typename BoundingBoxType::PointsContainer;
  const PointsContainer *corners = bb->GetCorners();
  typename PointsContainer::Pointer transformedCorners =
    PointsContainer::New();
  transformedCorners->Reserve(
    static_cast<typename PointsContainer::ElementIdentifier>(
      corners->size() ) );

  auto it = corners->begin();
  auto itTrans = transformedCorners->begin();
  while ( it != corners->end() )
    {
    PointType pnt = this->GetObjectToWorldTransform()->TransformPoint(*it);
    *itTrans = pnt;
    ++it;
    ++itTrans;
    }

  // refresh the object's bounding box with the transformed corners
  const_cast< BoundingBoxType * >( this->GetMyBoundingBox() )
    ->SetPoints(transformedCorners);
  this->GetMyBoundingBox()->ComputeBoundingBox();

  return true;
}

/** Test if a world-coordinate point is inside of this object or its
 *    children, if they match the search depth and name */
template< unsigned int TDimension, class TSpatialObjectPointType >
bool
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::IsInside( const PointType & point, unsigned int depth,
    const std::string & name) const
{
  if( this->GetTypeName.find( name ) != std::string::npos )
    {
    if( this->GetMyBoundingBox()->IsInside( point ) )
      {
      auto it = m_Points.begin();
      auto itEnd = m_Points.end();

      PointType transformedPoint =
        this->GetObjectToWorldTransform()->GetInverseTransform()->
          TransformPoint(point);

      while ( it != itEnd )
        {
        bool equals = true;
        for( unsigned int i=0; i<ObjectDimension; ++i )
          {
          if( ! Math::AlmostEquals( transformedPoint[i],
              it->GetPosition()[i] ) )
            {
            equals = false;
            break;
            }
          }
        if( equals )
          {
          return true;
          }
        it++;
        }
      }
    }

  if( depth > 0 )
    {
    return Superclass::IsInsideChildren(point, depth-1, name);
    }

  return false;
}


/** Print the object */
template< unsigned int TDimension, class TSpatialObjectPointType >
void
PointBasedSpatialObject< TDimension, TSpatialObjectPointType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  os << indent << "PointBasedSpatialObject(" << this << ")" << std::endl;
  os << indent << "Number of points: " << m_Points.size() << std::endl;
  Superclass::PrintSelf(os, indent);
}
} // end namespace itk

#endif // end itkPointBasedSpatialObject_hxx