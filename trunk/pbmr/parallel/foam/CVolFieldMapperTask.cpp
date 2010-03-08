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
#include "parallel/foam/CVolFieldMapperTask.h"

#include "parallel/foam/mapConsistentField.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace foam
  {
    //-----------------------------------------------------------------------
    CVolFieldMapperTask::CVolFieldMapperTask()
      : m_stop_i( "stop", true, *this )
      , m_SourceField_i( "theSourceField", true, *this )
      , m_SourceMesh_i( "theSourceMesh", true, *this )
      , m_TargetMesh_i( "theTargetMesh", true, *this )
      , m_TargetField_o( "aTargetField", false, *this )
      , m_finished_o( "finished", false, *this )
    {}


    //-----------------------------------------------------------------------
    void CVolFieldMapperTask::init()
    {
      std::cout << "\nStart of CVolFieldMapperTask[ " << this << " ]\n";

      this->m_SourceMesh_i.retrieve();
      this->m_TargetMesh_i.retrieve();
    }


    //-----------------------------------------------------------------------
    bool CVolFieldMapperTask::step()
    {
      std::cout << "\nCVolFieldMapperTask[ " << this << " ]::step\n";

      if ( !this->m_stop_i.retrieve() )
      {
        this->m_SourceField_i.retrieve( this->m_SourceMesh_i );
      
        std::cout << "\nCVolFieldMapperTask[ " << this << " ]"
                  << " | " << this->m_TargetMesh_i->time().timeName().c_str()
                  << " | \"" << this->m_SourceField_i->name().c_str() << "\"\n";
        
        this->m_TargetField_o.publish( clone( mapConsistentField( this->m_SourceField_i(), this->m_TargetMesh_i() ) ) );
      }

      this->m_finished_o.publish( this->m_stop_i() );

      return ! this->m_finished_o();
    }
    
    
    //-----------------------------------------------------------------------
    void CVolFieldMapperTask::destroy()
    {
      std::cout << "\nEnd of CVolFieldMapperTask[ " << this << " ]\n";
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
