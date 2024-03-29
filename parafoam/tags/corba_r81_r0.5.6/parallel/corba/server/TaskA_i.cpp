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
#include "parallel/corba/server/TaskA_i.hh"

#include "parallel/corba/server/PortBool_i.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  TaskA_i::TaskA_i( const CORBA::ORB_var& theORB, 
                    const PortableServer::POA_var& thePOA )
    : TransientObject_i( theORB, thePOA )
    , TaskBase_i( theORB, thePOA )
    , m_x( "x", eOutputPort, this )
    , m_sx( "sx", eOutputPort, this )
  {
    cout << "TaskA_i::TaskA_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  TaskA_i::~TaskA_i()
  {
    cout << "TaskA_i::~TaskA_i[ " << this << " ]" << endl;
  }


  //---------------------------------------------------------------------------
  CORBA::Boolean TaskA_i::step()
  {
    this->m_x.publish( false );

    this->m_sx.publish( 7 );

    cout << "TaskA_i::step[ " << this << " ]" << endl;
    
    return false;
  }
    
    
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
