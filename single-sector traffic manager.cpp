 #include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace atm
{
  class Aircraft
  {
  private:
    std::string callsign;
    int flightLevel;

  public:
    Aircraft(std::string callsign, int flightLevel):
    callsign(callsign),
    flightLevel(flightLevel)
    {
    }

    virtual ~Aircraft()
    {
      std::cout<<"DESTROYED Aircraft"<<std::endl; 
    }

    std::string GetCallsign(void) const
    {
      return callsign;
    }

    int GetFlightLevel(void) const
    {
      return flightLevel;
    }

    virtual void Print() const
    {
      std::cout<< callsign <<std::endl;
      std::cout<< "FL: " << flightLevel <<std::endl;
    }
  };

  class Sector
  {
  private:
    std::string name;
    int capacity;
    
  public:
    Sector(std::string name, int capacity):
    name(name),
    capacity(capacity)
    {
    }
    
    ~Sector()
    {
      std::cout<<"DESTROYED Sector"<<std::endl; 
    }

    int GetCapacity(void) const
    {
      return capacity;
    }
  };

  class SectorManager
  {
  private:
    std::map<std::string, std::shared_ptr<Aircraft>> aircraftInside;
    std::vector<std::string> Record;

  public:
    /* Constructor por defecto, llenaremos el mapa más tarde desde el main */
    SectorManager(void)
    {
      // Do nothing
    }

    void EnterAircraft(const std::shared_ptr<Aircraft>& track)
    {
      auto it = aircraftInside.find(track->GetCallsign());

      if (it != aircraftInside.end())
      {
        throw std::runtime_error("Error: This track already exists.");
      }
      else
      {
        aircraftInside[track->GetCallsign()] = track;
      }

      //Mostramos por pantalla la entrada del aircraft
      std::cout<< track->GetCallsign() << " entered sector" << std::endl;
      std::string new_line;
      new_line = "ENTER " + track->GetCallsign(); 
      Record.push_back(new_line);
    } 
    
    void ExitAircraft(const std::string& id)
    {
      std::string new_line;
      auto it = aircraftInside.find(id);

      if (it != aircraftInside.end())
      {
        aircraftInside.erase(it);
        //Mostramos por pantalla la salida del aircraft
        std::cout<< id << " left sector" << std::endl;
        new_line = "EXIT " + id; 
        Record.push_back(new_line);
      }
      else
      {
        throw std::runtime_error("Error: Track does not exist.");
      }
    }

    void CheckTrafficOverload(const Sector& atc_sector)
    {
      int capacity = atc_sector.GetCapacity();
      
      if(capacity < aircraftInside.size())
      {
        std::cout<< "*** SECTOR OVERLOAD ***" << std::endl;
        std::cout<< "Occupancy: " << aircraftInside.size() << "/" << capacity << std::endl;
      }
    }

    void DisplayCurrentTraffic(void)
    {
      std::cout<< "Aircraft currently inside:"<<std::endl;

      for(const auto& track : aircraftInside)
      {
        track.second->Print();
      }
    }

    void Print(const Sector& sector_atc) const
    {
      std::cout<< "Current occupancy: " << aircraftInside.size() << " / " << sector_atc.GetCapacity() <<std::endl;
    }

    void DisplayHighestAircraft(void)
    {
      auto highest_fl = std::max_element(
        aircraftInside.begin(),
        aircraftInside.end(),
        [](const auto&a, const auto&b)
        {
          return a.second->GetFlightLevel() < b.second->GetFlightLevel();
        }
      );

      std::cout<< "Highest aircraft: " <<std::endl;
      highest_fl->second->Print();
    }

    void DisplayAverageFL(void)
    {
      int avg = 0;
      int accumulate = 0;
      
      if (aircraftInside.empty())
      {
        std::cout << "No aircraft inside" << std::endl;
        return;
      }
      
      std::for_each
      (aircraftInside.begin(),
      aircraftInside.end(),
      [&](const auto& aircraft)
      { 
        accumulate += aircraft.second->GetFlightLevel();
      });
      
      avg = accumulate / aircraftInside.size();
      
      std::cout<< "Average FL: " << avg <<std::endl;
    }

    void DisplayAirTraffictRecord(void)
    {
      for(const auto& line : Record)
      {
        std::cout<< line <<std::endl;
      }
    }
  };
}

/* Enunciado del ejercicio:
 * Crear los vuelos --> IBE123; AFR456; RYR999 y BAW777;
 * 1- Aircraft Enter Sector.
 * 2- Aircraft Exit Sector.
 * 3- Mostrar tráfico actual.
 * 4- Mostrar ocupación.
 * 5- Detectar overload.
 * 6- Mostrar aeronave más alta.
 * 7- Mostrar FL medio.
 * 8- Evitar errores.
 * 9- Historial de movimientos.
 */
int main()
{
  atm::SectorManager manager;
  /* Initiallization of ATC sector */
  atm::Sector sector_atc("VALENCIA_HIGH", 2);
  
  auto track1 = std::make_shared<atm::Aircraft>("IBE123",320);
  auto track2 = std::make_shared<atm::Aircraft>("AFR456",500);
  auto track3 = std::make_shared<atm::Aircraft>("RYR999",200);
  auto track4 = std::make_shared<atm::Aircraft>("BAW777",320);
  
  // Ejercicio 1
  manager.EnterAircraft(track2);
  manager.EnterAircraft(track3);
  manager.EnterAircraft(track4);
  manager.EnterAircraft(track1);
  // Ejercicio 8 --> track 1 ya existe
  //manager.EnterAircraft(track1, sector_atc);
  // Ejercicio 2
  manager.ExitAircraft("AFR456");
  // Ejercicio 3
  manager.DisplayCurrentTraffic();
  // Ejercicio 4
  manager.Print(sector_atc);
  // Ejercicio 5
  manager.CheckTrafficOverload(sector_atc);
  // Ejercicio 6
  manager.DisplayHighestAircraft();
  // Ejercicio 7
  manager.DisplayAverageFL();
  // Ejercicio 9
  manager.DisplayAirTraffictRecord();
  std::cin.get();
  return 0;
}