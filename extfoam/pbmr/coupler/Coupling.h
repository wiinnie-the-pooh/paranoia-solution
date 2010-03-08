
#ifndef _COUPLING_HXX_
#define _COUPLING_HXX_

#include <Interfaces.h>
#include <string>

typedef struct
{
  std::string m_Port, m_TaskName;
} CppLinkInfo;

class Coupling
{
public:
  Coupling();
  virtual ~Coupling();

  void START( CppLinkInfo& res );
  void plus_x( const CppLinkInfo& arg, CppLinkInfo& res );
  void mult_x( const CppLinkInfo& arg, CppLinkInfo& res );
  void array_vis( const CppLinkInfo& arg );
  void END( const CppLinkInfo& arg );

protected:
  bool create_task( const std::string& theName, Base::TTaskPtr theTask );
  void create_input( const std::string& theTaskName, const std::string& thePortName, const CppLinkInfo& thePortData );
  void create_output( const std::string& theTaskName, const std::string& thePortName, CppLinkInfo& theData );

private:
  static std::map<std::string, Base::TTaskPtr> m_Tasks;
  static std::list<CppLinkInfo> m_StartTasks;
};

#endif
