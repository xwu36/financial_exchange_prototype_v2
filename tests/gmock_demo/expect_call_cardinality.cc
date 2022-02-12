

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::Exactly;
using ::testing::Gt;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

//-----------------------------------------------------------------------------
class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Deposit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(bool, DoubleTransaction, (int, int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

TEST(AtmMachine, CanWithdrawSpecifyTimesReturnOnce) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(AtMost(10))
      .WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result1);
  EXPECT_FALSE(withdraw_result2);
}

TEST(AtmMachine, CanWithdrawSpecifyTimesReturnTwice) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(AtMost(10))
      .WillOnce(Return(2000))
      .WillOnce(Return(3000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result1);
  EXPECT_TRUE(withdraw_result2);
}

TEST(AtmMachine, CanWithdrawSpecifyRepeatedly) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(AtMost(10))
      .WillOnce(Return(2000))
      .WillRepeatedly(Return(3000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result1);
  EXPECT_TRUE(withdraw_result2);
}

TEST(AtmMachine, CanWithdrawNoCardinality) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .WillRepeatedly(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result1);
  EXPECT_TRUE(withdraw_result2);
}