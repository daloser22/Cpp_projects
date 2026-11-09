#include <iostream>
#include <memory>
#include "Customers.h"
#include <algorithm>

Customer::Customer(std::string id, std::string name, std::string surname):
r_id(id), r_name(name), r_surname(surname)
{
}

void Customer::DisplayCustomer(void)
{
    std::cout<< "---------CUSTOMER---------" <<std::endl;
    std::cout<< r_id <<std::endl;
    std::cout<< r_name <<std::endl;
    std::cout<< r_surname <<std::endl;
}

std::string Customer::GetId(void) const
{
  return r_id;
}

std::string Customer::GetName(void) const
{
  return r_name;
}

std::string Customer::GetSurname(void) const
{
  return r_surname;
}

size_t Customer::GetSize(void) const
{
  return r_accounts.size();
}

void Customer::AddAccount(std::shared_ptr<Account> account)
{
    r_accounts.push_back(account);
}

bool Customer::DeleteAccount(std::shared_ptr<Account> account)
{
    bool retval = false;
    auto it = std::find(r_accounts.begin(),
                        r_accounts.end(),
                        account);

    if(it != r_accounts.end())
    {
        r_accounts.erase(it);

        auto it2 = std::find(r_closed_accounts.begin(),
                        r_closed_accounts.end(),
                        account);
        if(it2 == r_closed_accounts.end())
        {
          r_closed_accounts.push_back(account);
          retval = true;
        }
    }

    return retval;
}

void Customer::DisplayDeletedAccounts() const
{
    for(const auto& account : r_closed_accounts)
    {
        account->DisplayAccount();
        std::cout << std::endl;
    }
}


void Customer::DisplayAccounts() const
{
    for(const auto& account : r_accounts)
    {
        account->DisplayAccount();
        std::cout << std::endl;
    }
}

double Customer:: GetNetworth(void)
{
  double total_balance = 0.0;

  for(const auto& account: r_accounts)
  {
    total_balance += account->GetBalance();
  }

  return total_balance;
}
