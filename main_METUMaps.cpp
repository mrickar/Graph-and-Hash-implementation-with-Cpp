#include "METUMaps.h"
#include <iostream>

int main()
{
    METUMaps maps(2, "metu_campus.map");

    // Set Proper Locations
    maps.SetStartingLocation("Ceng");
    maps.SetDestination("Library");
    maps.StartJourney();
    std::cout << "================" << std::endl;
    maps.Display();
    std::cout << "================" << std::endl;
    maps.UpdateLocation("EE");
    std::cout << "================" << std::endl;
    maps.Display();
    std::cout << "================" << std::endl;
    maps.UpdateLocation("Library");
    maps.EndJourney();
    
    // ======================== //
    // OUTPUT OF THE CODE ABOVE //
    // ======================== //
    // Calculating Route(s) between "Ceng" and "Library"...
    // ================
    // Journey         : Ceng->Library
    // Current Location: Ceng
    // Time Left       : 7 minutes
    // Current Route   : Ceng--5->EE--2->Library
    // ================
    // Route between "EE" and "Library" is in cache, using that...
    // ================
    // Journey         : Ceng->Library
    // Current Location: EE
    // Time Left       : 2 minutes
    // Current Route   : EE--2->Library
    // ================
    // Journey Completed!

}