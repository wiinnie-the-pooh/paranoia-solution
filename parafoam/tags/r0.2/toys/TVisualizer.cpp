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
#include "parallel/toys/TVisualizer.h"

#include <iostream>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    void TVisualizer::init()
    {}

    void TVisualizer::destroy()
    {}


    //---------------------------------------------------------------------------
    bool StrVis::step()
    {
      TFieldPtr f = read( "arg" );
      char* str = (char*)f->get_raw_data();
      std::cout << "VIS: '" << str << "'" << std::endl;
      
      return true;
    }


    //---------------------------------------------------------------------------
    DoubleVis::DoubleVis( const std::string& theName )
      : m_Name( theName )
    {}
    
    bool DoubleVis::step()
    {
      TFieldPtr aField = read( "arg" );
      int aVal = *(int*)aField->get_raw_data();
      std::cout << "VIS " << m_Name << ": " << aVal << std::endl;
      
      return true;
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
