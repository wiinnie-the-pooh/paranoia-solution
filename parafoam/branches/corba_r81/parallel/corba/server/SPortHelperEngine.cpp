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
#include "parallel/corba/server/SPortHelperEngine.hh"

#include <iostream>

using namespace std;


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  SPortHelperEngine::SPortHelperEngine( const TPortPtr& thePort, bool theIsInput, const TTaskPtr& theTask )
    : m_port( thePort ) 
    , m_task( theTask ) 
  {
    if ( theIsInput )
      theTask->define_input_port( thePort );
    else
      theTask->define_output_port( thePort );
  }


  //---------------------------------------------------------------------------
  const std::string& SPortHelperEngine::c_name()
  {
    return this->m_port->c_name();
  }
      

  //---------------------------------------------------------------------------
  SPortHelperEngine::TTaskPtr SPortHelperEngine::task()
  {
    return this->m_task;
  }


  //---------------------------------------------------------------------------
  const CORBA::ORB_var& SPortHelperEngine::ORB()
  {
    return this->m_task->ORB();
  }


  //---------------------------------------------------------------------------
  const PortableServer::POA_var& SPortHelperEngine::POA()
  {
    return this->m_task->POA();
  }
  

  //---------------------------------------------------------------------------
  void SPortHelperEngine::__init__( const SPortHelperEngine::TTaskPtr& theTask,
				    const std::string& theName, 
				    const SPortHelperEngine::TDataHolderBaseVar& theDataHolder )
  {
    theTask->init_port( theName, theDataHolder );
  }

  
  //---------------------------------------------------------------------------
  void SPortHelperEngine::__publish__( const SPortHelperEngine::TTaskPtr& theTask,
				       const std::string& theName, 
				       const SPortHelperEngine::TDataHolderBaseVar& theDataHolder )
  {
    theTask->publish( theName, theDataHolder );
  }


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
