#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <set>

class Transaction
{
private:
    int id;
    std::string description;
    double amount;
    std::string category;
    std::string date;
public:
  Transaction(int id, std::string description, double amount,
              std::string category, std::string date):
              id(id), description(description), amount(amount),
              category(category), date(date)
  {
    if(amount == 0)
    {
      throw std::runtime_error("ERROR: Transaction with value 0 is not possible ");
    }
    
    if(description == "")
    {
      throw std::runtime_error("ERROR: Transaction with empty description is not possible ");
    }
    
    if(category == "")
    {
      throw std::runtime_error("ERROR: Transaction with empty category is not possible ");
    }
  }

  virtual ~Transaction()
  {
    std::cout<< "transaction destroyed" << std::endl;
  }

  int GetId(void) const
  {
    return id;
  }

  std::string GetDescription(void) const
  {
    return description;
  }

  std::string GetCategory(void) const
  {
    return category;
  }

  double GetAmount(void) const
  {
    return amount;
  }
};

class Budget
{
private:
    std::string category;
    double maximum;
public:
    Budget(std::string category, double maximum):
    category(category), maximum(maximum)
    {

    }

    virtual ~Budget()
    {

    }

    std::string GetCategory(void) const
    {
      return category;
    }
    
    double GetMaximum(void) const
    {
      return maximum;
    }
};

class FinanceManager
{
private:
  std::map<int, std::shared_ptr<Transaction>> transaction_list;
  std::map<std::string , double> incomes_list;
  std::map<std::string , double> expenses_list;
public:
  void AddTransaction(const std::shared_ptr<Transaction>& transaction)
  {
    auto it = transaction_list.find(transaction->GetId());

    if (it != transaction_list.end())
    {
      throw std::runtime_error("ERROR: This transaction is duplicated");
    }

    transaction_list[transaction->GetId()] = transaction;
    
    if (transaction->GetCategory() != "Income")
    {
      expenses_list[transaction->GetCategory()] += transaction->GetAmount();
    }
    else
    {
      incomes_list[transaction->GetCategory()] += transaction->GetAmount();
    }
  }

  void RemoveTransaction(int id)
  {
    auto it = transaction_list.find(id);

    if (it != transaction_list.end())
    {
      auto transaction = it->second;
      transaction_list.erase(id);
      expenses_list.erase(transaction->GetCategory());
      incomes_list.erase(transaction->GetCategory());
      std::cout<< "Deleted transaction" <<std::endl;
    }
    else
    {
      std::cout<< "Indicated transaction does not exist " <<std::endl;
    }
  }

  void DisplayAllTransactions(void)
  {
    std::string sign;

    if(transaction_list.size() < 1)
    {
      std::cout<< "Transaction list is empty" <<std::endl;
    }
    else
    {
      std::cout<< "ID  Description      Amount" <<std::endl;
      std::cout<< "--------------------------------" <<std::endl;
      for(auto const& transaction: transaction_list)
      {
        if("Income" != transaction.second->GetCategory())
        {
          sign = "-";
        }
        else
        {
          sign = "+";
        }
        
        std::cout<< transaction.second->GetId() << "  " << transaction.second->GetCategory()  << "     " << sign << transaction.second->GetAmount() <<std::endl;
      }
    }
  }

  void DisplayCurrentBalance(void)
  {
    double incomes = std::accumulate(
        incomes_list.begin(),
        incomes_list.end(),
        0.0,
        [](double total, const auto& item)
        {
            return total + item.second;
        });

    double expenses = std::accumulate(
        expenses_list.begin(),
        expenses_list.end(),
        0.0,
        [](double total, const auto& item)
        {
            return total + item.second;
        });

    double balance = incomes - expenses;

    std::cout << "Income: " << incomes << std::endl;
    std::cout << "Expenses: " << expenses << std::endl;
    std::cout << "Balance: " << balance << std::endl;
  }
  
  void DisplayTotalExpenses(void)
  {
    if (expenses_list.empty())
    {
        std::cout << "No expenses available." << std::endl;
        return;
    }

    double total_expense = std::accumulate(
    expenses_list.begin(),
    expenses_list.end(),
    0.0,
    [](double total, const auto& item)
    {
        return total + item.second;
    });
    
    std::cout<<"total expenses : " << total_expense << " €" <<std::endl;   
  }

  void DisplayTopExpense(void)
  {
    if (expenses_list.empty())
    {
        std::cout << "No expenses available." << std::endl;
        return;
    }
    auto it = std::max_element(
    expenses_list.begin(),
    expenses_list.end(),
    [](const auto& a, const auto& b)
    {
        return a.second < b.second;
    });

    std::cout<<"Largest Expense: " <<std::endl;
    std::cout<< it->first <<std::endl;
    std::cout<< it->second << " €" <<std::endl;
  }

  void DisplayTotalIncomes(void)
  {
    if (incomes_list.empty())
    {
        std::cout << "No incomes available." << std::endl;
        return;
    }

    double total_income = std::accumulate(
    incomes_list.begin(),
    incomes_list.end(),
    0.0,
    [](double total, const auto& item)
    {
        return total + item.second;
    });
    
    std::cout<<"total incomes : " << total_income << " €" <<std::endl;   
  }

  void DisplayTopIncome(void)
  {
    if (incomes_list.empty())
    {
        std::cout << "No incomes available." << std::endl;
        return;
    }
    
    auto it = std::max_element(
    incomes_list.begin(),
    incomes_list.end(),
    [](const auto& a, const auto& b)
    {
        return a.second < b.second;
    });

    std::cout<<"Highest Income: " <<std::endl;
    std::cout<< it->first <<std::endl;
    std::cout<< it->second << " €" <<std::endl;
  }

  void DisplayAverageExpense(void)
  {
    double avg_expense = 0;

    double total_expense = std::accumulate(
    expenses_list.begin(),
    expenses_list.end(),
    0.0,
    [](double total, const auto& item)
    {
        return total + item.second;
    });

    avg_expense = total_expense/expenses_list.size();
    std::cout<<"Average Expense: " <<std::endl;
    std::cout<< avg_expense << " €" << std::endl;
  }

  void DisplayTop3Expenses(void)
  {
    if (expenses_list.empty())
    {
        std::cout << "No expenses available." << std::endl;
        return;
    }

    /* Copiamos el contenido del map a un vector */
    std::vector<std::pair<std::string, double>> sorted_expenses(
        expenses_list.begin(),
        expenses_list.end());
    /* Ordeamos el vector por importe */
    std::sort(
        sorted_expenses.begin(),
        sorted_expenses.end(),
        [](const auto& a, const auto& b)
        {
            return a.second > b.second;
        });
    
    int i = 1;
     /* Mostramos los 3 primeros */
    for(const auto& expense : sorted_expenses)
    {
      /* Mostramos solo los TOP 3 */
      std::cout<< i << ". " << expense.first << "   " << expense.second <<std::endl;
      i++;

      if(i > 3)
      {
        break;
      }
    }
  }

  void DisplayExpensesPerCategory(void)
  {
    for(const auto& index: expenses_list)
    {
      std::cout<< index.first << "    " << index.second <<std::endl;
    }
  }

  void CheckBudgetOverstimated(const std::vector<Budget>& budget)
  {
    for(const auto& it1 : budget)
    {
      for(const auto& it2 : expenses_list)
      {
        if(it1.GetCategory() == it2.first)
        {
          if (it1.GetMaximum() < it2.second)
          {
            double exceed = it2.second - it1.GetMaximum(); 
            std::cout<< "BUDGET ALERT"<<std::endl;
            std::cout<< "Category: " << it1.GetCategory() <<std::endl;
            std::cout<< "Exceeded by "<< exceed <<std::endl;
          }
        }
      }
    }
  }
};

int main()
{ 
  FinanceManager manager;
  std::vector<Budget> budget;

  Budget element1("Food", 400);
  Budget element2("Entertainment", 100);
  
  budget.push_back(element1);
  budget.push_back(element2);

  auto mov1 = std::make_shared<Transaction>(1, "Salary", 2500, "Income", "2026-11-06");
  auto mov2 = std::make_shared<Transaction>(2, "Mercadona", 85, "Food", "2026-10-05");
  auto mov3 = std::make_shared<Transaction>(3, "Netflix", 50, "Entertainment", "2026-10-01");
  auto mov4 = std::make_shared<Transaction>(4, "Gym", 30, "Entertainment", "2026-09-05");
  auto mov5 = std::make_shared<Transaction>(5, "ETF S&P500", 400, "Income", "2025-10-01");

  manager.AddTransaction(mov1);
  manager.AddTransaction(mov2);
  manager.AddTransaction(mov3);
  manager.AddTransaction(mov4);
  manager.AddTransaction(mov5);
  manager.RemoveTransaction(2);

  manager.DisplayAllTransactions();
  manager.DisplayCurrentBalance();
  manager.DisplayTopIncome();
  manager.DisplayTopExpense();
  manager.DisplayAverageExpense();
  manager.CheckBudgetOverstimated(budget);
  manager.DisplayExpensesPerCategory();
  manager.DisplayTop3Expenses();


  std::cin.get();
  return 0;
}