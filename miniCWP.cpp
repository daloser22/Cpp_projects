#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace atm
{
  class AircraftTrack
  {
  private:
    std::string callsign;
    double latitude;
    double longitude;
    int flightLevel;
    double speed;
    double heading;

  public:
    AircraftTrack(std::string callsign, double latitude, double longitude, int flightLevel, double speed, double heading):
    callsign(callsign),
    latitude(latitude),
    longitude(longitude),
    flightLevel(flightLevel),
    speed(speed),
    heading(heading)
    {
    }

    virtual ~AircraftTrack()
    {
      std::cout<<"DESTROYED Aircraft"<<std::endl; 
    }

    std::string GetCallsign(void) const
    {
      return callsign;
    }

    double GetLatitude(void) const
    {
      return latitude;
    }

    double GetLongitude(void) const
    {
      return longitude;
    }

    void SetLatitude(double new_lat) 
    {
      latitude = new_lat;
    }

    void SetLongitude(double new_long) 
    {
      longitude = new_long;
    }

    int GetFlightLevel(void) const
    {
      return flightLevel;
    }

    double GetSpeed(void) const
    {
      return speed;
    }

    double GetHeading(void) const
    {
      return heading;
    }

    virtual void Print() const
    {
      std::cout<< callsign <<std::endl;
      std::cout<< "Position:" << latitude << " , " << longitude <<std::endl;
      std::cout<< "FL: " << flightLevel <<std::endl;
      std::cout<< "Speed: " << speed <<std::endl;
      std::cout<< "Heading: " << heading <<std::endl;
    }
  };

  class TrackManager
  {
  private:
    std::map<std::string, std::shared_ptr<AircraftTrack>> tracks;

  public:

    void AddTrack(const std::shared_ptr<AircraftTrack>& track)
    {
      auto it = tracks.find(track->GetCallsign());

      if (it != tracks.end())
      {
        throw std::runtime_error("Error: This track already exists.");
      }
      else
      {
        tracks[track->GetCallsign()] = track;
      }
    } 

    void RemoveTrack(const std::string& id)
    {
      auto it = tracks.find(id);

      if (it != tracks.end())
      {
        tracks.erase(it);
      }
      else
      {
        throw std::runtime_error("Error: Track does not exist.");
      }
      
    }

    std::shared_ptr<AircraftTrack> FindTrack(const std::string& id)
    {
      std::shared_ptr<AircraftTrack> ptr;

      auto it = tracks.find(id);
      
      if (it != tracks.end())
      {
        ptr = it->second;
      }
      else
      {
        ptr = nullptr;
      }

      return ptr; 
    }

    void DisplayAllTracks(void)
    {
      for(const auto& track : tracks)
      {
        track.second->Print();
      }
    }

    /* Funcion to show just Flights Above minFL input */
    void DisplayTracksAboveFL(int minFL)
    {
      for(const auto& track : tracks)
      {
        if(track.second->GetFlightLevel() > minFL)
        {
          track.second->Print();
        }
      }
    }

    void DisplayFastAircraft(void)
    {
      std::for_each
      (tracks.begin(),
      tracks.end(),
      [](const auto& track)
      {
        /* Just show aircrafts faster than 450km/h*/
        if(track.second->GetSpeed() > 450)
        {
          track.second->Print();
        } 
      });
    }

    void UpdateTrackPosition(std::string id, double new_lat, double new_long)
    {
      auto it = tracks.find(id);

      if(it!= tracks.end())
      {
        it->second->SetLatitude(new_lat);
        it->second->SetLongitude(new_long);
      }
    }

    void CheckConflict(const std::string& track1, const std::string& track2)
    {
      auto it1 = tracks.find(track1);
      auto it2 = tracks.find(track2);
      
      if(it1== tracks.end() || it2 == tracks.end())
      {
        throw std::runtime_error("Track not found");
      }
      else
      {
        double lat1, lat2, long1, long2;

        lat1 = it1->second->GetLatitude();
        long1 = it1->second->GetLongitude();
        lat2 = it2->second->GetLatitude();
        long2 = it2->second->GetLongitude();

        if((abs(lat1 - lat2) < 0.05) && (abs(long1 - long2) < 0.05))
        {
          std::cout << "*** STCA ALERT ***" << std::endl;
          std::cout << track1 <<" and " << track2 <<std::endl;
        }
        else
        {
          std::cout << "No conflict detected" << std::endl;
        }
      }
    }
  };
}

/* Enunciado del ejercicio:
 * Crear los vuelos --> IBE123; AFR456; RYR999 y BAW777;
 * 1- Añádelos al TrackManager.
 * 2- Muestra todos los tracks.
 * 3- Busca uno por callsign.
 * 4- Actualiza una posición.
 * 5- Muestra aeronaves por encima de FL300.
 * 6- Muestra aeronaves rápidas.
 * 7- Ejecuta una comprobación STCA.
 * 8- Elimina una aeronave.
 * 9- Vuelve a mostrar el listado final.
 */
int main()
{
  atm::TrackManager Manager;
  std::shared_ptr<atm::AircraftTrack> ptr;
  auto track1 = std::make_shared<atm::AircraftTrack>("IBE123",39.55,-0.31,320,480,50);
  auto track2 = std::make_shared<atm::AircraftTrack>("AFR456",45.55,-0.80,500,420,60);
  auto track3 = std::make_shared<atm::AircraftTrack>("RYR999",85.55,-0.89,200,250,30);
  auto track4 = std::make_shared<atm::AircraftTrack>("BAW777",39.55,-0.31,320,480,50);

  // Ejercicio 1
  Manager.AddTrack(track1);
  Manager.AddTrack(track2);
  Manager.AddTrack(track3);
  Manager.AddTrack(track4);
  // Ejercicio 2
  Manager.DisplayAllTracks();
  // Ejercicio 3
  ptr = Manager.FindTrack("IBE123");
  if(ptr)
  { 
    // Checkeamos que no sea nullptr
    ptr->Print();
  }
  // Ejercicio 4
  Manager.UpdateTrackPosition("RYR999", 55.55, 0.66);
  // Ejercicio 5
  Manager.DisplayTracksAboveFL(300);
  // Ejercicio 6
  Manager.DisplayFastAircraft();
  // Ejercicio 7
  Manager.CheckConflict("IBE123","BAW777");
  // Ejercicio 8
  Manager.RemoveTrack("RYR999");
  // Ejercicio 9
  Manager.DisplayAllTracks();
  std::cin.get();
  return 0;
}