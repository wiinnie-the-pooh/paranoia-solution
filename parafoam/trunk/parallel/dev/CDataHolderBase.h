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
