

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Args;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Gt;
using ::testing::Lt;
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

TEST(AtmMachine, OnCallWithZero) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // All default values.
  ON_CALL(mock_bankserver, Connect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Disconnect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Debit).WillByDefault(Return());
  ON_CALL(mock_bankserver, Credit).WillByDefault(Return());
  ON_CALL(mock_bankserver, GetBalance).WillByDefault(Return(0));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_FALSE(withdraw_result);
}

TEST(AtmMachine, OnCallWithOneFunctionSpecified) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // All default values.
  ON_CALL(mock_bankserver, Connect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Disconnect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Debit).WillByDefault(Return());
  ON_CALL(mock_bankserver, Credit).WillByDefault(Return());
  ON_CALL(mock_bankserver, GetBalance(account_number))
      .WillByDefault(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 =
      atm_machine.Withdraw(account_number + 1, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result1);
  EXPECT_FALSE(withdraw_result2);
}

TEST(AtmMachine, OnCallWithOneFunctionSpecifiedTwice) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  // All default values.
  ON_CALL(mock_bankserver, Connect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Disconnect).WillByDefault(Return());
  ON_CALL(mock_bankserver, Debit).WillByDefault(Return());
  ON_CALL(mock_bankserver, Credit).WillByDefault(Return());

  // GetBalance is specified twice. The more generic one is first.
  ON_CALL(mock_bankserver, GetBalance).WillByDefault(Return(2000));
  ON_CALL(mock_bankserver, GetBalance(account_number)).WillByDefault(Return(1));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result1 = atm_machine.Withdraw(account_number, withdraw_value);
  bool withdraw_result2 =
      atm_machine.Withdraw(account_number + 1, withdraw_value);

  // Assert
  EXPECT_FALSE(withdraw_result1);
  EXPECT_TRUE(withdraw_result2);
}

TEST(AtmMachine, CanWithdrawSimple) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  ON_CALL(mock_bankserver, GetBalance(account_number))
      .WillByDefault(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

TEST(AtmMachine, CanWithdrawSimpleWithWildcardMatcher) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  ON_CALL(mock_bankserver, GetBalance(_)).WillByDefault(Return(2000));
  ON_CALL(mock_bankserver, GetBalance(Gt(1000))).WillByDefault(Return(2000));
  ON_CALL(mock_bankserver, GetBalance(AllOf(Gt(1000), Lt(2000))))
      .WillByDefault(Return(2000));

  ON_CALL(mock_bankserver, DoubleTransaction(_, _, _))
      .With(AllOf(Args<0, 1>(Gt()), Args<1, 2>(Gt())))
      .WillByDefault(Return(true));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

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

  // Act
  AtmMachine atm_machine(&mock_bankserver_);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}