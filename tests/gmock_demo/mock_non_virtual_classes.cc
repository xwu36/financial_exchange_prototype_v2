

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine_template.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::Return;

//-----------------------------------------------------------------------------
class MockBankServer {
 public:
  MOCK_METHOD(void, Connect, (), ());
  MOCK_METHOD(void, Disconnect, (), ());
  MOCK_METHOD(void, Deposit, (int, int), ());
  MOCK_METHOD(void, Debit, (int, int), ());
  MOCK_METHOD(int, GetBalance, (int), (const));
};

TEST(AtmMachine, CanWithdrawSimple) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;

  MockBankServer mock_bankserver;

  ON_CALL(mock_bankserver, GetBalance(account_number))
      .WillByDefault(Return(2000));

  // Act
  AtmMachine<MockBankServer> atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
