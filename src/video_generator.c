
#include "tp/allocator.h"
#include "tp/audio.h"
#include "tp/stream.h"
#include "tp/video.h"

int main(void) {
  tp_allocator allocator = tp_allocator_create(
      tp_allocator_virtual_memory_alloc(1024 * 1024 * 1024));

  tp_video_writer video_writer = tp_video_create_avi_writer(
      &allocator, tp_string_from_string_constant("drum.avi"), 735, 1920);

  tp_image frame = {.rows = 735, .cols = 1920, .format = TP_PIXEL_FORMAT_RGB};
  frame.data.rgb.count = frame.rows * frame.cols;
  frame.data.rgb.data = tp_allocator_alloc(&allocator, frame.data.rgb.count,
                                           sizeof(*frame.data.rgb.data));

  tp_slice_i16 samples =
      tp_audio_read_wav(&allocator, tp_string_from_string_constant("drum.wav"));

  usize num_frames = samples.count / 735 + (samples.count % 735 == 0 ? 0 : 1);

  for (usize frame_ii = 0; frame_ii < num_frames; frame_ii++) {
    for (int row = 0; row < 735; row++) {
      usize sample_idx = frame_ii * 735 + row;
      i16 sample = 0;
      if (sample_idx < samples.count) {
        sample = samples.data[sample_idx];
      }

      usize num_white_pixels = (((f32)sample / 32768) / 2.0 + 0.5) * 1920;
      for (usize col = 0; col < 1920; col++) {
        if (col < num_white_pixels / 2 || col > 1920 - num_white_pixels / 2) {
          frame.data.rgb.data[row * 1920 + col].r = 255;
          frame.data.rgb.data[row * 1920 + col].g = 255;
          frame.data.rgb.data[row * 1920 + col].b = 255;
        } else {
          frame.data.rgb.data[row * 1920 + col].r = 0;
          frame.data.rgb.data[row * 1920 + col].g = 0;
          frame.data.rgb.data[row * 1920 + col].b = 0;
        }
      }
    }
    tp_video_write_frame(&video_writer, frame);
  }

  tp_video_close_writer(&video_writer);
}
