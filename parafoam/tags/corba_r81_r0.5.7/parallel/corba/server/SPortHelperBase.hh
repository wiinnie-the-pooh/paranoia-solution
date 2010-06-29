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
#ifndef corba_server_SPortHelperBase_hh
#define corba_server_SPortHelperBase_hh


//---------------------------------------------------------------------------
#include "parallel/corba/server/SPortHelperEngine.hh"


//---------------------------------------------------------------------------
namespace parallel
{
  //---------------------------------------------------------------------------
  template< class PortType >
  struct SPortHelperBase : SPortHelperEngine
  {
    typedef typename PortType::TDataHolder_i TDataHolder_i;
    typedef typename TDataHolder_i::Interface TDataHolder;
    typedef typename TDataHolder::_var_type DataHolderVarType;
    typedef typename corba::SmartPtrDef< DataHolderVarType >::type TDataHolderPtr;
    
    typedef typename PortType::TValueHelper TValueHelper;
    typedef typename TValueHelper::TValue TValue;
    
    typedef typename TValueHelper::TRef TRef;
    typedef typename TValueHelper::TConstRef TConstRef;
    
    typedef typename TValueHelper::TPtr TPtr;
    typedef typename TValueHelper::TConstPtr TConstPtr;
    
    //---------------------------------------------------------------------------
    SPortHelperBase( const std::string& theName, TaskBase_i::EPortType thePortType, TTaskPtr theTask )
      : SPortHelperEngine( new PortType( theName, theTask->ORB(), theTask->POA() ), thePortType, theTask )
    {}
    
    //---------------------------------------------------------------------------
    void init( const TValueHelper& the_value_helper )
    {
      this->value_helper = the_value_helper;
      
      TDataHolder_i* a_data_holder = new TDataHolder_i( the_value_helper, this->ORB(), this->POA() );
      
      this->__init__( this->task(), this->c_name(), a_data_holder->_this() );
    }
    
    void init()
    {
      this->init( this->value_helper );
    }
    
    //---------------------------------------------------------------------------
    void publish( const TValueHelper& the_value_helper )
    {
      this->value_helper = the_value_helper;
      
      TDataHolder_i* a_data_holder = new TDataHolder_i( the_value_helper, this->ORB(), this->POA() );
      
      this->__publish__( this->task(), this->c_name(), a_data_holder->_this() );
    }
    
    void publish()
    {
      this->publish( this->value_helper );
    }
    
    //---------------------------------------------------------------------------
    TValueHelper retrieve()
    {
      TDataHolderPtr a_data_holder = this->template __retrieve__< TDataHolder, DataHolderVarType >( this->task(), this->c_name() );
      
      this->value_helper = a_data_holder->value();
      
      return this->value_helper;
    }
    
    //---------------------------------------------------------------------------
    operator TValue ()
    {
      return this->value_helper.value;
    }
    
    //---------------------------------------------------------------------------
    TRef operator () ()
    {
      return this->value_helper;
    }
    
    //---------------------------------------------------------------------------
    TConstRef operator () () const
    {
      return this->value_helper;
    }
    
    //---------------------------------------------------------------------------
    TPtr operator -> ()
    {
      return this->value_helper.operator -> ();
    }
    
    //---------------------------------------------------------------------------
    TConstPtr operator -> () const
    {
      return this->value_helper.operator -> ();
    }
    
  protected:
    //---------------------------------------------------------------------------
    TValueHelper value_helper;
  };
  
  
  //---------------------------------------------------------------------------
}


//---------------------------------------------------------------------------
#endif
