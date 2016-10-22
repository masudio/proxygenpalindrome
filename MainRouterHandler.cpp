/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "MainRouterHandler.h"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <folly/FBString.h>

#include "MasudioStats.h"

using namespace proxygen;
using namespace folly;

namespace MasudioService {

MainRouterHandler::MainRouterHandler(MasudioStats* stats): stats_(stats) {
}

void MainRouterHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {
  stats_->recordRequest();
}

void MainRouterHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  if (body_) {
    body_->prependChain(std::move(body));
  } else {
    body_ = std::move(body);
  }
}

void MainRouterHandler::onEOM() noexcept {
  folly::IOBuf* rawInput = body_.get();
  fbstring input;
  fbstring output;
  if(NULL != rawInput && !rawInput->empty()) {
    input = rawInput->moveToFbString();
  }

  if(input.empty()) {
    output = "Give us a real request!\n";
  } else if(isPalindrome(input)) {
    output = input + " is a palindrome.\n";
  } else {
    output = input + " is NOT a palindrome.\n";
  }

  ResponseBuilder(downstream_)
    .status(200, "OK")
    .header("Request-Number",
            folly::to<std::string>(stats_->getRequestCount()))
    .header("MasudioHeader",
            folly::to<std::string>("imgonnahead"))
    .body(std::move(output))
    .sendWithEOM();
}

void MainRouterHandler::onUpgrade(UpgradeProtocol protocol) noexcept {
  // handler doesn't support upgrades
}

void MainRouterHandler::requestComplete() noexcept {
  delete this;
}

void MainRouterHandler::onError(ProxygenError err) noexcept {
  delete this;
}

bool MainRouterHandler::isPalindrome(fbstring s) {

  int length = s.length();
  for(int i = 0; i < (length / 2); i++) {
    if(s[i] != s[length - 1 - i]) {
      return false;
    }
  }

  return true;
}

}
