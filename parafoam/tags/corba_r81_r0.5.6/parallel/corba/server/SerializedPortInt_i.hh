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
#ifndef corba_server_SerializedPortInt_i_hh
#define corba_server_SerializedPortInt_i_hh


//---------------------------------------------------------------------------
#include "parallel/corba/idl/SerializedPortInt.hh"

#include "parallel/corba/server/SerializedPort_i.hh"

#include "parallel/corba/server/SSerializedValueHelper.hh"


//---------------------------------------------------------------------------
namespace parallel 
{
  //---------------------------------------------------------------------------
  struct SerializedPortInt_i : virtual POA_parallel::SerializedPortInt, 
			       virtual SerializedPort_i
  {
    SerializedPortInt_i( const std::string& theName,
			 const CORBA::ORB_var& theORB, 
			 const PortableServer::POA_var& thePOA );

    ~SerializedPortInt_i();

    CORBA::Boolean is_compatible( PortBase_ptr theArg );

    typedef SSerializedValueHelper< int > TValueHelper;
  };


  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
