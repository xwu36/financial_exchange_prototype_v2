

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
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
class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(bool, DoubleTransaction, (int, int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

// TEST(AtmMachine, CanWithdrawSimpleExpectation) {
//   // Arrange
//   const int account_number = 1234;
//   const int withdraw_value = 1000;
//   MockBankServer mock_bankserver;

//   // Expectations
//   EXPECT_CALL(mock_bankserver, GetBalance(account_number))
//       .Times(AtLeast(1))
//       .WillRepeatedly(Return(2000));

//   // Act
//   AtmMachine atm_machine(&mock_bankserver);
//   bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

//   // Assert
//   EXPECT_TRUE(withdraw_result);
// }

TEST(AtmMachine, CanWithdrawSpecifyTimess) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  MockBankServer mock_bankserver;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(1)
      .WillOnce(Return(2000));

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

  // Assert
  EXPECT_TRUE(withdraw_result);
}

// TEST(AtmMachine, CanWithdraw) {
//   // Arrange
//   const int account_number = 1234;
//   const int withdraw_value = 1000;

//   MockBankServer mock_bankserver;

//   // Expectations
//   EXPECT_CALL(mock_bankserver, Connect()).Times(1);

//   EXPECT_CALL(mock_bankserver, GetBalance(account_number))
//       .Times(1)
//       .WillOnce(Return(2000));

//   EXPECT_CALL(mock_bankserver, Debit(account_number, withdraw_value)).Times(1);

//   EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

//   // Act
//   AtmMachine atm_machine(&mock_bankserver);
//   bool withdraw_result = atm_machine.Withdraw(account_number, withdraw_value);

//   // Assert
//   EXPECT_TRUE(withdraw_result);
// }
