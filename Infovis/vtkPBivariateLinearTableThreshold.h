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
/*-------------------------------------------------------------------------
  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
#ifndef __vtkPBivariateLinearTableThreshold__h
#define __vtkPBivariateLinearTableThreshold__h

#include "vtkBivariateLinearTableThreshold.h"

class vtkIdTypeArray;
class vtkMultiProcessController;

class VTK_INFOVIS_EXPORT vtkPBivariateLinearTableThreshold : public vtkBivariateLinearTableThreshold
{
public:
  static vtkPBivariateLinearTableThreshold* New();
  vtkTypeRevisionMacro(vtkPBivariateLinearTableThreshold, vtkBivariateLinearTableThreshold);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual void SetController(vtkMultiProcessController*);
  vtkGetObjectMacro(Controller,vtkMultiProcessController);
protected:
  vtkPBivariateLinearTableThreshold();
  virtual ~vtkPBivariateLinearTableThreshold();

  virtual int RequestData(
    vtkInformation*,
    vtkInformationVector**,
    vtkInformationVector*);

  vtkMultiProcessController* Controller;
private:
  vtkPBivariateLinearTableThreshold(const vtkPBivariateLinearTableThreshold&); // Not implemented
  void operator=(const vtkPBivariateLinearTableThreshold&); // Not implemented
};

#endif
