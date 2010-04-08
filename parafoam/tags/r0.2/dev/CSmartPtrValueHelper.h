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
#ifndef dev_CSmartPtrValueHelper_h
#define dev_CSmartPtrValueHelper_h


//---------------------------------------------------------------------------
#include <loki/SmartPtr.h>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class Type >
    struct CSmartPtrValueHelper
    {
      typedef typename Loki::SmartPtrDef< Type >::type TValue;
      TValue value;
        
      typedef Type& TRef;
      typedef const Type& TConstRef;

      typedef Type* TPtr;
      typedef const Type* TConstPtr;
      
      CSmartPtrValueHelper()
      {}
            
      CSmartPtrValueHelper( const TValue& the_value )
        : value( the_value )
      {}

      CSmartPtrValueHelper( typename TValue::PointerType the_value )
        : value( the_value )
      {}

      CSmartPtrValueHelper( const CSmartPtrValueHelper& the_value_helper )
        : value( the_value_helper.value )
      {}
        
      virtual ~CSmartPtrValueHelper()
      {}
            
      operator TRef ()
      {
        return * this->value;
      }
      
      operator TConstRef () const
      {
        return * this->value;
      }
      
      TPtr operator -> ()
      {
        return this->value.operator -> ();
      }
      
      TConstPtr operator -> () const
      {
        return this->value.operator -> ();
      }
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
