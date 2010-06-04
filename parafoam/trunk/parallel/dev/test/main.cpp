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
// See http://sourceforge.net/projects/extfoam
//
// Author : Alexey PETROV
//


//---------------------------------------------------------------------------
#include "parallel/dev/CDataHolderBase.h"

#include "iostream"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class Type, class DataHolderType >
    struct CSimpleDataHolderBase : CDataHolderBase
    {
      typedef Type TValue;
      TValue value;
      
      typedef TValue& TRef;
      typedef const TValue& TConstRef;

      typedef TValue* TPointer;
      
      typedef typename base::SmartPtrDef< DataHolderType >::type TDataHolderPtr;

      CSimpleDataHolderBase( const TValue& the_value )
        : value( the_value )
      {}
        
      virtual ~CSimpleDataHolderBase()
      {}

      bool valid() const
      {
        return true;
      }
        
      TRef ref()
      {
        return this->value;
      }

      TConstRef ref() const
      {
        return this->value;
      }
      
      TPointer ptr()
      {
        return & this->value;
      }

      friend class boost::serialization::access;

      template< class ArchiveType >
      void serialize( ArchiveType & ar, const unsigned int /* file_version */)
      {
        ar & this->value;
      }

      virtual void save() const
      {
        parallel::dev::save( *this, this->data );
      }

      virtual void restore()
      {
        parallel::dev::restore( this->data, *this );
        this->data.clear();
      }

      virtual TDataHolderPtr serialize() const
      {
        return new DataHolderType( this->value );
      }
    };
    
    
    //---------------------------------------------------------------------------
    struct CDoubleHolder : CSimpleDataHolderBase< double, CDoubleHolder >
    {
      typedef double TValue;

      CDoubleHolder( const TValue& the_value = TValue() )
        : CSimpleDataHolderBase< TValue, CDoubleHolder >( the_value )
      {}
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
  using namespace parallel::dev;

  CDoubleHolder a( 2.0 );

  std::cout << "a : value = " << a.value << "; data = '" << a.data << "'" << std::endl;

  a.save();

  std::cout << "a : value = " << a.value << "; data = '" << a.data << "'" << std::endl;

  a.value = -1;

  std::cout << "a : value = " << a.value << "; data = '" << a.data << "'" << std::endl;

  a.restore();

  std::cout << "a : value = " << a.value << "; data = '" << a.data << "'" << std::endl;

  return 0;
}


//---------------------------------------------------------------------------
