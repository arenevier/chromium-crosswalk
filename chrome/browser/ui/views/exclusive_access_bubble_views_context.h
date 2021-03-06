// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_UI_VIEWS_EXCLUSIVE_ACCESS_BUBBLE_VIEWS_CONTEXT_H_
#define CHROME_BROWSER_UI_VIEWS_EXCLUSIVE_ACCESS_BUBBLE_VIEWS_CONTEXT_H_

#include "ui/gfx/geometry/rect.h"

class ExclusiveAccessManager;

namespace views {
class Widget;
}

// Context in which the exclusive access bubble view is initiated.
class ExclusiveAccessBubbleViewsContext {
 public:
  virtual ~ExclusiveAccessBubbleViewsContext() {}

  // Returns ExclusiveAccessManager controlling exclusive access for the given
  // webview.
  virtual ExclusiveAccessManager* GetExclusiveAccessManager() = 0;

  // Returns the Widget that hosts the view containing the exclusive access
  // bubble.
  virtual views::Widget* GetBubbleAssociatedWidget() = 0;

  // Returns true if immersive mode is enabled.
  virtual bool IsImmersiveModeEnabled() = 0;

  // Returns the bounds of the top level View in screen coordinate system.
  virtual gfx::Rect GetTopContainerBoundsInScreen() = 0;
};

#endif  // CHROME_BROWSER_UI_VIEWS_EXCLUSIVE_ACCESS_BUBBLE_VIEWS_CONTEXT_H_
