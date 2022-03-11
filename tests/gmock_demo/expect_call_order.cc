

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
using ::testing::Expectation;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::Sequence;

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(AtmMachine, CanWithdrawExpectInAnyOrder) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Connect()).Times(1);

  EXPECT_CALL(mock_bankserver, Debit(_, _)).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

// This test will fail if the order of EXPECT_CALLS is changed!
TEST(AtmMachine, CanWithdrawForceSequence) {
  InSequence seq;

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

// This test will fail if the order of EXPECT_CALLS is changed!
TEST(AtmMachine, CanWithdrawForceSequenceOnSomeCalls) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  {
    InSequence seq;
    EXPECT_CALL(mock_bankserver, Debit(_, _)).Times(1);
    EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);
  }

  {
    InSequence seq;
    EXPECT_CALL(mock_bankserver, Connect()).Times(1);
    EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));
  }

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

// Checks if Connect is called before Disconnect
TEST(AtmMachine, CanWithdrawExpectForcePartialOrderUsingSequence) {
  Sequence s1, s2;
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, Connect()).Times(1).InSequence(s1);

  EXPECT_CALL(mock_bankserver, Debit(_, _)).Times(1);
  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1).InSequence(s1);

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

// Checks if Connect is called before Disconnect
TEST(AtmMachine, CanWithdrawExpectForcePartialOrderWithAfter) {
  // Arrange
  MockBankServer mock_bankserver;

  // Expectations
  Expectation e_connect = EXPECT_CALL(mock_bankserver, Connect()).Times(1);
  Expectation e_debit = EXPECT_CALL(mock_bankserver, Debit(_, _)).Times(1);

  EXPECT_CALL(mock_bankserver, Disconnect()).Times(1).After(e_connect, e_debit);

  EXPECT_CALL(mock_bankserver, GetBalance(_)).Times(1).WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(1234, 1000);

  // Assert
  EXPECT_TRUE(withdraw_result);
}
