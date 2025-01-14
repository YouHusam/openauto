#ifndef COM_JCI_AAPA_OBJECTPROXY_H
#define COM_JCI_AAPA_OBJECTPROXY_H

#include <dbus-cxx.h>
#include <memory>
#include <stdint.h>
#include <string>
#include "com_jci_aapaProxy.h"
class com_jci_aapa_objectProxy
    : public DBus::ObjectProxy {
 public:
  com_jci_aapa_objectProxy(std::shared_ptr<DBus::Connection> conn, std::string dest, std::string path);
 public:
  static std::shared_ptr<com_jci_aapa_objectProxy> create(std::shared_ptr<DBus::Connection> conn,
                                                          std::string dest,
                                                          std::string path,
                                                          DBus::ThreadForCalling signalCallingThread = DBus::ThreadForCalling::DispatcherThread);
  std::shared_ptr<com_jci_aapaProxy> getcom_jci_aapaInterface();
 protected:
  std::shared_ptr<com_jci_aapaProxy> m_com_jci_aapaProxy;
};
#endif /* COM_JCI_AAPA_OBJECTPROXY_H */
