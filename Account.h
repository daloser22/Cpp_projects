#pragma once
#include <string>
#include <vector>
#include <chrono>

enum class e_state
{
  E_DEPOSIT = 0,
  E_TRANSFER,
  E_WITHDRAW,
  E_INTEREST
};

class Transaction
{
public:
  void SetHour(void)
  {
    auto ahora = std::chrono::system_clock::now();
    
    hour = std::chrono::system_clock::to_time_t(ahora);
  }

  void SetTypeOperation(e_state type)
  {
    typeOperation = type;
  }

  void SetAmount(double amount)
  {
    this->amount = amount;
  }

private:
  std::time_t hour;
  e_state typeOperation;
  double amount;
};

class Account
{
public:
    Account(const std::string& iban);

    virtual ~Account();

    double GetBalance(void) const;

    std::string GetIBAN(void) const;

    virtual void DisplayAccount(void) const;

    bool Deposit(double amount);

    virtual bool Withdraw(double amount);

    virtual void ApplyInterest();

protected:
    std::string r_iban;
    double r_balance;
    std::vector<Transaction> v_transaction;
};

class SavingsAccount: public Account
{
public:
SavingsAccount(const std::string& iban, double interest);
void DisplayAccount(void) const override;
void ApplyInterest(void) override;
private:
double r_interest_rate;
};

class CheckingAccount: public Account
{
public:
CheckingAccount(const std::string& iban, int overdraft);
void DisplayAccount(void) const override;

bool Withdraw(double amount) override;
private:
int r_overdraft_limit;
};