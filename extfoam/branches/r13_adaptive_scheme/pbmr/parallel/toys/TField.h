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
#ifndef toys_TField_h
#define toys_TField_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    /*
      Raw data transferred between solvers and modifiers (also known as tasks)
    */
    typedef void* TData;
  
    /*
      Wrapper for raw data
    */
    struct TField : base::TDataHolder
    {
      TField( const std::string& theName, TData theData, int theSize );
      virtual ~TField();

      virtual std::string get_name();
      virtual TData get_raw_data();
      
    private:
      std::string m_Name;
      TData m_Data;
    };

    typedef base::SmartPtrDef< TField >::type TFieldPtr;


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
