// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/browser/layout_test/layout_test_browser_context.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "content/public/browser/resource_context.h"
#include "content/shell/browser/shell_download_manager_delegate.h"
#include "content/shell/browser/shell_url_request_context_getter.h"

#if defined(OS_WIN)
#include "base/base_paths_win.h"
#elif defined(OS_LINUX)
#include "base/nix/xdg_util.h"
#elif defined(OS_MACOSX)
#include "base/base_paths_mac.h"
#endif

namespace content {

LayoutTestBrowserContext::LayoutTestBrowserContext(bool off_the_record,
                                                   net::NetLog* net_log)
    : ShellBrowserContext(off_the_record, net_log) {
}

LayoutTestBrowserContext::~LayoutTestBrowserContext() {
}

void LayoutTestBrowserContext::InitWhileIOAllowed() {
  ignore_certificate_errors_ = true;
  ShellBrowserContext::InitWhileIOAllowed();
}

DownloadManagerDelegate*
LayoutTestBrowserContext::GetDownloadManagerDelegate() {
  if (!download_manager_delegate_.get()) {
    download_manager_delegate_.reset(new ShellDownloadManagerDelegate());
    download_manager_delegate_->SetDownloadManager(
        BrowserContext::GetDownloadManager(this));
    // TODO(mkwst): We can avoid this bit in the future by defining a
    // LayoutTestDownloadManagerDelegate.
    download_manager_delegate_->SetDownloadBehaviorForTesting(
        GetPath().Append(FILE_PATH_LITERAL("downloads")));
  }

  return download_manager_delegate_.get();
}

}  // namespace content
