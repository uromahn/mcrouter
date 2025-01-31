/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <memory>

#include <folly/fibers/FiberManager.h>

#include "mcrouter/McrouterFiberContext.h"

namespace facebook { namespace memcache { namespace mcrouter {

template <class RouteHandleIf, class ShadowPolicy>
template <class Request>
void ShadowRoute<RouteHandleIf, ShadowPolicy>::dispatchShadowRequest(
    std::shared_ptr<RouteHandleIf> shadow,
    std::shared_ptr<Request> adjustedReq) const {
  folly::fibers::addTask(
      [shadow = std::move(shadow), adjustedReq = std::move(adjustedReq)]() {
        // we don't want to spool shadow requests
        fiber_local::clearAsynclogName();
        fiber_local::addRequestClass(RequestClass::kShadow);
        shadow->route(*adjustedReq);
      });
}

}}} // facebook::memcache::mcrouter
