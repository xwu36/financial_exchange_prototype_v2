
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
  MOCK_METHOD(void, Deposit, (int, int), (override));
  MOCK_METHOD(void, Withdraw, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

using ::testing::_;  // Matches anything
using ::testing::Assign;
using ::testing::DoAll;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(AtmMachine, CanWithdrawSideEffect) {
  bool done = false;
  int account_number;
  int value;

  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Withdraw(_, _))
      .Times(1)
      .WillOnce(DoAll(SaveArg<0>(&account_number), SaveArg<1>(&value)));

  // EXPECT_CALL(mock_bankserver, Withdraw(_, _))
  //     .Times(1)
  //     .WillOnce((SaveArg<0>(&account_number)));

  EXPECT_CALL(mock_bankserver, Disconnect())
      .Times(1)
      .WillOnce(Assign(&done, true));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);

  EXPECT_TRUE(done);
  EXPECT_EQ(account_number, 1234);
  EXPECT_EQ(value, 1000);
}
