// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#include <selene/base/io/FileWriter.hpp>

#include <selene/old_img/ImageTypeAliases.hpp>
#include <selene/old_img/ImageToImageData.hpp>
#include <selene/old_img_io/IO.hpp>
#include <selene/old_img_ops/ImageConversions.hpp>

#include <iostream>
#include <string>

#include "Utils.hpp"

constexpr auto output_filename_y = "bike_duck_converted_to_grayscale.png";
constexpr auto output_filename_rgba = "bike_duck_converted_to_rgba.png";
constexpr auto output_filename_bgr = "bike_duck_converted_to_bgr.png";

using namespace sln;  // Never outside of example code

int main(int argc, char** argv)
{
  // Read data path as optional command line argument
  const char* data_path = (argc >= 2) ? argv[1] : nullptr;

  // Read in the example image (check the implementation in Utils.hpp);
  // `PixelRGB_8u` designates 3 channels (R, G, B) of unsigned 8-bit data for each pixel.

  const auto img_rgb = sln_examples::read_example_image<PixelRGB_8u>("bike_duck.png", data_path);

  // We will now perform three color format conversions:
  // - From RGB to Y (grayscale)
  std::cout << "Converting the image from RGB to Y (grayscale)...\n";
  const auto img_y = convert_image<PixelFormat::Y>(img_rgb);

  // - From RGB to RGBA (adding an alpha channel with the extra argument to convert_image())
  std::cout << "Converting the image from RGB to RGBA (slightly transparent)...\n";
  const auto img_rgba = convert_image<PixelFormat::RGBA>(img_rgb, std::uint8_t{192});

  // - From RGB to BGR
  std::cout << "Converting the image from RGB to BGR...\n";
  const auto img_bgr = convert_image<PixelFormat::BGR>(img_rgb);

  // Finally, we write out all resulting images to disk

  std::cout << "Writing the result to disk: '" << output_filename_y << "'...\n";
  write_image(to_image_data_view(img_y), ImageFormat::PNG,
              FileWriter(output_filename_y));

  std::cout << "Writing the result to disk: '" << output_filename_rgba << "'...\n";
  write_image(to_image_data_view(img_rgba), ImageFormat::PNG,
              FileWriter(output_filename_rgba));

  std::cout << "Writing the result to disk: '" << output_filename_bgr << "'...\n";
  write_image(to_image_data_view(img_bgr), ImageFormat::PNG,
                   FileWriter(output_filename_bgr));

  return 0;
}
