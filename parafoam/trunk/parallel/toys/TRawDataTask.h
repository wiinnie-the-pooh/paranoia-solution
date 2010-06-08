// VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
// Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
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
