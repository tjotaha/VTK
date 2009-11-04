
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkMatlabEngineInterface.cxx,v $

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

#include "vtkObjectFactory.h"
#include "vtkMatlabEngineInterface.h"
#include "vtkMatlabMexAdapter.h"
#include "vtkErrorCode.h"
#include "vtkDataArray.h"
#include "vtkArray.h"
#include "engine.h"

#include <stdlib.h>
#include <vtkstd/string>
#include <vtksys/ios/sstream>
#include <sys/stat.h>
#include <vtkstd/string>

vtkCxxRevisionMacro(vtkMatlabEngineInterface, "$Revision: 14946 $");
vtkStandardNewMacro(vtkMatlabEngineInterface);

class SingletonDestroyer;

class MatlabEngineSingleton
{
public:
  static MatlabEngineSingleton* Instance();

  int EngineOpen()
  {
    if(this->ep)
      return(1);
    else
      return(0);
  };

  void OpenEngine()
  {
    if(this->ep)
      {
      this->refcount++;
      return;
      }

    if(!(this->ep = engOpen("\0")))
      {
      cerr << endl << "Can't start MATLAB engine" << endl;
      return;
      }

    engSetVisible(this->ep, 0);
    this->refcount++;

  };

  void CloseEngine()
  {
    this->refcount--;

    if(this->refcount == 0)
      {
      if(engClose(this->ep))
        {
        cerr << "Can't shutdown MATLAB engine" << endl;
        }
      }
  };

  int EngEvalString(const char* string)
  {
    return(engEvalString(this->ep, string));
  };

  int EngPutVariable(const char *name, const mxArray *mxa)
  {
    return(engPutVariable(this->ep, name, mxa));
  };

  mxArray* EngGetVariable(const char *name)
  {
    return(engGetVariable(this->ep, name));
  };

  int EngSetVisible(bool value)
  {
    return(engSetVisible(this->ep, value));
  };

  int EngOutputBuffer(char* p, int n)
  {
    return(engOutputBuffer(this->ep, p, n));
  };

protected:

  friend class SingletonDestroyer;

  ~MatlabEngineSingleton();

  MatlabEngineSingleton();

  MatlabEngineSingleton(const MatlabEngineSingleton&);

  MatlabEngineSingleton& operator=(const MatlabEngineSingleton&);

private:

  int refcount;
  static MatlabEngineSingleton* ins;
  static SingletonDestroyer destroyer;
  Engine* ep;

};

class SingletonDestroyer {
public:
  SingletonDestroyer();
  ~SingletonDestroyer();

  void SetSingleton(MatlabEngineSingleton* s);
private:
  MatlabEngineSingleton* _singleton;
};


MatlabEngineSingleton* MatlabEngineSingleton::ins = 0;

SingletonDestroyer MatlabEngineSingleton::destroyer;

SingletonDestroyer::SingletonDestroyer () {
  _singleton = 0;
}

SingletonDestroyer::~SingletonDestroyer () {
  delete _singleton;
}

void SingletonDestroyer::SetSingleton (MatlabEngineSingleton* s) {
  _singleton = s;
}


MatlabEngineSingleton* MatlabEngineSingleton::Instance()
{

  if(ins == 0)
    {
    ins = new MatlabEngineSingleton;
    destroyer.SetSingleton(ins);
    }

  ins->OpenEngine();
  return(ins);

}


MatlabEngineSingleton::~MatlabEngineSingleton()
{


}

MatlabEngineSingleton::MatlabEngineSingleton()
{

  this->refcount = 0;
  this->ep = 0;

}


vtkMatlabEngineInterface::vtkMatlabEngineInterface()
{

  this->meng = MatlabEngineSingleton::Instance();

}

vtkMatlabEngineInterface::~vtkMatlabEngineInterface()
{

  this->meng->CloseEngine();

}

void vtkMatlabEngineInterface::PrintSelf(ostream& os, vtkIndent indent)
{

  this->Superclass::PrintSelf(os,indent);

}

int vtkMatlabEngineInterface::EvalString(const char* string)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute EvalString() command" << endl;
    return(1);
    }

  return(this->meng->EngEvalString(string));

}

int vtkMatlabEngineInterface::PutVtkDataArray(const char* name, vtkDataArray* vda)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute PutVtkDataArray() command" << endl;
    return(1);
    }

  mxArray* mxa = vtkMatlabMexAdapter::vtkDataArrayToMxArray(vda, true);

  int rc = this->meng->EngPutVariable(name, mxa);

  if(!rc)
    {

    vtkstd::string s;

    s.append(name);
    s.append(" = transpose(");
    s.append(name);
    s.append(");");

    this->EvalString(s.c_str()); 

    }

  mwSize dims[1];
  dims[0] = 0;
  mxSetDimensions(mxa, dims, 1);
  mxSetData(mxa, 0);
  mxDestroyArray(mxa);

  return(rc);

}

vtkDataArray* vtkMatlabEngineInterface::GetVtkDataArray(const char* name)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute GetVtkDataArray() command" << endl;
    return(0);
    }

  vtkstd::string s;

  s.append(name);
  s.append(" = transpose(");
  s.append(name);
  s.append(");");

  this->EvalString(s.c_str());

  mxArray* mxa = meng->EngGetVariable(name);

  s.clear();
  s.append(name);
  s.append(" = transpose(");
  s.append(name);
  s.append(");");

  this->EvalString(s.c_str()); 

  if(!mxa)
    {
    return(0);
    }

  mwSize d[2];
  d[0] = mxGetN(mxa);
  d[1] = mxGetM(mxa);
  mxSetDimensions(mxa,d,2);
  vtkDataArray* vda = vtkMatlabMexAdapter::mxArrayTovtkDataArray(mxa, true);

  mwSize dims[1];
  dims[0] = 0;
  mxSetDimensions(mxa, dims, 1);
  mxSetData(mxa, 0); 
  mxDestroyArray(mxa);

  return(vda);

}


int vtkMatlabEngineInterface::PutVtkArray(const char* name, vtkArray* vda)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute PutVtkArray() command" << endl;
    return(1);
    }

  mxArray* mxa = vtkMatlabMexAdapter::vtkArrayToMxArray(vda);

  int rc = this->meng->EngPutVariable(name, mxa);

  mxDestroyArray(mxa);

  return(rc);

}

 
vtkArray* vtkMatlabEngineInterface::GetVtkArray(const char* name)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute GetVtkArray() command" << endl;
    return(0);
    }

  mxArray* mxa = meng->EngGetVariable(name);

  vtkArray* vda = vtkMatlabMexAdapter::mxArrayTovtkArray(mxa);

  mxDestroyArray(mxa);

  return(vda);

}

int vtkMatlabEngineInterface::OutputBuffer(char* p, int n)
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute OuputBuffer() command" << endl;
    return(1);
    }

  return(this->meng->EngOutputBuffer(p, n));

}

int vtkMatlabEngineInterface::SetVisibleOn()
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute SetVisibleOn() command" << endl;
    return(1);
    }

  return(this->meng->EngSetVisible(1));

}

int vtkMatlabEngineInterface::SetVisibleOff()
{

  if(!this->meng->EngineOpen())
    {
    cerr << "Matlab engine not open, cannot execute SetVisibleOff() command" << endl;
    return(1);
    }

  return(this->meng->EngSetVisible(0));

}

int vtkMatlabEngineInterface::EngineOpen()
{

  return(this->meng->EngineOpen());

}
