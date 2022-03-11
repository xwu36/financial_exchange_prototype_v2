#ifndef TESTS_GMOCK_SIMPLE_BANKSERVER_ATM_MACHINE
#define TESTS_GMOCK_SIMPLE_BANKSERVER_ATM_MACHINE
#include "bankserver.h"

class AtmMachine {
  // The bank server we are talking to.
  BankServer* bankServer_;

 public:
  // Constructor.
  AtmMachine(BankServer* bankServer) : bankServer_(bankServer) {}

  // Withdraws value from account_number.
  bool Withdraw(int account_number, int value) {
    bool result = false;
    bankServer_->Connect();
    auto available_balance = bankServer_->GetBalance(account_number);

    if (available_balance >= value) {
      bankServer_->Debit(account_number, value);
      result = true;
    }

    bankServer_->Disconnect();
    return result;
  }

  void test(){
    
  }
};

#endif /* TESTS_GMOCK_SIMPLE_BANKSERVER_ATM_MACHINE */
