#include <string>
#include <map>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include "BankManager.h"


static std::string InsufficientFundsException = "Insuficient founds :(";

void BankManager::CreateCustomer(std::string id, std::string name,
                                 std::string surname)
{
    auto it =  m_customers.find(id);

    if (it != m_customers.end())
    {
        throw std::runtime_error("This ID is already exist");
    }
    else
    {
        m_customers[id] = Customer(id, name, surname);
    }
}

void BankManager::DeleteCustomer(std::string id)
{
    auto it =  m_customers.find(id);

    if (it != m_customers.end())
    {
        m_customers.erase(id);
    }
    else
    {
        throw std::runtime_error("This client does not exist");
    }
}

Customer& BankManager::FindCustomer(std::string id)
{
    auto it =  m_customers.find(id);

    if (it != m_customers.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("This client does not exist");
    }
}

std::shared_ptr<Account> BankManager::FindAccount(const std::string& iban)
{
    auto it =  m_accounts.find(iban);

    if (it != m_accounts.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("This account does not exist");
    }        
}
  void BankManager::CreateSavingsAccount(const std::string& customer_id,
                                const std::string& iban, double interest)
  {
    auto it = m_accounts.find(iban);

    if(it != m_accounts.end())
    {
        throw std::runtime_error("Account already exists");
    }

    auto account_tmp = std::make_shared<SavingsAccount>(iban, interest);

    m_accounts[iban] = account_tmp;

    auto& customer = FindCustomer(customer_id);

    customer.AddAccount(account_tmp);
  }

  void BankManager::CreateCheckingAccount(const std::string& customer_id,
                                const std::string& iban, int overdraft)
  {
    auto it = m_accounts.find(iban);

    if(it != m_accounts.end())
    {
        throw std::runtime_error("Account already exists");
    }

    auto account_tmp = std::make_shared<CheckingAccount>(iban, overdraft);

    m_accounts[iban] = account_tmp;

    auto& customer = FindCustomer(customer_id);

    customer.AddAccount(account_tmp);
  }
void BankManager::CreateAccount(const std::string& customer_id,
                                const std::string& iban)
{
    auto it = m_accounts.find(iban);

    if(it != m_accounts.end())
    {
        throw std::runtime_error("Account already exists");
    }

    auto account_tmp = std::make_shared<Account>(iban);

    m_accounts[iban] = account_tmp;

    auto& customer = FindCustomer(customer_id);

    customer.AddAccount(account_tmp);
}

void BankManager::ApplyInterest(const std::string& iban)
{
    auto it = m_accounts.find(iban);

    if(it != m_accounts.end())
    {
        it->second->ApplyInterest();
    }
    else
    {
        throw std::runtime_error("ERROR:Account does not exist");
    }

}

void BankManager::CloseAccount(const std::string& customer_id,
                               const std::string& iban)
{

    auto it = m_accounts.find(iban);

    if(it != m_accounts.end())
    {
        if(it->second->GetBalance() != 0)
        {
            throw std::runtime_error(
                "ERROR:It can not possible to close an account with positive balance.");
        }
        else
        {
            auto account_tmp = std::make_shared<Account>(iban);
            /* Added deleted account */
            m_deleted_accounts[iban] = it->second;
            /* Deleted account according to IBAN */
            m_accounts.erase(it->first);
            auto& customer = FindCustomer(customer_id);
            if(!customer.DeleteAccount(account_tmp))
            {
                throw std::runtime_error("ERROR. It can not possible to close the requested account.");
            }
        }
    }
    else
    {
        throw std::runtime_error("ERROR:Account not exists");
    }
}

void BankManager::DisplayCustomerAccounts(const std::string& customer_id)
{
    auto& customer = FindCustomer(customer_id);
    customer.DisplayAccounts();
}

void BankManager::Deposit(const std::string& iban, double amount)
{
    auto account = FindAccount(iban);
    (void)account->Deposit(amount);
}

void BankManager::Transfer(std::string origin, std::string destination, double amount)
{
    if(amount <= 0.0)
    {
        throw std::runtime_error("Amount to transfer must be greater than 0€");
    }
        
    auto origin_tmp = FindAccount(origin);
    auto destination_tmp = FindAccount(destination);

    /* Origin account must have at least required amount */
    if(origin_tmp->GetBalance() >= amount)
    {
        if(false == origin_tmp->Withdraw(amount))
        {
            throw std::runtime_error(InsufficientFundsException);
        }
        if(false == destination_tmp->Deposit(amount))
        {
            throw std::runtime_error(InsufficientFundsException);
        }
    }
    else
    {
        std::cout<< origin << " does not have enougth money to transfer." << std::endl;
    }
}

void BankManager::RichestPoorestAccounts(void)
{
    
    /*Convertimos el map en un vector que se pueda recorrer por balance*/
    std::vector<std::pair<std::string, std::shared_ptr<Account>>> sorted_accounts(
    m_accounts.begin(),
    m_accounts.end());

    if(sorted_accounts.empty())
    {
        std::cout << "No accounts available" << std::endl;
        return;
    }

    std::sort
    (
      sorted_accounts.begin(),
      sorted_accounts.end(),
      [](const auto& a, const auto& b)
      { 
          return a.second->GetBalance() > b.second->GetBalance();
      }
    );

    std::cout << "Richest Account:" << std::endl;
    std::cout << sorted_accounts[0].second->GetIBAN() << std::endl;
    std::cout << sorted_accounts[0].second->GetBalance() << std::endl;

    std::cout << "Poorest Account:" << std::endl;
    std::cout << sorted_accounts[sorted_accounts.size() - 1].second->GetIBAN() << std::endl;
    std::cout << sorted_accounts[sorted_accounts.size() - 1].second->GetBalance() << std::endl;
}

void BankManager::AverageAccounts(void)
{
    double accumulate = 0.0;
    if(m_accounts.size() != 0)
    {
        /*Convertimos el map en un vector que se pueda recorrer por balance*/
        for(const auto& account : m_accounts)
        {
            accumulate += account.second->GetBalance();
        }
        auto average = accumulate/m_accounts.size();
        std::cout<< "Average Balance: " << average <<std::endl;
    }
    else
    {
        std::cout << "Average Balance: 0" << std::endl;
    }
}

void BankManager::CustomerWithMoreAccounts(void)
{
    if(m_customers.empty())
    {
        std::cout << "No customers available" << std::endl;
        return;
    }

    size_t number_account = 0;
    std::string customer_name;
    std::string customer_surname;
    
    for(const auto& customer : m_customers)
    {
        if (number_account < customer.second.GetSize())
        {
            number_account = customer.second.GetSize();
            customer_name = customer.second.GetName();
            customer_surname = customer.second.GetSurname();
        }
    }
    std::cout<<"Customer with more accounts:" <<std::endl;
    std::cout<< customer_name <<std::endl;
    std::cout<< customer_surname <<std::endl;
    std::cout<< "(" << number_account <<" accounts)" <<std::endl;

    
}

void BankManager::DisplayStatisticsBank(void)
{
    std::cout<<"====== BANK STATISTICS ======"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Total Customers: " << m_customers.size() <<std::endl;
    std::cout<<"Total Accounts: " << m_accounts.size() <<std::endl;
    double sumaTotal = 0.0;

    for(const auto& account: m_accounts)
    {
        sumaTotal += account.second->GetBalance(); 
    }

    std::cout<<"Total Money Managed: " << sumaTotal <<std::endl;
    RichestPoorestAccounts();
    AverageAccounts();
    CustomerWithMoreAccounts();
    std::cout<<"Total Active Accounts: " << m_accounts.size() <<std::endl;
    std::cout<<"otal Closed Accounts: " << m_deleted_accounts.size() <<std::endl;
}

void BankManager::DisplayNetworthPerClient(void)
{
    
    std::string name, surname;

    for(const auto& customer : m_customers)
    {
        std::cout<< customer.second.GetName() + " " + 
                    customer.second.GetSurname() << std::endl; 
        
        if(customer.second.GetSize() != 0)
        {
            customer.second.DisplayAccounts();
            std::cout<<"Total Networth: " << customer.second.GetNetworth() <<std::endl;
        }
        else
        {
            std::cout<<"This client does not have any account." <<std::endl;
        }
    } 
}

void BankManager::DisplayAverageNetworth(void)
{
    if(m_customers.empty())
    {
        std::cout << "No customer available" << std::endl;
        return;
    }

    std::string name, surname;
    double accumulate = 0.0;
    for(const auto& customer : m_customers)
    {   
        if(customer.second.GetSize() != 0)
        {
            accumulate += customer.second.GetNetworth();
        }
        else
        {
            std::cout<<"This client does not have any account." <<std::endl;
        }
    } 

    std::cout<<"Average Customer Networth: " << (accumulate/m_customers.size()) <<" €" <<std::endl;
}

void BankManager::DisplayTOP3Clients(void)
{
    std::vector<std::pair<std::string, Customer>> TOP3Customer(
    m_customers.begin(),
    m_customers.end());

    if(TOP3Customer.empty())
    {
        std::cout << "No customers available" << std::endl;
        return;
    }

    std::sort
    (
      TOP3Customer.begin(),
      TOP3Customer.end(),
      [](const auto& a, const auto& b)
      { 
          return a.second.GetNetworth() > b.second.GetNetworth();
      }
    );
    
    std::cout<< "---------- TOP 3 Clients ---------- " <<std::endl;

    for (size_t i = 0; i < TOP3Customer.size(); ++i)
    {
        if(i <= 2)
        {
            std::cout << (i + 1) <<". "<<TOP3Customer[i].second.GetName()
                << TOP3Customer[i].second.GetSurname()
                << " "
                << TOP3Customer[i].second.GetNetworth() << " €"
                << std::endl;
        }
    }

}

void BankManager::PoorestClient(void)
{
    std::vector<std::pair<std::string, Customer>> sorted_customer(
    m_customers.begin(),
    m_customers.end());

    if(sorted_customer.empty())
    {
        std::cout << "No customer available" << std::endl;
        return;
    }

    std::sort
    (
      sorted_customer.begin(),
      sorted_customer.end(),
      [](const auto& a, const auto& b)
      { 
          return a.second.GetNetworth() < b.second.GetNetworth();
      }
    );
    // En caso de empata me da igual cual mostrar
    std::cout << "Poorest client in the bank: "<<sorted_customer[0].second.GetName()
        << "    Networth : "
        << sorted_customer[0].second.GetNetworth() << " €"
        << std::endl;
}

void BankManager::RichestClient(void)
{
    std::vector<std::pair<std::string, Customer>> sorted_customer(
    m_customers.begin(),
    m_customers.end());

    if(sorted_customer.empty())
    {
        std::cout << "No customer available" << std::endl;
        return;
    }

    std::sort
    (
      sorted_customer.begin(),
      sorted_customer.end(),
      [](const auto& a, const auto& b)
      { 
          return a.second.GetNetworth() > b.second.GetNetworth();
      }
    );
    // En caso de empata me da igual cual mostrar
    std::cout << "Richest client in the bank: "<<sorted_customer[0].second.GetName()
        << "    Networth : "
        << sorted_customer[0].second.GetNetworth() << " €"
        << std::endl;
}