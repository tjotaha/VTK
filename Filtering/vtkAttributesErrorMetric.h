/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkAttributesErrorMetric -  Objects that compute
// attribute-based error during cell tessellation.
//
// .SECTION Description
// It is a concrete error metric, based on an attribute criterium:
// the variation of the active attribute/component value from a linear ramp
//
// .SECTION See Also
// vtkGenericCellTessellator vtkGenericSubdivisionErrorMetric

#ifndef __vtkAttributesErrorMetric_h
#define __vtkAttributesErrorMetric_h

#include "vtkGenericSubdivisionErrorMetric.h"

class vtkGenericAttributeCollection;
class vtkGenericDataSet;

class VTK_FILTERING_EXPORT vtkAttributesErrorMetric : public vtkGenericSubdivisionErrorMetric
{
public:
  // Description:
  // Construct the error metric with a default relative attribute accuracy
  // equal to 0.1.
  static vtkAttributesErrorMetric *New();
  
  // Description:
  // Standard VTK type and error macros.
  vtkTypeRevisionMacro(vtkAttributesErrorMetric,vtkGenericSubdivisionErrorMetric);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Relative tolerance of the active scalar (attribute+component).
  // Subdivision is required if the square distance between the real attribute
  // at the mid point on the edge and the interpolated attribute is greater
  // than AttributeTolerance.
  // This is the attribute accuracy.
  // 0.01 will give better result than 0.1.
  // \pre valid_range_value: value>0 && value<1
  vtkGetMacro(AttributeTolerance, double);
  vtkSetClampMacro(AttributeTolerance, double, 0.0, 1.0);

  // Description:
  // Does the edge need to be subdivided according to the distance between
  // the value of the active attribute/component at the midpoint and the mean
  // value between the endpoints?
  // The edge is defined by its `leftPoint' and its `rightPoint'.
  // `leftPoint', `midPoint' and `rightPoint' have to be initialized before
  // calling NeedEdgeSubdivision().
  // Their format is global coordinates, parametric coordinates and
  // point centered attributes: xyx rst abc de...
  // `alpha' is the normalized abscissa of the midpoint along the edge.
  // (close to 0 means close to the left point, close to 1 means close to the
  // right point)
  // \pre leftPoint_exists: leftPoint!=0
  // \pre midPoint_exists: midPoint!=0
  // \pre rightPoint_exists: rightPoint!=0
  // \pre clamped_alpha: alpha>0 && alpha<1
  // \pre valid_size: sizeof(leftPoint)=sizeof(midPoint)=sizeof(rightPoint)
  //          =GetAttributeCollection()->GetNumberOfPointCenteredComponents()+6
  int NeedEdgeSubdivision(double *leftPoint,
                          double *midPoint,
                          double *rightPoint,
                          double alpha);
  
protected:
  vtkAttributesErrorMetric();
  virtual ~vtkAttributesErrorMetric();
  
  // Description:
  // Compute the absolute attribute tolerance, only if the cached value is
  // obsolete.
  void ComputeAbsoluteAttributeTolerance();
  
  double AttributeTolerance;
  
  double AbsoluteAttributeTolerance; // cached value computed from
  // AttributeTolerance and active attribute/component
  vtkTimeStamp AbsoluteAttributeToleranceComputeTime;
  
  vtkGenericAttributeCollection *AttributeCollection;

private:
  vtkAttributesErrorMetric(const vtkAttributesErrorMetric&);  // Not implemented.
  void operator=(const vtkAttributesErrorMetric&);  // Not implemented.
};

#endif

