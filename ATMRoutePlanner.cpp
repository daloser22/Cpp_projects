#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <set>

class Waypoint
{
private:
  std::string name;

public:
  Waypoint(std::string name):
  name(name)
  {
  }

  virtual ~Waypoint(){}
};

class RouteNetwork
{
private:
  std::map<std::string, std::vector<std:: string>> Route;

public:
  RouteNetwork(void)
  {
    /* Do nothing */
  }

  virtual ~RouteNetwork(){}
  
  void PrintNetwork(std::string id)
  {
    auto mapIt = Route.find(id);

    if( mapIt != Route.end())
    {
      std::cout << id << ":" << std::endl;

      for(const auto& point : mapIt->second)
      {
        std::cout << " --> " << point <<std::endl;
      }
    }
  }

  void AddWaypoint(std::string id)
  {
    auto mapIt = Route.find(id);

    if( mapIt != Route.end())
    {
      throw std::runtime_error("Error: This track already exists. You are duplicating the waypoint");
    }
    else
    {
      Route[id] = {};
    }  
  }

 /* Funcion con recursividad para ir viajando dentro de cada nivel de ruta */
bool HasRoute(
    const std::string& origin,
    const std::string& destination,
    std::set<std::string>& visited)
{
  

  if (origin == destination)
  {
    return true;
  }

  visited.insert(origin);

  auto mapIt = Route.find(origin);

  if (mapIt == Route.end())
  {
    return false;
  }

  for (const auto& neighbour : mapIt->second)
  {
    if (visited.find(neighbour) == visited.end())
    {
      if (HasRoute(neighbour, destination, visited))
      {
        return true;
      }
    }
  }

  return false;
}

bool HasDirectConnection(
    const std::string& origin,
    const std::string& destination)
{
    auto mapIt = Route.find(origin);

    if (mapIt == Route.end())
    {
        throw std::runtime_error(
            "Origin waypoint does not exist.");
    }

    auto vecIt = std::find(
        mapIt->second.begin(),
        mapIt->second.end(),
        destination);

    return vecIt != mapIt->second.end();
}

  void Connect(const std::string& origin, const std::string& destination)
  {
    auto originIt = Route.find(origin);
    auto destinationIt = Route.find(destination);

    if( originIt == Route.end())
    {
      throw std::runtime_error("Error. Origin track does not exist.");
    }
    if( destinationIt == Route.end())
    {
      throw std::runtime_error("Error. Destination track dos not exist.");
    }
    
    if(!HasDirectConnection(origin, destination))
    {
      Route[origin].push_back(destination);
    }
  }
};

int main()
{
  RouteNetwork network;
  std::set<std::string> visited;

  network.AddWaypoint("VALENCIA");
  //network.AddWaypoint("VALENCIA"); // Duplicado de network. RuntimeError
  network.AddWaypoint("BARCELONA");
  network.AddWaypoint("SEVILLA");
  network.AddWaypoint("MALAGA");
  network.AddWaypoint("MADRID");

  network.Connect("VALENCIA","SEVILLA");
  network.Connect("VALENCIA","SEVILLA"); // Duplicar conexiones.
  network.Connect("SEVILLA","MALAGA");
  //network.Connect("SEVILLA","BILBAO"); // RuntimeError porque no existe MALAGA. RuntimeError

  if(network.HasRoute("VALENCIA", "MALAGA", visited))
  {
    std::cout << "Route found!" << std::endl;
  } 
  else
  {
    std::cout << "Route not found!" << std::endl;
  }
  
  network.PrintNetwork("VALENCIA");

  std::cin.get();
  return 0;
}