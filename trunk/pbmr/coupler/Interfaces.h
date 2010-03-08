
#ifndef INTERFACES_HEADER
#define INTERFACES_HEADER

#include <TSmartPtr.h>

#include <map>
#include <string>
#include <list>

namespace Base
{
  /*
    Raw data transferred between solvers and modifiers (also known as tasks)
  */
  typedef void* TData;
  typedef double TDataItem;

  /*
    Wrapper for raw data
  */
  struct TField : virtual TGenericObj
  {
    virtual std::string get_name() = 0;
    virtual int get_subdomain() = 0;
    virtual TData get_raw_data() = 0;
    virtual int get_size() = 0;
  };
  typedef SmartPtrDef< TField >::type TFieldPtr;

  /*
    Queue
  */
  struct TQueue : virtual TGenericObj
  {
    virtual void write( TFieldPtr theField ) = 0;
    virtual TFieldPtr read() = 0;
  };
  typedef SmartPtrDef< TQueue >::type TQueuePtr;

  /*
    Link
  */
  struct TLink : virtual TGenericObj
  {
    virtual void write( TFieldPtr theField ) = 0;
    virtual TFieldPtr read() = 0;
    virtual TFieldPtr read_domain( int theSubDomain ) = 0;
  };
  typedef SmartPtrDef< TLink >::type TLinkPtr;

  /*
    Link factory
  */
  struct TDomain;
  typedef SmartPtrDef< TDomain >::type TDomainPtr;

  struct TLinkFactory : virtual TGenericObj
  {
    virtual TLinkPtr create_cable_link() const = 0;
    virtual TLinkPtr create_collector_link( const TDomainPtr& the_domain ) const = 0;
    virtual TLinkPtr create_diffusor_link( const TDomainPtr& the_domain ) const = 0;
    virtual TLinkPtr create_tube_link() const = 0;
  };
  typedef SmartPtrDef< TLinkFactory >::type TLinkFactoryPtr;

  /*
    Sub task
  */
  struct TTaskMgr;
  typedef SmartPtrDef< TTaskMgr >::type TTaskMgrPtr;

  struct TSubTask : virtual TGenericObj
  {
    virtual void invoke( TTaskMgrPtr theMgr ) = 0;
  };
  typedef SmartPtrDef< TSubTask >::type TSubTaskPtr;

  /*
    Task
  */
  struct TTask : virtual TSubTask
  {
    virtual void add_link( TLinkPtr theLink, 
                           const std::string& theName, 
                           bool theIsInput ) = 0;
    virtual TFieldPtr read( const std::string& theName, 
                            int theSubDomain ) = 0;
    virtual void write( const std::string& theName, 
                        TFieldPtr theField ) = 0;
    virtual bool is_multi_subdomain() = 0;
  };
  typedef SmartPtrDef< TTask >::type TTaskPtr;

  /*
    Domain
  */
  struct TDomain : virtual TGenericObj
  {
    virtual int get_count() = 0;
    virtual std::list<int> get_subdomains() = 0;
    virtual std::list<int> get_subdomain_indices( int theSubDomain ) = 0;
  };

  /*
    Task manager
  */
  struct TTaskMgr : virtual TGenericObj
  {
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual bool is_run() = 0;
    virtual void add_task( TTaskPtr theTask ) = 0;
    virtual void connect( TTaskPtr theSource, 
                          const std::string& theSourceName,
                          TTaskPtr theDest, 
                          const std::string& theDestName ) = 0;
    virtual TDomainPtr get_domain() = 0;
  };
}

#endif
