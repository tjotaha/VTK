/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: MatlabEngineFilter.cxx,v $
  
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

#include <vtkMatlabEngineFilter.h>
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkArrayExtents.h>

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

int TestMatlabEngineFilter(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  try
    {
	int i;
	vtkCylinderSource* cs = vtkCylinderSource::New();
	vtkMatlabEngineFilter* ef = vtkMatlabEngineFilter::New();
	//vtkBoostRandomSparseArraySource* sas = vtkBoostRandomSparseArraySource::New();
	//vtkMatlabEngineFilter* ef1 = vtkMatlabEngineFilter::New();
	vtkArrayExtents extents(9,9,9);
	vtkDataSet* ds;
	vtkPointData* pd;
	vtkDoubleArray* da;

	cs->SetResolution(10);

	cs->Update();

	ds = vtkDataSet::SafeDownCast(cs->GetOutput());

	pd = ds->GetPointData();

	cout << "TCoords" << endl << endl;

	da = (vtkDoubleArray*) pd->GetArray("TCoords");

	for(i=0;i<da->GetNumberOfTuples();i++)
	{
		double* tup = da->GetTuple2(i);
		cout << tup[0] << " " << tup[1] << endl;
	}

	ef->SetInputConnection(cs->GetOutputPort());

	ef->PutArray("Normals", "Norm");

	ef->PutArray("TCoords", "TCoords");

	ef->GetArray("Normalsnew", "Norm");

	ef->GetArray("TCoords", "TCoords");

	//ef->SetMatlabScript("Norm = Norm.^2\nTCoords = TCoords + TCoords\n");

	ef->SetMatlabScript("pwd");

	ef->SetScriptFname("engfiltertest.m");

	ef->Update();

	ds = vtkDataSet::SafeDownCast(ef->GetOutput());

	pd = ds->GetPointData();

	cout << "Normals" << endl << endl;

	da = (vtkDoubleArray*) pd->GetArray("Normals");

	for(i=0;i<da->GetNumberOfTuples();i++)
	{
		double* tup = da->GetTuple3(i);
		cout << tup[0] << " " << tup[1] << " " << tup[2] << endl;
	}

	cout << "Normalsnew" << endl << endl;

	da = (vtkDoubleArray*) pd->GetArray("Normalsnew");

	for(i=0;i<da->GetNumberOfTuples();i++)
	{
		double* tup = da->GetTuple3(i);
		cout << tup[0] << " " << tup[1] << " " << tup[2] << endl;
	}

	cout << "TCoords" << endl << endl;

	da = (vtkDoubleArray*) pd->GetArray("TCoords");

	for(i=0;i<da->GetNumberOfTuples();i++)
	{
		double* tup = da->GetTuple2(i);
		cout << tup[0] << " " << tup[1] << endl;
	} 

	/*sas->SetExtents(extents);

	ef1->PutArray("0","sa");

	ef1->SetMatlabScript("sa = sa + sa\n");

	ef1->GetArray("0","sa");

	sas->Update();

	cout << vtkArrayData::SafeDownCast(sas->GetOutput())->GetArray(0)->GetVariantValue(4,4,1).ToDouble() << endl; 

	ef1->SetInputConnection(sas->GetOutputPort());

	ef1->Update();

	cout << vtkArrayData::SafeDownCast(ef1->GetOutput())->GetArray(0)->GetVariantValue(4,4,1).ToDouble() << endl; */
	
	cs->Delete();

	ef->Delete();
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

