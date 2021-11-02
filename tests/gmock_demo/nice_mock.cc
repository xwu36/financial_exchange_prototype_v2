

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Gt;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

//-----------------------------------------------------------------------------
// bankserver.h
class BankServer {
 public:
  virtual ~BankServer() {}
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;
  virtual void Deposit(int account_number, int value) = 0;
  virtual void Withdraw(int account_number, int value) = 0;
  virtual int GetBalance(int account_number) const = 0;
};
//-----------------------------------------------------------------------------

class BankOfAriServer : public BankServer {
 public:
  virtual ~BankOfAriServer() {}
  virtual void Connect();
  virtual void Disconnect();
  virtual void Deposit(int account_number, int value);
  virtual void Withdraw(int account_number, int value);
  virtual int GetBalance(int account_number) const;
};

class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Deposit, (int, int), (override));
  MOCK_METHOD(void, Withdraw, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
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
      bankServer_->Withdraw(account_number, value);
      result = true;
    }

    bankServer_->Disconnect();
    return result;
  }
};

TEST(AtmMachine, CanWithdrawSimple) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  NiceMock<MockBankServer> mock_bankserver;

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
