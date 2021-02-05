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

#include <ajabase/common/types.h>
#include <ajabase/system/memory.h>
#include <ajabase/system/thread.h>
#include <ajantv2/includes/ntv2card.h>
#include <ajantv2/includes/ntv2devicefeatures.h>
#include <ajantv2/includes/ntv2devicescanner.h>
#include <ajantv2/includes/ntv2enums.h>
#include <ajantv2/includes/ntv2signalrouter.h>
#include <gst/base/base.h>
#include <gst/gst.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

typedef struct {
  GstMeta meta;

  GstBuffer *buffer;
} GstAjaAudioMeta;

G_GNUC_INTERNAL
GType gst_aja_audio_meta_api_get_type(void);
#define GST_AJA_AUDIO_META_API_TYPE (gst_aja_audio_meta_api_get_type())

G_GNUC_INTERNAL
const GstMetaInfo *gst_aja_audio_meta_get_info(void);
#define GST_AJA_AUDIO_META_INFO (gst_aja_audio_meta_get_info())

#define gst_buffer_get_aja_audio_meta(b) \
  ((GstAjaAudioMeta *)gst_buffer_get_meta((b), GST_AJA_AUDIO_META_API_TYPE))

G_GNUC_INTERNAL
GstAjaAudioMeta *gst_buffer_add_aja_audio_meta(GstBuffer *buffer,
                                               GstBuffer *audio_buffer);

G_GNUC_INTERNAL
GstCaps *gst_ntv2_supported_caps(NTV2DeviceID device_id);
G_GNUC_INTERNAL
GstCaps *gst_ntv2_video_format_to_caps(NTV2VideoFormat format);
G_GNUC_INTERNAL
NTV2VideoFormat gst_ntv2_video_format_from_caps(GstCaps *caps);

typedef struct {
  CNTV2Card *device;
} GstAjaDevice;

G_GNUC_INTERNAL
GstAjaDevice *gst_aja_device_obtain(const gchar *device_identifier);
G_GNUC_INTERNAL
GstAjaDevice *gst_aja_device_ref(GstAjaDevice *device);
G_GNUC_INTERNAL
void gst_aja_device_unref(GstAjaDevice *device);

#define GST_AJA_ALLOCATOR_MEMTYPE "aja"

#define GST_TYPE_AJA_ALLOCATOR (gst_aja_allocator_get_type())
#define GST_AJA_ALLOCATOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_AJA_ALLOCATOR, GstAjaAllocator))
#define GST_AJA_ALLOCATOR_CLASS(klass)                      \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_AJA_ALLOCATOR, \
                           GstAjaAllocatorClass))
#define GST_IS_Aja_ALLOCATOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_AJA_ALLOCATOR))
#define GST_IS_Aja_ALLOCATOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_AJA_ALLOCATOR))
#define GST_AJA_ALLOCATOR_CAST(obj) ((GstAjaAllocator *)(obj))

typedef struct _GstAjaAllocator GstAjaAllocator;
typedef struct _GstAjaAllocatorClass GstAjaAllocatorClass;

struct _GstAjaAllocator {
  GstAllocator allocator;

  GstAjaDevice *device;
};

struct _GstAjaAllocatorClass {
  GstAllocatorClass parent_class;
};

G_GNUC_INTERNAL
GType gst_aja_allocator_get_type(void);
G_GNUC_INTERNAL
GstAllocator *gst_aja_allocator_new(GstAjaDevice *device);

typedef enum {
  GST_AJA_AUDIO_SYSTEM_AUTO,
  GST_AJA_AUDIO_SYSTEM_1,
  GST_AJA_AUDIO_SYSTEM_2,
  GST_AJA_AUDIO_SYSTEM_3,
  GST_AJA_AUDIO_SYSTEM_4,
  GST_AJA_AUDIO_SYSTEM_5,
  GST_AJA_AUDIO_SYSTEM_6,
  GST_AJA_AUDIO_SYSTEM_7,
  GST_AJA_AUDIO_SYSTEM_8,
} GstAjaAudioSystem;

#define GST_TYPE_AJA_AUDIO_SYSTEM (gst_aja_audio_system_get_type())
G_GNUC_INTERNAL
GType gst_aja_audio_system_get_type(void);

typedef enum {
  GST_AJA_OUTPUT_DESTINATION_AUTO,
  GST_AJA_OUTPUT_DESTINATION_ANALOG,
  GST_AJA_OUTPUT_DESTINATION_SDI1,
  GST_AJA_OUTPUT_DESTINATION_SDI2,
  GST_AJA_OUTPUT_DESTINATION_SDI3,
  GST_AJA_OUTPUT_DESTINATION_SDI4,
  GST_AJA_OUTPUT_DESTINATION_SDI5,
  GST_AJA_OUTPUT_DESTINATION_SDI6,
  GST_AJA_OUTPUT_DESTINATION_SDI7,
  GST_AJA_OUTPUT_DESTINATION_SDI8,
  GST_AJA_OUTPUT_DESTINATION_HDMI,
} GstAjaOutputDestination;

#define GST_TYPE_AJA_OUTPUT_DESTINATION (gst_aja_output_destination_get_type())
G_GNUC_INTERNAL
GType gst_aja_output_destination_get_type(void);

typedef enum {
  GST_AJA_REFERENCE_SOURCE_AUTO,
  GST_AJA_REFERENCE_SOURCE_FREERUN,
  GST_AJA_REFERENCE_SOURCE_EXTERNAL,
  GST_AJA_REFERENCE_SOURCE_INPUT_1,
  GST_AJA_REFERENCE_SOURCE_INPUT_2,
  GST_AJA_REFERENCE_SOURCE_INPUT_3,
  GST_AJA_REFERENCE_SOURCE_INPUT_4,
  GST_AJA_REFERENCE_SOURCE_INPUT_5,
  GST_AJA_REFERENCE_SOURCE_INPUT_6,
  GST_AJA_REFERENCE_SOURCE_INPUT_7,
  GST_AJA_REFERENCE_SOURCE_INPUT_8,
} GstAjaReferenceSource;

#define GST_TYPE_AJA_REFERENCE_SOURCE (gst_aja_reference_source_get_type())
G_GNUC_INTERNAL
GType gst_aja_reference_source_get_type(void);

G_GNUC_INTERNAL
void gst_aja_common_init(void);

G_END_DECLS

class ShmMutexLocker {
 public:
  ShmMutexLocker();
  ~ShmMutexLocker();
};
