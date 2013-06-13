// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// From dev/ppb_tcp_socket_dev.idl modified Wed Jun 05 23:11:18 2013.

#include "ppapi/c/dev/ppb_tcp_socket_dev.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/shared_impl/tracked_callback.h"
#include "ppapi/thunk/enter.h"
#include "ppapi/thunk/ppb_instance_api.h"
#include "ppapi/thunk/ppb_tcp_socket_api.h"
#include "ppapi/thunk/resource_creation_api.h"
#include "ppapi/thunk/thunk.h"

namespace ppapi {
namespace thunk {

namespace {

PP_Resource Create(PP_Instance instance) {
  VLOG(4) << "PPB_TCPSocket_Dev::Create()";
  EnterResourceCreation enter(instance);
  if (enter.failed())
    return 0;
  return enter.functions()->CreateTCPSocket(instance);
}

PP_Bool IsTCPSocket(PP_Resource resource) {
  VLOG(4) << "PPB_TCPSocket_Dev::IsTCPSocket()";
  EnterResource<PPB_TCPSocket_API> enter(resource, false);
  return PP_FromBool(enter.succeeded());
}

int32_t Connect(PP_Resource tcp_socket,
                PP_Resource addr,
                struct PP_CompletionCallback callback) {
  VLOG(4) << "PPB_TCPSocket_Dev::Connect()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, callback, true);
  if (enter.failed())
    return enter.retval();
  return enter.SetResult(enter.object()->Connect(addr, enter.callback()));
}

PP_Resource GetLocalAddress(PP_Resource tcp_socket) {
  VLOG(4) << "PPB_TCPSocket_Dev::GetLocalAddress()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, true);
  if (enter.failed())
    return 0;
  return enter.object()->GetLocalAddress();
}

PP_Resource GetRemoteAddress(PP_Resource tcp_socket) {
  VLOG(4) << "PPB_TCPSocket_Dev::GetRemoteAddress()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, true);
  if (enter.failed())
    return 0;
  return enter.object()->GetRemoteAddress();
}

int32_t Read(PP_Resource tcp_socket,
             char* buffer,
             int32_t bytes_to_read,
             struct PP_CompletionCallback callback) {
  VLOG(4) << "PPB_TCPSocket_Dev::Read()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, callback, true);
  if (enter.failed())
    return enter.retval();
  return enter.SetResult(enter.object()->Read(buffer,
                                              bytes_to_read,
                                              enter.callback()));
}

int32_t Write(PP_Resource tcp_socket,
              const char* buffer,
              int32_t bytes_to_write,
              struct PP_CompletionCallback callback) {
  VLOG(4) << "PPB_TCPSocket_Dev::Write()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, callback, true);
  if (enter.failed())
    return enter.retval();
  return enter.SetResult(enter.object()->Write(buffer,
                                               bytes_to_write,
                                               enter.callback()));
}

void Close(PP_Resource tcp_socket) {
  VLOG(4) << "PPB_TCPSocket_Dev::Close()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, true);
  if (enter.failed())
    return;
  enter.object()->Close();
}

int32_t SetOption(PP_Resource tcp_socket,
                  PP_TCPSocket_Option_Dev name,
                  struct PP_Var value,
                  struct PP_CompletionCallback callback) {
  VLOG(4) << "PPB_TCPSocket_Dev::SetOption()";
  EnterResource<PPB_TCPSocket_API> enter(tcp_socket, callback, true);
  if (enter.failed())
    return enter.retval();
  return enter.SetResult(enter.object()->SetOption(name,
                                                   value,
                                                   enter.callback()));
}

const PPB_TCPSocket_Dev_0_1 g_ppb_tcpsocket_dev_thunk_0_1 = {
  &Create,
  &IsTCPSocket,
  &Connect,
  &GetLocalAddress,
  &GetRemoteAddress,
  &Read,
  &Write,
  &Close,
  &SetOption
};

}  // namespace

const PPB_TCPSocket_Dev_0_1* GetPPB_TCPSocket_Dev_0_1_Thunk() {
  return &g_ppb_tcpsocket_dev_thunk_0_1;
}

}  // namespace thunk
}  // namespace ppapi
