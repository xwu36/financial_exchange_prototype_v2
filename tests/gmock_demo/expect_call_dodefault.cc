

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::DoDefault;
using ::testing::Exactly;
using ::testing::Gt;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

//-----------------------------------------------------------------------------
class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(bool, DoubleTransaction, (int, int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

//-----------------------------------------------------------------------------
class DoDefaultTest : public ::testing::Test {
 public:
  void SetUp() override {
    ON_CALL(mock_bankserver_, GetBalance).WillByDefault(Return(1000));
  }

  MockBankServer mock_bankserver_;
};

TEST_F(DoDefaultTest, CanWithdraw) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver_, GetBalance(account_number))
      .WillRepeatedly(DoDefault());

  // Act
  AtmMachine atm_machine(&mock_bankserver_);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

TEST_F(DoDefaultTest, CannotWithdraw) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver_, GetBalance(account_number))
      .WillRepeatedly(Return(0));

  // Act
  AtmMachine atm_machine(&mock_bankserver_);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_FALSE(withdraw_result);
}