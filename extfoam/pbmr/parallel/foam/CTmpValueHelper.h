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
#ifndef foam_CTmpValueHelper_h
#define foam_CTmpValueHelper_h


//---------------------------------------------------------------------------
#include <tmp.H>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //---------------------------------------------------------------------------
    template< class Type >
    struct CTmpValueHelper
    {
      typedef Foam::tmp< Type > TValue;
      TValue value;
        
      typedef Type& TRef;
      typedef const Type& TConstRef;

      typedef Type* TPtr;
      typedef const Type* TConstPtr;
      
      CTmpValueHelper()
      {}
            
      CTmpValueHelper( const TValue& the_value )
        : value( the_value )
      {}

      CTmpValueHelper( const Type& the_value )
        : value( the_value )
      {}

      CTmpValueHelper( const CTmpValueHelper& the_value_helper )
        : value( the_value_helper.value )
      {}
        
      virtual ~CTmpValueHelper()
      {}
            
      operator TRef ()
      {
        return this->value();
      }
      
      operator TConstRef () const
      {
        return this->value();
      }
      
      TPtr operator -> ()
      {
        return & this->value();
      }

      TConstPtr operator -> () const
      {
        return & this->value();
      }
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
