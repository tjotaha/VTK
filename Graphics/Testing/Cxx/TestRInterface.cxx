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

int TestRInterface(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  try
    {

  int i;
  int j;
  int k;
  char* s = "d <- d + 1\n z <- sqrt(d)";
  vtkDoubleArray* da = vtkDoubleArray::New();
  vtkDataArray* ra;
  vtkTable* tableResult;
  da->SetNumberOfTuples(10);
  da->SetNumberOfComponents(1);
  int buf_size = 20000;
  char* buffer = new char[buf_size];

  for(i=0;i<10;i++)
    {
    da->InsertValue(i,i);
    }

  vtkRInterface* rint = vtkRInterface::New();
  rint->AssignVTKDataArrayToRVariable(da,"d");
  rint->OutputBuffer(buffer, buf_size);
  rint->EvalRscript(s);
  cout << buffer << endl;
  ra = rint->AssignRVariableToVTKDataArray("z");

  for(i=0;i<10;i++)
    {
    cout << "i = " << i << " value = " << ra->GetTuple1(i) << endl;
    }


  da->Delete();
  ra->Delete();

  vtkArray* vr = vtkArray::CreateArray(vtkArray::DENSE, VTK_DOUBLE);
  vtkArray* za;

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

  rint->AssignVTKArrayToRVariable(vr,"diag");

  rint->EvalRscript("diag\ndiag = sqrt(diag)\n");
  cout << buffer << endl;

  za = vtkArray::SafeDownCast(rint->AssignRVariableToVTKArray("diag"));

  for(i=0;i<10;i++)
    {
    for(j=0;j<10;j++)
      {
      for(k=0;k<10;k++)
        {
        //cout << i << " " << j << " " << " " << k << " " << za->GetVariantValue(i,j,k).ToDouble() << endl;
        }
      }
    }  

  za->Delete();
  vr->Delete();

  vtkRRandomTableSource* rts = vtkRRandomTableSource::New();
  rts->SetStatisticalDistributionForColumn(vtkRRandomTableSource::NORMAL,0.0,1.0,0.0,"Variable One",0);
  rts->SetStatisticalDistributionForColumn(vtkRRandomTableSource::NORMAL,0.0,1.0,0.0,"Variable Two",1);
  rts->SetStatisticalDistributionForColumn(vtkRRandomTableSource::NORMAL,0.0,1.0,0.0,"Variable Three",2);
  rts->SetStatisticalDistributionForColumn(vtkRRandomTableSource::NORMAL,0.0,1.0,0.0,"Variable Four",3);
  rts->SetNumberOfRows(20);
  rts->Update();
  rts->GetOutput(0)->Dump();
  rint->AssignVTKTableToRVariable(rts->GetOutput(0),"x");
  rint->EvalRscript("x\nz = matrix(unlist(x), nrow=length(x[[1]]), ncol=length(x))\nz = z + 5.0");
  cout << buffer << endl;
  tableResult = rint->AssignRVariableToVTKTable("z");
  tableResult->Dump();

  tableResult->Delete();
  rint->Delete();
  rts->Delete();
  delete buffer;


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

