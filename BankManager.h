#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "Customers.h"
#include <chrono>

class BankManager
{
public:
  void CreateCustomer(std::string id, std::string name, std::string surname);

  void DeleteCustomer(std::string id);

  Customer& FindCustomer(std::string id);

  std::shared_ptr<Account> FindAccount(const std::string& iban);

  void CreateAccount(const std::string& customer_id,
    const std::string& iban);

  void CloseAccount(const std::string& customer_id, const std::string& iban);

  void DisplayCustomerAccounts(const std::string& customer_id);

  void Deposit(const std::string& iban, double amount);

  void Transfer(std::string origin, std::string destination, double amount);

  void DisplayStatisticsBank(void);

  void RichestPoorestAccounts(void);

  void AverageAccounts(void);

  void CustomerWithMoreAccounts(void);

  void DisplayNetworthPerClient(void);

  void DisplayTOP3Clients(void);

  void DisplayAverageNetworth(void);

  void PoorestClient(void);

  void RichestClient(void);

  void ApplyInterest(const std::string& iban);

  void CreateSavingsAccount(const std::string& customer_id,
                                const std::string& iban, double interest);

  void CreateCheckingAccount(const std::string& customer_id,
                                const std::string& iban, int overdraft);

private:
    std::map<std::string, Customer> m_customers;
    std::map<std::string, std::shared_ptr<Account>> m_accounts;
    std::map<std::string, std::shared_ptr<Account>> m_deleted_accounts;
};