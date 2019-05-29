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
#include <swoc/swoc_file.h>
#include <yaml-cpp/yaml.h>

#include "txn_box/common.h"
#include "txn_box/Directive.h"

using TSCont = struct tsapi_cont *;
using TSHttpTxn = struct tsapi_httptxn *;

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

  /** Load directives at the top level.
   *
   * @param node Base plugin configuation node.
   * @return Errors, if any.
   *
   * Processing of directives directly in the base node value is handled specially
   * by this method.
   */
  Errata load_top_level_directive(YAML::Node node);

  /** Load / create a directive from a node.
   *
   * @param drtv_node Directive node.
   * @return A new directive instance, or errors if loading failed.
   */
  swoc::Rv<Directive::Handle> load_directive(YAML::Node drtv_node);

  /// Check for active directives.
  /// @return @a true if there are any top level directives, @c false if not.
  bool is_active() const;

  /** Set any required hooks for a transaction, based on this configuration.
   *
   * @param txn Transaction.
   * @return Errors if any.
   */
  Errata set_txn_hooks(TSHttpTxn txn, TSCont cont);

protected:
  friend class When;

  /// Mark whether there are any top level directives.
  bool _active_p { false };

  /// Top level directives for each hook. Always invoked.
  std::array<std::vector<Directive::Handle>, std::tuple_size<Hook>::value> _roots;

  /// Maximum number of directives that can execute in a specific hook. These are updated during
  /// directive load, if needed. This includes the top level directives.
  std::array<size_t, std::tuple_size<Hook>::value> _directive_count { 0 };
};

inline bool Config::is_active() const { return _active_p; }