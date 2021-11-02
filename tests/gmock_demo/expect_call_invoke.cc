

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
  MOCK_METHOD(void, Deposit, (int, int), (override));
  MOCK_METHOD(void, Withdraw, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

using ::testing::_;
using ::testing::Assign;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ThrowsMessage;
using ::testing::Unused;
using ::testing::WithArgs;

double Square(double x) { return (x * x); }
int Sum(int x, int y) { return x + y; }
int Return10000() { return 10000; }

class Helper {
 public:
  int ComplexJobSingleParameter(int x) { return (x * x); }
  int ComplexJobMultiParameter(int x, int y, int z) { return (x * y * z); }
};

TEST(AtmMachine, CanWithdrawWithMultipleInvoke) {
  // Arrange
  const int account_number = 1234;
  const int withdraw_value = 1000;
  const int number_of_calls = 10;

  MockBankServer mock_bankserver;
  Helper helper;

  // Expectations
  EXPECT_CALL(mock_bankserver, GetBalance(account_number))
      .Times(number_of_calls)
      .WillOnce(Square)
      .WillOnce([](double n) { return Square(n); })
      .WillOnce([](double n) { return Sum(n, 1000); })
      .WillOnce([]() { return Return10000(); })
      .WillOnce(Return10000)
      .WillOnce(Return(Return10000()))
      .WillOnce(
          [&helper](int n) { return helper.ComplexJobSingleParameter(n); })
      .WillOnce([&helper](int n) {
        return helper.ComplexJobMultiParameter(10, 20, n);
      })
      .WillRepeatedly(
          [&helper](int n) { return helper.ComplexJobSingleParameter(n); });

  // Act
  AtmMachine atm_machine(&mock_bankserver);
  bool withdraw_result = true;
  for (int i = 0; i < number_of_calls; i++) {
    withdraw_result =
        withdraw_result && atm_machine.Withdraw(account_number, withdraw_value);
  }

  // Assert
  EXPECT_TRUE(withdraw_result);
}

// TEST(AtmMachine, CanWithdrawComposit) {
//   // Arrange
//   bool done = false;

//   MockBankServer mock_bankserver;

//   // Expectations
//   EXPECT_CALL(mock_bankserver, Connect()).Times(1);

//   EXPECT_CALL(mock_bankserver, GetBalance(_))
//       .Times(1)
//       .WillOnce(DoAll(Assign(&done, true), Invoke(Square)));

//   EXPECT_CALL(mock_bankserver, Withdraw(_, _)).Times(1);

//   EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

//   // Act
//   AtmMachine atm_machine(&mock_bankserver);
//   bool withdraw_result = atm_machine.Withdraw(1234, 1000);

//   std::cout << "done: " << done << std::endl;

//   // Assert
//   EXPECT_TRUE(withdraw_result);
// }

// TEST(AtmMachine, CanWithdrawComposit2) {
//   // Arrange
//   bool done = false;

//   MockBankServer mock_bankserver;

//   // Expectations
//   EXPECT_CALL(mock_bankserver, Connect()).Times(1);

//   EXPECT_CALL(mock_bankserver, GetBalance(_))
//       .Times(1)
//       .WillOnce(DoAll(Assign(&done, true), Return(2000)));

//   EXPECT_CALL(mock_bankserver, Withdraw(_, _)).Times(1);

//   EXPECT_CALL(mock_bankserver, Disconnect()).Times(1);

//   // Act
//   AtmMachine atm_machine(&mock_bankserver);
//   bool withdraw_result = atm_machine.Withdraw(1234, 1000);

//   std::cout << "done: " << done << std::endl;

//   // Assert
//   EXPECT_TRUE(withdraw_result);
// }