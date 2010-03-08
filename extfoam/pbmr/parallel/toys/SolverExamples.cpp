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
#include "parallel/toys/SolverExamples.h"

#include <iostream>
#include <string.h>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    SleepSolver::SleepSolver( int theMSecs )
      : m_MSecs( theMSecs )
    {}

    void SleepSolver::sleep()
    {
      usleep( m_MSecs * 1000 );
    }


    //---------------------------------------------------------------------------
    DataSource::DataSource( int theMSecs )
      : SleepSolver( theMSecs )
    {}

    void DataSource::init()
    {
      m_Iter = 0;
      step();
    }

    void DataSource::destroy()
    {}

    bool DataSource::step()
    {
      //std::cout << "DataSource::step()" << std::endl;
      char buf[100];
      sprintf( buf, (char*)"%s_%i", "source", m_Iter );
      sleep();
      write_raw( "res", buf, strlen( buf ) + 1 );
      m_Iter++;
      
      return true;
    }
    

    //---------------------------------------------------------------------------
    Copy::Copy( int theMSecs )
      : SleepSolver( theMSecs )
    {}

    void Copy::init()
    {
      write_raw( "res", (void*)"", 1 );
    }
    
    void Copy::destroy()
    {}

    bool Copy::step()
    {
      TFieldPtr f = read( "arg" );
      char* str = (char*)f->get_raw_data();
      sleep();
      write_raw( "res", str, strlen( str ) + 1 );
      
      return true;
    }


    //---------------------------------------------------------------------------
    Palindrome::Palindrome( int theMSecs )
      : SleepSolver( theMSecs )
    {}

    void Palindrome::init()
    {
      write_raw( "res", (void*)"", 1 );
    }
    
    void Palindrome::destroy()
    {}

    bool Palindrome::step()
    {
      TFieldPtr field = read( "arg" );
      char* str = strdup( (char*)field->get_raw_data() );
      char* f = str, *l = str + strlen(str) - 1;
      while( l-f > 0 )
      {
        char tmp = *f;
        *f = *l;
        *l = tmp;
        f++;
        l--;
      }
      sleep();
      write_raw( "res", str, strlen( str ) + 1 );
      free( str );

      return true;
    }


    //---------------------------------------------------------------------------
    Concat::Concat( int theMSecs )
      : SleepSolver( theMSecs )
    {}

    void Concat::init()
    {
      write_raw( "res", (void*)"", 1 );
    }
    
    void Concat::destroy()
    {}

    bool Concat::step()
    {
      TFieldPtr f1 = read( "arg1" ), f2 = read( "arg2" );
      char* s1 = (char*)f1->get_raw_data();
      char* s2 = (char*)f2->get_raw_data();
      int len = strlen( s1 ) + strlen( s2 ) + 1;
      char* res = (char*)malloc( len );
      strcpy( res, s1 );
      strcat( res, s2 );
      sleep();
      write_raw( "res", res, len );
      free( res );

      return true;
    }


    //---------------------------------------------------------------------------
    PlusX::PlusX( int theMSecs, int theStart, int theDelta )
      : SleepSolver( theMSecs ), m_Start( theStart ), m_Delta( theDelta )
    {}

    void PlusX::init()
    {
      write_raw( "res", &m_Start, sizeof( int ) );
    }
    
    void PlusX::destroy()
    {}

    bool PlusX::step()
    {
      TFieldPtr aField = read( "arg" );
      int aVal = *(int*)aField->get_raw_data();
      aVal += m_Delta;
      sleep();
      write_raw( "res", &aVal, sizeof( int ) );

      return true;
    }


    //---------------------------------------------------------------------------
    MultX::MultX( int theMSecs, int theStart, int theMult )
      : SleepSolver( theMSecs ), m_Start( theStart ), m_Mult( theMult )
    {}

    void MultX::init()
    {
      write_raw( "res", &m_Start, sizeof( int ) );
    }
    
    void MultX::destroy()
    {}

    bool MultX::step()
    {
      TFieldPtr aField = read( "arg" );
      int aVal = *(int*)aField->get_raw_data();
      aVal *= m_Mult;
      sleep();
      write_raw( "res", &aVal, sizeof( int ) );
      
      return true;
    }
    

    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
