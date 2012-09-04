// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/media/media_stream_devices_menu_model.h"

#include <utility>

#include "base/utf_string_conversions.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/ui/media_stream_infobar_delegate.h"
#include "content/public/common/media_stream_request.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"

MediaStreamDevicesMenuModel::MediaStreamDevicesMenuModel(
    MediaStreamInfoBarDelegate* delegate)
    : ALLOW_THIS_IN_INITIALIZER_LIST(ui::SimpleMenuModel(this)),
      media_stream_delegate_(delegate) {
  bool audio = delegate->HasAudio() && !delegate->GetAudioDevices().empty();
  bool video = delegate->HasVideo() && !delegate->GetVideoDevices().empty();
  if (video) {
    AddDevices(delegate->GetVideoDevices());
    if (audio)
      AddSeparator(ui::NORMAL_SEPARATOR);
  }
  if (audio) {
    AddDevices(delegate->GetAudioDevices());
  }

  // Show "always allow" option only for the secure connection.
  if (delegate->GetSecurityOrigin().SchemeIsSecure())
    AddAlwaysAllowOption(audio, video);
}

MediaStreamDevicesMenuModel::~MediaStreamDevicesMenuModel() {
}

bool MediaStreamDevicesMenuModel::IsCommandIdChecked(int command_id) const {
  if (command_id == IDC_MEDIA_STREAM_DEVICE_ALWAYS_ALLOW)
    return media_stream_delegate_->always_allow();

  CommandMap::const_iterator it = commands_.find(command_id);
  DCHECK(it != commands_.end());

  const std::string& device_id = it->second.device_id;
  return (device_id == media_stream_delegate_->selected_audio_device() ||
          device_id == media_stream_delegate_->selected_video_device());
}

bool MediaStreamDevicesMenuModel::IsCommandIdEnabled(int command_id) const {
  return true;
}

bool MediaStreamDevicesMenuModel::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) {
  return false;
}

void MediaStreamDevicesMenuModel::ExecuteCommand(int command_id) {
  if (command_id == IDC_MEDIA_STREAM_DEVICE_ALWAYS_ALLOW) {
    media_stream_delegate_->toggle_always_allow();
    return;
  }

  CommandMap::const_iterator it = commands_.find(command_id);
  DCHECK(it != commands_.end());
  if (it->second.type == content::MEDIA_STREAM_DEVICE_TYPE_AUDIO_CAPTURE)
    media_stream_delegate_->set_selected_audio_device(it->second.device_id);
  else
    media_stream_delegate_->set_selected_video_device(it->second.device_id);
}

void MediaStreamDevicesMenuModel::AddDevices(
    const content::MediaStreamDevices& devices) {
  for (size_t i = 0; i < devices.size(); ++i) {
    int command_id = commands_.size();
    commands_.insert(std::make_pair(command_id, devices[i]));
    int message_id = (devices[i].type ==
        content::MEDIA_STREAM_DEVICE_TYPE_AUDIO_CAPTURE) ?
        IDS_MEDIA_CAPTURE_AUDIO : IDS_MEDIA_CAPTURE_VIDEO;
    AddCheckItem(command_id,
                 l10n_util::GetStringFUTF16(message_id,
                                            UTF8ToUTF16(devices[i].name)));
  }
}

void MediaStreamDevicesMenuModel::AddAlwaysAllowOption(bool audio, bool video) {
  int command_id = IDC_MEDIA_STREAM_DEVICE_ALWAYS_ALLOW;
  int message_id = IDS_MEDIA_CAPTURE_ALWAYS_ALLOW_AUDIO_AND_VIDEO;
  if (audio && !video)
    message_id = IDS_MEDIA_CAPTURE_ALWAYS_ALLOW_AUDIO_ONLY;
  else if (!audio && video)
    message_id = IDS_MEDIA_CAPTURE_ALWAYS_ALLOW_VIDEO_ONLY;

  AddSeparator(ui::NORMAL_SEPARATOR);
  AddCheckItem(command_id, l10n_util::GetStringUTF16(message_id));
}
