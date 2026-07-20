// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROMEA_CORE_COMMON__FSM__FSM_EVENT_NOTIFIER_HPP_
#define ROMEA_CORE_COMMON__FSM__FSM_EVENT_NOTIFIER_HPP_

// romea
#include "romea_core_common/fsm/FSMEvent.hpp"

// std
#include <functional>
#include <utility>

namespace romea
{
namespace core
{

using FSMEventCallback = std::function<void(const FSMEvent &)>;

class FSMEventNotifier
{
public:
  void register_callback(FSMEventCallback callback) { callback_ = std::move(callback); }

  void notify(const FSMEvent & event) const
  {
    if (callback_) {
      callback_(event);
    }
  }

private:
  FSMEventCallback callback_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__FSM__FSM_EVENT_NOTIFIER_HPP_
