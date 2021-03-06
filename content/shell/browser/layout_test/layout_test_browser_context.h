// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_LAYOUT_TEST_LAYOUT_TEST_BROWSER_CONTEXT_H_
#define CONTENT_SHELL_BROWSER_LAYOUT_TEST_LAYOUT_TEST_BROWSER_CONTEXT_H_

#include "base/compiler_specific.h"
#include "content/shell/browser/shell_browser_context.h"

namespace net {
class NetLog;
}

namespace content {

class DownloadManagerDelegate;
class LayoutTestPushMessagingService;
class PermissionManager;
class PushMessagingService;

class LayoutTestBrowserContext : public ShellBrowserContext {
 public:
  LayoutTestBrowserContext(bool off_the_record, net::NetLog* net_log);
  ~LayoutTestBrowserContext() override;

  // BrowserContext implementation.
  DownloadManagerDelegate* GetDownloadManagerDelegate() override;
  PushMessagingService* GetPushMessagingService() override;
  PermissionManager* GetPermissionManager() override;

  LayoutTestPushMessagingService* GetLayoutTestPushMessagingService();

 protected:
  ShellURLRequestContextGetter* CreateURLRequestContextGetter(
      ProtocolHandlerMap* protocol_handlers,
      URLRequestInterceptorScopedVector request_interceptors) override;

 private:
  scoped_ptr<LayoutTestPushMessagingService> push_messaging_service_;
  scoped_ptr<PermissionManager> permission_manager_;

  DISALLOW_COPY_AND_ASSIGN(LayoutTestBrowserContext);
};

}  // namespace content

#endif  // CONTENT_SHELL_BROWSER_LAYOUT_TEST_LAYOUT_TEST_BROWSER_CONTEXT_H_
