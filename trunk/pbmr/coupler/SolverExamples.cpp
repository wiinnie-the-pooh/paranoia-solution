
#include <SolverExamples.h>
#include <iostream>

namespace Examples {

DataSource::DataSource()
: TTask( false )
{
}

void DataSource::init( Base::TDomainPtr theDomain )
{
  m_Iter = 0;
  step( theDomain );
}

void DataSource::destroy()
{
}

void DataSource::step( Base::TDomainPtr theDomain )
{
  //std::cout << "DataSource::step()" << std::endl;
  char buf[100];
  sprintf( buf, (char*)"%s_%i", "source", m_Iter );
  usleep( 200000 );
  write_raw( "res", 0, buf, strlen( buf ) + 1 );
  m_Iter++;
}



Copy::Copy()
: TTask( false )
{
}

void Copy::init( Base::TDomainPtr theDomain )
{
  write_raw( "res", 0, (void*)"", 1 );
}

void Copy::destroy()
{
}

void Copy::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr f = read( "arg" );
  char* str = (char*)f->get_raw_data();
  usleep( 200000 );
  write_raw( "res", 0, str, strlen( str ) + 1 );
}



Palindrome::Palindrome()
: TTask( false )
{
}

void Palindrome::init( Base::TDomainPtr theDomain )
{
  write_raw( "res", 0, (void*)"", 1 );
}

void Palindrome::destroy()
{
}

void Palindrome::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr field = read( "arg" );
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
  usleep( 200000 );
  write_raw( "res", 0, str, strlen( str ) + 1 );
  free( str );
}



Concat::Concat()
: TTask( false )
{
}

void Concat::init( Base::TDomainPtr theDomain )
{
  write_raw( "res", 0, (void*)"", 1 );
}

void Concat::destroy()
{
}

void Concat::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr f1 = read( "arg1" ), f2 = read( "arg2" );
  char* s1 = (char*)f1->get_raw_data();
  char* s2 = (char*)f2->get_raw_data();
  int len = strlen( s1 ) + strlen( s2 ) + 1;
  char* res = (char*)malloc( len );
  strcpy( res, s1 );
  strcat( res, s2 );
  usleep( 200000 );
  write_raw( "res", 0, res, len );
  free( res );
}








PlusXSub::PlusXSub( int theSubDomain, Base::TTaskPtr theParent, double theStart, double theDelta )
: TSubTask( theSubDomain, theParent ), m_Start( theStart ), m_Delta( theDelta ), m_Count( 0 )
{
}

void PlusXSub::init( Base::TDomainPtr theDomain )
{
  m_Count = theDomain->get_subdomain_indices( get_subdomain() ).size();
  Base::TDataItem* aData = new Base::TDataItem[m_Count];
  for( int i=0; i<m_Count; i++ )
    aData[i] = m_Start;
  write( "res", aData, sizeof( Base::TDataItem ) * m_Count );
  delete[] aData;
}

void PlusXSub::destroy()
{
}

void PlusXSub::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr aField = read( "arg" );
  Base::TDataItem* aValues = (Base::TDataItem*)aField->get_raw_data();
  for( int i=0; i<m_Count; i++ )
    aValues[i] += m_Delta;
  usleep( 200000 );
  write( "res", aValues, sizeof( Base::TDataItem ) * m_Count );
}


PlusX::PlusX( double theDelta )
  : TTask( true ), m_Delta( theDelta ), m_SubTasks( 0 )
{
}

PlusX::~PlusX()
{
  delete[] m_SubTasks;
}

void PlusX::init( Base::TDomainPtr theDomain )
{
  std::list<int> aSubdomains = theDomain->get_subdomains();
  int aSubCount = aSubdomains.size();
  m_SubTasks = new Base::TSubTaskPtr[aSubCount];
  std::list<int>::const_iterator anIt = aSubdomains.begin(), aLast = aSubdomains.end();
  for( int i=0; anIt!=aLast; anIt++, i++ )
  {
    m_SubTasks[i] = Base::TSubTaskPtr( new PlusXSub( *anIt, get_this(), i+1, m_Delta ) );
    start_sub_task( m_SubTasks[i] );
  }
}





MultXSub::MultXSub( int theSubDomain, Base::TTaskPtr theParent, double theStart, double theMult )
: TSubTask( theSubDomain, theParent ), m_Start( theStart ), m_Mult( theMult )
{
}

void MultXSub::init( Base::TDomainPtr theDomain )
{
  m_Count = theDomain->get_subdomain_indices( get_subdomain() ).size();
  Base::TDataItem* aData = new Base::TDataItem[m_Count];
  for( int i=0; i<m_Count; i++ )
    aData[i] = m_Start;
  write( "res", aData, sizeof( Base::TDataItem ) * m_Count );
  delete[] aData;
}

void MultXSub::destroy()
{
}

void MultXSub::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr aField = read( "arg" );
  Base::TDataItem* aValues = (Base::TDataItem*)aField->get_raw_data();
  for( int i=0; i<m_Count; i++ )
    aValues[i] *= m_Mult;
  usleep( 200000 );
  write( "res", aValues, sizeof( Base::TDataItem ) * m_Count );
}



MultX::MultX( double theMult )
: TTask( true ), m_Mult( theMult ), m_SubTasks( 0 )
{
}

MultX::~MultX()
{
  delete[] m_SubTasks;
}

void MultX::init( Base::TDomainPtr theDomain )
{
  std::list<int> aSubdomains = theDomain->get_subdomains();
  int aSubCount = aSubdomains.size();
  m_SubTasks = new Base::TSubTaskPtr[aSubCount];
  std::list<int>::const_iterator anIt = aSubdomains.begin(), aLast = aSubdomains.end();
  for( int i=0; anIt!=aLast; anIt++, i++ )
  {
    m_SubTasks[i] = Base::TSubTaskPtr( new MultXSub( *anIt, get_this(), i+1, m_Mult ) );
    start_sub_task( m_SubTasks[i] );
  }
}





StrVis::StrVis()
: TVisualizer( false )
{
}

void StrVis::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr f = read( "arg" );
  char* str = (char*)f->get_raw_data();
  std::cout << "VIS: '" << str << "'" << std::endl;
}





ArrayVis::ArrayVis( const std::string& theName )
: TVisualizer( false ), m_Name( theName )
{
}

void ArrayVis::step( Base::TDomainPtr theDomain )
{
  Base::TFieldPtr aField = read( "arg" );
  Base::TDataItem* aValues = (Base::TDataItem*)aField->get_raw_data();

  printf( "VIS %s: [ ", m_Name.c_str() );
  for( int i=0; i<theDomain->get_count(); i++ )
    printf( "%lf ", aValues[i] );
  printf( "]\n" );
}





int ArrayDomain::get_count()
{
  return 10;
}

std::list<int> ArrayDomain::get_subdomains()
{
  std::list<int> res;
  res.push_back( 1 );
  res.push_back( 3 );
  res.push_back( 5 );
  return res;
}

std::list<int> ArrayDomain::get_subdomain_indices( int theSubDomain )
{
  std::list<int> res;
  switch( theSubDomain )
    {
    case 1:
      res.push_back( 0 );
      res.push_back( 5 );
      res.push_back( 7 );
      res.push_back( 8 );
      break;
    case 3:
      res.push_back( 1 );
      res.push_back( 2 );
      res.push_back( 9 );
      break;
    case 5:
      res.push_back( 3 );
      res.push_back( 4 );
      res.push_back( 6 );
      break;
    }
  return res;
}

}
