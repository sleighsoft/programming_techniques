#include <iostream>
#include <cassert>
#include <list>
#include <cstdlib>
#include <string>
#include <initializer_list>
#include <vector>

using namespace std;

class Account {
public:
  Account(long accountNo, double balance, double limit, bool locked);
  ~Account();

  long accountNo() const { return accountNo_; }

  void setLimit(double limit) { limit_ = limit; }
  double getLimit() const { return limit_; }

  bool credit(double amount);
  bool debit(double amount);
  double balance() const { return balance_; }

  void setLock(bool locked) { locked_ = locked; }
  bool getLock() const { return locked_; }

private:
  long accountNo_;
  double balance_;
  double limit_;
  bool locked_;
};

Account::Account(long accountNo, double balance, double limit, bool locked)
    : accountNo_{accountNo}, balance_{balance}, limit_(limit), locked_{locked} {}

Account::~Account() {}

bool Account::credit(double amount)
{
  assert(amount >= 0.0);

  if (locked_)
    return false;

  balance_ = balance_ + amount;
  return true;
}

bool Account::debit(double amount)
{
  assert(amount >= 0.0);

  if (locked_)
    return false;

  if (balance_ - amount < limit_)
    return false;

  balance_ = balance_ - amount;
  return true;
}

class LoggedAccount : public Account {
private:
  enum TransactionType {
    CREDIT,
    DEBIT,
    SETLIMIT,
    INITIAL_BALANCE,
    CURRENT_BALANCE
  };
  struct Transaction {
    TransactionType type;
    double amount;
  };
  vector<Transaction> transactionList_;

  bool logTransaction(TransactionType type, double amount);

public:
  LoggedAccount(long accountNo, double balance, double limit, bool locked);
  void setLimit(double limit);
  bool credit(double amount);
  bool debit(double amount);
  vector<Transaction> transactions()
  { return transactionList_; };
};

LoggedAccount::LoggedAccount(long accountNo, double balance, double limit,
                             bool locked)
    : Account(accountNo, balance, limit, locked)
    {
  Transaction initial_t{INITIAL_BALANCE, balance};
  transactionList_.push_back(initial_t);
  Transaction current_t{CURRENT_BALANCE, balance};
  transactionList_.push_back(current_t);
}

bool LoggedAccount::logTransaction(TransactionType type, double amount)
{
  Transaction t{type, amount};
  bool transactinSuccess = true;
  switch (type)
  {
  case CREDIT:
    transactinSuccess = Account::credit(amount);
    break;
  case DEBIT:
    transactinSuccess = Account::debit(amount);
    break;
  case SETLIMIT:
    Account::setLimit(amount);
    transactinSuccess = false;
    break;
  case INITIAL_BALANCE:
  case CURRENT_BALANCE:
    return false;
  }
  transactionList_.insert(transactionList_.end() - 1, t);
  if (transactinSuccess)
  {
    transactionList_.pop_back();
    Transaction current_t{CURRENT_BALANCE, Account::balance()};
    transactionList_.push_back(current_t);
  }
  return transactinSuccess;
}

bool LoggedAccount::debit(double amount)
{
  return logTransaction(DEBIT, amount);
}

bool LoggedAccount::credit(double amount)
{
  return logTransaction(CREDIT, amount);
}

void LoggedAccount::setLimit(double amount)
{
  logTransaction(SETLIMIT, amount);
}

std::string getReadableType(int type)
{
  std::string readableType;
  switch(type)
  {
  case 0:
    readableType = "Credit";
    break;
  case 1:
    readableType = "Debit";
    break;
  case 2:
    readableType = "SetLimit";
    break;
  case 3:
    readableType = "InitialBalance";
    break;
  case 4:
    readableType = "CurrentBalance";
    break;
  default:
    readableType = "";
  }
  return readableType;
}

int main(int argc, char **argv)
{
  Account A1(19920, 0.0, -1000.0, false);
  LoggedAccount A2(20020, 0.0, -1000.0, false);

  A1.credit(500.0);
  A2.credit(500.0);
  A2.debit(100.0);
  A2.setLimit(-2000.);

  for (const auto &x : A2.transactions())
  {
    std::string readableType;
    cout << getReadableType(x.type) << ": " << x.amount << endl;
  }

  return 0;
}
