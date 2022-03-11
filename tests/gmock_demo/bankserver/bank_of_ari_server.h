#ifndef TESTS_GMOCK_SIMPLE_BANK_OF_ARI_SERVER
#define TESTS_GMOCK_SIMPLE_BANK_OF_ARI_SERVER

// This is an actual implementation of BankServer and it is supposed to be a
// complicated class and should not be used in tests!
class BankOfAriServer : public BankServer {
 public:
  virtual ~BankOfAriServer() {}
  virtual void Connect();
  virtual void Disconnect();
  virtual void Credit(int account_number, int value);
  virtual void Debit(int account_number, int value);
  virtual int GetBalance(int account_number) const;
};

#endif /* TESTS_GMOCK_SIMPLE_BANK_OF_ARI_SERVER */
