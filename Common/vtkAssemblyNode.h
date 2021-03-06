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
// .NAME vtkAssemblyNode - represent a node in an assembly
// .SECTION Description
// vtkAssemblyNode represents a node in an assembly. It is used by
// vtkAssemblyPath to create hierarchical assemblies of props. The
// props can be either 2D or 3D.
//
// An assembly node refers to a vtkProp, and possibly a vtkMatrix4x4.
// Nodes are used by vtkAssemblyPath to build fully evaluated path
// (matrices are concatenated through the path) that is used by picking
// and other operations involving assemblies.

// .SECTION Caveats
// The assembly node is guaranteed to contain a reference to an instance
// of vtkMatrix4x4 if the prop referred to by the node is of type
// vtkProp3D (or subclass). The matrix is evaluated through the assembly
// path, so the assembly node's matrix is a function of its location in 
// the vtkAssemblyPath.
//
// vtkAssemblyNode does not reference count its association with vtkProp.
// Therefore, do not create an assembly node, associate a prop with it,
// delete the prop, and then try to dereference the prop. The program
// will break! (Reason: vtkAssemblyPath (which uses vtkAssemblyNode)
// create self-referencing loops that destroy reference counting.)

// .SECTION see also
// vtkAssemblyPath vtkProp vtkPicker vtkMatrix4x4

#ifndef __vtkAssemblyNode_h
#define __vtkAssemblyNode_h

#include "vtkObject.h"

class vtkProp;
class vtkMatrix4x4;

class VTK_COMMON_EXPORT vtkAssemblyNode : public vtkObject
{
public:
  // Description:
  // Create an assembly node.
  static vtkAssemblyNode *New();

  vtkTypeRevisionMacro(vtkAssemblyNode,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get the prop that this assembly node refers to.
  virtual void SetViewProp(vtkProp* prop);
  vtkGetObjectMacro(ViewProp, vtkProp);

  // Description:
  // Specify a transformation matrix associated with the prop.
  // Note: if the prop is not a type of vtkProp3D, then the
  // transformation matrix is ignored (and expected to be NULL).
  // Also, internal to this object the matrix is copied because
  // the matrix is used for computation by vtkAssemblyPath.
  void SetMatrix(vtkMatrix4x4 *matrix);
  vtkGetObjectMacro(Matrix, vtkMatrix4x4);
  
  // Description:
  // Override the standard GetMTime() to check for the modified times
  // of the prop and matrix.
  virtual unsigned long GetMTime();

// Disable warnings about qualifiers on return types.
#if defined(_COMPILER_VERSION)
# pragma set woff 3303
#endif
#if defined(__INTEL_COMPILER)
# pragma warning (push)
# pragma warning (disable:858)
#endif

#ifdef VTK_WORKAROUND_WINDOWS_MANGLE
# define SetPropA SetProp
# define SetPropW SetProp
# define GetPropA GetProp
# define GetPropW GetProp
#endif

  // Description:
  // @deprecated Replaced by vtkAssemblyNode::SetViewProp() as of VTK 5.0.
  VTK_LEGACY(virtual void SetProp(vtkProp* prop));

  // Description:
  // @deprecated Replaced by vtkAssemblyNode::GetViewProp() as of VTK 5.0.
  VTK_LEGACY(virtual vtkProp* GetProp());

#ifdef VTK_WORKAROUND_WINDOWS_MANGLE
# undef SetPropW
# undef SetPropA
# undef GetPropW
# undef GetPropA
  //BTX
  VTK_LEGACY(virtual void SetPropA(vtkProp* prop));
  VTK_LEGACY(virtual void SetPropW(vtkProp* prop));
  VTK_LEGACY(virtual vtkProp* GetPropA());
  VTK_LEGACY(virtual vtkProp* GetPropW());
  //ETX
#endif

// Reset disabled warning about qualifiers on return types.
#if defined(__INTEL_COMPILER)
# pragma warning (pop)
#endif
#if defined(_COMPILER_VERSION)
# pragma reset woff 3303
#endif

protected:
  vtkAssemblyNode();
  ~vtkAssemblyNode();

private:
  vtkProp *ViewProp; //reference to vtkProp
  vtkMatrix4x4 *Matrix; //associated matrix
  
private:
  void operator=(const vtkAssemblyNode&); // Not implemented.
  vtkAssemblyNode(const vtkAssemblyNode&); // Not implemented.
};

#endif
