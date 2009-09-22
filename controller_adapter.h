#ifndef __DBUS_ADAPTER_DBUS_CONTROLLER_H
#define __DBUS_ADAPTER_DBUS_CONTROLLER_H

#include <dbus-cxx.h>

#include "Controller.h"


namespace DBus {

  class ControllerAdapter : public ::DBus::Object
  {
    protected:
      ControllerAdapter( Controller* adaptee=NULL, const std::string& path="/proactive/agent/Controller"):
        ::DBus::Object(path),
        m_adaptee(adaptee)
      {

        ::DBus::MethodBase::pointer temp_method;

        temp_method = this->create_method<int32_t,std::string,std::string,std::string,std::string,std::string>( "proactive.agent.controller", "SetStartConfiguration", sigc::mem_fun(*this, &ControllerAdapter::SetStartConfiguration_adapter_stub_isssss) );
        temp_method->set_arg_name( 0, "success" );
        temp_method->set_arg_name( 1, "security_policy" );
        temp_method->set_arg_name( 2, "log4j_file" );
        temp_method->set_arg_name( 3, "proactive_home" );
        temp_method->set_arg_name( 4, "classpath" );
        temp_method->set_arg_name( 5, "java_bin" );
        temp_method = this->create_method<int32_t,std::string,std::string>( "proactive.agent.controller", "StartNode", sigc::mem_fun(*this, &ControllerAdapter::StartNode_adapter_stub_iss) );
        temp_method->set_arg_name( 0, "pid" );
        temp_method->set_arg_name( 1, "name" );
        temp_method->set_arg_name( 2, "java_class" );
        temp_method = this->create_method<int32_t,std::string,std::string,std::string,std::string,std::string>( "proactive.agent.controller", "StartRMNode", sigc::mem_fun(*this, &ControllerAdapter::StartRMNode_adapter_stub_isssss) );
        temp_method->set_arg_name( 0, "pid" );
        temp_method->set_arg_name( 1, "name" );
        temp_method->set_arg_name( 2, "java_class" );
        temp_method->set_arg_name( 3, "user" );
        temp_method->set_arg_name( 4, "password" );
        temp_method->set_arg_name( 5, "url" );
        temp_method = this->create_method<int32_t,std::string,std::string,std::string>( "proactive.agent.controller", "StartP2PNode", sigc::mem_fun(*this, &ControllerAdapter::StartP2PNode_adapter_stub_isss) );
        temp_method->set_arg_name( 0, "pid" );
        temp_method->set_arg_name( 1, "name" );
        temp_method->set_arg_name( 2, "java_class" );
        temp_method->set_arg_name( 3, "contact" );
        temp_method = this->create_method<int32_t,std::string,std::string,std::string>( "proactive.agent.controller", "StartCustomNode", sigc::mem_fun(*this, &ControllerAdapter::StartCustomNode_adapter_stub_isss) );
        temp_method->set_arg_name( 0, "pid" );
        temp_method->set_arg_name( 1, "name" );
        temp_method->set_arg_name( 2, "java_class" );
        temp_method->set_arg_name( 3, "arguments" );
        temp_method = this->create_method<int32_t,int32_t>( "proactive.agent.controller", "StopNode", sigc::mem_fun(*this, &ControllerAdapter::StopNode_adapter_stub_ii) );
        temp_method->set_arg_name( 0, "breaks_without" );
        temp_method->set_arg_name( 1, "pid" );
      }

    public:

      typedef DBusCxxPointer<ControllerAdapter> pointer;

      static pointer create( Controller* adaptee=NULL, const std::string& path="/proactive/agent/Controller")
        { return pointer( new ControllerAdapter(adaptee, path)); }

      Controller* adaptee() { return m_adaptee; }

      void set_adaptee( Controller* adaptee ) {
        m_adaptee = adaptee;
      }

      void set_adaptee( Controller& adaptee ) {
        this->set_adaptee(&adaptee);
      }


    private:

      Controller* m_adaptee;


      void check_adaptee() { if ( not m_adaptee) throw ::DBus::ErrorInvalidAdaptee(); }

      int32_t SetStartConfiguration_adapter_stub_isssss( std::string security_policy, std::string log4j_file, std::string proactive_home, std::string classpath, std::string java_bin ) { this->check_adaptee(); return m_adaptee->SetStartConfiguration( security_policy, log4j_file, proactive_home, classpath, java_bin); }
      int32_t StartNode_adapter_stub_iss( std::string name, std::string java_class ) { this->check_adaptee(); return m_adaptee->StartNode( name, java_class); }
      int32_t StartRMNode_adapter_stub_isssss( std::string name, std::string java_class, std::string user, std::string password, std::string url ) { this->check_adaptee(); return m_adaptee->StartRMNode( name, java_class, user, password, url); }
      int32_t StartP2PNode_adapter_stub_isss( std::string name, std::string java_class, std::string contact ) { this->check_adaptee(); return m_adaptee->StartP2PNode( name, java_class, contact); }
      int32_t StartCustomNode_adapter_stub_isss( std::string name, std::string java_class, std::string arguments ) { this->check_adaptee(); return m_adaptee->StartCustomNode( name, java_class, arguments); }
      int32_t StopNode_adapter_stub_ii( int32_t pid ) { this->check_adaptee(); return m_adaptee->StopNode( pid); }


  };

}

#endif
