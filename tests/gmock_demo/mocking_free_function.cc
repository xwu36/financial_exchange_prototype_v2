

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

bool Log(std::string message) {
  std::cout << "message: " << message << std::endl;
  return true;
}

class LogWrapperInterface {
 public:
  virtual bool Log(std::string msg) = 0;
};

class LogWrapper : public LogWrapperInterface {
 public:
  bool Log(std::string msg) override { return ::Log(msg); }
};

class MockLogWrapper : public LogWrapperInterface {
 public:
  MOCK_METHOD(bool, Log, (std::string), (override));
};

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

class BankOfAriServer : public BankServer {
 public:
  virtual ~BankOfAriServer() {}
  virtual void Connect();
  virtual void Disconnect();
  virtual void Credit(int account_number, int value);
  virtual void Debit(int account_number, int value);
  virtual int GetBalance(int account_number) const;
};

class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

class AtmMachine {
  BankServer* bankServer_;
  LogWrapperInterface* logWrapper_;

 public:
  AtmMachine(BankServer* bankServer, LogWrapperInterface* logWrapper)
      : bankServer_(bankServer), logWrapper_(logWrapper) {}

  bool Withdraw(int account_number, int value) {
    bool result = false;
    bankServer_->Connect();
    logWrapper_->Log("Connected.");
    auto available_balance = bankServer_->GetBalance(account_number);

    if (available_balance >= value) {
      bankServer_->Debit(account_number, value);
      logWrapper_->Log("Withdraw done.");
      result = true;
    }

    bankServer_->Disconnect();
    logWrapper_->Log("Disconnected.");
    return result;
  }
};

TEST(AtmMachine, CanWithdrawWithLog) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;
  MockLogWrapper mock_log;

  ON_CALL(mock_bankserver, GetBalance(account_number))
      .WillByDefault(Return(2000));

  // EXPECT_CALL(mock_log, Log(_)).Times(3).WillRepeatedly(Return(true));

  // Act
  AtmMachine atm_machine(&mock_bankserver, &mock_log);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
