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
#ifndef dev_CSimpleValueHelper_h
#define dev_CSimpleValueHelper_h


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class Type >
    struct CSimpleValueHelperBase
    {
      typedef Type TValue;
      TValue value;
      
      typedef Type& TRef;
      typedef const Type& TConstRef;

      typedef Type* TPtr;
      typedef const Type* TConstPtr;
      
      CSimpleValueHelperBase( const TValue& the_value )
        : value( the_value )
      {}
        
      virtual ~CSimpleValueHelperBase()
      {}

      operator TRef ()
      {
        return this->value;
      }

      operator TConstRef () const
      {
        return this->value;
      }
      
      TPtr operator -> ()
      {
        return & this->value;
      }

      TConstPtr operator -> () const
      {
        return & this->value;
      }
    };
    
    
    //---------------------------------------------------------------------------
    template< class Type >
    struct CSimpleValueHelper : CSimpleValueHelperBase< Type >
    {
      typedef typename CSimpleValueHelperBase< Type >::TValue TValue;

      CSimpleValueHelper( const TValue& the_value = TValue() )
        : CSimpleValueHelperBase< Type >( the_value )
      {}
        
      CSimpleValueHelper( const CSimpleValueHelper& the_value_helper )
        : CSimpleValueHelperBase< Type >( the_value_helper.value )
      {}
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
