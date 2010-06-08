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
#include "parallel/dev/TQueue.h"

#include "parallel/dev/boost_threading.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    TQueue::TQueue()
    {
      parallel::threading::lock( m_Read );
    }


    //---------------------------------------------------------------------------
    void TQueue::publish( const base::TDataHolderPtr& theDataHolder )
    {
      parallel::threading::lock( m_List );
      
      m_DataHolders.push_back( theDataHolder );
      
      parallel::threading::unlock( m_List );
      parallel::threading::unlock( m_Read );
    }


    //---------------------------------------------------------------------------
    base::TDataHolderPtr TQueue::retrieve()
    {
      parallel::threading::lock( m_Read );
      parallel::threading::lock( m_List );
      
      base::TDataHolderPtr aDataHolder;
      if ( !m_DataHolders.empty() )
      {
        aDataHolder = m_DataHolders.front();
        m_DataHolders.pop_front();
      }
      
      // "publish" could be called many times before first "retrive" will be called
      // we should be able read all the available data
      if ( !m_DataHolders.empty() )
        parallel::threading::unlock( m_Read );
      
      parallel::threading::unlock( m_List );
      
      return aDataHolder;
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
