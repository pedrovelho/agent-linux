#ifndef __DBUS_PROXY_DBUS_CONTROLLER_H
#define __DBUS_PROXY_DBUS_CONTROLLER_H

#include <dbus-cxx.h>

#include "Controller.h"


namespace DBus {

  class ControllerProxy : public ::DBus::ObjectProxy
  {
    protected:
      ControllerProxy( ::DBus::Connection::pointer conn, const std::string& dest="proactive.agent.controller", const std::string& path="/proactive/agent/Controller"):
        ::DBus::ObjectProxy(conn, dest, path)
      {
        ::DBus::Path child_path;

        m_method_SetStartConfiguration_vsssss = this->create_method< void,std::string,std::string,std::string,std::string,std::string >( "proactive.agent.controller", "SetStartConfiguration" );
        m_method_SetJVMSettings_vsbiiib = this->create_method< void,std::string,bool,int32_t,int32_t,int32_t,bool >( "proactive.agent.controller", "SetJVMSettings" );
        m_method_SetNetworkSettings_vis = this->create_method< void,int32_t,std::string >( "proactive.agent.controller", "SetNetworkSettings" );
        m_method_StartNode_iss = this->create_method< int32_t,std::string,std::string >( "proactive.agent.controller", "StartNode" );
        m_method_StartRMNode_isssss = this->create_method< int32_t,std::string,std::string,std::string,std::string,std::string >( "proactive.agent.controller", "StartRMNode" );
        m_method_StartP2PNode_isss = this->create_method< int32_t,std::string,std::string,std::string >( "proactive.agent.controller", "StartP2PNode" );
        m_method_StartCustomNode_isss = this->create_method< int32_t,std::string,std::string,std::string >( "proactive.agent.controller", "StartCustomNode" );
        m_method_StopNode_bi = this->create_method< bool,int32_t >( "proactive.agent.controller", "StopNode" );
      }

    public:

      typedef DBusCxxPointer<ControllerProxy> pointer;

      static pointer create( ::DBus::Connection::pointer conn,
                             const std::string& dest="proactive.agent.controller",
                             const std::string& path="/proactive/agent/Controller"
                           )
      { return pointer( new ControllerProxy(conn, dest, path)); }


      void SetStartConfiguration( std::string security_policy, std::string log4j_file, std::string proactive_home, std::string classpath, std::string java_bin ) { return (*m_method_SetStartConfiguration_vsssss)( security_policy, log4j_file, proactive_home, classpath, java_bin); }

      void SetJVMSettings( std::string jvm_args, bool enable_mem_mngmnt, int32_t java_memory, int32_t native_memory, int32_t no_processes, bool use_all_cpus ) { return (*m_method_SetJVMSettings_vsbiiib)( jvm_args, enable_mem_mngmnt, java_memory, native_memory, no_processes, use_all_cpus); }

      void SetNetworkSettings( int32_t port_value, std::string protocol ) { return (*m_method_SetNetworkSettings_vis)( port_value, protocol); }

      int32_t StartNode( std::string name, std::string java_class ) { return (*m_method_StartNode_iss)( name, java_class); }

      int32_t StartRMNode( std::string name, std::string java_class, std::string user, std::string password, std::string url ) { return (*m_method_StartRMNode_isssss)( name, java_class, user, password, url); }

      int32_t StartP2PNode( std::string name, std::string java_class, std::string contact ) { return (*m_method_StartP2PNode_isss)( name, java_class, contact); }

      int32_t StartCustomNode( std::string name, std::string java_class, std::string arguments ) { return (*m_method_StartCustomNode_isss)( name, java_class, arguments); }

      bool StopNode( int32_t pid ) { return (*m_method_StopNode_bi)( pid); }


    protected:


      ::DBus::MethodProxy<void,std::string,std::string,std::string,std::string,std::string>::pointer m_method_SetStartConfiguration_vsssss;
      ::DBus::MethodProxy<void,std::string,bool,int32_t,int32_t,int32_t,bool>::pointer m_method_SetJVMSettings_vsbiiib;
      ::DBus::MethodProxy<void,int32_t,std::string>::pointer m_method_SetNetworkSettings_vis;
      ::DBus::MethodProxy<int32_t,std::string,std::string>::pointer m_method_StartNode_iss;
      ::DBus::MethodProxy<int32_t,std::string,std::string,std::string,std::string,std::string>::pointer m_method_StartRMNode_isssss;
      ::DBus::MethodProxy<int32_t,std::string,std::string,std::string>::pointer m_method_StartP2PNode_isss;
      ::DBus::MethodProxy<int32_t,std::string,std::string,std::string>::pointer m_method_StartCustomNode_isss;
      ::DBus::MethodProxy<bool,int32_t>::pointer m_method_StopNode_bi;


  };

}

#endif
