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
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/

#include "vtkArrayReader.h"

#include <vtkArrayCoordinateIterator.h>
#include <vtkCommand.h>
#include <vtkDenseArray.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>
#include <vtkUnicodeString.h>

#include <sstream>
#include <vtkstd/stdexcept>
#include <vtkstd/string>

vtkCxxRevisionMacro(vtkArrayReader, "$Revision$");
vtkStandardNewMacro(vtkArrayReader);

namespace {

template<typename ValueT>
void ExtractValue(istream& stream, ValueT& value)
{
  stream >> value;
}

void ExtractValue(istream& stream, vtkStdString& value)
{
  vtkstd::getline(stream, value);
  vtkStdString::size_type begin, end;
  begin = 0;  end = value.size();
  while ((begin < end) && isspace(value[begin])) begin++;
  while ((begin < end) && isspace(value[end-1])) end--;
  value = value.substr(begin, end);
}

void ExtractValue(istream& stream, vtkUnicodeString& value)
{
  vtkstd::string buffer;
  vtkstd::getline(stream, buffer);
  vtkStdString::size_type begin, end;
  begin = 0;  end = buffer.size();
  while ((begin < end) && isspace(buffer[begin])) begin++;
  while ((begin < end) && isspace(buffer[end-1])) end--;
  buffer = buffer.substr(begin, end);
  value = vtkUnicodeString::from_utf8(buffer);
}

void ReadHeader(istream& stream, vtkArrayExtents& extents, vtkIdType& non_null_size, vtkArray* array)
{
  if(!array)
    throw vtkstd::runtime_error("Missing array.");

  // Load the array name ...
  vtkstd::string name;
  vtkstd::getline(stream, name);
  array->SetName(name);

  // Load array extents ...
  vtkstd::string extents_string;
  vtkstd::getline(stream, extents_string);
  vtkstd::istringstream extents_buffer(extents_string);

  vtkIdType extent;
  vtkstd::vector<vtkIdType> temp_extents;
  for(extents_buffer >> extent; extents_buffer; extents_buffer >> extent)
    temp_extents.push_back(extent);

  if(temp_extents.size() < 2)
    throw vtkstd::runtime_error("Missing array extents.");

  extents.SetDimensions(0);
  for(vtkstd::vector<vtkIdType>::size_type i = 0; i + 1 < temp_extents.size(); ++i)
    extents.Append(temp_extents[i]);

  non_null_size = temp_extents.back();

  array->Resize(extents);

  // Load dimension-labels ...
  for(vtkIdType i = 0; i != extents.GetDimensions(); ++i)
    {
    vtkstd::string label;
    vtkstd::getline(stream, label);
    array->SetDimensionLabel(i, label);
    }
}

void ReadEndianOrderMark(istream& stream, bool& swap_endian)
{
  // Load the endian-order mark ...
  vtkTypeUInt32 endian_order = 0;
  stream.read(
    reinterpret_cast<char*>(&endian_order),
    sizeof(endian_order));

  swap_endian = endian_order == 0x12345678 ? false : true;
}

template<typename ValueT>
vtkSparseArray<ValueT>* ReadSparseArrayBinary(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkSparseArray<ValueT> > array = vtkSmartPointer<vtkSparseArray<ValueT> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read the array NULL value ...
  ValueT null_value;
  stream.read(
    reinterpret_cast<char*>(&null_value),
    sizeof(ValueT));
  array->SetNullValue(null_value);

  // Read array coordinates ...
  array->ReserveStorage(non_null_size);

  for(vtkIdType i = 0; i != array->GetDimensions(); ++i)
    {
    stream.read(
      reinterpret_cast<char*>(array->GetCoordinateStorage(i)),
      non_null_size * sizeof(vtkIdType));
    }

  // Read array values ...
  stream.read(
    reinterpret_cast<char*>(array->GetValueStorage()),
    non_null_size * sizeof(ValueT));

  array->Register(0); 
  return array;
}

template<>
vtkSparseArray<vtkStdString>* ReadSparseArrayBinary<vtkStdString>(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkSparseArray<vtkStdString> > array = vtkSmartPointer<vtkSparseArray<vtkStdString> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read the array NULL value ...
  vtkstd::string null_value;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetNullValue(null_value);
      break;
      }
    else
      {
      null_value += character;
      }
    }

  // Read array coordinates ...
  array->ReserveStorage(non_null_size);

  for(vtkIdType i = 0; i != array->GetDimensions(); ++i)
    {
    stream.read(
      reinterpret_cast<char*>(array->GetCoordinateStorage(i)),
      non_null_size * sizeof(vtkIdType));
    }

  // Read array values ...
  vtkstd::string buffer;
  vtkIdType index = 0;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetValueN(index++, buffer);
      buffer.resize(0);
      }
    else
      {
      buffer += character;
      }
    }

  array->Register(0);
  return array;
}

template<>
vtkSparseArray<vtkUnicodeString>* ReadSparseArrayBinary<vtkUnicodeString>(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkSparseArray<vtkUnicodeString> > array = vtkSmartPointer<vtkSparseArray<vtkUnicodeString> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read the array NULL value ...
  vtkstd::string null_value;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetNullValue(vtkUnicodeString::from_utf8(null_value));
      break;
      }
    else
      {
      null_value += character;
      }
    }

  // Read array coordinates ...
  array->ReserveStorage(non_null_size);

  for(vtkIdType i = 0; i != array->GetDimensions(); ++i)
    {
    stream.read(
      reinterpret_cast<char*>(array->GetCoordinateStorage(i)),
      non_null_size * sizeof(vtkIdType));
    }

  // Read array values ...
  vtkstd::string buffer;
  vtkIdType index = 0;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetValueN(index++, vtkUnicodeString::from_utf8(buffer));
      buffer.resize(0);
      }
    else
      {
      buffer += character;
      }
    }

  array->Register(0);
  return array;
}

template<typename ValueT>
vtkDenseArray<ValueT>* ReadDenseArrayBinary(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkDenseArray<ValueT> > array = vtkSmartPointer<vtkDenseArray<ValueT> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read array values ...
  stream.read(
    reinterpret_cast<char*>(array->GetStorage()),
    non_null_size * sizeof(ValueT));

  array->Register(0);
  return array;
}

template<>
vtkDenseArray<vtkStdString>* ReadDenseArrayBinary<vtkStdString>(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkDenseArray<vtkStdString> > array = vtkSmartPointer<vtkDenseArray<vtkStdString> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read array values ...
  vtkstd::string buffer;
  vtkIdType index = 0;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetValueN(index++, buffer);
      buffer.resize(0);
      }
    else
      {
      buffer += character;
      }
    }

  array->Register(0);
  return array;
}

template<>
vtkDenseArray<vtkUnicodeString>* ReadDenseArrayBinary<vtkUnicodeString>(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkDenseArray<vtkUnicodeString> > array = vtkSmartPointer<vtkDenseArray<vtkUnicodeString> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  bool swap_endian = false;
  ReadHeader(stream, extents, non_null_size, array);
  ReadEndianOrderMark(stream, swap_endian);

  // Read array values ...
  vtkstd::string buffer;
  vtkIdType index = 0;
  for(int character = stream.get(); stream; character = stream.get())
    {
    if(character == 0)
      {
      array->SetValueN(index++, vtkUnicodeString::from_utf8(buffer));
      buffer.resize(0);
      }
    else
      {
      buffer += character;
      }
    }

  array->Register(0);
  return array;
}

template<typename ValueT>
vtkSparseArray<ValueT>* ReadSparseArrayAscii(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkSparseArray<ValueT> > array = vtkSmartPointer<vtkSparseArray<ValueT> >::New();

  // Read the stream header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  ReadHeader(stream, extents, non_null_size, array);

  if(non_null_size > extents.GetSize())
    throw vtkstd::runtime_error("Too many values for a sparse array.");

  // Read the array NULL value ...
  vtkstd::string line_buffer;
  vtkstd::getline(stream, line_buffer);
  if(!stream)
    throw vtkstd::runtime_error("Premature end-of-stream reading NULL value.");

  vtkstd::istringstream line_stream(line_buffer);
  ValueT null_value;
  ExtractValue(line_stream, null_value);
  if(!line_stream)
    throw vtkstd::runtime_error("Missing NULL value.");
  array->SetNullValue(null_value);

  // Setup storage for the stream contents ...
  array->ReserveStorage(non_null_size);
  vtkstd::vector<vtkIdType*> coordinates(array->GetDimensions());
  for(vtkIdType j = 0; j != array->GetDimensions(); ++j)
    coordinates[j] = array->GetCoordinateStorage(j);
  ValueT* value = array->GetValueStorage();

  // Read the stream contents ...
  vtkIdType value_count = 0;
  for(vtkstd::getline(stream, line_buffer); stream; vtkstd::getline(stream, line_buffer), ++value_count)
    {
    if(value_count + 1 > non_null_size)
      throw vtkstd::runtime_error("Stream contains too many values.");

    line_stream.clear();
    line_stream.str(line_buffer);
 
    for(vtkIdType j = 0; j != array->GetDimensions(); ++j)
      {
      line_stream >> *(coordinates[j] + value_count);
      if(*(coordinates[j] + value_count) >= extents[j])
        throw vtkstd::runtime_error("Coordinate out-of-bounds.");
      if(!line_stream)
        throw vtkstd::runtime_error("Missing coordinate.");
      }

    ExtractValue(line_stream, *(value + value_count));
    if(!line_stream)
      throw vtkstd::runtime_error("Missing value.");
    }

  // Ensure we loaded enough values ...
  if(value_count != non_null_size)
    throw vtkstd::runtime_error("Stream doesn't contain enough values.");

  array->Register(0); 
  return array;
}

template<typename ValueT>
vtkDenseArray<ValueT>* ReadDenseArrayAscii(istream& stream)
{
  // Create the array ...
  vtkSmartPointer<vtkDenseArray<ValueT> > array = vtkSmartPointer<vtkDenseArray<ValueT> >::New();

  // Read the file header ...
  vtkArrayExtents extents;
  vtkIdType non_null_size = 0;
  ReadHeader(stream, extents, non_null_size, array);

  if(non_null_size != extents.GetSize())
    throw vtkstd::runtime_error("Incorrect number of values for a dense array.");

  // Read the file contents ...
  vtkSmartPointer<vtkArrayCoordinateIterator> iterator = vtkSmartPointer<vtkArrayCoordinateIterator>::New();
  iterator->SetExtents(extents);

  ValueT value;
  vtkIdType value_count = 0;
  for(ExtractValue(stream, value); stream; ExtractValue(stream, value), ++value_count)
    {
    if(value_count + 1 > non_null_size)
      throw vtkstd::runtime_error("Stream contains too many values.");

    array->SetValue(iterator->Next(), value);
    }

  if(value_count != non_null_size)
    throw vtkstd::runtime_error("Stream doesn't contain enough values.");

  array->Register(0); 
  return array;
}

} // End anonymous namespace

vtkArrayReader::vtkArrayReader() :
  FileName(0)
{
  this->SetNumberOfInputPorts(0);
}

vtkArrayReader::~vtkArrayReader()
{
  this->SetFileName(0);
}

void vtkArrayReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "FileName: " 
     << (this->FileName ? this->FileName : "(none)") << endl;
}

int vtkArrayReader::RequestData(
  vtkInformation*, 
  vtkInformationVector**, 
  vtkInformationVector* outputVector)
{
  try
    {
    if(!this->FileName)
      throw vtkstd::runtime_error("FileName not set.");

    ifstream file(this->FileName);

    vtkArray* const array = this->Read(file);
    if(!array)
      throw vtkstd::runtime_error("Error reading array.");

    vtkArrayData* const array_data = vtkArrayData::GetData(outputVector);
    array_data->ClearArrays();
    array_data->AddArray(array);
    array->Delete();

    return 1;
    }
  catch(vtkstd::exception& e)
    {
    vtkErrorMacro(<< e.what());
    }

  return 0;
}

vtkArray* vtkArrayReader::Read(istream& stream)
{
  try
    {
    // Read enough of the file header to identify the type ...
    vtkstd::string header_string; 
    vtkstd::getline(stream, header_string);
    vtkstd::istringstream header_buffer(header_string);

    vtkstd::string header_magic;
    vtkstd::string header_type;
    header_buffer >> header_magic >> header_type;

    // Read input file type, binary or ascii
    vtkstd::string header_file_string;
    vtkstd::string header_file_type;
    vtkstd::getline(stream, header_file_string);
    vtkstd::istringstream header_file_type_buffer(header_file_string);
    header_file_type_buffer >> header_file_type;

    bool read_binary = false;
    if(header_file_type == "binary")
      {
      read_binary = true;
      }
    else if(header_file_type != "ascii")
      {
      throw vtkstd::runtime_error("Unknown file type: " + header_file_type);
      }

    if(header_magic == "vtk-sparse-array")
      {
      if(header_type == "integer")
        {
        return (read_binary ? ReadSparseArrayBinary<vtkIdType>(stream) : ReadSparseArrayAscii<vtkIdType>(stream));
        }
      else if(header_type == "double")
        {
        return (read_binary ? ReadSparseArrayBinary<double>(stream) : ReadSparseArrayAscii<double>(stream));
        }
      else if(header_type == "string")
        {
        return (read_binary ? ReadSparseArrayBinary<vtkStdString>(stream) : ReadSparseArrayAscii<vtkStdString>(stream));
        }
      else if(header_type == "unicode-string")
        {
        return (read_binary ? ReadSparseArrayBinary<vtkUnicodeString>(stream) : ReadSparseArrayAscii<vtkUnicodeString>(stream));
        }
      else
        {
        throw vtkstd::runtime_error("Unknown array type: " + header_type);
        }
      }
    else if(header_magic == "vtk-dense-array")
      {
      if(header_type == "integer")
        {
        return (read_binary ? ReadDenseArrayBinary<vtkIdType>(stream) : ReadDenseArrayAscii<vtkIdType>(stream));
        }
      else if(header_type == "double")
        {
        return (read_binary ? ReadDenseArrayBinary<double>(stream) : ReadDenseArrayAscii<double>(stream));
        }
      else if(header_type == "string")
        {
        return (read_binary ? ReadDenseArrayBinary<vtkStdString>(stream) : ReadDenseArrayAscii<vtkStdString>(stream));
        }
      else if(header_type == "unicode-string")
        {
        return (read_binary ? ReadDenseArrayBinary<vtkUnicodeString>(stream) : ReadDenseArrayAscii<vtkUnicodeString>(stream));
        }
      else
        {
        throw vtkstd::runtime_error("Unknown array type: " + header_type);
        }
      }
    else
      {
      throw vtkstd::runtime_error("Unknown file type: " + header_magic);
      }
    }
  catch(vtkstd::exception& e)
    {
    vtkGenericWarningMacro(<< e.what());
    }
      
  return 0;
}


