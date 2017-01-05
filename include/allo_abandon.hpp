/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

namespace allo
{

template <typename T> void abandon(T& t)
{
   struct
   {
      char a[sizeof(T)];
   } x;
   new (&x) auto(std::move(t));
}

} // namespace allo
