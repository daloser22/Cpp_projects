
#include "Account.h"
#include <iostream>
#include <memory>

Account::Account(const std::string& iban):
r_iban(iban),r_balance(0.0)
{       
}

Account::~Account()
{}

double Account::GetBalance(void) const
{
  return r_balance;
}

std::string Account::GetIBAN(void) const
{
  return r_iban;
}

void Account::DisplayAccount(void) const
{
    std::cout<< "Account Data: " << std::endl;
    std::cout<< "IBAN: " << r_iban << std::endl;
    std::cout<< "Balance: " << r_balance << " €" <<std::endl;
}

bool Account::Deposit(double amount)
{
  if(amount < 0.0)
  {
    throw std::runtime_error("Amount can not be negative (-) ");
  }
  r_balance += amount;

  Transaction element;
  element.SetHour();
  element.SetAmount(amount);
  element.SetTypeOperation(e_state::E_DEPOSIT);
  v_transaction.push_back(element);
  return true;
}

void Account::ApplyInterest(void)
{
 /* Do nothing, only to apply polymorfism from SavingAccount */
}

bool Account::Withdraw(double amount)
{
  if(amount < 0.0)
  {
    throw std::runtime_error("Amount can not be negative (-) ");
  }
  
  if((r_balance >= amount))
  {
    r_balance -= amount;
    
    Transaction element;
    element.SetHour();
    element.SetAmount(amount);
    element.SetTypeOperation(e_state::E_WITHDRAW);
    v_transaction.push_back(element);
    return true;
  }
  else
  {
    return false;
  }
}

bool CheckingAccount::Withdraw(double amount)
{ 
  if(amount < 0.0)
  {
    throw std::runtime_error("Amount can not be negative (-) ");
  }
  
  double result = r_balance - amount;

  if (result >= -r_overdraft_limit)
  {
    r_balance -= amount;
    Transaction element;
    element.SetHour();
    element.SetAmount(amount);
    element.SetTypeOperation(e_state::E_WITHDRAW);
    v_transaction.push_back(element);
    return true;
  }
  else
  {
    return false;
  } 
}

CheckingAccount::CheckingAccount(const std::string& iban, int overdraft)
 : Account(iban), r_overdraft_limit(overdraft)
 {
 }

void CheckingAccount::DisplayAccount(void) const
{
    std::cout<< "Account Data: " << std::endl;
    std::cout<<"Account Overdraft Limit: " << r_overdraft_limit << " €" <<std::endl;
    std::cout<< "Type: Checking" << std::endl;
    std::cout<< "IBAN: " << r_iban << std::endl;
    std::cout<< "Balance: " << r_balance << " €" <<std::endl;
}

void SavingsAccount::ApplyInterest(void)
{
    double benefit = r_balance * (r_interest_rate/100.00);
    r_balance += benefit; 
    Transaction element;
    element.SetHour();
    element.SetAmount(benefit);
    element.SetTypeOperation(e_state::E_INTEREST);
    v_transaction.push_back(element);
}

SavingsAccount::SavingsAccount(const std::string& iban, double interest)
 : Account(iban), r_interest_rate(interest)
{

}

void SavingsAccount::DisplayAccount(void) const
{
    std::cout<< "Account Data: " << std::endl;
    std::cout<<"Interest Rate: " << r_interest_rate << " %" <<std::endl;
    std::cout<< "Type: Saving" << std::endl;
    std::cout<< "IBAN: " << r_iban << std::endl;
    std::cout<< "Balance: " << r_balance << " €" <<std::endl;
}

