

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Gt;
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
  virtual void Credit(int account_number, int value) = 0;
  virtual void Debit(int account_number, int value) = 0;
  virtual int GetBalance(int account_number) const = 0;
};
//-----------------------------------------------------------------------------

class FakeBankServer : public BankServer {
 public:
  void Connect() override {}
  void Disconnect() override {}
  void Credit(int account_number, int value) override {}
  void Debit(int account_number, int value) override {}
  int GetBalance(int account_number) const override {
    switch (account_number) {
      case 1234:
      case 4567:
        return 2000;

      case 1000:
      case 2000:
        return 5000;

      default:
        return account_number * account_number;
    }
  }
};

class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));

  // Delegates the default actions of the methods to a FakeBankServer object.
  // This must be called *before* the custom ON_CALL() statements.
  void DelegateToFake() {
    ON_CALL(*this, GetBalance).WillByDefault([this](int account_number) {
      return fake_.GetBalance(account_number);
    });
  }

 private:
  FakeBankServer fake_;  // Keeps an instance of the fake in the mock.
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

TEST(AtmMachine, CanWithdrawSimple) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  mock_bankserver.DelegateToFake();  // Enables the fake for delegation.

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
