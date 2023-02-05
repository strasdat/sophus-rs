#include "sophus-rs/include/sophus_wrapper.h"

#include "sophus-rs/src/glue.rs.h"

namespace sophus {

namespace conv {

sophus::ImageSize toImageSize(FfiImageSize image_size) {
  return sophus::ImageSize(image_size.width, image_size.height);
}

FfiImageSize fromImageSize(ImageSize image_size) {
  return FfiImageSize{size_t(image_size.width), size_t(image_size.height)};
}

sophus::ImageLayout toImageLayout(FfiImageLayout s) {
  return ::sophus::ImageLayout(s.size.width, s.size.height, s.pitch_in_bytes);
}

FfiImageLayout fromImageLayout(ImageLayout shape) {
  return FfiImageLayout{fromImageSize(shape.imageSize()), shape.pitchBytes()};
}

sophus::PixelFormat toPixelFormat(FfiPixelFormat t) {
  sophus::NumberType type = t.is_floating_point
                                ? sophus::NumberType::floating_point
                                : sophus::NumberType::fixed_point;

  return sophus ::PixelFormat{
      .number_type = type,
      .num_channels = int(t.num_channels),
      .num_bytes_per_pixel_channel = int(t.num_bytes_per_pixel_channel),
  };
}

FfiPixelFormat fromPixelFormat(sophus::PixelFormat pixel_type) {
  return FfiPixelFormat{
      .is_floating_point =
          pixel_type.number_type == sophus::NumberType::floating_point,
      .num_channels = size_t(pixel_type.num_channels),
      .num_bytes_per_pixel_channel =
          size_t(pixel_type.num_bytes_per_pixel_channel)};
}

class IntensityImageHelper : public sophus::IntensityImage<> {
 public:
  IntensityImageHelper(sophus::IntensityImage<> s)
      : sophus::IntensityImage<>(s) {}
  IntensityImageHelper(
      ImageSize const& size, PixelFormat const& pixel_type)
      : sophus::IntensityImage<>(size, pixel_type) {}
  IntensityImageHelper(FfiIntensityImage wrapper)
      : sophus::IntensityImage<>(
            toImageLayout(wrapper.layout),
            toPixelFormat(wrapper.pixel_format),
            wrapper.data) {}
  IntensityImageHelper(
      ImageLayout const& shape,
      PixelFormat const& pixel_type,
      std::shared_ptr<uint8_t> data)
      : sophus::IntensityImage<>(shape, pixel_type, data) {}

  std::shared_ptr<uint8_t> sharedPtr() const {
    return sophus::IntensityImage<>::shared_;
  }

  sophus::IntensityImage<> super() { return *this; }

  FfiIntensityImage wrapper() const {
    return FfiIntensityImage{
        .layout = fromImageLayout(this->layout_),
        .pixel_format = fromPixelFormat(this->pixel_format_),
        .data = this->sharedPtr()};
  }
};

}  // namespace conv

std::unique_ptr<FfiMutIntensityImage> create_mut_intensity_image_from_size(
    FfiImageSize s, FfiPixelFormat t) {
  FfiMutIntensityImage img(sophus::MutIntensityImage<>(
      conv::toImageSize(s), conv::toPixelFormat(t)));
  return std::make_unique<FfiMutIntensityImage>(std::move(img));
}

FfiIntensityImage create_intensity_image_from_mut(
    std::unique_ptr<FfiMutIntensityImage>& mut_image) {
  return conv::IntensityImageHelper(
             std::move(SOPHUS_UNWRAP(mut_image).mut_dyn_image))
      .wrapper();
}

uint8_t const* get_raw_ptr(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).rawPtr();
}

uint8_t* get_mut_raw_ptr(std::unique_ptr<FfiMutIntensityImage> const& img) {
  return SOPHUS_UNWRAP(img).mut_dyn_image.rawMutPtr();
}

bool has_u8_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<uint8_t>();
}

bool has_u16_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<uint16_t>();
}

bool has_f32_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<float>();
}

bool has_3u8_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<Eigen::Vector3<uint8_t>>();
}

bool has_3u16_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<Eigen::Vector3<uint16_t>>();
}

bool has_3f32_img(FfiIntensityImage const& img) {
  return conv::IntensityImageHelper(img).has<Eigen::Vector3<float>>();
}

}  // namespace sophus
