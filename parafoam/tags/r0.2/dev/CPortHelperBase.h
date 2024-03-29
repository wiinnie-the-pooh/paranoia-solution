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
#ifndef dev_CPortHelperBase_h
#define dev_CPortHelperBase_h


//---------------------------------------------------------------------------
#include "parallel/dev/CPortHelperEngine.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace dev
  {
    //---------------------------------------------------------------------------
    template< class PortType >
    struct CPortHelperBase : CPortHelperEngine
    {
      typedef typename PortType::TDataHolder TDataHolder;
      typedef typename base::SmartPtrDef< TDataHolder >::type TDataHolderPtr;
      
      typedef typename PortType::TValueHelper TValueHelper;
      typedef typename TValueHelper::TValue TValue;

      typedef typename TValueHelper::TRef TRef;
      typedef typename TValueHelper::TConstRef TConstRef;

      typedef typename TValueHelper::TPtr TPtr;
      typedef typename TValueHelper::TConstPtr TConstPtr;

      //---------------------------------------------------------------------------
      CPortHelperBase( const std::string& theName, bool theIsInput, TTask& theTask )
        : CPortHelperEngine( new PortType( theName, theTask ), theIsInput )
      {}

      //---------------------------------------------------------------------------
      void init( const TValueHelper& the_value_helper )
      {
        this->value_helper = the_value_helper;

        TDataHolderPtr a_data_holder = new TDataHolder( the_value_helper );
        
        this->__init__( this->task(), this->name(), a_data_holder );
      }
      
      void init()
      {
        this->init( this->value_helper );
      }
      
      //---------------------------------------------------------------------------
      void publish( const TValueHelper& the_value_helper )
      {
        this->value_helper = the_value_helper;

        TDataHolderPtr a_data_holder = new TDataHolder( the_value_helper );

        this->__publish__( this->task(), this->name(), a_data_holder );
      }

      void publish()
      {
        this->publish( this->value_helper );
      }

      //---------------------------------------------------------------------------
      TValueHelper retrieve()
      {
        TDataHolderPtr a_data_holder = this->template __retrieve__< TDataHolder >( this->task(), this->name() );

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
      TPtr operator ->  ()
      {
        return this->value_helper.operator -> ();
      }

      //---------------------------------------------------------------------------
      TConstPtr operator ->  () const
      {
        return this->value_helper.operator -> ();
      }

    protected:
      //---------------------------------------------------------------------------
      TValueHelper value_helper;
    };
    
    
    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
