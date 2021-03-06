// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_AGENT_H_
#define CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_AGENT_H_

#include <string>

#include "content/public/common/console_message_level.h"
#include "content/public/renderer/render_frame_observer.h"
#include "third_party/WebKit/public/web/WebDevToolsAgentClient.h"

namespace blink {
class WebDevToolsAgent;
}

namespace content {

class RenderFrameImpl;

// DevToolsAgent belongs to the inspectable RenderFrameImpl and communicates
// with WebDevToolsAgent. There is a corresponding DevToolsAgentHost
// on the browser side.
class DevToolsAgent : public RenderFrameObserver,
                      public blink::WebDevToolsAgentClient {
 public:
  explicit DevToolsAgent(RenderFrameImpl* frame);
  ~DevToolsAgent() override;

  // Returns agent instance for its routing id.
  static DevToolsAgent* FromRoutingId(int routing_id);

  static void SendChunkedProtocolMessage(
      IPC::Sender* sender,
      int routing_id,
      int call_id,
      const std::string& message,
      const std::string& post_state);

  blink::WebDevToolsAgent* GetWebAgent();

  bool IsAttached();

 private:
  // RenderFrameObserver implementation.
  bool OnMessageReceived(const IPC::Message& message) override;

  // WebDevToolsAgentClient implementation.
  void sendProtocolMessage(int call_id,
                           const blink::WebString& response,
                           const blink::WebString& state) override;
  blink::WebDevToolsAgentClient::WebKitClientMessageLoop*
      createClientMessageLoop() override;
  void willEnterDebugLoop() override;
  void didExitDebugLoop() override;

  void enableTracing(const blink::WebString& category_filter) override;
  void disableTracing() override;

  void OnAttach(const std::string& host_id);
  void OnReattach(const std::string& host_id,
                  const std::string& agent_state);
  void OnDetach();
  void OnDispatchOnInspectorBackend(const std::string& message);
  void OnInspectElement(const std::string& host_id, int x, int y);
  void OnAddMessageToConsole(ConsoleMessageLevel level,
                             const std::string& message);
  void ContinueProgram();
  void OnSetupDevToolsClient();

  bool is_attached_;
  bool is_devtools_client_;
  bool paused_in_mouse_move_;
  RenderFrameImpl* frame_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsAgent);
};

}  // namespace content

#endif  // CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_AGENT_H_
