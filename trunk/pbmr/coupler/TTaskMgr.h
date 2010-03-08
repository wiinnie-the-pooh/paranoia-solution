
#ifndef TASK_MANAGER_HEADER
#define TASK_MANAGER_HEADER

#include <Interfaces.h>
#include <Holders.h>

#include <list>

namespace Impl
{
  class TTaskMgr : public virtual Base::TTaskMgr
  {
  public:
    typedef Base::HTaskMgr< TTaskMgr > THolder;
    typedef THolder::TPtr TPtr;
    
    static TPtr get_instance();

    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual bool is_run();
    virtual void add_task( Base::TTaskPtr theTask );
    virtual void connect( Base::TTaskPtr theSource, const std::string& theSourceName,
                          Base::TTaskPtr theDest, const std::string& theDestName );
    virtual Base::TDomainPtr get_domain();

    void set_domain( Base::TDomainPtr theDomain );
    void set_link_factory( Base::TLinkFactoryPtr the_link_factory );

  protected:
    TTaskMgr();

  private:
    typedef std::list<Base::TTaskPtr> TaskList;
    typedef TaskList::const_iterator TaskListIt;
    TaskList m_Tasks;
    bool m_IsRun;
    pthread_mutex_t m_Pause;

    Base::TDomainPtr m_Domain;
    Base::TLinkFactoryPtr m_link_factory;
  };
}

#endif
