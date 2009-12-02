/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: RInterface.cxx.cxx,v $
  
-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkRInterface.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkRRandomTableSource.h>
#include <vtkArray.h>
#include <vtkTable.h>
#include <vtkDenseArray.h>

#include <vtksys/ios/iostream>
#include <vtksys/ios/sstream>
#include <vtksys/stl/stdexcept>
#include <stdio.h>
#include <string.h>

namespace
{

#define test_expression(expression) \
{ \
  if(!(expression)) \
    { \
    vtksys_ios::ostringstream buffer; \
    buffer << "Expression failed at line " << __LINE__ << ": " << #expression; \
    throw vtkstd::runtime_error(buffer.str()); \
    } \
}

bool doubleEquals(double left, double right, double epsilon) {
  return (fabs(left - right) < epsilon);
}

}

int TestRInterface(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  try
    {
    int buf_size = 2000;
    char* buffer = new char[buf_size];
    buffer[0] = '\0';
    vtkDoubleArray* da = vtkDoubleArray::New();
    vtkDenseArray<double>* dda = vtkDenseArray<double>::New();
    vtkRRandomTableSource* rts = vtkRRandomTableSource::New();
    vtkRInterface* rint = vtkRInterface::New();

    rint->OutputBuffer(buffer, buf_size);
    rint->EvalRscript("1:10\n");
    cout << buffer << endl;
    test_expression(strlen(buffer) > 10);



    delete [] buffer;
    rts->Delete();
    dda->Delete();
    da->Delete();
    rint->Delete();
    return 0;
    }
  catch(vtkstd::exception& e)
    {
    cerr << e.what() << endl;
    return 1;
    }
}

