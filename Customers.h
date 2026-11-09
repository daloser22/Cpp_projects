#include <string>
#include <vector>
#include "Account.h"

class Account;

class Customer
{
public:
    Customer(std::string id, std::string name, std::string surname);

    void DisplayCustomer(void);

    std::string GetId(void) const;

    void AddAccount(std::shared_ptr<Account> account);

    bool DeleteAccount(std::shared_ptr<Account> account);

    void DisplayAccounts() const;

    void DisplayDeletedAccounts() const;

    std::string GetName(void) const;
    
    std::string GetSurname(void) const;

    size_t GetSize(void) const;

    double GetNetworth(void);

private:
    std::string r_id;
    std::string r_name;
    std::string r_surname;
    std::vector<std::shared_ptr<Account>> r_closed_accounts;
    std::vector<std::shared_ptr<Account>> r_accounts;
};