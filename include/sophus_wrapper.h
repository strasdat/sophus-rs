#pragma once

#include "rust/cxx.h"

#include <sophus/image/dyn_image_types.h>

namespace sophus {

struct FfiIntensityImage;
struct FfiImageLayout;
struct FfiImageSize;
struct FfiPixelFormat;

struct FfiMutIntensityImage {
  FfiMutIntensityImage() = default;

  FfiMutIntensityImage(
      MutDynImage<IntensityImagePredicate>&& mut_dyn_image)
      : mut_dyn_image(std::move(mut_dyn_image)) {}

  MutDynImage<IntensityImagePredicate> mut_dyn_image;
};

std::unique_ptr<FfiMutIntensityImage> create_mut_intensity_image_from_size(
    FfiImageSize s, FfiPixelFormat t);

FfiIntensityImage create_intensity_image_from_mut(
    std::unique_ptr<FfiMutIntensityImage>& mut_image);

uint8_t const* get_raw_ptr(FfiIntensityImage const& img);
uint8_t* get_mut_raw_ptr(std::unique_ptr<FfiMutIntensityImage> const& img);

bool has_u8_img(FfiIntensityImage const& img);
bool has_u16_img(FfiIntensityImage const& img);
bool has_f32_img(FfiIntensityImage const& img);
bool has_3u8_img(FfiIntensityImage const& img);
bool has_3u16_img(FfiIntensityImage const& img);
bool has_3f32_img(FfiIntensityImage const& img);
bool has_4u8_img(FfiIntensityImage const& img);
bool has_4u16_img(FfiIntensityImage const& img);
bool has_4f32_img(FfiIntensityImage const& img);

}  // namespace sophus
