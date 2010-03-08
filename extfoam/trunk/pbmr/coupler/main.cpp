
#include <Interfaces.h>
#include <Holders.h>
#include <TTaskMgr.h>
#include <TLinkFactory.h>

#include <SolverExamples.h>
#include <iostream>
#include "Coupling.h"

//---------------------------------------------------------------------------
int main1()
{
  Base::TTaskMgrPtr aMgr( Impl::TTaskMgr::get_instance() );

  Base::TTaskPtr a( new Examples::DataSource() );
  Base::TTaskPtr b( new Examples::Copy() );
  Base::TTaskPtr c( new Examples::Palindrome() );
  Base::TTaskPtr d( new Examples::Concat() );

  Base::TTaskPtr v1( new Examples::StrVis() );
  Base::TTaskPtr v2( new Examples::StrVis() );
  Base::TTaskPtr v3( new Examples::StrVis() );

  aMgr->add_task( a );
  aMgr->add_task( b );
  aMgr->add_task( c );
  aMgr->add_task( d );
  aMgr->add_task( v1 );
  aMgr->add_task( v2 );
  aMgr->add_task( v3 );

  aMgr->connect( a, "res", b, "arg" );
  aMgr->connect( a, "res", c, "arg" );
  aMgr->connect( b, "res", d, "arg1" );
  aMgr->connect( c, "res", d, "arg2" );

  aMgr->connect( c, "res", v1, "arg" );
  aMgr->connect( d, "res", v2, "arg" );
  aMgr->connect( b, "res", v3, "arg" );

  std::cout << "Start" << std::endl;
  aMgr->start();
  sleep( 5 );
  std::cout << "Pause" << std::endl;
  aMgr->pause();
  sleep( 2 );
  std::cout << "Resume" << std::endl;
  aMgr->resume();
  sleep( 5 );
  std::cout << "Stop" << std::endl;
  aMgr->stop();
  return 0;
}


//---------------------------------------------------------------------------
int main()
{
  using namespace Base;

  HTaskMgr< Impl::TTaskMgr >::TPtr aMgr( Impl::TTaskMgr::get_instance() );

  HDomain<>::TPtr aDomain = HDomain<>::create( new Examples::ArrayDomain() );
  aMgr->device()->set_domain( aDomain );

  HLinkFactory< Impl::TLinkFactory >::TPtr a_link_factory =
      HLinkFactory< Impl::TLinkFactory >::create( new Impl::TLinkFactory() );
  aMgr->device()->set_link_factory( a_link_factory );

  HTask<>::TPtr a = HTask<>::create( new Examples::PlusX( 1 ) );
  HTask<>::TPtr b = HTask<>::create( new Examples::MultX( 2 ) );

  HTask<>::TPtr v1 = HTask<>::create( new Examples::ArrayVis( "a" ) );
  HTask<>::TPtr v2 = HTask<>::create( new Examples::ArrayVis( "b" ) );

  aMgr->add_task( a );
  aMgr->add_task( b );
  aMgr->add_task( v1 );
  aMgr->add_task( v2 );

  aMgr->connect( a, "res", b, "arg" );
  aMgr->connect( b, "res", a, "arg" );

  aMgr->connect( a, "res", v1, "arg" );
  aMgr->connect( b, "res", v2, "arg" );

  std::cout << "Start" << std::endl;
  aMgr->start();
  sleep( 5 );
  std::cout << "Pause" << std::endl;
  aMgr->pause();
  sleep( 2 );
  std::cout << "Resume" << std::endl;
  aMgr->resume();
  sleep( 5 );
  std::cout << "Stop" << std::endl;
  aMgr->stop();
  return 0;
}


//---------------------------------------------------------------------------
int main3()
{
  Coupling cp;
  CppLinkInfo arg, res;

  Base::HTaskMgr< Impl::TTaskMgr >::TPtr aMgr( Impl::TTaskMgr::get_instance() );
  aMgr->device()->set_domain( new Examples::ArrayDomain() );

  for( int i=0; i<2; i++ )
  {
    cp.START( res );

    arg = res;
    cp.plus_x( arg, res );

    arg = res;
    cp.mult_x( arg, res );

    CppLinkInfo varg = res;
    cp.array_vis( varg );

    arg = res;
    cp.END( arg );
  }

  sleep( 5 );
  aMgr->stop();
  
  return 0;
}


//---------------------------------------------------------------------------

