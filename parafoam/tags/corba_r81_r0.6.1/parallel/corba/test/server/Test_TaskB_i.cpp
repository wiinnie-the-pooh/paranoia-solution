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
#include "parallel/corba/test/server/Test_TaskB_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
   namespace test
  {
    //---------------------------------------------------------------------------
    TaskB_i::TaskB_i( const CORBA::ORB_var& theORB, 
                      const PortableServer::POA_var& thePOA )
      : TransientObject_i( theORB, thePOA )
      , TaskBase_i( theORB, thePOA )
      , m_y( "y", eInputPort, this )
      , m_sy( "sy", eInputPort, this )
    {
      cout << "TaskB_i::TaskB_i[ " << this << " ]" << endl;
    }
    
    
    //---------------------------------------------------------------------------
    TaskB_i::~TaskB_i()
    {
      cout << "TaskB_i::~TaskB_i[ " << this << " ]" << endl;
    }
    
    
    //---------------------------------------------------------------------------
    void TaskB_i::prepare()
    {
      this->m_y.init( true );
      
      cout << "TaskB_i::prepare[ " << this << " ]" << endl;
      
      this->m_sy.init( -1 );
    }
    
    
    //---------------------------------------------------------------------------
    bool TaskB_i::step()
    {
      cout << "TaskB_i::step[ " << this << " ]" << endl;
      
      this->m_sy.retrieve();
      
      cout << "TaskB_i::step[ " << this << " ] = " << this->m_sy() << endl;
      
      return this->m_y.retrieve( );
    }
    
    
    //---------------------------------------------------------------------------
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
