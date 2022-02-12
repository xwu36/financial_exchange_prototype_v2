#ifndef TESTS_GMOCK_DEMO_BANKSERVER_ATM_MACHINE_TEMPLATE
#define TESTS_GMOCK_DEMO_BANKSERVER_ATM_MACHINE_TEMPLATE

template <class BankServer>
class AtmMachine {
  BankServer* bankServer_;

 public:
  AtmMachine(BankServer* bankServer) : bankServer_(bankServer) {}

  bool Withdraw(int account_number, int value) {
    bool result = false;
    bankServer_->Connect();
    auto available_balance = bankServer_->GetBalance(account_number);
    std::cout << "account_number: " << account_number << std::endl;
    std::cout << "available_balance: " << available_balance << std::endl;

    if (available_balance >= value) {
      bankServer_->Debit(account_number, value);
      result = true;
    }

    bankServer_->Disconnect();
    return result;
  }
};

#endif /* TESTS_GMOCK_DEMO_BANKSERVER_ATM_MACHINE_TEMPLATE */
