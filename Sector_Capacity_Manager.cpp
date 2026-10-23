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
    std::map<std::string, std::shared_ptr<Aircraft>> aircraftInside;
    
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

    size_t GetOccupancy() const
    {
      return aircraftInside.size();
    }

    int GetCapacity(void) const
    {
      return capacity;
    }

    std::string GetSectorID(void) const
    {
      return name;
    }

    bool HasAircraft(const std::shared_ptr<Aircraft>& track)
    {
      bool retval = false;

      auto it = aircraftInside.find(track->GetCallsign());
      if (it != aircraftInside.end())
      {
        retval = true;
      }

      return retval;
    }

    void EnterAircraft(const std::shared_ptr<Aircraft>& track)
    {
      auto it = aircraftInside.find(track->GetCallsign());

      if (it != aircraftInside.end())
      {
        throw std::runtime_error("Error: This track already exists. You are trying to duplicate this flight :( ");
      }
      else
      {
        aircraftInside[track->GetCallsign()] = track;
      }

      //Mostramos por pantalla la entrada del aircraft
      std::cout<< track->GetCallsign() << " enters " << name << std::endl;
    } 
    
    void ExitAircraft(const std::string& id)
    {
      auto it = aircraftInside.find(id);

      if (it != aircraftInside.end())
      {
        aircraftInside.erase(it);
        //Mostramos por pantalla la salida del aircraft
        std::cout<< id << " leaves " << name << std::endl;
      }
      else
      {
        throw std::runtime_error("Error: Track does not exist in this sector.");
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

    void DisplayAirTrafficSector(void)
    {
      std::cout<< name << std::endl;
      std::cout<< "-------------" << std::endl;
      for(const auto& flight: aircraftInside)
      {
        flight.second->Print();
      }
    }

  };

  class SectorManager
  {
  private:
    std::map<std::string, std::shared_ptr<Sector>> sector_map;
    std::vector<std::string> Record;

  public:
    /* Constructor por defecto, llenaremos el mapa más tarde desde el main */
    SectorManager(void)
    {
      // Do nothing
    }

    void AddSector(const std::shared_ptr<Sector>& sector)
    {
      auto [it, inserted] =
          sector_map.emplace(sector->GetSectorID(), sector);

      if (!inserted)
      {
          throw std::runtime_error("Error: This sector already exists.");
      }
      
      std::cout<< "CREATED SECTOR: " << sector->GetSectorID() << std::endl;
    } 
    
    void RemoveSector(const std::string& id)
    {
      auto it = sector_map.find(id);

      if (it != sector_map.end())
      {
        sector_map.erase(it);
        std::cout<< "DELETED SECTOR: " << id << std::endl;
      }
      else
      {
        throw std::runtime_error("Error: Sector does not exist.");
      }
    }

    void DisplayAirTrafficPerSector(void)
    {
      for(const auto& sector : sector_map)
      {
        sector.second->DisplayAirTrafficSector();
      }
    }

    void DisplayOccupancyPerSector(void)
    {
      for(const auto& sector : sector_map)
      {
        std::cout<< sector.second->GetSectorID() <<
        sector.second->GetOccupancy() << " / "
        << sector.second->GetCapacity() << std::endl;
      }
    }

    void DisplayAirTrafficRecord(void)
    {
      for(const auto& line : Record)
      {
        std::cout<< line <<std::endl;
      }
    }

    void TransferAircraft(const std::shared_ptr<Aircraft>&    track,
                          const std::shared_ptr<Sector>&      ExitSector,
                          const std::shared_ptr<Sector>&      EntrySector)
    {
      if(false != EntrySector->HasAircraft(track))
      {
        throw std::runtime_error("TRANSFER CANCELED. This flight already exists in EntrySector");
      }
      if(false == ExitSector->HasAircraft(track))
      {
        throw std::runtime_error("TRANSFER CANCELED. This flight doesn't exist in ExitSector");
      }

      std::string new_line;
      ExitSector->ExitAircraft(track->GetCallsign());
      EntrySector->EnterAircraft(track);

      std::cout<< "Transfer completed" << std::endl;
      std::cout<< track->GetCallsign() << " moved from "<< ExitSector->GetSectorID() << " to " << EntrySector->GetSectorID() << std::endl;
      
      new_line = "TRANSFER " + track->GetCallsign();
      Record.push_back(new_line);
      new_line = "FROM " + ExitSector->GetSectorID();
      Record.push_back(new_line);
      new_line = "TO " + EntrySector->GetSectorID();
      Record.push_back(new_line);
    }

    void CheckOccupancy(const std::shared_ptr<Sector>& sector_atc)
    {
      if (sector_atc->GetCapacity() < sector_atc->GetOccupancy())
      {
        std::cout<< "**** OVERLOAD DETECTED ****"<< std::endl;
      }
    }
  };
}

/* Enunciado del ejercicio:
 * RF-1 Creación de sectores --> El sistema deberá permitir la existencia de múltiples sectores.
 * RF-2 Entrada de aeronaves --> Una aeronave podrá entrar en un sector específico.
 * RF-3 Salida de aeronaves --> Una aeronave podrá abandonar un sector.
 * RF-4 Transferencia --> Una aeronave podrá ser transferida desde un sector origen a un sector destino.
 */
int main()
{
  atm::SectorManager manager;
  /* ATC sector initialization */
  auto sector1 = std::make_shared<atm::Sector>("VALENCIA_HIGH", 2);
  auto sector2 = std::make_shared<atm::Sector>("VALENCIA_LOW", 5);
  auto sector3 = std::make_shared<atm::Sector>("BARCELONA_HIGH", 1);
  auto sector4 = std::make_shared<atm::Sector>("BARCELONA_LOW", 2);
  
  /* Track initialization */
  auto track1 = std::make_shared<atm::Aircraft>("IBE123",320);
  auto track2 = std::make_shared<atm::Aircraft>("AFR456",500);
  auto track3 = std::make_shared<atm::Aircraft>("RYR999",200);
  auto track4 = std::make_shared<atm::Aircraft>("BAW777",320);
  auto track5 = std::make_shared<atm::Aircraft>("ABC888",400);
  auto track6 = std::make_shared<atm::Aircraft>("DEF666",440);
  auto track7 = std::make_shared<atm::Aircraft>("BYY444",350);
  auto track8 = std::make_shared<atm::Aircraft>("BUU555",390);

  // Ejercicio 1; 2 y 3
  /* RB-1 La aeronave debe existir */
  /* RB-2 La aeronave debe estar en el sector origen */
  /* RB-3 El sector destino no puede contener ya la aeronave */

  sector1->EnterAircraft(track1);
  sector1->EnterAircraft(track2);
  sector2->EnterAircraft(track3);
  sector2->EnterAircraft(track4);
  sector3->EnterAircraft(track5);
  sector3->EnterAircraft(track6);
  sector4->EnterAircraft(track7);
  sector4->EnterAircraft(track8);

  manager.AddSector(sector1);
  manager.AddSector(sector2);
  manager.AddSector(sector3);
  manager.AddSector(sector4);

  manager.TransferAircraft(track4, sector2, sector4);
  /* Ejercicio 4 Comprobar la capacidad de los sectores */
  manager.CheckOccupancy(sector2);
  manager.CheckOccupancy(sector4);
  /* Ejercicio 5 Registrar las transferencias */
  manager.DisplayAirTrafficRecord();
  /* Ejercicio 6 - Mostrar trafico de todos los sectores */
  manager.DisplayAirTrafficPerSector();
  /* Ejercicio 7 - Mostrar ocupación de todos los sectores */
  manager.DisplayOccupancyPerSector();

  std::cin.get();
  return 0;
}