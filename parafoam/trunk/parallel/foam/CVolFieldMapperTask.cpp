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
      MSG( "Start of CVolFieldMapperTask[ " << this << " ]\n" );

      this->m_SourceMesh_i.retrieve();
      this->m_TargetMesh_i.retrieve();
    }


    //-----------------------------------------------------------------------
    bool CVolFieldMapperTask::step()
    {
      if ( !this->m_stop_i.retrieve() )
      {
        this->m_SourceField_i.retrieve( this->m_SourceMesh_i );
      
        MSG( "CVolFieldMapperTask[ " << this << " ]"
	     << " | " << this->m_TargetMesh_i->time().timeName().c_str()
	     << " | \"" << this->m_SourceField_i->name().c_str() << "\"\n" );
        
        this->m_TargetField_o.publish( clone( mapConsistentField( this->m_SourceField_i(), this->m_TargetMesh_i() ) ) );
      }

      this->m_finished_o.publish( this->m_stop_i() );

      return ! this->m_finished_o();
    }
    
    
    //-----------------------------------------------------------------------
    void CVolFieldMapperTask::destroy()
    {
      MSG( "End of CVolFieldMapperTask[ " << this << " ]\n" );
    }


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
