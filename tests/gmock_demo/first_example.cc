

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

TEST(AtmMachine, OnCallWithZero) {
  // Arrange
  MockBankServer mock_bankserver;

  // Specify the mock's behavior.
  // ...

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result =
      atm_machine.Withdraw(/*account_number=*/1234, /*value=*/1000);

  // Assert
  EXPECT_FALSE(withdraw_result);
}
