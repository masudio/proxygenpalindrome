/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <proxygen/httpserver/Mocks.h>
#include "../MainRouterHandler.h"
#include "../MasudioStats.h"

using namespace MasudioService;
using namespace proxygen;
using namespace testing;

class MockMasudioStats : public MasudioStats {
 public:
  MOCK_METHOD0(recordRequest, void());
  MOCK_METHOD0(getRequestCount, uint64_t());
};

class MainRouterHandlerFixture : public testing::Test {
 public:
  void SetUp() override {
    handler = new MainRouterHandler(&stats);
    responseHandler = folly::make_unique<MockResponseHandler>(handler);
    handler->setResponseHandler(responseHandler.get());
  }

  void TearDown() override {
    Mock::VerifyAndClear(&stats);
    Mock::VerifyAndClear(responseHandler.get());

    // Since there is no easy way to verify that handler has deleted
    // itself, its advised to run test binary under AddressSanitzer
    // or valgrind to verify that.
  }

 protected:
  MainRouterHandler* handler{nullptr};
  StrictMock<MockMasudioStats> stats;
  std::unique_ptr<MockResponseHandler> responseHandler;
};

TEST_F(MainRouterHandlerFixture, OnProperRequestSendsResponse) {
  EXPECT_CALL(stats, recordRequest()).WillOnce(Return());
  EXPECT_CALL(stats, getRequestCount()).WillOnce(Return(5));

  HTTPMessage response;
  EXPECT_CALL(*responseHandler, sendHeaders(_)).WillOnce(
      DoAll(SaveArg<0>(&response), Return()));
  EXPECT_CALL(*responseHandler, sendEOM()).WillOnce(Return());

  // Since we know we dont touch request, its ok to pass `nullptr` here.
  handler->onRequest(nullptr);
  handler->onEOM();
  handler->requestComplete();

  EXPECT_EQ("5", response.getHeaders().getSingleOrEmpty("Request-Number"));
  EXPECT_EQ(200, response.getStatusCode());
}

TEST_F(MainRouterHandlerFixture, ReplaysBodyProperly) {
  EXPECT_CALL(stats, recordRequest()).WillOnce(Return());
  EXPECT_CALL(stats, getRequestCount()).WillOnce(Return(5));

  HTTPMessage response;
  folly::fbstring body;

  EXPECT_CALL(*responseHandler, sendHeaders(_)).WillOnce(
      DoAll(SaveArg<0>(&response), Return()));

  EXPECT_CALL(*responseHandler, sendBody(_)).WillRepeatedly(
      DoAll(
          Invoke([&] (std::shared_ptr<folly::IOBuf> b) {
            body += b->moveToFbString();
          }),
          Return()));

  EXPECT_CALL(*responseHandler, sendEOM()).WillOnce(Return());

  // Since we know we dont touch request, its ok to pass `nullptr` here.
  handler->onRequest(nullptr);
  handler->onBody(folly::IOBuf::copyBuffer("part1"));
  handler->onBody(folly::IOBuf::copyBuffer("part2"));
  handler->onEOM();
  handler->requestComplete();

  EXPECT_EQ("5", response.getHeaders().getSingleOrEmpty("Request-Number"));
  EXPECT_EQ(200, response.getStatusCode());
  EXPECT_EQ("part1part2", body);
}
