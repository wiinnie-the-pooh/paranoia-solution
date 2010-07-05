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
#ifndef toys_SolverExamples_h
#define toys_SolverExamples_h


//---------------------------------------------------------------------------
#include "parallel/test/threading/toys/TRawDataTask.h"


//---------------------------------------------------------------------------
namespace parallel
{
  namespace toys
  {
    //---------------------------------------------------------------------------
    struct SleepSolver : TRawDataTask
    {
      SleepSolver( int theMSecs );

    protected:
      void sleep();

    private:
      int m_MSecs;
    };

  
    //---------------------------------------------------------------------------
    struct DataSource : SleepSolver
    {
      DataSource( int theMSecs );

    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Iter;
    };


    //---------------------------------------------------------------------------
    struct Copy : SleepSolver
    {
      Copy( int theMSecs );

    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct Palindrome : SleepSolver
    {
      Palindrome( int theMSecs );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct Concat : SleepSolver
    {
      Concat( int theMSecs );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
    };


    //---------------------------------------------------------------------------
    struct PlusX : SleepSolver
    {
      PlusX( int theMSecs, int theStart, int theDelta );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Start, m_Delta;
    };


    //---------------------------------------------------------------------------
    struct MultX : SleepSolver
    {
      MultX( int theMSecs, int theStart, int theMult );
      
    protected:
      virtual void init();
      virtual void destroy();
      virtual bool step();
      
    private:
      int m_Start, m_Mult;
    };


    //---------------------------------------------------------------------------
  }
}


//---------------------------------------------------------------------------
#endif
