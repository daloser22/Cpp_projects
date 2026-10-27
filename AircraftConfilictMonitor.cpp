#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <set>
#include <cmath> // Necesario para la función sqrt()

class Aircraft
{
private:
  std::string callsign;
  double x;
  double y;
  int FlightLevel;
  int speed;
public:
  
  Aircraft(std::string callsign, double x, double y, int FlightLevel, int speed):
  callsign(callsign),
  x(x),
  y(y),
  FlightLevel(FlightLevel),
  speed(speed)
  {
  }

  virtual ~Aircraft()
  {
    std::cout<< "Aircraft Destroyed " <<std::endl;
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

  int GetSpeed(void) const
  {
    return speed;
  }

  double GetXPosition(void) const
  {
    return x;
  }

  double GetYPosition(void) const
  {
    return y;
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

  double GetDistance(void)
  {
    return distance;
  }

  std::string GetAircraftA(void)
  {
    return aircraftA;
  }
  
  std::string GetAircraftB(void)
  {
    return aircraftB;
  }
};

class ATCManager
{
private:
  std::map<std::string, std::shared_ptr<Aircraft>> tracks;
  std::vector<Conflict> ConflictList;
public:

  void AddTrack(const std::shared_ptr<Aircraft>& track)
  {
    auto it = tracks.find(track->GetCallsign());
    
    if (it != tracks.end())
    {
        throw std::runtime_error(
            "Track already exists.");
    }
    
    tracks[track->GetCallsign()] = track;
  }

  void DisplayAllTraffict(void)
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
      /* No repites pasos innecesarios que ya has ido comprobando de atrás hacia delante */
      auto it2 = std::next(it);

      for (; it2 != tracks.end(); ++it2)
      {
        distance_tmp = CalculateDistance(it->second, it2->second);
        
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

  void FindAllPotentialConflicts(int separation_min, int distance_min)
  {
    double distance;
    int separation;
    std::string pairs;
    Conflict conflict;

    /* First of all we need to see if there is more than 1 flight in tracks */
    if (tracks.size() < 2)
    {
      throw std::runtime_error("Tracks must be at least bigger than 1 tracks");
    }
    
    for (auto it = tracks.begin(); it != tracks.end(); ++it)
    {
      /* No repites pasos innecesarios que ya has ido comprobando de atrás hacia delante */
      auto it2 = std::next(it);

      for (; it2 != tracks.end(); ++it2)
      {
        distance = CalculateDistance(it->second, it2->second);

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
          /* Updated aircraft pair with the minimum distance */
          pairs = it->second->GetCallsign() + " <-> " + it2->second->GetCallsign();
          
          conflict.SetAircraftA(it->second->GetCallsign());
          conflict.SetAircraftB(it2->second->GetCallsign());
          conflict.SetDistance(distance);

          ConflictList.push_back(conflict);
          
          std::cout<< "Potencial conflict: " << std::endl;
          std::cout<< it->second->GetCallsign() <<std::endl;
          std::cout<< it2->second->GetCallsign() <<std::endl;
          std::cout <<  "Lateral: " << distance << " NM" << std::endl;
          std::cout <<  "Vertical: " << separation << " ft" << std::endl;
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

  double CalculateDistance(const std::shared_ptr<Aircraft>& track1,
                         const std::shared_ptr<Aircraft>& track2)
  {
    // distance = √((x2-x1)^2 + (y2-y1)^2)
    double operand1, operand2, distance;

    operand1 = (track2->GetXPosition() - track1->GetXPosition());
    operand2 = (track2->GetYPosition() - track1->GetYPosition());
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
};

int main()
{ 
  ATCManager manager;
  auto track1 = std::make_shared<Aircraft>("IBE123", 10, 10, 320, 450);
  auto track2 = std::make_shared<Aircraft>("AFR456", 15, 12, 330, 470);
  auto track3 = std::make_shared<Aircraft>("RYR999", 80, 60, 200, 380);
  auto track4 = std::make_shared<Aircraft>("BAW777", 13, 11, 325, 460);
  auto track5 = std::make_shared<Aircraft>("KLM789", 55, 40, 360, 470);
  auto track6 = std::make_shared<Aircraft>("DLH333", 17, 14, 330, 450);
  auto track7 = std::make_shared<Aircraft>("UAE777", 100, 90, 390, 500);
  auto track8 = std::make_shared<Aircraft>("AAL111", 14, 10, 325, 435);

  manager.AddTrack(track1);
  manager.AddTrack(track2);
  manager.AddTrack(track3);
  manager.AddTrack(track4);
  manager.AddTrack(track5);
  manager.AddTrack(track6);
  manager.AddTrack(track7);
  manager.AddTrack(track8);

  manager.DisplayAllTraffict();
  manager.FindClosestAircraft();
  manager.FindAllPotentialConflicts(1000, 5);
  manager.DisplayStatistics();
  manager.DisplayAllConflictList();

  std::cin.get();
  return 0;
}