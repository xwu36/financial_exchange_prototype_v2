

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::DoDefault;
using ::testing::Gt;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::ReturnPointee;
using ::testing::ReturnRef;
using ::testing::ReturnRoundRobin;
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
TEST(AtmMachine, CanWithdrawReturnRoundRobin) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(3)
      .WillRepeatedly(ReturnRoundRobin({2000, 3000, 4000}));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = true;
  for (int i = 0; i < 3; i++) {
    withdraw_result =
        withdraw_result && atm_machine.Withdraw(account_number, withdraw_value);
  }

  // Assert
  EXPECT_TRUE(withdraw_result);
}

TEST(AtmMachine, CanWithdrawReturnArg) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(3)
      .WillRepeatedly(ReturnArg<0>());

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = true;
  for (int i = 0; i < 3; i++) {
    withdraw_result =
        withdraw_result && atm_machine.Withdraw(account_number, withdraw_value);
  }

  // Assert
  EXPECT_TRUE(withdraw_result);
}

TEST(AtmMachine, CanWithdrawReturnMultiple) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  int current_balance = 10000;
  int* ptr = &current_balance;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).WillRepeatedly(Return());

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .WillOnce(ReturnPointee(ptr))
      .WillOnce(ReturnArg<0>())
      .WillRepeatedly(Return(current_balance));

  EXPECT_CALL(mock_bankserver, Disconnect()).WillRepeatedly(DoDefault());

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = true;
  for (int i = 0; i < 10; i++) {
    withdraw_result =
        withdraw_result && atm_machine.Withdraw(account_number, withdraw_value);
  }

  // Assert
  EXPECT_TRUE(withdraw_result);
}
