

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

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

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Assign;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::Gt;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;
using ::testing::Unused;
using ::testing::WithArgs;

TEST(AtmMachine, CanWithdrawNonOverlapping) {
  // Arrange
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(Gt(1234)))
      .Times(AnyNumber())
      .WillRepeatedly(Return(2000));

  EXPECT_CALL(mock_bankserver, GetBalance(1234))
      .Times(1)
      .WillOnce(Return(1000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(1234, withdraw_value);
  auto result2 = atm_machine.Withdraw(5678, withdraw_value);
  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

// This test is disabled since it fails due to stickiness of the rules.
TEST(AtmMachine, DISABLED_CanWithdraw_MultipleCalls) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(1000));

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(AtmMachine, CanWithdraw_MultipleCalls_Retire) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(1000));

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(2000))
      .RetiresOnSaturation();

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(AtmMachine, CanWithdraw_MultipleCalls_WillOnce) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(2)
      .WillOnce(Return(1000))
      .WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

// This test is disabled since it fails due to stickiness of the rules.
TEST(AtmMachine, DISABLED_CanWithdraw_GenericRuleFirst) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(_))
      .Times(1)
      .WillRepeatedly(Return(2000));

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(1000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(AtmMachine, CanWithdraw_GenericRuleFirst_Retire) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(_))
      .Times(1)
      .WillRepeatedly(Return(2000));

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(1000))
      .RetiresOnSaturation();

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(AtmMachine, CanWithdraw_GenericRuleFirst_DifferentValues) {
  // Arrange
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(_))
      .Times(1)
      .WillRepeatedly(Return(2000));

  EXPECT_CALL(mock_bankserver, GetBalance(1234))
      .Times(1)
      .WillOnce(Return(1000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(1234, withdraw_value);
  auto result2 = atm_machine.Withdraw(5678, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

// This test is disabled since it fails due to stickiness of the rules.
TEST(AtmMachine, DISABLED_CanWithdraw_GenericRuleLast_DifferentValues) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(1000));

  EXPECT_CALL(mock_bankserver, GetBalance(_))
      .Times(1)
      .WillRepeatedly(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  auto result1 = atm_machine.Withdraw(account_number, withdraw_value);
  auto result2 = atm_machine.Withdraw(account_number + 1, withdraw_value);

  // Assert
  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(AtmMachine, CanWithdraw_RetireStickyRules) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  const int number_of_calls = 10;

  NiceMock<MockBankServer> mock_bankserver;

  // Expectations
  for (int i = 0; i < number_of_calls; i++) {
    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(1)
        .WillOnce(Return(1000 + i))
        .RetiresOnSaturation();
  }

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = true;
  for (int i = 0; i < number_of_calls; i++) {
    withdraw_result = withdraw_result &&
                      atm_machine.Withdraw(account_number + i, withdraw_value);
  }

  // Assert
  EXPECT_TRUE(withdraw_result);
}