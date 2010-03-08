
#ifndef TASK_HEADER
#define TASK_HEADER

#include <TSubTask.h>
#include <list>

namespace Impl
{
  class TTask : public virtual Base::TTask, virtual TSubTask
  {
  public:
    TTask( bool theIsMultiSubDomain );

    virtual void invoke( Base::TTaskMgrPtr theMgr );
    virtual void add_link( Base::TLinkPtr theLink, const std::string& theName, bool theIsInput );
    virtual Base::TFieldPtr read( const std::string& theName, int theSubDomain = 0 );
    virtual void write( const std::string& theName, Base::TFieldPtr theField );
    virtual void start_sub_task( Base::TSubTaskPtr theSubTask );
    virtual bool is_multi_subdomain();

    void write_raw( const std::string& theName, int theSubDomain, Base::TData theData, int theSize );
    Base::TTaskPtr get_this();

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );

  private:
    typedef std::map<std::string, Base::TLinkPtr> LinkMap;
    typedef std::map<std::string, std::list<Base::TLinkPtr> > LinkListMap;
    LinkMap m_In;
    LinkListMap m_Out;
    Base::TTaskPtr m_This;
    bool m_IsMultiSubDomain;
  };
}

#endif
