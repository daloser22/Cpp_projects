#include <iostream>
#include <limits>

#include "BankManager.h"

void ShowMenu()
{
    std::cout << "\n================================\n";
    std::cout << "     BANK MANAGEMENT SYSTEM\n";
    std::cout << "================================\n";

    std::cout << "1. Create Customer\n";
    std::cout << "2. Create Savings Account\n";
    std::cout << "3. Create Checking Account\n";
    std::cout << "4. Deposit\n";
    std::cout << "5. Transfer\n";

    std::cout << "\n--- Customer Information ---\n";
    std::cout << "6. Display Customer Accounts\n";
    std::cout << "7. Display Customer Net Worth\n";

    std::cout << "\n--- Statistics ---\n";
    std::cout << "8. Bank Statistics\n";
    std::cout << "9. Top 3 Customers\n";
    std::cout << "10. Richest Customer\n";
    std::cout << "11. Poorest Customer\n";
    std::cout << "12. Average Net Worth\n";

    std::cout << "\n--- Savings Accounts ---\n";
    std::cout << "13. Apply Interest\n";

    std::cout << "\n--- Accounts ---\n";
    std::cout << "14. Close Account\n";

    std::cout << "\n0. Exit\n";

    std::cout << "\nSelect an option: ";
}


int main()
{
    BankManager manager;

    bool running = true;

    while(running)
    {
        try
        {
            ShowMenu();

            int option;
            std::cin >> option;

            switch(option)
            {
                case 1:
                {
                    std::string id;
                    std::string name;
                    std::string surname;

                    std::cout << "ID: ";
                    std::cin >> id;

                    std::cout << "Name: ";
                    std::cin >> name;

                    std::cout << "Surname: ";
                    std::cin >> surname;

                    manager.CreateCustomer(
                        id,
                        name,
                        surname);

                    break;
                }

                case 2:
                {
                    std::string customerId;
                    std::string iban;
                    double interest;

                    std::cout << "Customer ID: ";
                    std::cin >> customerId;

                    std::cout << "IBAN: ";
                    std::cin >> iban;

                    std::cout << "Interest (%): ";
                    std::cin >> interest;

                    manager.CreateSavingsAccount(
                        customerId,
                        iban,
                        interest);

                    break;
                }

                case 3:
                {
                    std::string customerId;
                    std::string iban;
                    int overdraft;

                    std::cout << "Customer ID: ";
                    std::cin >> customerId;

                    std::cout << "IBAN: ";
                    std::cin >> iban;

                    std::cout << "Overdraft Limit: ";
                    std::cin >> overdraft;

                    manager.CreateCheckingAccount(
                        customerId,
                        iban,
                        overdraft);

                    break;
                }

                case 4:
                {
                    std::string iban;
                    double amount;

                    std::cout << "IBAN: ";
                    std::cin >> iban;

                    std::cout << "Amount: ";
                    std::cin >> amount;

                    manager.Deposit(
                        iban,
                        amount);

                    break;
                }

                case 5:
                {
                    std::string origin;
                    std::string destination;
                    double amount;

                    std::cout << "Origin IBAN: ";
                    std::cin >> origin;

                    std::cout << "Destination IBAN: ";
                    std::cin >> destination;

                    std::cout << "Amount: ";
                    std::cin >> amount;

                    manager.Transfer(
                        origin,
                        destination,
                        amount);

                    break;
                }

                case 6:
                {
                    std::string customerId;

                    std::cout << "Customer ID: ";
                    std::cin >> customerId;

                    manager.DisplayCustomerAccounts(
                        customerId);

                    break;
                }

                case 7:
                {
                    manager.DisplayNetworthPerClient();
                    break;
                }

                case 8:
                {
                    manager.DisplayStatisticsBank();
                    break;
                }

                case 9:
                {
                    manager.DisplayTOP3Clients();
                    break;
                }

                case 10:
                {
                    manager.RichestClient();
                    break;
                }

                case 11:
                {
                    manager.PoorestClient();
                    break;
                }

                case 12:
                {
                    manager.DisplayAverageNetworth();
                    break;
                }

                case 13:
                {
                    std::string iban;

                    std::cout << "Savings Account IBAN: ";
                    std::cin >> iban;


                    manager.ApplyInterest(iban);

                    break;
                }

                case 14:
                {
                    std::string iban;

                    std::cout << "IBAN: ";
                    std::cin >> iban;
                    std::string customerId;

                    std::cout << "Customer ID: ";
                    std::cin >> customerId;
                    manager.CloseAccount(customerId, iban);

                    break;
                }

                case 0:
                {
                    running = false;
                    break;
                }

                default:
                {
                    std::cout
                        << "Invalid option"
                        << std::endl;
                    break;
                }
            }
        }
        catch(const std::exception& ex)
        {
            std::cout
                << "\nERROR: "
                << ex.what()
                << std::endl;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');
    }

    return 0;
}