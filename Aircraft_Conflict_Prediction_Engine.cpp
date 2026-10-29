#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <set>
#include <cmath>

class Aircraft
{
private:
  std::string callsign;
  double x;
  double y;
  int FlightLevel;
  int speed;
  int heading;
public:
  
  Aircraft(std::string callsign, double x, double y, int FlightLevel, int speed,int heading):
  callsign(callsign),
  x(x),
  y(y),
  FlightLevel(FlightLevel),
  speed(speed),
  heading(heading)
  {
  }

  virtual ~Aircraft()
  {
  }

  virtual void Print(void) const
  {
    std::cout<< callsign << std::endl;
    std::cout<< "Position: (" << x << "," << y << ")" << std::endl;
    std::cout<< "FL: FL" << FlightLevel << std::endl;
    std::cout<< "Speed: " << speed << "kt" << std::endl;
  }

  std::string GetCallsign(void) const
  {
    return callsign;
  }

  int GetFlightLevel(void) const
  {
    return FlightLevel;
  }

  void SetFlightLevel(int fl) 
  {
    this->FlightLevel = fl;
  }  

  int GetSpeed(void) const
  {
    return speed;
  }

  void SetSpeed(int speed)
  {
     this->speed = speed;
  }

  double GetXPosition(void) const
  {
    return x;
  }

  void SetXPosition(double x)
  {
     this->x = x;
  }

  double GetYPosition(void) const
  {
    return y;
  }

  void SetYPosition(double y)
  {
     this->y = y;
  }

  int GetHeading(void) const
  {
    return heading;
  }

  void SetHeading(int heading) 
  {
    this->heading = heading;
  } 

  double CalculateTravelDistance(int minutes) const
  {
    
    double hours = minutes / 60.0;
    double distance = speed * hours;
    
    return distance;
  }
};

class Conflict
{
private:
    std::string aircraftA;
    std::string aircraftB;
    double distance;
public:

  void SetDistance(double distance)
  {
    this->distance = distance;
  }

  void SetAircraftA(std::string a)
  {
    aircraftA = a;
  }
  
  void SetAircraftB(std::string b)
  {
    aircraftB = b;
  }

  double GetDistance(void) const
  {
    return distance;
  }

  std::string GetAircraftA(void) const
  {
    return aircraftA;
  }
  
  std::string GetAircraftB(void) const
  {
    return aircraftB;
  }
};

class ConflictManager
{
private:
  std::map<std::string, std::shared_ptr<Aircraft>> tracks;
  std::map<std::string, Aircraft> future_tracks;
  std::vector<Conflict> ConflictList;
public:

  void AddTrack(const std::shared_ptr<Aircraft>& track)
  {
    auto it = tracks.find(track->GetCallsign());
    
    if (it != tracks.end())
    {
        throw std::runtime_error("Track already exists.");
    }
    
    tracks[track->GetCallsign()] = track;
  }

  void RemoveTrack(const std::shared_ptr<Aircraft>& track)
  {
    auto it = tracks.find(track->GetCallsign());
    
    if (it != tracks.end())
    {
       tracks.erase(track->GetCallsign());
    }
    else
    {
      throw std::runtime_error("Track doesn't exist.");
    } 
  }

  void DisplayAllTraffic(void)
  {
    for(const auto& track : tracks)
    {
      track.second->Print();
    }
  }

  void FindClosestAircraft(void)
  {
    double distance_tmp;

    double closest_distance = std::numeric_limits<double>::max();
    std::string aircraft_a;
    std::string aircraft_b;

    /* First of all we need to see if there is more than 1 flight in tracks */
    if (tracks.size() < 2)
    {
      throw std::runtime_error("Tracks must be at least bigger than 1 tracks");
    }
    
    for (auto it = tracks.begin(); it != tracks.end(); ++it)
    {
      auto it2 = std::next(it);

      for (; it2 != tracks.end(); ++it2)
      {
        distance_tmp = CalculateDistance(*it->second, *it2->second);
        
        if(distance_tmp < closest_distance)
        {
          /* Updated aircraft pair with the minimum distance */
          closest_distance = distance_tmp;
          aircraft_a = it->second->GetCallsign();
          aircraft_b = it2->second->GetCallsign();
        }
      }
    }

    /* Display the result of minimum distance between pairs */
    std::cout << "Closest aircraft pair:" <<std::endl;
    std::cout << aircraft_a <<std::endl;
    std::cout << aircraft_b <<std::endl;
    std::cout << "Distance: " << closest_distance << " NM" <<std::endl;
  }

  void DisplayFutureClosestPairs(double max_distance)
  {
    double distance_tmp;
    std::map<std::string, double> closest_map;
    double closest_distance = std::numeric_limits<double>::max();
    std::string aircraft_a;
    std::string aircraft_b;

    /* First of all we need to see if there is more than 1 flight in tracks */
    if (future_tracks.size() < 2)
    {
      throw std::runtime_error("Tracks must be at least bigger than 1 tracks");
    }
    
    for (auto it = future_tracks.begin(); it != future_tracks.end(); ++it)
    {
      auto it2 = std::next(it);

      for (; it2 != future_tracks.end(); ++it2)
      {
        distance_tmp = CalculateDistance(it->second, it2->second);
        
        if(distance_tmp < max_distance)
        {
          /* Updated aircraft pair with the minimum distance */
          aircraft_a = it->second.GetCallsign();
          aircraft_b = it2->second.GetCallsign();
          std::string index = aircraft_a +" - " + aircraft_b; 
          closest_map[index] = distance_tmp;
        }
      }
    }

    /* Copiamos el contenido del map a un vector, para ordenarlos despues.
     * Un map tiene iteradores bidireccionales 'pair', no puedes ordenar un map a tu gusto
     * ya que se ordena por clave, por tanto hemos de usar una copia vector y luego aplicar un 
     * lamba sort para ordenar como nosotros queramos.
     */
    std::vector<std::pair<std::string, double>> sorted_distances(
        closest_map.begin(),
        closest_map.end());
        
    std::sort(
        sorted_distances.begin(),
        sorted_distances.end(),
        [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });

    int i = 1;
    /* Display TOP 3 */
    for(const auto& item : sorted_distances)
    {
      std::cout<< i << ". " << item.first << "   " << item.second << " NM" <<std::endl;
      i++;

      if(i > 3)
      {
        break;
      }
    }
  }

  void FindAllPotentialConflicts(int separation_min, int distance_min)
  {
    double distance;
    int separation;
    Conflict conflict;

    /* First of all we need to see if there is more than 1 flight in tracks */
    if (tracks.size() < 2)
    {
      throw std::runtime_error("Tracks must be at least bigger than 1 tracks");
    }

    /* Init the conflict list */
    ConflictList.clear();
    
    for (auto it = tracks.begin(); it != tracks.end(); ++it)
    {
      auto it2 = std::next(it);

      for (; it2 != tracks.end(); ++it2)
      {
        distance = CalculateDistance(*it->second, *it2->second);

        if(it->second->GetFlightLevel() > it2->second->GetFlightLevel())
        {
          separation = (it->second->GetFlightLevel() - it2->second->GetFlightLevel()) * 100;
        }
        else
        {
          separation = (it2->second->GetFlightLevel() - it->second->GetFlightLevel()) * 100; 
        }

        if(((distance) < distance_min) && (separation < separation_min))
        {          
          conflict.SetAircraftA(it->second->GetCallsign());
          conflict.SetAircraftB(it2->second->GetCallsign());
          conflict.SetDistance(distance);

          /* Solo añadimos una vez el conflicto si se repite en proximos minutos */
          for(const auto& item: ConflictList)
          {
            if((item.GetAircraftA() != it->second->GetCallsign()) && (item.GetAircraftB() != it2->second->GetCallsign()))
            {
              ConflictList.push_back(conflict);
            }
          }

          std::cout<< "Potencial conflict: " << std::endl;
          std::cout<< it->second->GetCallsign() <<std::endl;
          std::cout<< it2->second->GetCallsign() <<std::endl;
          std::cout <<  "Lateral: " << distance << " NM" << std::endl;
          std::cout <<  "Vertical: " << separation << " ft" << std::endl;
        }
      }
    }
  }

  void FindAllFuturePotentialConflicts(int separation_min, int distance_min, int minutes)
  {
    double distance;
    int separation;
    
    /* First of all we need to see if there is more than 1 flight in tracks */
    if (future_tracks.size() < 2)
    {
      throw std::runtime_error("Tracks must be at least bigger than 1 tracks");
    }
    
    for (auto it = future_tracks.begin(); it != future_tracks.end(); ++it)
    {
      auto it2 = std::next(it);

      for (; it2 != future_tracks.end(); ++it2)
      {
        distance = CalculateDistance(it->second, it2->second);

        /* Utilizamos el valor absoluto para calculos de restas */
        separation = std::abs(
                          it->second.GetFlightLevel() -
                          it2->second.GetFlightLevel()) * 100;

        if(((distance) < distance_min) && (separation < separation_min))
        { 
          Conflict conflict;        
          conflict.SetAircraftA(it->second.GetCallsign());
          conflict.SetAircraftB(it2->second.GetCallsign());
          conflict.SetDistance(distance);

          bool already_exists = false;

          for(const auto& item : ConflictList)
          {
            if((item.GetAircraftA() == it->second.GetCallsign() &&
              item.GetAircraftB() == it2->second.GetCallsign())
              ||
              (item.GetAircraftA() == it2->second.GetCallsign() &&
              item.GetAircraftB() == it->second.GetCallsign()) )
            {
                already_exists = true;
                break;
            }
          }

          if(!already_exists)
          {
            ConflictList.push_back(conflict);
          }
          
          std::cout<< "Future Conflict Detected!!: " << std::endl;
          std::cout <<  "Time to conflict: " << minutes << " min" << std::endl;
          std::cout<< it->second.GetCallsign() <<std::endl;
          std::cout<< it2->second.GetCallsign() <<std::endl;
          std::cout <<  "Predicted distance: " << distance << " NM" << std::endl;
          std::cout <<  "Predicted separation: " << separation << " ft" << std::endl;
        }
      }
    }
  }
  
  void DisplayAllConflictList(void)
  {
    std::cout<< "Current Conflicts" <<std::endl;
    std::cout<< "-----------------" <<std::endl;
    
    for(auto index = ConflictList.begin(); index < ConflictList.end() ;index++)
    {
      std::cout<< index->GetAircraftA() << " <-> "  << index->GetAircraftB() <<std::endl;
      std::cout<< "Distance :  "  << index->GetDistance() << " NM" <<std::endl;
    }
  }

  double CalculateDistance(const Aircraft& track1,
                         const Aircraft& track2)
  {
    // distance = √((x2-x1)^2 + (y2-y1)^2)
    double operand1, operand2, distance;

    operand1 = (track2.GetXPosition() - track1.GetXPosition());
    operand2 = (track2.GetYPosition() - track1.GetYPosition());
    operand1 = std::pow(operand1, 2); // Potencia de 2
    operand2 = std::pow(operand2, 2); // Potencia de 2
    distance = sqrt(operand1 + operand2); 
    
    return distance;
  }

  void DisplayStatistics(void)
  {
    int high_fl, low_fl, avg_fl, number_tracks;
    double avg_speed = 0;
    
    number_tracks = tracks.size();
    avg_fl = 0;
    high_fl = 0;
    low_fl = std::numeric_limits<int>::max();

    for (const auto& track: tracks)
    {
      avg_fl += track.second->GetFlightLevel();
      avg_speed += track.second->GetSpeed(); 
      if(high_fl < track.second->GetFlightLevel())
      {
        high_fl = track.second->GetFlightLevel();
      } 
      if(low_fl > track.second->GetFlightLevel())
      {
        low_fl = track.second->GetFlightLevel();
      } 
    }
    avg_fl = avg_fl/number_tracks;
    avg_speed = avg_speed/number_tracks;

    std::cout<< "Aircraft in sector: " << number_tracks <<std::endl;
    std::cout<< "Highest FL: FL " << high_fl <<std::endl;
    std::cout<< "Lowest FL: FL " << low_fl <<std::endl;
    std::cout<< "Average FL: FL" << avg_fl <<std::endl;
    std::cout<< "Average Speed: " << avg_speed << "kt" <<std::endl;
  }

  void FuturePosition(int heading, double& x, double& y, double distance)
  {
    /* La trigonometría en aviacion es al revés ya que: 
     * 0°   -> Norte
     * 90°  -> Este
     * 180° -> Sur
     * 270° -> Oeste
    */
    auto heading_rad = heading * M_PI / 180.0;
    x += distance * sin(heading_rad);
    y += distance * cos(heading_rad);
  }  

  void DisplayFuturePosition(std::string id, int minutes)
  {
    auto it = tracks.find(id);

    if (it != tracks.end())
    {
      auto x = it->second->GetXPosition();
      auto y = it->second->GetYPosition();
      auto heading = it->second->GetHeading();
      double distance = it->second->CalculateTravelDistance(minutes);
      std::cout << "Aircraft : " << id << std::endl;
      std::cout << "Current position: (" << x << "," << y << ")" << std::endl;
      FuturePosition(heading, x, y, distance); /* Updated x and y coordinates */
      std::cout << "Future position after: " << minutes << " min:"<< std::endl;
      std::cout << "(" << x << "," << y << ")" << std::endl;
    }
  }

  void CheckFutureConflicts(int minutes)
  {
    ConflictList.clear(); 
    future_tracks.clear();
    /* Copy tracks map into future_tracks map*/
    for (const auto& track : tracks)
    {
      future_tracks.insert(
      {
          track.first,
          *(track.second)
      });
    }

    int counter = 1;
    while(counter <= minutes)
    {
      for(auto& track : future_tracks)
      {
        auto x = track.second.GetXPosition();
        auto y = track.second.GetYPosition();
        double distance = track.second.CalculateTravelDistance(1);
        
        /* Updated X and Y position in future tracks map */
        FuturePosition(track.second.GetHeading(), x, y, distance);
        track.second.SetXPosition(x);
        track.second.SetYPosition(y);
      }
      /* Check all possible potential conflicts every min .*/
      FindAllFuturePotentialConflicts(1000, 5, counter);
      counter++;
    }
  }

  void WhatIfFlightLevel(std::string id, int fl, int minutes)
  {
    auto it = tracks.find(id);

    if (it != tracks.end())
    {
      int init_conflicts, end_conflicts;

      init_conflicts = ConflictList.size();
      end_conflicts = ConflictList.size();
      std::cout<< "Current conflicts: " << init_conflicts << std::endl;
      
      /* Store the previous FlightLevel to reset at the end of this function */
      int old_fl = it->second->GetFlightLevel();
      it->second->SetFlightLevel(fl);
      CheckFutureConflicts(minutes);
      /* Updated end_conflicts */
      end_conflicts = ConflictList.size();
      std::cout<< "After climb to: " << fl << std::endl;
      std::cout<< "Conflicts: " << end_conflicts << std::endl;
      
      while(ConflictList.size() > init_conflicts)
      {
        /* eliminamos los posibles conflictos nuevos que se hayan creado
         * por este what-if modo LIFO (Last Input First Ouput).
         */
        ConflictList.pop_back();
      }
      /* Reset correct FlightLevel*/
      it->second->SetFlightLevel(old_fl);
    }
    else
    {
      throw std::runtime_error("Track " + id  + " does not exist. ");
    }
  }

};

int main()
{ 
  ConflictManager manager;
  auto track1 = std::make_shared<Aircraft>("IBE123", 10, 10, 320, 450, 90);
  auto track2 = std::make_shared<Aircraft>("AFR456", 15, 12, 330, 470, 180);
  auto track3 = std::make_shared<Aircraft>("RYR999", 80, 60, 200, 380, 0);
  auto track4 = std::make_shared<Aircraft>("BAW777", 13, 11, 325, 460, 270);
  auto track5 = std::make_shared<Aircraft>("KLM789", 55, 40, 360, 470, 90);
  auto track6 = std::make_shared<Aircraft>("DLH333", 17, 14, 330, 450, 90);
  auto track7 = std::make_shared<Aircraft>("UAE777", 100, 90, 390, 500, 0);
  auto track8 = std::make_shared<Aircraft>("AAL111", 14, 10, 325, 435, 0);

  manager.AddTrack(track1);
  manager.AddTrack(track2);
  manager.AddTrack(track3);
  manager.AddTrack(track4);
  manager.AddTrack(track5);
  manager.AddTrack(track6);
  manager.AddTrack(track7);
  manager.AddTrack(track8);

  manager.DisplayAllTraffic();
  manager.FindClosestAircraft();
  manager.FindAllPotentialConflicts(1000, 5);
  manager.DisplayStatistics();
  manager.DisplayAllConflictList();
  manager.DisplayFuturePosition("IBE123", 10);
  manager.CheckFutureConflicts(10);
  manager.DisplayFutureClosestPairs(10);
  manager.WhatIfFlightLevel("IBE123", 370, 10);

  std::cin.get();
  return 0;
}