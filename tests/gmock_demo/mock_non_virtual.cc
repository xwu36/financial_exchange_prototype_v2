

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class BankServer {
 public:
  MOCK_METHOD(void, Connect, ());
  MOCK_METHOD(void, Disconnect, ());
  MOCK_METHOD(void, Deposit, (int, int));
  MOCK_METHOD(void, Debit, (int, int));
  MOCK_METHOD(int, GetBalance, (int), (const));
};

class AtmMachine {
  BankServer* bankServer_;

 public:
  AtmMachine(BankServer* bankServer) : bankServer_(bankServer) {}

  bool Withdraw(int account_number, int value) {
    bool result = false;
    bankServer_->Connect();
    auto available_balance = bankServer_->GetBalance(account_number);

    if (available_balance >= value) {
      bankServer_->Debit(account_number, value);
      result = true;
    }

    bankServer_->Disconnect();
    return result;
  }
};

using ::testing::_;
using ::testing::AtLeast;  // #1
using ::testing::Exactly;  // #1
using ::testing::NiceMock;
using ::testing::Return;  // #1
using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(AtmMachine, CanWithdraw) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  BankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit(account_number, withdraw_value))
      .Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
