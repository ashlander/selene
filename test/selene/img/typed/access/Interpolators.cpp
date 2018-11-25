// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#include <catch.hpp>

#include <selene/img/typed/access/Interpolators.hpp>

#include <selene/img/pixel/PixelTypeAliases.hpp>

#include <test/selene/img/typed/_Utils.hpp>

using namespace sln::literals;

template <typename ImageType>
void test_nn_8u1(ImageType& img)
{
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 0.0, 0.0) == 10);
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 0.51, 0.0) == 20);
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 1.11, 0.88) == 50);
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 1.8, 1.6) == 90);

  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor,
      sln::BorderAccessMode::Replicated>::interpolate(img, -2.0, 1.0)
          == 40);
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor,
      sln::BorderAccessMode::Replicated>::interpolate(img, 0.0, 3.0)
          == 70);
}

template <typename ImageType>
void test_nn_8u3(ImageType& img)
{
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 0.0, 0.0)
          == sln::Pixel_8u3(10, 11, 12));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 0.51, 0.0)
          == sln::Pixel_8u3(20, 21, 22));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 1.11, 0.88)
          == sln::Pixel_8u3(50, 51, 52));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor>::interpolate(img, 1.8, 1.6)
          == sln::Pixel_8u3(90, 91, 92));

  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor,
      sln::BorderAccessMode::Replicated>::interpolate(img, -2.0, 1.0)
          == sln::Pixel_8u3(40, 41, 42));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::NearestNeighbor,
      sln::BorderAccessMode::Replicated>::interpolate(img, 0.0, 3.0)
          == sln::Pixel_8u3(70, 71, 72));
}

template <typename ImageType>
void test_bilinear_8u1(ImageType& img)
{
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 0.0, 0.0) == Approx(10.0));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 0.51, 0.0) == Approx(15.1));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 1.11, 0.88)
          == Approx(47.5));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 1.8, 1.6) == Approx(76.0));
}

template <typename ImageType>
void test_bilinear_8u3(ImageType& img)
{
  const auto px_interp_0 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 0.0, 0.0);
  REQUIRE(px_interp_0[0] == Approx(10.0));
  REQUIRE(px_interp_0[1] == Approx(11.0));
  REQUIRE(px_interp_0[2] == Approx(12.0));

  const auto px_interp_1 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 0.51, 0.0);
  REQUIRE(px_interp_1[0] == Approx(15.1));
  REQUIRE(px_interp_1[1] == Approx(16.1));
  REQUIRE(px_interp_1[2] == Approx(17.1));

  const auto px_interp_2 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 1.11,
                                                                                                      0.88);
  REQUIRE(px_interp_2[0] == Approx(47.5));
  REQUIRE(px_interp_2[1] == Approx(48.5));
  REQUIRE(px_interp_2[2] == Approx(49.5));

  const auto px_interp_3 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(img, 1.8, 1.6);
  REQUIRE(px_interp_3[0] == Approx(76.0));
  REQUIRE(px_interp_3[1] == Approx(77.0));
  REQUIRE(px_interp_3[2] == Approx(78.0));
}

template <typename ImageType>
void test_bilinear_relative_8u1(ImageType& img)
{
  auto r_img = sln::relative_accessor(img, 1_idx, 1_idx);

  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, -1.0, -1.0)
          == Approx(10.0));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, -0.49, -1.0)
          == Approx(15.1));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, 0.11, -0.12)
          == Approx(47.5));
  REQUIRE(sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, 0.8, 0.6)
          == Approx(76.0));
}

template <typename ImageType>
void test_bilinear_relative_8u3(ImageType& img)
{
  // relative access
  const auto r_img = sln::relative_accessor(img, 1_idx, 1_idx);

  const auto px_interp_0 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, -1.0,
                                                                                                      -1.0);
  REQUIRE(px_interp_0[0] == Approx(10.0));
  REQUIRE(px_interp_0[1] == Approx(11.0));
  REQUIRE(px_interp_0[2] == Approx(12.0));

  const auto px_interp_1 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, -0.49,
                                                                                                      -1.0);
  REQUIRE(px_interp_1[0] == Approx(15.1));
  REQUIRE(px_interp_1[1] == Approx(16.1));
  REQUIRE(px_interp_1[2] == Approx(17.1));

  const auto px_interp_2 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, 0.11,
                                                                                                      -0.12);
  REQUIRE(px_interp_2[0] == Approx(47.5));
  REQUIRE(px_interp_2[1] == Approx(48.5));
  REQUIRE(px_interp_2[2] == Approx(49.5));

  const auto px_interp_3 = sln::ImageInterpolator<sln::ImageInterpolationMode::Bilinear>::interpolate(r_img, 0.8,
                                                                                                      0.6);
  REQUIRE(px_interp_3[0] == Approx(76.0));
  REQUIRE(px_interp_3[1] == Approx(77.0));
  REQUIRE(px_interp_3[2] == Approx(78.0));
}

// ---

TEST_CASE("Image interpolators, 8u1", "[img]")
{
  auto img = sln_test::make_3x3_test_image_8u1();
  const auto view_c = sln::ImageView<sln::Pixel_8u1, sln::ImageModifiability::Constant>(img.byte_ptr(), img.layout());
  const auto view_m = sln::ImageView<sln::Pixel_8u1, sln::ImageModifiability::Mutable>(img.byte_ptr(), img.layout());

  SECTION("Nearest neighbor")
  {
    test_nn_8u1(img);
    test_nn_8u1(view_c);
    test_nn_8u1(view_m);
  }

  SECTION("Bilinear")
  {
    test_bilinear_8u1(img);
    test_bilinear_8u1(view_c);
    test_bilinear_8u1(view_m);
  }

  SECTION("Bilinear, relative access")
  {
    test_bilinear_relative_8u1(img);
    test_bilinear_relative_8u1(view_c);
    test_bilinear_relative_8u1(view_m);
  }


  // TODO: ADD MORE TESTS
  // ...
}

TEST_CASE("Image interpolators, 8u3", "[img]")
{
  auto img = sln_test::make_3x3_test_image_8u3();
  const auto view_c = sln::ImageView<sln::Pixel_8u3, sln::ImageModifiability::Constant>(img.byte_ptr(), img.layout());
  const auto view_m = sln::ImageView<sln::Pixel_8u3, sln::ImageModifiability::Mutable>(img.byte_ptr(), img.layout());

  SECTION("Nearest neighbor")
  {
    test_nn_8u3(img);
    test_nn_8u3(view_c);
    test_nn_8u3(view_m);
  }

  SECTION("Bilinear")
  {
    test_bilinear_8u3(img);
    test_bilinear_8u3(view_c);
    test_bilinear_8u3(view_m);
  }

  SECTION("Bilinear, relative access")
  {
    test_bilinear_relative_8u3(img);
    test_bilinear_relative_8u3(view_c);
    test_bilinear_relative_8u3(view_m);
  }

  // TODO: ADD MORE TESTS
  // ...
}