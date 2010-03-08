
#ifndef HOLDERS_HEADER
#define HOLDERS_HEADER

#include <Interfaces.h>

namespace Base
{
  //---------------------------------------------------------------------------
  template< class TInerface, class THolder, class TDevice >
  struct TDeviceHolder : virtual TInerface
  {
    typedef typename SmartPtrDef< TDevice >::type TDevicePtr;

    typedef typename SmartPtrDef< THolder >::type TPtr;

    static TPtr create( TDevice* the_device )
    {
      THolder* aHolder = new THolder();
      aHolder->m_device_ptr = TDevicePtr( the_device );
      return aHolder;
    }

    TDevice* device() const
    {
      return this->m_device_ptr.operator->();
    }

    TDevice* device()
    {
      return this->m_device_ptr.operator->();
    }

  protected:
    TDeviceHolder()
    {}

  private:
    mutable TDevicePtr m_device_ptr;

    TDeviceHolder( const TDeviceHolder& ); // not implemented
    TDeviceHolder& operator = ( const TDeviceHolder& ); // not implemented
  };


  //---------------------------------------------------------------------------
  template< class TDevice >
  struct HField : virtual TDeviceHolder< TField, HField< TDevice >, TDevice >
  {
    virtual std::string get_name()
    {
      return this->device()->get_name();
    }

    virtual int get_subdomain()
    {
      return this->device()->get_subdomain();
    }

    virtual TData get_raw_data()
    {
      return this->device()->get_raw_data();
    }

    virtual int get_size()
    {
      return this->device()->get_size();
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TQueue >
  struct HQueue : virtual TDeviceHolder< TQueue, HQueue< TDevice >, TDevice >
  {
    virtual void write( TFieldPtr theField )
    {
      return this->device()->write( theField );
    }

    virtual TFieldPtr read()
    {
      return this->device()->read();
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TLink >
  struct HLink : virtual TDeviceHolder< TLink, HLink< TDevice >, TDevice >
  {
    virtual void write( TFieldPtr theField )
    {
      return this->device()->write( theField );
    }

    virtual TFieldPtr read()
    {
      return this->device()->read();
    }

    virtual TFieldPtr read_domain( int theSubDomain )
    {
      return this->device()->read_domain( theSubDomain );
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TLinkFactory >
  struct HLinkFactory : virtual TDeviceHolder< TLinkFactory, HLinkFactory< TDevice >, TDevice >
  {
    virtual TLinkPtr create_cable_link() const
    {
      return this->device()->create_cable_link();
    }

    virtual TLinkPtr create_collector_link( const TDomainPtr& the_domain ) const
    {
      return this->device()->create_cable_link();
    }

    virtual TLinkPtr create_diffusor_link( const TDomainPtr& the_domain ) const
    {
      return this->device()->create_cable_link();
    }

    virtual TLinkPtr create_tube_link() const
    {
      return this->device()->create_tube_link();
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TSubTask >
  struct HSubTask : virtual TDeviceHolder< TSubTask, HSubTask< TDevice >, TDevice >
  {
    virtual void invoke( TTaskMgrPtr theMgr )
    {
      return this->device()->invoke( theMgr );
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TTask >
  struct HTask : virtual TDeviceHolder< TTask, HTask< TDevice >, TDevice >
  {
    virtual void invoke( TTaskMgrPtr theMgr )
    {
      return this->device()->invoke( theMgr );
    }

    virtual void add_link( TLinkPtr theLink, 
                           const std::string& theName, 
                           bool theIsInput )
    {
      return this->device()->add_link( theLink, theName, theIsInput );
    }

    virtual TFieldPtr read( const std::string& theName, 
                                  int theSubDomain )
    {
      return this->device()->read( theName, theSubDomain );
    }

    virtual void write( const std::string& theName, 
                        TFieldPtr theField )
    {
      return this->device()->write( theName, theField );
    }

    virtual bool is_multi_subdomain()
    {
      return this->device()->is_multi_subdomain();
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TDomain >
  struct HDomain : virtual TDeviceHolder< TDomain, HDomain< TDevice >, TDevice >
  {
    virtual int get_count()
    {
      return this->device()->get_count();
    }

    virtual std::list<int> get_subdomains()
    {
      return this->device()->get_subdomains();
    }

    virtual std::list<int> get_subdomain_indices( int theSubDomain )
    {
      return this->device()->get_subdomain_indices( theSubDomain );
    }
  };


  //---------------------------------------------------------------------------
  template< class TDevice = TTaskMgr >
  struct HTaskMgr : virtual TDeviceHolder< TTaskMgr, HTaskMgr< TDevice >, TDevice >
  {
    virtual void start()
    {
      return this->device()->start();
    }

    virtual void stop()
    {
      return this->device()->stop();
    }

    virtual void pause()
    {
      return this->device()->pause();
    }

    virtual void resume()
    {
      return this->device()->resume();
    }

    virtual bool is_run()
    {
      return this->device()->is_run();
    }

    virtual void add_task( TTaskPtr theTask )
    {
      return this->device()->add_task( theTask );
    }

    virtual void connect( TTaskPtr theSource, 
                          const std::string& theSourceName,
                          TTaskPtr theDest, 
                          const std::string& theDestName )
    {
      return this->device()->connect( theSource, theSourceName,
                                      theDest, theDestName );
    }
    
    virtual TDomainPtr get_domain()
    {
      return this->device()->get_domain();
    }
  };


  //---------------------------------------------------------------------------
}

#endif
