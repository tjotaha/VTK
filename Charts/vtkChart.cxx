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

#include "vtkChart.h"

// Get my new commands
#include "vtkCommand.h"

#include "vtkAnnotationLink.h"
#include "vtkContextScene.h"
#include "vtkInteractorStyle.h"
#include "vtkInteractorStyleRubberBand2D.h"
#include "vtkObjectFactory.h"

//-----------------------------------------------------------------------------
// Minimal command class to handle callbacks.
class vtkChart::Command : public vtkCommand
{
public:
  static Command* New() { return new Command(); }
  virtual void Execute(vtkObject *caller, unsigned long eventId,
                       void *callData)
    {
    if (this->Target)
      {
      switch (eventId)
        {
        case vtkCommand::SelectionChangedEvent :
          this->Target->ProcessSelectionEvent(caller, callData);
          break;
        default:
          this->Target->ProcessEvents(caller, eventId, callData);
        }
      }
    }

  void SetTarget(vtkChart* t) { this->Target = t; }

private:
  Command() { this->Target = 0; }
  vtkChart *Target;
};

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkChart, "$Revision$");
vtkCxxSetObjectMacro(vtkChart, AnnotationLink, vtkAnnotationLink);

//-----------------------------------------------------------------------------
vtkChart::vtkChart()
{
  int i=0;
  while(i<6)
    {
    this->Geometry[i]=0;
    ++i;
    }
  this->Observer = vtkChart::Command::New();
  this->Observer->SetTarget(this);
  this->AnnotationLink = NULL;
}

//-----------------------------------------------------------------------------
vtkChart::~vtkChart()
{
  this->Observer->Delete();
}

//-----------------------------------------------------------------------------
vtkPlot * vtkChart::AddPlot(Type)
{
  return NULL;
}

//-----------------------------------------------------------------------------
vtkIdType vtkChart::GetNumberPlots()
{
  return 0;
}

//-----------------------------------------------------------------------------
void vtkChart::AddInteractorStyle(vtkInteractorStyle *interactor)
{
  interactor->AddObserver(vtkCommand::SelectionChangedEvent, this->Observer);
}

//-----------------------------------------------------------------------------
void vtkChart::ProcessEvents(vtkObject* caller, unsigned long eventId,
                             void*)
{
  cout << "ProcessEvents called! " << caller->GetClassName() << "\t"
      << vtkCommand::GetStringFromEventId(eventId)
      << "\n\t" << vtkInteractorStyleRubberBand2D::SafeDownCast(caller)->GetInteraction() << endl;
  return;
}

//-----------------------------------------------------------------------------
void vtkChart::ProcessSelectionEvent(vtkObject* caller, void* callData)
{
  cout << "ProcessSelectionEvent called! " << caller << "\t" << callData << endl;
  unsigned int *rect = reinterpret_cast<unsigned int *>(callData);
  cout << "Rect:";
  for (int i = 0; i < 5; ++i)
    {
    cout << "\t" << rect[i];
    }
  cout << endl;
}

//-----------------------------------------------------------------------------
void vtkChart::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // Print out the chart's geometry if it has been set
  os << indent << "Origin: " << this->Geometry[0] << "\t" << this->Geometry[1]
     << endl;
  os << indent << "Width: " << this->Geometry[2] << endl
     << indent << "Height: " << this->Geometry[3] << endl;
  os << indent << "Right border: " << this->Geometry[4] << endl
     << indent << "Top border: " << this->Geometry[5] << endl;
}
