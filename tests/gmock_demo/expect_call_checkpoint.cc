

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
class MockBankServer : public BankServer {
 public:
  MOCK_METHOD(void, Connect, (), (override));
  MOCK_METHOD(void, Disconnect, (), (override));
  MOCK_METHOD(void, Credit, (int, int), (override));
  MOCK_METHOD(void, Debit, (int, int), (override));
  MOCK_METHOD(bool, DoubleTransaction, (int, int, int), (override));
  MOCK_METHOD(int, GetBalance, (int), (const, override));
};

class Checkpoint {
 public:
  MOCK_METHOD(void, Check, (std::string), ());
};
//-----------------------------------------------------------------------------

class Withdraw10kProductsTest : public ::testing::Test {
 public:
  MockBankServer mock_bank_server;

  // If value is any product of 10000, it withdraws 10000 from account 1234.
  void Withdraw10kProducts(int value) {
    if (value % 10000 == 0) {
      mock_bank_server.Debit(1234, 10000);
    }
  }
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
TEST_F(Withdraw10kProductsTest, WithdrawIsCalledTwice) {
  EXPECT_CALL(mock_bank_server, Debit(1234, 10000)).Times(2);

  Withdraw10kProducts(20000);
  Withdraw10kProducts(2000);
  Withdraw10kProducts(3000);
  Withdraw10kProducts(4000);
  Withdraw10kProducts(30000);
}

// The expectation spec says that the first Withdraw(1234, 10000) call must
// happen before checkpoint 1, the second Withdraw(1234, 10000) call must happen
// after checkpoint 2, and nothing should happen between the two checkpoints.
TEST_F(Withdraw10kProductsTest, WithdrawIsCalledCorrectlyCheckpoint) {
  Checkpoint checkpoint;

  {
    InSequence s;

    EXPECT_CALL(mock_bank_server, Debit(1234, 10000));
    EXPECT_CALL(checkpoint, Check("1"));
    EXPECT_CALL(checkpoint, Check("2"));
    EXPECT_CALL(mock_bank_server, Debit(1234, 10000));
  }

  // Withdraw should be called once
  Withdraw10kProducts(20000);
  checkpoint.Check("1");

  // Withdraw should not be called
  Withdraw10kProducts(2000);
  Withdraw10kProducts(3000);
  Withdraw10kProducts(4000);

  // Withdraw should be called once
  checkpoint.Check("2");
  Withdraw10kProducts(30000);
}

// The expectation spec says that the first Withdraw(1234, 10000) call must
// happen before checkpoint 1, the second Withdraw(1234, 10000) call must happen
// after checkpoint 2, and nothing should happen between the two checkpoints.
TEST_F(Withdraw10kProductsTest, WithdrawIsCalledCorrectly) {
  // Class MockFunction<F> has exactly one mock method.  It is named
  // Call() and has type F.
  MockFunction<void(std::string check_point_name)> checkpoint;

  {
    InSequence s;

    EXPECT_CALL(mock_bank_server, Debit(1234, 10000));
    EXPECT_CALL(checkpoint, Call("1"));
    EXPECT_CALL(checkpoint, Call("2"));
    EXPECT_CALL(mock_bank_server, Debit(1234, 10000));
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
