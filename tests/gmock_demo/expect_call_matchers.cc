

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
using ::testing::_;  // Matches anything
using ::testing::Gt;
using ::testing::Lt;

using ::testing::AllArgs;
using ::testing::AllOf;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(AtmMachine, CanWithdrawWithMatchers1) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);
  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit(_, _)).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
//-----------------------------------------------------------------------------
TEST(AtmMachine, CanWithdrawWithMatchers2) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance(Gt(1000)))
      .Times(1)
      .WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit(_, Lt(10000))).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

//-----------------------------------------------------------------------------
TEST(AtmMachine, CanWithdrawWithMultiArgumentMatchers) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations

  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit(_, _)).With(Gt()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
//-----------------------------------------------------------------------------
TEST(AtmMachine, CanWithdrawWithCompositMatchers) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance(AllOf(Gt(1000), Lt(10000))))
      .Times(1)
      .WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit(_, Lt(10000))).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
//-----------------------------------------------------------------------------
TEST(AtmMachine, CanWithdrawIgnoreArguments) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect).Times(1);

  EXPECT_CALL(mock_bankserver, GetBalance).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Debit).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}