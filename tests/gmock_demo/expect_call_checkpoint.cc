

#include <exception>
#include <iostream>
#include <vector>

#include "bankserver/atm_machine.h"
#include "bankserver/bankserver.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
//-----------------------------------------------------------------------------
using ::testing::InSequence;
using ::testing::MockFunction;
//-----------------------------------------------------------------------------
class mock_bank_server : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Deposit, (int, int), (override));
  MOCK_METHOD(void, Withdraw, (int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};
//-----------------------------------------------------------------------------
mock_bank_server mock_bank_server;
//-----------------------------------------------------------------------------
// If value is any product of 10000, it withdraws 10000 from account 1234.
void Withdraw10kProducts(int value) {
  if (value % 10000 == 0) {
    mock_bank_server.Withdraw(1234, 10000);
  }
}
//-----------------------------------------------------------------------------
TEST(Withdraw10kProductsTest, WithdrawIsCalledTwice) {
  EXPECT_CALL(mock_bank_server, Withdraw(1234, 10000)).Times(2);

  Withdraw10kProducts(20000);
  Withdraw10kProducts(2000);
  Withdraw10kProducts(3000);
  Withdraw10kProducts(4000);
  Withdraw10kProducts(30000);
}

// The expectation spec says that the first Withdraw(1234, 10000) call must
// happen before checkpoint 1, the second Withdraw(1234, 10000) call must happen
// after checkpoint 2, and nothing should happen between the two checkpoints.
TEST(Withdraw10kProductsTest, WithdrawIsCalledCorrectly) {
  // Class MockFunction<F> has exactly one mock method.  It is named
  // Call() and has type F.
  MockFunction<void(std::string check_point_name)> checkpoint;

  {
    InSequence s;

    EXPECT_CALL(mock_bank_server, Withdraw(1234, 10000));
    EXPECT_CALL(checkpoint, Call("1"));
    EXPECT_CALL(checkpoint, Call("2"));
    EXPECT_CALL(mock_bank_server, Withdraw(1234, 10000));
  }

  // Withdraw should be called once
  Withdraw10kProducts(20000);
  checkpoint.Call("1");

  // Withdraw should not be called
  Withdraw10kProducts(2000);
  Withdraw10kProducts(3000);
  Withdraw10kProducts(4000);

  // Withdraw should be called once
  checkpoint.Call("2");
  Withdraw10kProducts(30000);
}
