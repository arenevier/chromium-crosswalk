// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_EDK_EMBEDDER_PROCESS_TYPE_H_
#define MOJO_EDK_EMBEDDER_PROCESS_TYPE_H_

namespace mojo {
namespace embedder {

enum class ProcessType {
  // |InitIPCSupport()| has not been called (or |ShutdownIPCSupport()| has been
  // called).
  UNINITIALIZED,
  // Process without connection management.
  NONE,
  // Master process.
  MASTER,
  // Slave process.
  SLAVE,
};

}  // namespace embedder
}  // namespace mojo

#endif  // MOJO_EDK_EMBEDDER_PROCESS_TYPE_H_
