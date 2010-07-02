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
#ifndef dev_boost_threading_h
#define dev_boost_threading_h

#include <boost/thread.hpp>
#include <boost/version.hpp>


//---------------------------------------------------------------------------
namespace parallel
{
  namespace threading
  {
    //-----------------------------------------------------------------------
    template< class MutexType >
    void lock( MutexType& the_mutex )
    {
#if BOOST_VERSION >= 103500
      the_mutex.lock();
#else
      boost::detail::thread::lock_ops< MutexType >::lock( the_mutex );
#endif
    };


    //-----------------------------------------------------------------------
    template< class MutexType >
    void unlock( MutexType& the_mutex )
    {
#if BOOST_VERSION >= 103500
      the_mutex.unlock();
#else
      boost::detail::thread::lock_ops< MutexType >::unlock( the_mutex );
#endif
    };


    //-----------------------------------------------------------------------
 }
}


//---------------------------------------------------------------------------
#endif
