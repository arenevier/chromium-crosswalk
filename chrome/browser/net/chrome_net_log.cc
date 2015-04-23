// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/net/chrome_net_log.h"

#include <stdio.h>

#include "base/command_line.h"
#include "base/files/scoped_file.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "chrome/browser/net/net_log_temp_file.h"
#include "chrome/browser/ui/webui/net_internals/net_internals_ui.h"
#include "chrome/common/chrome_switches.h"
#include "content/public/common/content_switches.h"
#include "net/log/trace_net_log_observer.h"
#include "net/log/write_to_file_net_log_observer.h"

namespace {

net::NetLogCaptureMode GetCaptureModeFromCommandLine(
    const base::CommandLine& command_line) {
  const net::NetLogCaptureMode kDefault = net::NetLogCaptureMode::Default();

  // TODO(eroman): The NetLog "capture mode" used to be called the "log
  // level". To preserve backwards compatibility the log level of old is
  // converted into a capture mode.
  if (!command_line.HasSwitch(switches::kNetLogLevel))
    return kDefault;

  std::string log_level_string =
      command_line.GetSwitchValueASCII(switches::kNetLogLevel);

  int level;
  if (!base::StringToInt(log_level_string, &level))
    return kDefault;

  switch (level) {
    case 0:
      return net::NetLogCaptureMode::IncludeSocketBytes();
    case 1:
      return net::NetLogCaptureMode::IncludeCookiesAndCredentials();
    case 2:
      return net::NetLogCaptureMode::Default();
    case 3:
      return net::NetLogCaptureMode::None();
    default:
      LOG(ERROR) << "Unrecognized --" << switches::kNetLogLevel;
      break;
  }

  return kDefault;
}

}  // namespace

ChromeNetLog::ChromeNetLog()
    : net_log_temp_file_(new NetLogTempFile(this)) {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

  if (command_line.HasSwitch(switches::kLogNetLog)) {
    base::FilePath log_path =
        command_line.GetSwitchValuePath(switches::kLogNetLog);
    // Much like logging.h, bypass threading restrictions by using fopen
    // directly.  Have to write on a thread that's shutdown to handle events on
    // shutdown properly, and posting events to another thread as they occur
    // would result in an unbounded buffer size, so not much can be gained by
    // doing this on another thread.  It's only used when debugging Chrome, so
    // performance is not a big concern.
    base::ScopedFILE file;
#if defined(OS_WIN)
    file.reset(_wfopen(log_path.value().c_str(), L"w"));
#elif defined(OS_POSIX)
    file.reset(fopen(log_path.value().c_str(), "w"));
#endif

    if (!file) {
      LOG(ERROR) << "Could not open file " << log_path.value()
                 << " for net logging";
    } else {
      scoped_ptr<base::Value> constants(NetInternalsUI::GetConstants());
      net_log_logger_.reset(new net::WriteToFileNetLogObserver());

      net::NetLogCaptureMode capture_mode =
          GetCaptureModeFromCommandLine(command_line);
      net_log_logger_->set_capture_mode(capture_mode);

      net_log_logger_->StartObserving(this, file.Pass(), constants.get(),
                                      nullptr);
    }
  }

  trace_net_log_observer_.reset(new net::TraceNetLogObserver());
  trace_net_log_observer_->WatchForTraceStart(this);
}

ChromeNetLog::~ChromeNetLog() {
  net_log_temp_file_.reset();
  // Remove the observers we own before we're destroyed.
  if (net_log_logger_)
    net_log_logger_->StopObserving(nullptr);
  if (trace_net_log_observer_)
    trace_net_log_observer_->StopWatchForTraceStart();
}

