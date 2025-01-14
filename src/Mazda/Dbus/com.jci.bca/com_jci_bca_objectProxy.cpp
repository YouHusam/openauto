#include "com_jci_bca_objectProxy.h"

com_jci_bca_objectProxy::com_jci_bca_objectProxy(std::shared_ptr<DBus::Connection> conn,
                                                 std::string dest,
                                                 std::string path) : DBus::ObjectProxy(conn, dest, path) {
  m_com_jci_bcaProxy = com_jci_bcaProxy::create("com.jci.bca");
  this->add_interface(m_com_jci_bcaProxy);

}
std::shared_ptr<com_jci_bca_objectProxy> com_jci_bca_objectProxy::create(std::shared_ptr<DBus::Connection> conn,
                                                                         std::string dest,
                                                                         std::string path,
                                                                         DBus::ThreadForCalling signalCallingThread) {
  std::shared_ptr<com_jci_bca_objectProxy>
      created = std::shared_ptr<com_jci_bca_objectProxy>(new com_jci_bca_objectProxy(conn, dest, path));
  conn->register_object_proxy(created, signalCallingThread);
  return created;

}
std::shared_ptr<com_jci_bcaProxy> com_jci_bca_objectProxy::getcom_jci_bcaInterface() {
  return m_com_jci_bcaProxy;

}
