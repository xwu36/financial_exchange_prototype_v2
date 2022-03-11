

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class InvalidValueException : public virtual std::exception {
 public:
  const char* what() const noexcept override { return "Invalid value"; }
};

class BankServer {
 public:
  virtual ~BankServer() {}
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;
  virtual void Credit(int account_number, int value) = 0;
  virtual void Debit(int account_number, int value) = 0;
  virtual int GetBalance(int account_number) const = 0;
};

class MockBankServer : public BankServer {
 public:
  MOCK_METHOD0(Connect, void());
  MOCK_METHOD0(Disconnect, void());
  MOCK_METHOD2(Credit, void(int account_number, int value));
  MOCK_METHOD2(Withdraw, void(int account_number, int value));
  MOCK_CONST_METHOD1(GetBalance, int(int account_number));
};

class AtmMachine {
  BankServer* bankServer_;
  int max_withdraw_per_transaction_;

 public:
  AtmMachine(BankServer* bankServer, int max_withdraw_per_transaction)
      : bankServer_(bankServer),
        max_withdraw_per_transaction_(max_withdraw_per_transaction) {}

  bool Withdraw(int account_number, int value) {
    if (value < 0) {
      throw InvalidValueException();
    }
    bankServer_->Connect();
    auto available_balance = bankServer_->GetBalance(account_number);
    if (available_balance >= value) {
      for (int i = 0; i < value / max_withdraw_per_transaction_; ++i) {
        bankServer_->Debit(account_number, max_withdraw_per_transaction_);
      }
      if (value % max_withdraw_per_transaction_ > 0) {
        bankServer_->Debit(account_number,
                           value % max_withdraw_per_transaction_);
      }
      bankServer_->Disconnect();
      return true;
    } else {
      bankServer_->Disconnect();

      return false;
    }
  }
};

using ::testing::_;
using ::testing::AtLeast;  // #1
using ::testing::Exactly;  // #1
using ::testing::NiceMock;
using ::testing::Return;  // #1
using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(AtmMachineTest, CanWithdraw) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  const int max_withdraw_per_transaction = 500;

  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver,
              Withdraw(account_number, max_withdraw_per_transaction))
      .Times(2);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver, max_withdraw_per_transaction);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

TEST(AtmMachineTest, InvalidWithdrawValueThrows) {
  const int account_number = 1234;
  const int withdraw_value = -1;
  const int max_withdraw_per_transaction = 500;

  MockBankServer mock_bankserver;

  AtmMachine atm_machine(&mock_bankserver, max_withdraw_per_transaction);

  EXPECT_THAT([&]() { atm_machine.Withdraw(account_number, withdraw_value); },
              ThrowsMessage<InvalidValueException>(StrEq("Invalid value")));
}

TEST(AtmMachineTest, InvalidWithdrawValueThrowsWithoutGmock) {
  const int account_number = 1234;
  const int withdraw_value = -1;
  const int max_withdraw_per_transaction = 500;

  MockBankServer mock_bankserver;

  AtmMachine atm_machine(&mock_bankserver, max_withdraw_per_transaction);

  EXPECT_THROW(
      {
        try {
          atm_machine.Withdraw(account_number, withdraw_value);
        } catch (const InvalidValueException& e) {
          EXPECT_STREQ("Invalid value", e.what());
          throw;
        }
      },
      InvalidValueException);
}
