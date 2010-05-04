// Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License. 
// 
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details. 
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#ifndef toys_TRawDataTask_h
#define toys_TRawDataTask_h


//---------------------------------------------------------------------------
#include "parallel/dev/TTask.h"
#include "parallel/toys/TField.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    struct TRawDataTask : public dev::TTask
    {
    protected:
      virtual base::TPortPtr get_input_port( const std::string& theName );
      virtual base::TPortPtr get_output_port( const std::string& theName );
      
      TFieldPtr read( const std::string& theName );
      
      void write( const std::string& theName, TFieldPtr theField );
      void write_raw( const std::string& theName, TData theData, int theSize );
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
