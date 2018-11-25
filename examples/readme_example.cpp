#include <selene/base/io/FileUtils.hpp>

#include <selene/old_img/ImageTypeAliases.hpp>
#include <selene/old_img/ImageDataToImage.hpp>
#include <selene/old_img/ImageToImageData.hpp>
#include <selene/old_img_io/IO.hpp>
#include <selene/old_img_ops/Algorithms.hpp>
#include <selene/old_img_ops/ImageConversions.hpp>
#include <selene/old_img_ops/Transformations.hpp>

#include <cassert>
#include <filesystem>

#include "Utils.hpp"

using namespace sln;  // Never outside of example code

int main(int argc, char** argv)
{
  // Read data path as optional command line argument
  const char* data_path = (argc >= 2) ? argv[1] : nullptr;

  const auto example_img_path = sln_examples::full_data_path("bike_duck.jpg", data_path);

  // Decode JPEG image data from disk
  ImageData<> img_data = read_image(FileReader(example_img_path.string()));
  assert(img_data.nr_channels() == 3 && img_data.nr_bytes_per_channel() == 1);

  // Convert to strongly typed RGB image
  Image<PixelRGB_8u> img_rgb = to_image<PixelRGB_8u>(std::move(img_data));
  assert(img_rgb.width() > 400_px && img_rgb.height() > 350_px);

  // Create non-owning view on part of the image
  Image<PixelRGB_8u> img_part = view(img_rgb, 100_idx, 100_idx, 300_px, 250_px);

  // Darken this part
  for_each_pixel(img_part, [](auto& px){ px /= 4; });

  // Flip this part horizontally
  flip_horizontally_in_place(img_part);

  // Convert whole image to RGBA, adding semi-transparent alpha channel
  const Image<PixelRGBA_8u> img_rgba =
      convert_image<PixelFormat::RGBA>(img_rgb, std::uint8_t{128});

  // Encode in-memory to PNG
  std::vector<std::uint8_t> encoded_png_data;
  write_image(to_image_data_view(img_rgba), ImageFormat::PNG,
              VectorWriter(encoded_png_data));

  // Write encoded binary data to disk (or do something else with it...)
  write_data_contents("example_out.png", encoded_png_data);
}
