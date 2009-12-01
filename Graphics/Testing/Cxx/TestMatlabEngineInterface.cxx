/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: MatlabEngineInterface.cxx,v $
  
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

#include <vtkMatlabEngineInterface.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include "vtkArray.h"

#include <vtksys/ios/iostream>
#include <vtksys/ios/sstream>
#include <vtksys/stl/stdexcept>

#define test_expression(expression) \
{ \
  if(!(expression)) \
    { \
    vtksys_ios::ostringstream buffer; \
    buffer << "Expression failed at line " << __LINE__ << ": " << #expression; \
    throw vtkstd::runtime_error(buffer.str()); \
    } \
}

int TestMatlabEngineInterface(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  try
    {
    char buffer[2048];
    int i;
    int j;
    int k;

	vtkDoubleArray* arr = vtkDoubleArray::New();
    vtkDoubleArray* marr; 
    arr->SetName("NumList");
    for(i=0;i<100;i++)
      arr->InsertNextValue(i);

	vtkMatlabEngineInterface* ei = vtkMatlabEngineInterface::New();

	ei->SetVisibleOn();

	ei->OutputBuffer(buffer,2048);

	ei->PutVtkDataArray("NumList",arr);

	ei->EvalString("NumList\n");

	ei->EvalString("A = NumList.^2\n");

	ei->EvalString("plot(0:99,A)\n");

	cout << buffer << endl;

	marr = vtkDoubleArray::SafeDownCast(ei->GetVtkDataArray("A"));

	for(i=0;i<100;i++)
		cout << marr->GetValue(i) << endl;

	printf("Hit return to continue\n\n");
	fgetc(stdin);

	vtkArray* vr = vtkArray::CreateArray(vtkArray::DENSE, VTK_DOUBLE);
	vtkArray* ra;

	vr->Resize(10,10,10);

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			for(k=0;k<10;k++)
			{
				vr->SetVariantValue(i,j,k,i*j*k);
			}
		}
	}

	ei->PutVtkArray("diag",vr);

	ei->EvalString("diag = sqrt(diag)\n");

	ra = vtkArray::SafeDownCast(ei->GetVtkArray("diag"));

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			for(k=0;k<10;k++)
			{
			    cout << i << " " << j << " " << " " << k << " " << ra->GetVariantValue(i,j,k).ToDouble() << endl;
			}
		}
	}

	ra->Delete();
	//vr->Delete();
	ei->Delete();
	arr->Delete();
	//marr->Delete();


    test_expression(1 == 1);




/*
    vtkSmartPointer<vtkDenseArray<double> > a = vtkSmartPointer<vtkDenseArray<double> >::New();
    vtkSmartPointer<vtkDenseArray<double> > b = vtkSmartPointer<vtkDenseArray<double> >::New();

    a->Resize(5);
    b->Resize(vtkArrayExtents(5));
    test_expression(a->GetExtents() == b->GetExtents());
   
    a->SetValue(2, 3);
    b->SetValue(vtkArrayCoordinates(2), 3);
    test_expression(a->GetValue(2) == b->GetValue(vtkArrayCoordinates(2))); 
    
    a->Resize(5, 6);
    b->Resize(vtkArrayExtents(5, 6));
    test_expression(a->GetExtents() == b->GetExtents()); 
    
    a->SetValue(2, 3, 4);
    b->SetValue(vtkArrayCoordinates(2, 3), 4);
    test_expression(a->GetValue(2, 3) == b->GetValue(vtkArrayCoordinates(2, 3))); 
    
    a->Resize(5, 6, 7);
    b->Resize(vtkArrayExtents(5, 6, 7));
    test_expression(a->GetExtents() == b->GetExtents()); 
    
    a->SetValue(2, 3, 4, 5);
    b->SetValue(vtkArrayCoordinates(2, 3, 4), 5);
    test_expression(a->GetValue(2, 3, 4) == b->GetValue(vtkArrayCoordinates(2, 3, 4))); 
*/
    
    return 0;
    }
  catch(vtkstd::exception& e)
    {
    cerr << e.what() << endl;
    return 1;
    }
}

