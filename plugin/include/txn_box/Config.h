/*
   Licensed to the Apache Software Foundation (ASF) under one or more contributor license agreements.
   See the NOTICE file distributed with this work for additional information regarding copyright
   ownership.  The ASF licenses this file to you under the Apache License, Version 2.0 (the
   "License"); you may not use this file except in compliance with the License.  You may obtain a
   copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

*/

#pragma once

#include <array>
#include <vector>

#include <swoc/TextView.h>
#include <yaml-cpp/yaml.h>

#include "txn_box/common.h"
#include "txn_box/Directive.h"

/// Contains a configuration and configuration helper methods.
class Config {
  using self_type = Config; ///< Self reference type.
  using Errata = swoc::Errata;
public:

  static constexpr swoc::TextView PLUGIN_NAME { "Transaction Tool Box" };
  static constexpr swoc::TextView PLUGIN_TAG { "txn_box" };

  static const std::string ROOT_KEY; ///< Root key for plugin configuration.

  Config() = default;

  /** Load the configuration from the file @a file_path.
   *
   * @param file_path Path to configuration file.
   * @return Errors, if any.
   */
  Errata load_file(swoc::file::path const& file_path);

  Errata load_top_level_directive(YAML::Node node);

protected:

  std::array<Directive::Handle, std::tuple_size<Hook>::value> _roots;
};
