
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkRInterface.h,v $

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

// .NAME vtkRInterface
//
// .SECTION Description
//
// This class defines a VTK interface to an embedded GNU R intepreter instance.
//
// .SECTION See Also
//  vtkRadapter vtkRcalculatorFilter
//
// .SECTION Thanks
//  Developed by Thomas Otahal at Sandia National Laboratories.
//


#ifndef __vtkRInterface_h
#define __vtkRInterface_h

#include "vtkObject.h"

class vtkDataArray;
class vtkArray;
class vtkTable;
class R_Singleton;

class VTK_GRAPHICS_EXPORT vtkRInterface : public vtkObject
{
public:

  static vtkRInterface* New();
  vtkTypeRevisionMacro(vtkRInterface,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  int EvalRcommand(const char *commandName, int param);

  int EvalRscript(const char *string, bool showRoutput = true);

  int OutputBuffer(char* p, int n);

  void AssignVTKDataArrayToRVariable(vtkDataArray* da, const char* RVariableName);

  void AssignVTKArrayToRVariable(vtkArray* da, const char* RVariableName);

  vtkDataArray* AssignRVariableToVTKDataArray(const char* RVariableName);

  vtkArray* AssignRVariableToVTKArray(const char* RVariableName);

  vtkTable* AssignRVariableToVTKTable(const char* RVariableName);

  void AssignVTKTableToRVariable(vtkTable* table, const char* RVariableName);

protected:
  vtkRInterface();
  ~vtkRInterface();

private:
  int FillOutputBuffer();
  vtkRInterface(const vtkRInterface&); // Not implemented
  void operator=(const vtkRInterface&);   // Not implemented

  R_Singleton* rs;
  vtkstd::string tmpFilePath;
  char* buffer;
  int buffer_size;

};

#endif

