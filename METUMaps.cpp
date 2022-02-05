#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const
{
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const
{
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const
{
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const
{
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const
{
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const
{
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const
{
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string &location0,
                                        const std::string &location1) const
{
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string &location0,
                                      const std::string &location1) const
{
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string &location0,
                                  const std::string &location1)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount,
                   const std::string &mapFilePath)
{
    // TODO
    inJourney = false;
    map = Graph(mapFilePath);
    this->potentialPathCount = potentialPathCount;
    cachedPaths = KeyedHashTable(potentialPathCount * map.TotalVertexCount());
}

void METUMaps::SetDestination(const std::string &name)
{
    // TODO
    if (inJourney)
    {
        PrintUnableToChangeDestination();
        return;
    }
    destination = name;
}

void METUMaps::SetStartingLocation(const std::string &name)
{
    // TODO
    if (inJourney)
    {
        PrintUnableToChangeDestination();
        return;
    }
    startingLoc = name;
}

void METUMaps::StartJourney()
{
    // TODO
    if (inJourney)
    {
        PrintAlreadyInJourney();
        return;
    }
    PrintCalculatingRoutes(startingLoc, destination);
    std::vector<std::vector<int> > orderedVertexIdList;
    int pathNum = map.MultipleShortPaths(orderedVertexIdList, startingLoc, destination, potentialPathCount);

    if (pathNum == 0)
    {
        PrintLocationNotFound();
        return;
    }

    inJourney = true;
    for (int i = 0; i < pathNum; i++)
    {
        for (int j = 0; j < orderedVertexIdList[i].size() - 1; j++)
        {
            std::vector<int> intArray;
            std::string loc0 = map.VertexName(orderedVertexIdList[i][j]);
            std::string cache = GenerateKey(loc0, destination);
            for (int k = j; k < orderedVertexIdList[i].size(); k++)
            {
                intArray.push_back(orderedVertexIdList[i][k]);
            }
            if (intArray.size() != 0)
            {
                cachedPaths.Insert(cache, intArray);
            }
        }
    }
    currentLoc = startingLoc;
    currentRoute = orderedVertexIdList[0];
}

void METUMaps::EndJourney()
{
    // TODO
    if (!inJourney)
    {
        PrintJourneyIsAlreadFinished();
        return;
    }
    // clear cachetable
    cachedPaths.ClearTable();
    inJourney = false;
    destination = "";
    startingLoc = "";
    currentLoc = "";
}

void METUMaps::UpdateLocation(const std::string &name)
{
    // TODO
    bool locExist = false;
    for (int i = 0; i < map.TotalVertexCount(); i++)
    {
        if (name == map.VertexName(i))
        {
            locExist = true;
        }
    }
    if (locExist)
    {
        currentLoc = name;
    }
    if (!inJourney)
    {
        PrintNotInJourney();
        return;
    }
    if (!locExist)
    {
        PrintLocationNotFound();
        return;
    }
    if (name == destination)
    {
        PrintJourneyCompleted();
        return;
    }
    std::vector<int> vc;
    std::string key = GenerateKey(name, destination);
    if (cachedPaths.Find(vc, key))
    {
        PrintCachedLocationFound(name, destination);
        currentRoute = vc;
    }
    else
    {
        PrintCalculatingRoutes(name, destination);
        std::vector<int> shortestPath;
        map.ShortestPath(shortestPath, currentLoc, destination);
        currentRoute = shortestPath;
        for (int j = 0; j < shortestPath.size() - 1; j++)
        {
            std::vector<int> intArray;
            std::string loc0 = map.VertexName(shortestPath[j]);
            std::string cache = GenerateKey(loc0, destination);
            for (int k = j; k < shortestPath.size(); k++)
            {
                intArray.push_back(shortestPath[k]);
            }

            cachedPaths.Insert(cache, intArray);
        }
    }
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if (!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}