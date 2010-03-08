
#ifndef SUB_TASK_HEADER
#define SUB_TASK_HEADER

#include <Interfaces.h>

namespace Impl
{
  class TSubTask : public virtual Base::TSubTask
  {
  public:
    TSubTask( int theSubDomain = -1, Base::TTaskPtr theParent = Base::TTaskPtr() );

    virtual void invoke( Base::TTaskMgrPtr theMgr );

  protected:
    int get_subdomain();
    Base::TFieldPtr read( const std::string& theName );
    void write( const std::string& theName, Base::TData theData, int theSize );

  protected:
    virtual void init( Base::TDomainPtr theDomain ) = 0;
    virtual void destroy() = 0;
    virtual void step( Base::TDomainPtr theDomain ) = 0;
    
  private:
    int m_SubDomain;
    Base::TTaskPtr m_Parent;
  };
}

#endif
