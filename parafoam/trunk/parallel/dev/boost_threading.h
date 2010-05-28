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
