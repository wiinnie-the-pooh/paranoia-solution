
#ifndef SOLVER_EXAMPLES_HEADER
#define SOLVER_EXAMPLES_HEADER

#include <TVisualizer.h>

namespace Examples
{
  class DataSource : public Impl::TTask
  {
  public:
    DataSource();

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );

  private:
    int m_Iter;
  };

  class Copy : public Impl::TTask
  {
  public:
    Copy();

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );
  };

  class Palindrome : public Impl::TTask
  {
  public:
    Palindrome();

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );
  };

  class Concat : public Impl::TTask
  {
  public:
    Concat();

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );
  };

  class StrVis : public Impl::TVisualizer
  {
  public:
    StrVis();

  protected:
    virtual void step( Base::TDomainPtr theDomain );
  };




  /*
    Arithmetic examples
  */
  class PlusXSub : public Impl::TSubTask
  {
  public:
    PlusXSub( int theSubDomain, Base::TTaskPtr theParent, double theStart, double theDelta );

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );

  private:
    double m_Start, m_Delta;
    int m_Count;
  };


  class PlusX : public Impl::TTask
  {
  public:
    PlusX( double theDelta );
    virtual ~PlusX();

  protected:
    virtual void init( Base::TDomainPtr theDomain );

  private:
    double m_Delta;
    Base::TSubTaskPtr* m_SubTasks;
  };

  class MultXSub : public Impl::TSubTask
  {
  public:
    MultXSub( int theSubDomain, Base::TTaskPtr theParent, double theStart, double theMult );

  protected:
    virtual void init( Base::TDomainPtr theDomain );
    virtual void destroy();
    virtual void step( Base::TDomainPtr theDomain );

  private:
    double m_Start, m_Mult;
    int m_Count;
  };

  class MultX : public Impl::TTask
  {
  public:
    MultX( double theMult );
    virtual ~MultX();

  protected:
    virtual void init( Base::TDomainPtr theDomain );

  private:
    double m_Mult;
    Base::TSubTaskPtr* m_SubTasks;
  };

  class ArrayVis : public Impl::TVisualizer
  {
  public:
    ArrayVis( const std::string& theName );

  protected:
    virtual void step( Base::TDomainPtr theDomain );

  private:
    std::string m_Name;
  };

  class ArrayDomain : public Base::TDomain
  {
  public:
    virtual int get_count();
    virtual std::list<int> get_subdomains();
    virtual std::list<int> get_subdomain_indices( int theSubDomain );
  };
}

#endif
