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
#include "parallel/corba/server/Link_i.hh"

#include "parallel/corba/idl/DataHolderBase.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  Link_i::Link_i( const CORBA::ORB_var& theORB, 
                  const PortableServer::POA_var& thePOA )
    : GenericObject_i( theORB, thePOA )
  {
    cout << "Link_i::Link_i[ " << this << " ]" << endl;

    this->m_read_mutex.lock();
  }


  //---------------------------------------------------------------------------
  Link_i::~Link_i()
  {
    cout << "Link_i::~Link_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  void Link_i::publish( DataHolderBase_ptr theDataHolder )
  {
    this->m_list_mutex.lock();
      
    TDataHolderPtr aDataHolder( DataHolderBase::_duplicate( theDataHolder ) );

    this->m_data_holders.push( aDataHolder );
      
    this->m_list_mutex.unlock();
    this->m_read_mutex.unlock();
  }
    
    
  //---------------------------------------------------------------------------
  DataHolderBase_ptr Link_i::retrive()
  {
    this->m_read_mutex.lock();
    this->m_list_mutex.lock();
      
    TDataHolderPtr aDataHolder;
    if ( !this->m_data_holders.empty() )
    {
      aDataHolder = this->m_data_holders.front();
      this->m_data_holders.pop();
    }
      
    // "Publish" could be called many times before first "retrive" will be called
    // So, we should be able "publish" all the available data
    if ( !this->m_data_holders.empty() )
      this->m_read_mutex.unlock();
      
    this->m_list_mutex.unlock();
      
    return DataHolderBase::_duplicate( *aDataHolder );
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
