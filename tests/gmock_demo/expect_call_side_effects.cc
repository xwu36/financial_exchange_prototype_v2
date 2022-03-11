
#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(bool, DoubleTransaction, (int, int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

using ::testing::_;  // Matches anything
using ::testing::Assign;
using ::testing::DoAll;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetErrnoAndReturn;
using ::testing::StrEq;
using ::testing::Throw;
using ::testing::Throws;
using ::testing::ThrowsMessage;

class InvalidValueException : public virtual std::exception {
 public:
  const char* what() const noexcept override { return "Invalid value"; }
};

TEST(AtmMachine, CanWithdrawSideEffect) {
  bool done = false;
  int account_number;
  int value;

  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Debit(_, _))
      .Times(1)
      .WillOnce(DoAll(SaveArg<0>(&account_number), SaveArg<1>(&value)));

  EXPECT_CALL(mock_bankserver, Disconnect())
      .Times(1)
      .WillOnce(Assign(&done, true));

  EXPECT_CALL(mock_bankserver, GetBalance(1234))
      .Times(1)
      .WillOnce(Return(1000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);

  EXPECT_TRUE(done);
  EXPECT_EQ(account_number, 1234);
  EXPECT_EQ(value, 1000);
}

TEST(AtmMachine, CanWithdrawSideEffectErrno) {
  // Arrange
  MockBankServer mock_bankserver;
  errno = 0;  // Defined in errno.h

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(4567))
      .Times(1)
      .WillOnce(SetErrnoAndReturn(EINVAL, 0));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(4567, 1000);

  // Assert
  EXPECT_FALSE(withdraw_result);
  EXPECT_EQ(errno, EINVAL);
}

TEST(AtmMachine, ThrowException) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect())
      .WillRepeatedly(Throw(InvalidValueException()));

  // Act and Assert
  AtmMachine atm_machine(&mock_bankserver);

  EXPECT_THROW(atm_machine.Withdraw(account_number, withdraw_value),
               InvalidValueException);

  EXPECT_THAT([&]() { atm_machine.Withdraw(account_number, withdraw_value); },
              Throws<InvalidValueException>());

  EXPECT_THAT([&]() { atm_machine.Withdraw(account_number, withdraw_value); },
              ThrowsMessage<InvalidValueException>(StrEq("Invalid value")));
}