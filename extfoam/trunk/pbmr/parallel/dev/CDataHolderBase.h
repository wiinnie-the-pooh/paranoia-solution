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
#ifndef dev_CDataHolderBase_h
#define dev_CDataHolderBase_h


//---------------------------------------------------------------------------
#include "parallel/base/Interfaces.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <sstream>

#include <string>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class DataHolderType >
    void save( const DataHolderType& the_engine, typename std::string& the_data )
    {
      typename std::ostringstream os;
      typename boost::archive::text_oarchive oa( os );
      
      oa << the_engine;
      
      the_data = os.str();
    }

    template< class DataHolderType >
    void restore( const typename std::string& the_data, DataHolderType& the_engine )
    {
      typename std::istringstream is( the_data );
      
      typename boost::archive::text_iarchive ia( is );
      
      ia >> the_engine;
    }


    //---------------------------------------------------------------------------
    struct CDataHolderBase : base::TDataHolder
    {
      virtual void save() const = 0;
      virtual void restore() = 0;

      //protected:
      mutable std::string data;
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
