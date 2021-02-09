/* GStreamer
 * Copyright (C) 2021 Sebastian Dröge <sebastian@centricular.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include <gst/base/base.h>
#include <gst/gst.h>
#include <gst/video/video.h>

#include "gstajacommon.h"

G_BEGIN_DECLS

#define GST_TYPE_AJA_SRC (gst_aja_src_get_type())
#define GST_AJA_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_AJA_SRC, GstAjaSrc))
#define GST_AJA_SRC_CAST(obj) ((GstAjaSrc *)obj)
#define GST_AJA_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_AJA_SRC, GstAjaSrcClass))
#define GST_IS_AJA_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_AJA_SRC))
#define GST_IS_AJA_SRC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_AJA_SRC))

typedef struct _GstAjaSrc GstAjaSrc;
typedef struct _GstAjaSrcClass GstAjaSrcClass;

struct _GstAjaSrc {
  GstPushSrc parent;

  // Everything below protected by queue lock
  GMutex queue_lock;
  GCond queue_cond;
  GstQueueArray *queue;
  gboolean playing;
  gboolean shutdown;
  gboolean flushing;

  GstAjaDevice *device;
  NTV2DeviceID device_id;
  GstAllocator *allocator;
  GstBufferPool *buffer_pool;
  GstBufferPool *audio_buffer_pool;
  GstBufferPool *anc_buffer_pool;

  // Properties
  gchar *device_identifier;
  NTV2Channel channel;
  GstAjaAudioSystem audio_system_setting;
  GstAjaVideoFormat video_format_setting;
  GstAjaInputSource input_source;
  GstAjaAudioSource audio_source;
  GstAjaTimecodeIndex timecode_index;
  GstAjaReferenceSource reference_source;
  guint queue_size;
  guint capture_cpu_core;

  NTV2AudioSystem audio_system;
  NTV2VideoFormat video_format;
  guint32 f2_start_line;
  NTV2TCIndex tc_index;

  GstCaps *configured_caps;
  GstVideoInfo configured_info;
  gint configured_audio_channels;

  AJAThread *capture_thread;
};

struct _GstAjaSrcClass {
  GstPushSrcClass parent_class;
};

G_GNUC_INTERNAL
GType gst_aja_src_get_type(void);

G_END_DECLS