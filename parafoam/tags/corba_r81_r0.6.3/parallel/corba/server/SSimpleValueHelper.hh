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
#ifndef corba_server_SSimpleValueHelper_hh
#define corba_server_SSimpleValueHelper_hh


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  template< class Type >
  struct SSimpleValueHelperBase
  {
    typedef Type TValue;
    TValue value;
    
    typedef Type& TRef;
    typedef const Type& TConstRef;

    typedef Type* TPtr;
    typedef const Type* TConstPtr;
    
    SSimpleValueHelperBase()
    {}
        
    SSimpleValueHelperBase( const TValue& the_value )
      : value( the_value )
    {}
        
    virtual ~SSimpleValueHelperBase()
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
  // To define a new template class to be able to reuse the same "base"
  // implementation in partial specialized templates
  template< class Type >
  struct SSimpleValueHelper : SSimpleValueHelperBase< Type >
  {
    typedef typename SSimpleValueHelperBase< Type >::TValue TValue;
    
    SSimpleValueHelper( const TValue& the_value = TValue() )
      : SSimpleValueHelperBase< Type >( the_value )
    {}
    
    SSimpleValueHelper( const SSimpleValueHelper& the_value_helper )
      : SSimpleValueHelperBase< Type >( the_value_helper.value )
    {}
  };
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
