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
// .NAME vtkHierarchicalDataLevelFilter - generate scalars from levels
// .SECTION Description
// Legacy class. Use vtkMultiGroupDataGroupIdScalars instead.
//
// .SECTION See Also
// vtkMultiGroupDataGroupIdScalars

#ifndef __vtkHierarchicalDataLevelFilter_h
#define __vtkHierarchicalDataLevelFilter_h

#include "vtkMultiGroupDataGroupIdScalars.h"

class VTK_GRAPHICS_EXPORT vtkHierarchicalDataLevelFilter : public vtkMultiGroupDataGroupIdScalars 
{
public:
  vtkTypeRevisionMacro(vtkHierarchicalDataLevelFilter,vtkMultiGroupDataGroupIdScalars);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with PointIds and CellIds on; and ids being generated
  // as scalars.
  static vtkHierarchicalDataLevelFilter *New();

protected:
  vtkHierarchicalDataLevelFilter();
  ~vtkHierarchicalDataLevelFilter();

private:
  vtkHierarchicalDataLevelFilter(const vtkHierarchicalDataLevelFilter&);  // Not implemented.
  void operator=(const vtkHierarchicalDataLevelFilter&);  // Not implemented.
};

#endif


