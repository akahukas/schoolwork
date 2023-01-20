// Datastructures.cc
//
// Student name: Saku Hakamäki
// Student email: *********************
// Student number: *******

#include "datastructures.hh"

#include <random>

#include <iterator>

#include <QDebug>
#include <QString>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

/**
 * @brief Datastructures::Datastructures -constructor initializes
 * all data structures required by the class.
 */
Datastructures::Datastructures()
{
    // STL unordered_map for train stations by their StationID. Used for faster lookup.
    std::unordered_map<StationID, Station*> Stations;

    // Reserves space for stations.
    Stations.reserve(1000000);

    // STL map for StationIDs sorted by their coordinates. Used for its sorted feature.
    std::map<Coord, StationID> Stations_by_Coord;

    // STL map for StationIDs sorted by their names. Used for its sorted feature.
    std::map<Name, StationID> Stations_Alphabetized;

    // STL unordered_map for regions by their RegionID. Used for faster lookup.
    std::unordered_map<RegionID, Region*> Regions;

    // Reserves space for regions.
    Regions.reserve(100000);

    // STL unordered_map for trains by their TrainID. Used for faster lookup.
    std::unordered_map<TrainID, Train*> Trains;

    // Reserves space for Trains.
    Trains.reserve(100000);
}

/**
 * @brief Datastructures::~Datastructures -destructor destructs all data structures
 * and attributes that need destructing when the program is closed.
 */
Datastructures::~Datastructures()
{
    // Delete memory allocated for each <Station>-struct.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        delete it->second;
    }

    // Delete memory allocated for each <Region>-struct.
    for (auto it = Regions.begin() ; it != Regions.end() ; it++) {
        delete it->second;
    }

    // Delete memory allocated for each <Train>-struct.
    for (auto it = Trains.begin() ; it != Trains.end() ; it++) {

        // Delete memory allocated for each <Stop>-struct inside each <Train>-struct.
        for (auto it2 = it->second->stops.begin() ; it2 != it->second->stops.end() ; it2++) {
            delete it2->second;
        }

        delete it->second;
    }
}

/**
 * @brief Datastructures::station_count calculates the number different of train
 * stations which are stored to the <Stations> -data structure.
 * @return the number of train stations in <Stations> -data structure.
 */
unsigned int Datastructures::station_count()
{
    // Return the size of the <Stations> -data structure.
    return (Stations.size());
}

/**
 * @brief Datastructures::clear_all clears every data structure used by the program.
 */
void Datastructures::clear_all()
{
    // Delete memory allocated for each <Station>-struct.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        delete it->second;
    }

    // Delete memory allocated for each <Region>-struct.
    for (auto it = Regions.begin() ; it != Regions.end() ; it++) {
        delete it->second;
    }

    // Use the .clear() -member function for all used data structures.
    Stations.clear();
    Stations_by_Coord.clear();
    Stations_Alphabetized.clear();
    Regions.clear();
}

/**
 * @brief Datastructures::all_stations finds and returns StationIDs for all train
 * stations stored in <Stations> -data structure.
 * @return all the different StationIDs as members of a vector.
 */
std::vector<StationID> Datastructures::all_stations()
{
    // Initialize vector where all StationIDs are stored.
    std::vector<StationID> all_stations;

    // Iterate through <Stations> -data structure and
    // push the key of every element to the back of the vector.
    for (auto it = Stations.begin(); it != Stations.end(); it++) {
        all_stations.push_back(it->first);
    }

    // Return created vector.
    return all_stations;
}

/**
 * @brief Datastructures::add_station constructs a new train station and stores
 * it in the data structures for further use if it already doesn't exist.
 * @param id the StationID of the train station.
 * @param name the Name of the new train station.
 * @param xy the location (x- & y-coordinate) of the new train station.
 * @return boolean false if the addition wasn't successful and boolean true if it was.
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // Struct for a possibly new station.
    Station* new_station = new Station;

    // Set name, id and coordinates for new station.
    new_station->name = name;
    new_station->id = id;
    new_station->location = xy;

    // Try inserting a new member. Returns a
    // boolean regarding insertion success.
    auto result = Stations.try_emplace(id, new_station);

    // If station is a new one, add
    // it also to the other data structures.
    if (result.second) {
        Stations_Alphabetized.try_emplace(name ,id);
        Stations_by_Coord.try_emplace(xy, id);
    }

    // Return a boolean regarding addition success.
    return result.second;
}

/**
 * @brief Datastructures::get_station_name searches for train station from the
 * <Stations> -data structure by the id given as parameter and returns its Name.
 * @param id the StationID of the train station to search for.
 * @return <NO_NAME> -constant if no corresponding station wasn't found. Otherwise
 * returns the Name of the found train station.
 */
Name Datastructures::get_station_name(StationID id)
{
    // Look for corresponding train station.
    auto search_result = Stations.find(id);

    // If not found, return <NO_NAME>-constant.
    if (search_result == Stations.end()) {
        return NO_NAME;
    }

    // Return the name of the found station.
    return search_result->second->name;
}

/**
 * @brief Datastructures::get_station_coordinates searches for train station from the
 * <Stations> -data structure by the id given as parameter and returns its coordinates.
 * @param id the StationID of the train station to search for.
 * @return <NO_COORD> -constant if no corresponding station wasn't found. Otherwise
 * returns the Coord-struct (location) of the found train station.
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    // Look for corresponding train station.
    auto search_result = Stations.find(id);

    // If not found, return <NO_COORD>-constant.
    if (search_result == Stations.end()) {
        return NO_COORD;
    }

    // Return location (coordinates) of the found station.
    return search_result->second->location;
}

/**
 * @brief Datastructures::stations_alphabetically creates a vector whose members are
 * StationIDs alphabetized by the Names of the train stations.
 * @return a vector whose members are StationIDs alphabetized by the corresponding
 * train station Name.
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Initialize vector to return.
    std::vector<StationID> stations_alphabetized;

    // Iterate through <Stations_Alphabetized> -map, where the <Name> -keys are
    // automatically kept in an alphabetized order, and push the corresponding
    // <StationID> -value to the back of the created vector.
    for (auto it = Stations_Alphabetized.begin(); it != Stations_Alphabetized.end(); it++) {
        stations_alphabetized.push_back(it->second);
    }

    // Return the created result.
    return stations_alphabetized;
}

/**
 * @brief Datastructures::stations_distance_increasing creates and returns a vector
 * whose <StationID> -members are sorted by their location (coordinates).
 * @return a vector full of StationIDs in ascending order by their location.
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    // Initialize the vector to be returned.
    std::vector<StationID> station_id_by_location;

    // Iterate through <Stations_by_Coord> -map, where the <Coord> -keys are
    // automatically kept in an ascending order, and push the corresponding
    // <StationID> -value to the back of the created vector.
    for (auto it = Stations_by_Coord.begin(); it != Stations_by_Coord.end(); it++) {
        station_id_by_location.push_back(it->second);
    }

    // Return the created vector.
    return station_id_by_location;
}

/**
 * @brief Datastructures::find_station_with_coord searches for a train station
 * in the data structure with the given parameter.
 * @param xy the location of the train station to search for.
 * @return <NO_STATION> -constant if no corresponding train station found.
 * Otherwise return the StationID of the found train station.
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    // Search for corresponding station.
    auto result = Stations_by_Coord.find(xy);

    // If corresponding station not found, return <NO_STATION>-constant.
    if (result == Stations_by_Coord.end()) {
        return NO_STATION;
    }

    // Otherwise return StationID of the found station.
    return (result->second);
}

/**
 * @brief Datastructures::change_station_coord searches for corresponding
 * station with the given <id> -parameter and changes its location
 * (coordinates) according to the given <newcoord> -parameter.
 * @param id the StationID of the station to search for.
 * @param newcoord the new location (<Coord> -struct) of the found station.
 * @return boolean false if the modification was unsuccessful and boolean
 * true if the modification was successful.
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    // Look for corresponding station with given ID.
    auto result = Stations.find(id);

    // If no corresponding station found, return false.
    if (result == Stations.end()) {
        return false;
    }

    // Extract corresponding node from <Stations_by_Coord> -map.
    auto station_node = Stations_by_Coord.extract(result->second->location);

    // Change the key of the node.
    station_node.key() = newcoord;

    // Insert/link modified node back to the same data structure.
    Stations_by_Coord.insert(std::move(station_node));

    // Adjust the location of the station in the <Stations> -data structure.
    result->second->location = newcoord;

    // Indication of successful adjustment.
    return true;
}

/**
 * @brief Datastructures::add_departure adds a new train departure
 * at a specific time to the station corresponding the given
 * <stationid> -parameter.
 * @param stationid the StationID of the stations to search for
 * and where the new train departure is added.
 * @param trainid the TrainID of the new departing train.
 * @param time the Time when the new train is departing the
 * corresponding station.
 * @return boolean false if no corresponding station found or a
 * departing train with equal TrainID and Time already exist.
 * Boolean true if the addition was successful.
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    // Look for corresponding station.
    auto result = Stations.find(stationid);

    // Return false if such station not found.
    if (result == Stations.end()) {
        return false;
    }

    // Check if a departing train with equal TrainID and Time already exist.
    auto train = std::find_if(result->second->departures.begin(), result->second->departures.end(),
                           [trainid](auto train){ return (train.second == trainid); });

    // If the train doesn't exist, add a new departure and return true.
    if (train == result->second->departures.end()) {

        // Add the train to the departures of the corresponding station.
        result->second->departures.push_back(std::make_pair(time, trainid));

        // Indication of successful addition.
        return true;
    }

    // Otherwise return false.
    return false;

}

/**
 * @brief Datastructures::remove_departure removes a given train departure
 * at a given time from a given train station.
 * @param stationid the StationID which to search for and where the
 * departure is removed.
 * @param trainid the TrainID of the departing train to be removed.
 * @param time the Time when the departure to be removed is departing.
 * @return boolean false if no corresponding station found or a
 * departing train with equal TrainID and Time doesn't exist. Boolean
 * true if the removal process is successful.
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    // Look for corresponding station.
    auto result = Stations.find(stationid);

    // Return false if no such station found.
    if (result == Stations.end()) {
        return false;
    }

    // Initialize variable where the possible new list departures is saved.
    std::vector<std::pair<Time, TrainID>> new_departures;

    // Look for corresponding train in departures. If corresponding train is
    // found, it is not copied to the freshly initialized vector.
    std::copy_if(result->second->departures.begin(), result->second->departures.end(),
                 std::back_inserter(new_departures),
                 [trainid, time](auto train){
        return ((train.second != trainid) && (train.first != time));
    });

    // If the new and old list of departures are equal in size, corresponding
    // departure doesn't exist. Return false as indication of unsuccessful removal.
    if (new_departures.size() == result->second->departures.size()) {
        return false;
    }

    // Otherwise corresponding departure was found, and it's removed by replacing
    // the new departures list of the station in the <Stations> -data structure.
    result->second->departures = new_departures;

    // Return true as an indication of a successful removal.
    return true;
}

/**
 * @brief Datastructures::station_departures_after creates and returns a vector
 * whose members are departures from a given train station after a given time.
 * @param stationid the StationID of the station to search for and whose
 * departures we are interested in.
 * @param time the Time before or at the departures we are interested in.
 * @return <{{NO_TIME, NO_TRAIN}}> -constant vector if no corresponding station
 * found. Otherwise return a vector whose members are departure-pairs of Time and
 * TrainID after the <time> -parameter.
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Look for corresponding station.
    auto result = Stations.find(stationid);

    // If not found, return <{{NO_TIME, NO_TRAIN}}> -constant
    // as indication of unsuccessful search.
    if (result == Stations.end()) {
        return {{NO_TIME, NO_TRAIN}};
    }

    // Save the departures of the corresponding station to a variable.
    std::vector<std::pair<Time, TrainID>> departures = result->second->departures;

    // Sort departing trains in ascending order.
    std::sort(departures.begin(), departures.end(),
              [](auto t1, auto t2){
        // If departure time is equal, sort by TrainID.
        if (t1.first == t2.first) {
            return (t1.second < t2.second);
        }
        // Otherwise sort by departure time.
        return (t1.first < t2.first);
    });

    // Initialize a new vector where the
    // corresponding departures are saved.
    std::vector<std::pair<Time, TrainID>> departures_after;

    // Copy trains, which have same or later departure
    // time than <time> -paremeter, to the new vector.
    std::copy_if(departures.begin(), departures.end(), std::back_inserter(departures_after),
                           [time](auto train){ return (train.first >= time); });

    // Return the corresponding trains.
    return departures_after;
}

/**
 * @brief Datastructures::add_region adds a new region to the <Regions> -data
 * structure if it doesn't already exist.
 * @param id the RegionID of the existing or new region.
 * @param name the Name of the new region.
 * @param coords list of coordinates which create the border of the new region.
 * @return boolean false if a region with given id already exists, and
 * boolean true if the addition of the new region was successful.
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    // Struct for a possibly new region.
    Region* new_region = new Region;

    // Set name, id and shape of the new region.
    new_region->name = name;
    new_region->id = id;
    new_region->shape = coords;

    // Try inserting a new member.
    auto result = Regions.try_emplace(id, new_region);

    // Return a boolean regarding insertion success.
    return result.second;
}

/**
 * @brief Datastructures::all_regions creates and returns a vector
 * whose members are the RegionIDs of all of the different regions
 * stored in <Regions> -data structure.
 * @return a vector of different RegionIDs existing in
 * <Regions> -data structure.
 */
std::vector<RegionID> Datastructures::all_regions()
{
    // Initialize vector where all different RegionIDs are stored.
    std::vector<RegionID> all_regions;

    // Iterate through <Regions> -data structure and
    // add every <RegionID> -key to the back of the vector.
    for (auto it = Regions.begin(); it != Regions.end(); it++) {
        all_regions.push_back(it->first);
    }

    // Return the created result.
    return all_regions;
}

/**
 * @brief Datastructures::get_region_name searches for corresponding
 * region with a given id from the <Regions> -data structure and
 * returns the regions Name.
 * @param id the RegionID that is searched for.
 * @return <NO_NAME> -constant if no corresponding region found, or
 * the Name of the found region.
 */
Name Datastructures::get_region_name(RegionID id)
{
    // Look for corresponding region with given ID.
    auto result = Regions.find(id);

    // If not found, return <NO_NAME> -constant.
    if (result == Regions.end()) {
        return NO_NAME;
    }

    // Return the name of the found region.
    return result->second->name;
}

/**
 * @brief Datastructures::get_region_coords searches for corresponding
 * region with a given id from the <Regions> -data structure and
 * returns the list of the coordinates which form the border of
 * the region.
 * @param id the RegionID that is searched for.
 * @return <NO_COORD> -constant if no corresponding region found, or
 * the list of the coordinates which form the border of the region.
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    // Look for the corresponding region with the given ID.
    auto result = Regions.find(id);

    // If not found, return <NO_COORD> -constant.
    if (result == Regions.end()) {
        return {NO_COORD};
    }

    // Return the border coordinates of the found region.
    return result->second->shape;
}
/**
 * @brief Datastructures::add_subregion_to_region adds region as
 * others subregion if both regions exist with given ids.
 * @param id the RegionID of the new subregion.
 * @param parentid the RegionID of the new parent region.
 * @return boolean false if one or both regions not found with the given ids.
 * Returns also boolean false if the subregion already has a parent region.
 * Boolean true if the connection between regions was successfully created.
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Look for corresponding regions with the given IDs.
    auto parent = Regions.find(parentid);
    auto child = Regions.find(id);

    // If one or both not found, return false.
    if (parent == Regions.end() || child == Regions.end()) {
        return false;
    }

    // If child-region already doesn't a parent-region,
    // create the connection between parent- & child-regions
    // and return indication of successful connection creation.
    else if ((child->second->parent_region) == nullptr) {

        parent->second->subregions.push_back(child->second);
        child->second->parent_region = parent->second;

        return true;
    }

    // Child-region already has a parent, return false.
    return false;
}

/**
 * @brief Datastructures::add_station_to_region adds a station to a given
 * region if they both exist and the station doesn't already belong to a region.
 * @param id the StationID of the station to be added to the region.
 * @param parentid the RegionID of the region where the station is added to.
 * @return boolean false if station or region not found. Returns also false
 * if the station already belongs to a region. Boolean true if the
 * addition was successful.
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Looks for corresponding station and region.
    auto station = Stations.find(id);
    auto region = Regions.find(parentid);

    // If station or region not found, return false.
    if (station == Stations.end() || region == Regions.end()) {
        return false;
    }

    // Check if station already belongs to a region.
    // If not, save station to region and return
    // indication of succesful addition.
    if (station->second->parent == nullptr) {
        station->second->parent = region->second;
        return true;
    }

    // If it does, return false.
    return false;
}

/**
 * @brief Datastructures::station_in_regions creates and returns a vector
 * whose members are RegionIDs of the regions and their parent regions
 * where the station with given id belongs to.
 * @param id the StationID of the station whose region and parent regions
 * we are interested in.
 * @return <NO_REGION> -constant if no corresponding station found. Empty
 * vector if the station doesn't belong to any region. List of RegionIDs
 * where the station belongs to directly or indirectly. The direct region
 * is the first member of the vector.
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    // Look for corresponding station.
    auto station = Stations.find(id);

    // If not found, return <NO_REGION>-constant.
    if (station == Stations.end()) {
        return {NO_REGION};
    }

    // If station doesn't belong to any
    // region, return empty vector.
    if (station->second->parent == nullptr) {
        return {};
    }

    // Initialize vector to be returned.
    std::vector<RegionID> recursive_regions;

    // Call recursive method which creates the vector.
    recursive_station_in_regions(recursive_regions, station->second->parent);

    // Reverse vector order.
    std::reverse(recursive_regions.begin(), recursive_regions.end());

    // Return the created vector.
    return recursive_regions;
}

/**
 * @brief Datastructures::all_subregions_of_region creates and returns
 * a vector whose members are RegionIDs of all the different subregions
 * of a region with a given id.
 * @param id the RegionID of the region to search for and the region
 * whose direct and indirect subregions we are interested in.
 * @return <NO_REGION> -constant if no corresponding region was found.
 * Empty vector if the region with given id doesn't have any subregions.
 * A vector whose members are direct and indirect subregions of the
 * region with the given id.
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    // Look for corresponding region.
    auto region = Regions.find(id);

    // If not found, return <NO_REGION>-constant.
    if (region == Regions.end()) {
        return {NO_REGION};
    }
    // If region has no subregions, return empty vector.
    else if (region->second->subregions.empty()) {
        return {};
    }

    // Initialize vector where subregion RegionIDs are saved.
    std::vector<RegionID> subregions;

    // Call recursive function which creates the vector.
    recursive_subregions(subregions, region->second);

    // Remove the last member of the vector which is the region itself.
    subregions.pop_back();

    // Return created vector.
    return subregions;
}

/**
 * @brief Datastructures::stations_closest_to returns in distance order
 * the three or less StationIDs of the stations which are the closest
 * ones to the coordinate given as parameter.
 * @param xy the coordinate (<Coord>-struct) where the distance of the
 * stations is calculated to.
 * @return a vector whose members are StationIDs of the three or less
 * closest stations to the coordinate given as parameter.
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    // Initialize the vector for calculation results.
    std::vector<StationDistance> id_and_distance;

    // Find the corresponding station from the coordinate given as parameter.
    auto station = Stations_by_Coord.find(xy);

    // If less than eight stations exist, calculate distance for all of them.
    if (Stations.size() < 8) {
        for (auto it = Stations.begin(); it != Stations.end(); it++) {
            calculate_distance(id_and_distance, it->first, xy);
        }
    }
    // Otherwise the closest stations are the three stations
    // before or after the found station.
    else {
        // Distance from the start of the <Stations_by_Coord> -map to
        // the found station.
        int dist = std::distance(Stations_by_Coord.begin(), station);

        // If distance is less than 3, take more stations after
        // the found station.
        if (dist < 3) {
            for (auto it = std::prev(station, dist); it != std::next(station, (7 - dist)); it++) {
                calculate_distance(id_and_distance, it->second, xy);
            }
        }
        // Otherwise calculate the distances for three stations
        // before and three stations after the found station.
        else {
            for (auto it = std::prev(station, 3); it != std::next(station, 3); it++) {
                calculate_distance(id_and_distance, it->second, xy);
            }
        }
    }

    // Sort the calculated distances in ascending order.
    std::sort(id_and_distance.begin(), id_and_distance.end(),
              [](auto pair1, auto pair2){
        // If equal distance, sort by y-coordinate.
        if (pair1.distance == pair2.distance) {
            return (pair1.location.y <= pair2.location.y);
        }
        // Otherwise sort by distance.
        return (pair1.distance < pair2.distance);
    });

    // A vector for the three or less closest stations.
    std::vector<StationID> shortest_three_or_less;

    // If three or less closest stations.
    if ((id_and_distance.size()) <= 3) {
        std::for_each(id_and_distance.begin(), id_and_distance.end(),
                      [&shortest_three_or_less](auto station){
            shortest_three_or_less.push_back(station.id);
        });
    }
    // Otherwise choose only the three closest stations.
    else {
        std::for_each(id_and_distance.begin(), (id_and_distance.begin() + 3),
                      [&shortest_three_or_less](auto station){
            shortest_three_or_less.push_back(station.id);
        });
    }

    // Return created result.
    return shortest_three_or_less;
}

/**
 * @brief Datastructures::remove_station if corresponding station
 * found, removes it from the used data structures.
 * @param id the StationID of the station to be removed.
 * @return boolean false if no corresponding station was found.
 * Boolean true if station with corresponding StationID was
 * successfully removed from the existing data structures.
 */
bool Datastructures::remove_station(StationID id)
{
    // Look for corresponding station.
    auto station = Stations.find(id);

    // If not found, return false.
    if (station == Stations.end()) {
        return false;
    }

    // Avoid memory leaks by saving required keys to variables.
    Coord location = station->second->location;
    Name name = station->second->name;

    // Delete erased <Station>-struct from memory.
    delete station->second;

    // Erase the station from all used data structures.
    Stations.erase(id);
    Stations_by_Coord.erase(location);
    Stations_Alphabetized.erase(name);

    // Indication of successful removal.
    return true;
}

/**
 * @brief Datastructures::common_parent_of_regions searches for the closest
 * region in the hierarchy which is the parent region for both of the regions
 * given as parameter.
 * @param id1 the RegionID of the first region.
 * @param id2 the RegionID of the second region.
 * @return <NO_REGION> -constant if one or both regions corresponding the
 * parameters doesn't exist. Returns also <NO_REGION> -constant if one or both
 * of the regions doesn't have parent regions at all. Returns RegionID of the
 * closest common parent region in the hierarchy.
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    // Look for corresponding regions.
    auto region1 = Regions.find(id1);
    auto region2 = Regions.find(id2);

    // If one or both not found, return <NO_REGION>-constant.
    if (region1 == Regions.end() || region2 == Regions.end()) {
        return NO_REGION;
    }

    // Otherwise search for closest
    // common parent and return search result.
    return search_common_parent(region1->second, region2->second);
}

/**
 * @brief Datastructures::add_train adds a new train to the <Trains> -data
 * structure with a given unique TrainID. The train travels through given
 * stations and departs from them at a given time.
 * @param trainid the TrainID of the possibly new train to be added.
 * @param stationtimes a vector containing the StationIDs and the Times
 * when the train is departing from them.
 * @return boolean false if a train with the given TrainID already exists
 * or if atleast one of the Stations in the <stationtimes>-vector doesn't
 * exist. Boolean true if the addition of the new train is a success.
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)
{
    // If corresponding train does already exist, return false.
    if (Trains.find(trainid) != Trains.end()) {
        return false;
    }

    // If atleast one of the stations in <stationtimes
    // -parameter doesn't exist, return false.
    for (auto it = stationtimes.begin() ; it != stationtimes.end() ; it++) {
        auto result = Stations.find(it->first);

        if (result == Stations.end()) {
            return false;
        }
    }

    // Struct for a new train.
    Train* new_train = new Train;

    // Vector which stores a pair of the trains stops StationID and
    // pointer to a <Stop>-struct which stores other needed
    // information about the station/stop.
    std::vector<std::pair<StationID, Stop*>> stops;

    // Create a pair for each station in <stationtimes>-parameter.
    for (std::string::size_type i = 0 ; i != stationtimes.size() ; i++) {

        // Struct for a new stop.
        Stop* new_stop = new Stop;

        new_stop->trainid = trainid;

        // Set previous and next station -pointers
        // according to the train location on the route.
        if (i == 0) {
            auto station = Stations.find(stationtimes[i+1].first);

            new_stop->to = station->second;
        }
        else if (i == (stationtimes.size() - 1)) {
            auto station = Stations.find(stationtimes[i-1].first);

            new_stop->from = station->second;
        }
        else {
            auto station_to = Stations.find(stationtimes[i+1].first);
            auto station_from = Stations.find(stationtimes[i-1].first);

            new_stop->to = station_to->second;
            new_stop->from = station_from->second;
        }

        // Set the departing time from the station
        // or the arrival time to the final station.
        new_stop->time_at_station = stationtimes[i].second;

        // Push the new pair to the back of the vector.
        stops.push_back(std::make_pair(stationtimes[i].first, new_stop));
    }

    // Set id and stop times for new train.
    new_train->id = trainid;
    new_train->stops = stops;

    // Try inserting a new member. Returns a
    // boolean regarding insertion success.
    auto result = Trains.try_emplace(trainid, new_train);

    // Add the new information also to the correponding <Station>-struct.
    if (result.second) {

        for (auto it = stationtimes.begin() ; it != stationtimes.end() ; it++) {
            auto result = Stations.find(it->first);

            result->second->departures.push_back(std::make_pair(it->second, trainid));
        }
    }

    // Return a boolean regarding addition success.
    return result.second;
}

/**
 * @brief Datastructures::next_stations_from searches and returns all
 * StationIDs of the next stations on all train routes from a given station.
 * @param id the StationID of the train station whose next train stations
 * we are interested in.
 * @return an empty vector if no trains depart from the given station.
 * A vector whose only member is <NO_STATION>-constant if station with
 * the given StationID doesn't exist. A vector whoses members are the
 * StationIDs of the next stations when traveling on the train routes
 * from the station given as parameter.
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    // Look for corresponding station.
    auto result = Stations.find(id);

    // If no station found, return a vector whose
    // only member is <NO_STATION>-constant.
    if (result == Stations.end()) {
        return {NO_STATION};
    }
    // If the station is found but no trains
    // depart from it, return an empty vector.
    else if (result->second->departures.empty()) {
        return {};
    }

    // Result vector to return.
    std::vector<StationID> next_stations;

    // Iterate through the trains which depart from the station.
    for (auto it = result->second->departures.begin() ; it != result->second->departures.end() ; it++) {

        // Look for corresponding train from
        // the <Trains> -data structure.
        auto train = Trains.find(it->second);

        // If corresponding train is found, process its information.
        if (train != Trains.end()) {

            // Iterate through every station in which the found train is stopping at.
            for (auto it2 = train->second->stops.begin() ; it2 != train->second->stops.end() ; it2++) {

                // Interested only in the StationID given as parameter.
                if (it2->first == id) {

                    // If the corresponding station is the final station,
                    // the train doesn't stop at any other station after it.
                    if (train->second->stops.back().first == id) {
                        continue;
                    }


                    // Otherwise point the iterator at the next station and
                    // push the StationID of that station to the result vector.
                    it2++;
                    next_stations.push_back(it2->first);

                    // Only interested in one station after
                    // the station given as parameter, break loop.
                    break;
                }
            }
        }
    }

    // Return created result.
    return next_stations;
}

/**
 * @brief Datastructures::train_stations_from searches and returns a
 * list of stations which come after a given station on a given train route.
 * @param stationid the StationID of the train station.
 * @param trainid the TrainID of the train on a route we are interested in.
 * @return A vector whose only member is a <NO_STATION>-constant if no train
 * or station is found with the given IDs. Otherwise returns all the StationIDs
 * of the stations on a given train route after a given station.
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    // Look for corresponding train.
    auto train = Trains.find(trainid);

    // If no corresponding station or train is found,
    // return a vector whose only member is a <NO_STATION>-constant.
    if (Stations.find(stationid) == Stations.end() || train == Trains.end()) {
        return {NO_STATION};
    }

    // Look for the <Stop>-struct on the train route
    // which corresponds the station given as parameter.
    auto result = std::find_if(train->second->stops.begin(), train->second->stops.end(),
                               [stationid](auto stop){ return (stop.first == stationid); });

    // If the station doesn't exist on the train route or if exists, the found
    // station is the final destination of the train, (train doesn't leave the
    // station). Return a vector whose only member is a <NO_STATION>-constant.
    if (result == train->second->stops.end() || result->second->to == nullptr) {
        return {NO_STATION};
    }

    // Result vector for the corresponding StationIDs.
    std::vector<StationID> stations;

    // Iterate the stations on the train route after the station given
    // as parameter and push their StationIDs to the result vector.
    for (auto it = (result + 1) ; it != train->second->stops.end() ; it++) {
        stations.push_back(it->first);
    }

    // Return created result.
    return stations;
}

/**
 * @brief Datastructures::clear_trains deallocates all the memory reserved
 * for all the <Stop>- and <Train>-structs and clears the <Trains> -data structure.
 */
void Datastructures::clear_trains()
{
    // Delete memory allocated for each <Train>-struct.
    for (auto it = Trains.begin() ; it != Trains.end() ; it++) {

        // Delete memory allocated for each <Stop>-struct inside each <Train>-struct.
        for (auto it2 = it->second->stops.begin() ; it2 != it->second->stops.end() ; it2++) {
            delete it2->second;
        }

        delete it->second;
    }

    // Use the .clear() -member function for the <Trains>-data structure.
    Trains.clear();
}

/**
 * @brief Datastructures::route_any searches and returns any route
 * between the two stations given as parameter. In this case searches
 * for the shortest route using A* -algorithm.
 * @param fromid the StationID of the station where the route
 * is searched from.
 * @param toid the StationID of the station where the route is
 * searched to.
 * @return A vector whose only member is a pair of constants
 * {<NO_STATION>, <NO_DISTANCE>} if one or both stations given
 * as parameter not found. An empty vector if no route between
 * the two stations is found. Otherwise return a vector of pairs,
 * whose first element is a StationID and the second element is
 * a Distance between two stations. The first member of the vector
 * is the starting station on the route with the Distance 0, the last
 * member is the final station on the route with the total Distance
 * calculated between all the station on the route. The members between
 * these two members have their corresponding StationID and the
 * Distance calculated from the starting station until that station.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{
    // Look for corresponding stations.
    auto station_from = Stations.find(fromid);
    auto station_to = Stations.find(toid);

    // If one or both stations not found, return a vector
    // whose only member is a pair of constants {<NO_STATION>, <NO_DISTANCE>}.
    if (station_from == Stations.end() || station_to == Stations.end()) {
        return {{NO_STATION, NO_DISTANCE}};
    }

    // A* -algorithm which forms the shortest
    // route between the two given stations.
    Astar_shortest(*station_from->second, *station_to->second);

    // Pointer to the final station on the route.
    Station* it = station_to->second;

    // Result vector of pairs.
    std::vector<std::pair<StationID, Distance>> s_and_dist;

    // Loop until a certain condition is met.
    while (true) {

        // Insert the pair of {<StationID>, <Distance>} to the result vector.
        s_and_dist.insert(s_and_dist.begin(), std::make_pair(it->id, it->d));

        // If the starting station is reached,
        // stop looping and return created result.
        if (it->location == station_from->second->location) {
            return s_and_dist;
        }
        // If a dead end is reached, i.e. the starting
        // station can't be reached, return an empty vector.
        else if (it->pi == nullptr) {
            return {};
        }

        // Point the pointer to the previous
        // station on the shortest route.
        it = it->pi;
    }
}

/**
 * @brief Datastructures::route_least_stations searches and returns a route between
 * two stations that has the least possible amount of stations. If multiple routes
 * with same amount of stations is found, any of them can be returned.
 * @param fromid the StationID of the station where the route is searched from.
 * @param toid the StationID of the station where the route is searched to.
 * @return A vector whose only member is a pair {<NO_STATION>, <NO_DISTANCE>} if one
 * or both stations corresponding the given StationIDs is not found. An empty vector
 * if no route between ´the stations is found. Otherwise return a vector of pairs in
 * which the first element is the StationID of the station on the route and the second
 * element is the Distance on the route until that station. The first member of the
 * vector is the ID of the starting station with a Distance of 0. The last member is
 * the final station with the route total Distance.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid)
{
    // Look for stations corresponding the given StationIDs.
    auto station_from = Stations.find(fromid);
    auto station_to = Stations.find(toid);

    // If one or both not found, return a vector whose
    // only member is a pair {<NO_STATION>, <NO_DISTANCE>}.
    if (station_from == Stations.end() || station_to == Stations.end()) {
        return {{NO_STATION, NO_DISTANCE}};
    }

    // Use BFS-algorithm to search for the route with least stations.
    BFS_station(*station_from->second);

    // Pointer to the destination/final station on the possible route.
    Station* it = station_to->second;

    // Result vector.
    std::vector<std::pair<StationID, Distance>> s_and_dist;

    while (true) {

        // Insert the currently iterated station as a
        // new element to the beginning of the result vector.
        s_and_dist.insert(s_and_dist.begin(), std::make_pair(it->id, it->d));

        // If the starting station is reached, the route has been found.
        // Return created result.
        if (it->location == station_from->second->location) {
            return s_and_dist;
        }
        // If a dead end is reached, i.e. no route exists
        // between the stations, return an empty vector.
        else if (it->pi == nullptr) {
            return {};
        }

        // Point the iterator to the previous station on the route.
        it = it->pi;
    }
}

/**
 * @brief Datastructures::route_with_cycle searches for and returns a route from a
 * station that has a cycle in it, i.e. the route ends to a station that already
 * exists in the route. If multiple routes with a cycle exist, any of them can be
 * returned.
 * @param fromid the StationID of the station where the route is searched from.
 * @return A vector whose only member is <NO_STATION>-constant, if no station with
 * the given StationID is found. An empty vector if a route with a cycle is not found.
 * Otherwise return a vector of StationIDs of those stations that are on the found
 * route. The first member is the StationID of the starting station, and the last
 * member is the StationID of the station completing the route with a cycle, i.e.
 * a station that already exists on the route.
 */
std::vector<StationID> Datastructures::route_with_cycle(StationID fromid)
{
    // Look for corresponding train station.
    auto station_from = Stations.find(fromid);

    // If the station doesn't exist, return a vector
    // whose only member is a <NO_STATION>-constant.
    if (station_from == Stations.end()) {
        return {NO_STATION};
    }

    // Use DFS-algorithm to search for the route with a cycle.
    DFS_station(*station_from->second);

    // Pointer to the first station on the route.
    Station* it = station_from->second;

    // Result vector.
    std::vector<StationID> stationids;

    // If no route was found, return an empty vector.
    if (it->pi == nullptr && it->next == nullptr) {
        return {};
    }
    // If the starting station is the station completing the route with a cycle,
    // i.e. the starting station is the first and last station on the route.
    else if (it->pi != nullptr) {

        // Iterate through the found route.
        while (true) {

            // Insert the StationID of the currently iterated
            // station to the beginning of the result vector.
            stationids.insert(stationids.begin(), it->id);

            // Starting station is on the route again,
            // i.e. a route with a cycle is found.
            if ((stationids.size() > 1) && (it->id == stationids.back())) {

                // Insert the StationID of the starting
                // station to the beginning of the vector.
                if (it->pi2 != nullptr) {
                    it = it->pi2;
                    stationids.insert(stationids.begin(), it->id);
                }

                // Return created result.
                return stationids;
            }

            // Point the pointer to the previous station on the route.
            it = it->pi;
        }
    }
    // If some other station than the starting
    // station completes the route with a cycle.
    else if (it->pi == nullptr && it->next != nullptr) {

        // Push the ID of the starting station to the back of the result vector.
        stationids.push_back(it->id);

        // Point the iterator to the next station on the found route.
        it = it->next;

        // Iterate through the found route.
        while (true) {

            // Push the ID of the currently iterated
            // station to the back of the result vector.
            stationids.push_back(it->id);

            // Counter for number of times the currently
            // iterated station appears in the result vector.
            int counter = 0;

            // Iterate through the result vector.
            for (auto it2 = stationids.begin() ; it2 != stationids.end() ; it2++) {

                // If the currently iterated station appears atleast
                // twice in the result vector, a route with cycle
                // exists, return created result.
                if (*it2 == it->id) {
                    counter++;

                    if (counter >= 2) {
                        return stationids;
                    }
                }
            }

            // If a dead end is reached, a route with
            // cycle doesn't exist, return an empty vector.
            if (it->next == nullptr) {
                return {};
            }

            // Point the pointer to the next station on the route.
            it = it->next;
        }
    }
    // Otherwise the route wasn't found, return an empty vector.
    else {
        return {};
    }
}

/**
 * @brief Datastructures::route_shortest_distance searches and returns the
 * route between two stations that is the shortest possible measured in Distance.
 * If multiple routes have the same total Distance, any of them can be returned.
 * @param fromid the StationID of the station where the route is searched from.
 * @param toid the StationID of the station where the route is searched to.
 * @return A vector whose only member is a pair {<NO_STATION>, <NO_DISTANCE>} if
 * one or both of the stations corresponding the given IDs is not found. An empty
 * vector if no route is found. Otherwise return a vector of pairs in which the
 * first element is the StationID of a station on the route, and the second element
 * is the Distance on the route until that station. The first member of the vector
 * is the starting station with the Distance 0, and the last member is the final
 * station with the route total distance.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID fromid, StationID toid)
{
    // The functionality of this function is identical to route_any function
    // which uses A*-algorithm to search for the shortest possible route.
    return route_any(fromid, toid);
}

/**
 * @brief Datastructures::route_earliest_arrival searches and returns the route
 * between two stations that is shortest measured in Time, i.e. the route that
 * has the earliest possible arrival time to the destination station. If multiple
 * routes have the same arrival time, any of the routes can be returned.
 * @param fromid the StationID of the station where the route is searched from.
 * @param toid the StationID of the station where the route is searched to.
 * @param starttime the earliest Time at when one would like to depart the
 * starting station.
 * @return A vector whose only member is a pair {<NO_STATION>, <NO_TIME>} if
 * one or both of the stations corresponding the given IDs is not found. An
 * empty vector if no route is found. Otherwise return a vector of pairs in which
 * the first element is the StationID of the station on the found route, and the
 * second element is the departing Time of the used train on the found route.
 * The first member of the vector is the starting station and the departing Time
 * from that station, and the last element is the final station with the arrival
 * Time to that station.
 */
std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID fromid, StationID toid, Time starttime)
{
    // Look for corresponding stations.
    auto station_from = Stations.find(fromid);
    auto station_to = Stations.find(toid);

    // If one or both not found, return a vector
    // whose only member is a pair {<NO_STATION>, <NO_TIME>}.
    if (station_from == Stations.end() || station_to == Stations.end()) {
        return {{NO_STATION, NO_TIME}};
    }

    // Flag to indicate that if the route to
    // the destination is still possible to find.
    bool has_route = true;

    // Use Dijkstra's algorithm to search for
    // the earliest possible route.
    Dijkstra_earliest(*station_from->second, *station_to->second, starttime, has_route);

    // If no route found, return an empty vector.
    if (!has_route) {
        return {};
    }

    // Pointer to the destination station.
    Station* it = station_to->second;

    // Result vector.
    std::vector<std::pair<StationID, Time>> s_and_time;

    // Iterate through the found route.
    while (true) {

        // If the currently iterated station isn't on a train with
        // multiple stops, insert it to the beginning of the result
        // vector with the found departure time.
        if (!it->is_on_multistop_train) {
            s_and_time.insert(s_and_time.begin(), std::make_pair(it->id, it->d));
        }
        // If the station is on a train with multiple stops, insert
        // it to the beginning of the result vector with the found
        // departure time estimate.
        else {
            s_and_time.insert(s_and_time.begin(), std::make_pair(it->id, it->de));
        }

        // If the currently iterated station has the same location
        // as the starting station, the route has been found, return
        // created result vector.
        if (it->location == station_from->second->location) {
            return s_and_time;
        }
        // If a dead end is reached, a route
        // doesn't exist, return an empty vector.
        else if (it->pi == nullptr) {
            return {};
        }

        // Point the pointer to the previous station on the route.
        it = it->pi;
    }
}

/**
 * @brief Datastructures::recursive_station_in_regions searches recursively for
 * all of the parent regions of the RegionID given as parameter.
 * @param recursive_regions a vector of RegionIDs where all the parent regions
 * IDs are pushed.
 * @param region pointer to a <Region> -struct which is examined at the specific
 * recursion level.
 */
void Datastructures::recursive_station_in_regions(std::vector<RegionID> &recursive_regions, Region* region)
{
    // If the topmost region is found, end recursion.
    if (region->parent_region == nullptr) {

        // Add RegionID to vector.
        recursive_regions.push_back(region->id);
        return;
    }
    else {
        // Otherwise go one step deeper in recursion,
        // give parent regions ID as parameter.
        recursive_station_in_regions(recursive_regions, region->parent_region);

        // Add RegionID to vector.
        recursive_regions.push_back(region->id);

        return;
    }
}

/**
 * @brief Datastructures::calculate_distance calculates distance between
 * station and coordinate given as parameter and saves the result to the
 * vector parameter.
 * @param id_and_distance a vector whose members are <StationDistance>-structs,
 * which store the id, location and calculated distance to the <xy>-parameter.
 * @param id the StationID of the station where the distance is calculated from.
 * @param xy the location (<Coord>-struct) where the distance is calculated to.
 */
void Datastructures::calculate_distance(std::vector<StationDistance> &id_and_distance, StationID id, Coord xy)
{
    // Find corresponding station and save location to variable.
    auto station = Stations.find(id);
    Coord location = station->second->location;

    // Calculate Euclidean distance from station to given coordinate.
    float distance = sqrt(pow(xy.x - location.x, 2) + pow(xy.y - location.y, 2));

    // Initialize a new struct for results.
    StationDistance s_and_dist;

    // Set the id, location and distance of the handled station.
    s_and_dist.id = id;
    s_and_dist.location = location;
    s_and_dist.distance = distance;

    // Save pair to the results vector.
    id_and_distance.push_back(s_and_dist);
}

/**
 * @brief Datastructures::recursive_subregions finds recursively all the
 * subregions of the station corresponding the given <id> -parameter.
 * @param subregions a vector of RegionIDs, where the IDs of found
 * subregions are pushed to.
 * @param region pointer to a <Region>-struct whose subregions
 * we are interested in at the specific recursion level.
 */
void Datastructures::recursive_subregions(std::vector<RegionID> &subregions, Region* region)
{
    // Region has no subregion, end recursion
    // and push RegionID to vector.
    if (region->subregions.empty()) {

        subregions.push_back(region->id);
        return;
    }
    else {
        // Otherwise go one stage deeper in recursion and give RegionID
        // of subregion as parameter. Region can have multiple subregions,
        // so iterate through and handle all of them.
        for (auto it = region->subregions.begin(); it != region->subregions.end(); it++) {
            recursive_subregions(subregions, *it);
        }

        // Push RegionID of already found subregion to vector.
        subregions.push_back(region->id);

        return;
    }
}

/**
 * @brief Datastructures::search_common_parent searches for closest common
 * parent region of the regions given as parameter.
 * @param region1 pointer to the <Region>-struct of the first region.
 * @param region1 pointer to the <Region>-struct of the second region.
 * @return <NO_REGION> -constant if one or both of the regions don't have any
 * parent regions. Returns the RegionID of the closest common parent of the
 * two regions which both are subregions of.
 */
RegionID Datastructures::search_common_parent(Region* region1, Region* region2)
{
    // Vector for parent-regions of <region1>.
    std::vector<RegionID> parents1;

    // Find parent-regions of <region1>.
    recursive_parentregions(parents1, region1);

    // Vector for parent-regions of <region2>.
    std::vector<RegionID> parents2;

    // Find parent-regions of <region2>.
    recursive_parentregions(parents2, region2);

    // If one or both regions don't have any parents,
    // return <NO_REGION>-constant.
    if (parents1.empty() || parents2.empty()) {
        return NO_REGION;
    }

    // Pop the last region from the vector
    // which is the handled region itself.
    parents1.pop_back();
    parents2.pop_back();

    // Closest parent of regions.
    RegionID closest = NO_REGION;

    // Check if the two regions have a common parent.
    // Parents are already in "ascending" order (=closest first).
    std::find_if(parents1.rbegin(), parents1.rend(),
                  [&closest, &parents2](RegionID region){

        // Check if regions have the same parent.
        auto it2 = std::find(parents2.rbegin(), parents2.rend(), region);

        // If they do, save search result and stop
        // std::find_if -algorithm by returning true.
        if (it2 != parents2.rend()) {
            closest = region;
            return true;
        }

        // Otherwise keep searching, or if the end of the
        // vector is reached, regions do not have a common parent.
        return false;
    });

    // Return the created vector.
    return closest;
}

/**
 * @brief Datastructures::recursive_parentregions finds recursively all the
 * parent regions of the station corresponding the given <id> -parameter.
 * @param parentregions a vector of RegionIDs, where the IDs of found
 * parent regions are pushed to.
 * @param region pointer to the <Region>-struct whose parent regions
 * we are interested in at the specific recursion level.
 */
void Datastructures::recursive_parentregions(std::vector<RegionID> &parentregions, Region* region)
{
    // If the topmost region is found, end recursion and push
    // the id to the back of the result vector.
    if ((region->parent_region) == nullptr) {
        parentregions.push_back(region->id);
        return;
    }
    else {
        // Otherwise go one step deeper in recursion,
        // give parent regions ID as parameter.
        recursive_parentregions(parentregions, region->parent_region);

        // Add already found RegionID to the results vector.
        parentregions.push_back(region->id);

        return;
    }
}

/**
 * @brief Datastructures::BFS_station uses Breadth First Search to calculate the
 * shortest Distance from all other stations to the station given as parameter.
 * @param station_from the Station where the Distance is calculated from to
 * the other Stations.
 */
void Datastructures::BFS_station(Station &station_from)
{
    // Reset the colour, Distance, Distance-estimate
    // and pi-pointer in all train stations.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        it->second->colour = WHITE;
        it->second->d = INF;
        it->second->de = INF;
        it->second->pi = nullptr;
    }

    // Initialize priority queue.
    std::queue<std::pair<int, Station*>> Q;

    // Set the starting stations colour to gray.
    // Set also the Distance and the Distance-estimate to 0.
    station_from.colour = GRAY;
    station_from.d = 0;
    station_from.de = 0;

    // Push the starting station to the queue
    // with Distance-estimate as the priority.
    Q.push(std::make_pair(station_from.de, &station_from));

    // Iterate until the queue is empty.
    while (!Q.empty()) {

        // Extract the oldest member from the queue.
        auto u = Q.front();
        Q.pop();

        // Go through all the extracted stations adjacent stations.
        for (auto &next_station : next_stations_from(u.second->id)) {

            // Find the station corresponding the StationID.
            auto v = Stations.find(next_station);

            // If the adjacent station is a new one, set its colour to gray,
            // calculate the distance from it to the extracted station and
            // push it to the queue with the Distance-estimate as the priority.
            if (v->second->colour == WHITE) {
                v->second->colour = GRAY;
                v->second->d = u.second->d + cost_shortest(*u.second, *v->second);
                v->second->de = u.second->de + 1;
                v->second->pi = u.second;
                Q.push(std::make_pair(v->second->de, v->second));
            }
        }

        // The extracted station has been fully handled,
        // set its colour to black.
        u.second->colour = BLACK;
    }
}

/**
 * @brief Datastructures::DFS_station uses Depth First Search to calculate
 * some route from the station given as parameter to every other station.
 * The algorithm searches for cycles in routes.
 * @param station_from the Station where the search is started from.
 */
void Datastructures::DFS_station(Station &station_from)
{
    // Reset the colour, pi-pointers and
    // next-pointer in all train stations.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        it->second->colour = WHITE;
        it->second->pi = nullptr;
        it->second->pi2 = nullptr;
        it->second->next = nullptr;
    }

    // Initialize the stack used as priority queue.
    std::stack<Station*> S;

    // Push the starting station to the priority queue.
    S.push(&station_from);

    // Iterate until the stack is empty.
    while (!S.empty()) {

        // Extract the newest member from the stack.
        auto u = S.top();
        S.pop();

        // If the extracted member has not been seen before.
        if (u->colour == WHITE) {

            // Set its colour to gray and push it to the stack.
            u->colour = GRAY;
            S.push(u);

            // Go through all the extracted stations adjacent stations.
            for (auto &next_station : next_stations_from(u->id)) {

                // Find the station corresponding the StationID.
                auto v = Stations.find(next_station);

                // If the adjacent station is a new one,
                // set the next- and pi-pointer and
                // push it to the stack.
                if (v->second->colour == WHITE) {
                    u->next = v->second;
                    v->second->pi = u;
                    S.push(v->second);
                }
                // If the adjacent stations colour is gray,
                // the searched cycle is found. Set the pi-
                // and next-pointers and return.
                else if (v->second->colour == GRAY) {
                    if (v->second->pi == nullptr) {
                        v->second->pi = u;
                    }
                    else {
                        u->next = v->second;
                        v->second->pi2 = u;
                    }

                    return;
                }
            }
        }
        // Otherwise the extracted member ha
        // been handled, set its colour to black.
        else {
            u->colour = BLACK;
        }
    }
}

/**
 * @brief Datastructures::cost_shortest calculates the cost in Distance
 * to travel between the two stations given as parameter.
 * @param u the Station where the cost is calculated from.
 * @param v the Station where the cost is calculated to.
 * @return the calculated cost in Distance to travel between the two stations.
 */
Distance Datastructures::cost_shortest(Station &u, Station &v)
{
    // Calculate and return the cost in Distance.
    return floor(sqrt(pow(u.location.x - v.location.x, 2) + pow(u.location.y - v.location.y, 2)));
}

/**
 * @brief Datastructures::relax_astar_shortest is a relaxation function used
 * in A* -algorithm to search for the shortest possible route between two stations.
 * @param u the first <Station> to be compared.
 * @param v the second <Station> to be compared.
 * @param g the destination/final <Station> to be compared.
 */
void Datastructures::relax_astar_shortest(Station &u, Station &v, Station &g)
{
    // Calculate the cost to travel between
    // the two stations to be compared to each other.
    Distance cost = cost_shortest(u, v);

    // If a shorter Distance is found, set
    // the new variable values accordingly.
    if (v.d > (u.d + cost)) {
        // The shorter distance found.
        v.d = u.d + cost;

        // The new Distance-estimate to the final station.
        v.de = v.d + cost_shortest(v, g);

        // Set the pi-pointer. (<Station> where the shortest Distance is from.
        v.pi = &u;
    }
}

/**
 * @brief Datastructures::Astar_shortest is an A* -algorithm used to search
 * for the shortest Distance between two stations given as parameter.
 * @param station_from the starting <Station>.
 * @param station_to the destination/final <Station>.
 */
void Datastructures::Astar_shortest(Station &station_from, Station &station_to)
{
    // Reset the colour, Distance, Distance-estimate
    // and pi-pointer in all train stations.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        it->second->colour = WHITE;
        it->second->d = INF;
        it->second->de = INF;
        it->second->pi = nullptr;
    }

    // Initialize the used priority queue.
    std::priority_queue<std::pair<int, Station*>> Q;

    // Set values for starting station.
    station_from.colour = GRAY;
    station_from.d = 0;

    // Push starting station to the priority queue.
    Q.push(std::make_pair(-station_from.d, &station_from));

    // Loop until the queue is empty.
    while (!Q.empty()) {

        // Extract the highest priority member.
        // (Smallest negative Distance).
        auto u = Q.top();
        Q.pop();

        // Stop iterating if the final station is found.
        if (u.second->id == station_to.id) {
            return;
        }

        // Handle all the adjacent stations.
        for (auto &next_station : next_stations_from(u.second->id)) {

            // Find the corresponding station.
            auto v = Stations.find(next_station);

            // Old distance.
            int old_d = v->second->d;

            // Use the relaxation function to
            // check if shorter Distance is found.
            relax_astar_shortest(*u.second, *v->second, station_to);

            // If the adjacent station is a new one.
            if (v->second->colour == WHITE) {

                // Set color to gray and push it to
                // queue with the Distance-estimate as priority.
                v->second->colour = GRAY;
                Q.push(std::make_pair(-v->second->de, v->second));
            }
            // If the adjacent station isn't a new one but a shorter
            // Distance to it is found, push it to the priority
            // queue with the new Distance-estimate.
            else if (old_d > v->second->d) {
                Q.push(std::make_pair(-v->second->de, v->second));
            }
        }

        // Set the color of the extracted Station
        // to black since it has been handled.
        u.second->colour = BLACK;
    }
}

/**
 * @brief Datastructures::cost_earliest calculates the cost in Time to travel between
 * two stations given as parameter. The cost is calculated as time it takes to travel
 * between the two stations when the train is the earliest possible after Time given
 * as parameter and it's the fastest (shortest travel duration) train betweeb the
 * two stations.
 * @param u the Station where the cost is calculated from.
 * @param v the Station where the cost is calculated to.
 * @param time the Time after which the train to be used is chosen.
 * @return A pair in which the first element is a pair and the second element is the
 * TrainID of the train used to travel between the stations. The first elements first
 * element is the shortest duration it takes to travel between the stations plus the
 * waiting time if the train used doesn't leave right at the moment one has arrived
 * to the station <u>. The first elements second element is the waiting time.
 */
std::pair<std::pair<Time, Time>, TrainID> Datastructures::cost_earliest(Station &u, Station &v, Time time)
{
    // TrainID of the train to be used between the stations.
    TrainID train_between_stations = NO_TRAIN;

    // Departing Time of the train to be used.
    Time departing_time = NO_TIME;

    // The travel time of the train to be used.
    Time shortest_duration = NO_TIME;

    // Iterate through both stations departing trains.
    for (auto it = u.departures.begin() ; it != u.departures.end() ; it++) {
        for (auto it2 = v.departures.begin() ; it2 != v.departures.end() ; it2++) {

            // If the iterated trains have the same TrainID and
            // it leaves after Time given as parameter, a suitable
            // train has been found.
            if (it->second == it2->second && it->first >= time) {

                // Calculate the travel time with the found train.
                Time duration = it2->first - it->first;

                // If the calculated time is less than the previously
                // found trains travel time, set new variable values.
                if (duration < shortest_duration) {

                    // Departing time of the found train.
                    departing_time = it->first;

                    // Travel time of the found train.
                    shortest_duration = duration;

                    // The TrainID of the found train.
                    train_between_stations = it->second;
                }
            }
        }
    }

    // If the chosen trains departing time is later than the
    // arrival time to the first station, the waiting time
    // on the station needs to be calculated.
    if (departing_time > time) {

        // Calculate the waiting time.
        Time waiting_time = departing_time - time;

        // Return found values with waiting time.
        return std::make_pair(std::make_pair(shortest_duration + waiting_time, waiting_time), train_between_stations);
    }

    // Return found values without waiting time.
    return std::make_pair(std::make_pair(shortest_duration, 0), train_between_stations);
}

/**
 * @brief Datastructures::relax_dijkstra_earliest is a relax-function used in
 * Dijkstra's algorithm to calculate the route with earliest possible arrival
 * time to the destination station. This function sets the new attribute values
 * if a cheaper route is found.
 * @param u the Station where the cheaper route is searched from.
 * @param v the Station where the cheaper route is searched to.
 * @param time the Time which is used to determine the departing time of
 * the train to be used.
 * @param starttime the Time when one has been arrived at the very first
 * starting station.
 * @param has_route a boolean to indicate if it's still possible to find a
 * route to the destination station.
 */
void Datastructures::relax_dijkstra_earliest(Station &u, Station &v, Time time, Time starttime, bool &has_route)
{
    // Calculate the cost to travel between the two given stations, and
    // also get the TrainID of the train that has been used.
    std::pair<std::pair<Time, Time>, TrainID> cost_and_train = cost_earliest(u, v, time);

    // Set new variable values only if a cheaper route is found.
    if (v.d > (u.d + cost_and_train.first.first)) {

        // If the current time equals to the time at the starting station, the
        // possible waiting time there needs to be added (The cheapest train
        // doesn't depart immediately after arriving to the first station).
        if (time == starttime) {
            u.d = time + cost_and_train.first.second;
        }
        // If earlier a route with waiting time has been found, it has to be
        // subtracted from the cost since a new cheaper route has been found.
        else if (u.prev_waiting_time != 0) {
            u.d = u.d - u.prev_waiting_time + cost_and_train.first.second;
        }
        // If route between the stations wasn't found, change the flag
        // value to false and stop looking for the cheapest route.
        else if (cost_and_train.first.first == NO_TIME) {
            has_route = false;
            return;
        }
        // Otherwise add the possible waiting time to the cost.
        else {
            u.d = u.d + cost_and_train.first.second;
        }

        // Set the new waiting time, travel cost and cost estimate.
        u.prev_waiting_time = cost_and_train.first.second;
        v.d = time + cost_and_train.first.first;
        v.de = u.d + cost_and_train.first.first;

        // If the used train has more than one stop, and
        // the found route uses the same train.
        if (cost_and_train.second != NO_TRAIN && u.used_train == cost_and_train.second) {

            // Find the corresponding train.
            auto train = Trains.find(cost_and_train.second);

            // Iterate through the found trains stops.
            for (auto it = train->second->stops.begin() ; it != train->second->stops.end() ; it++) {

                // If the StationID on the route is the ID of the second station.
                if (it->first == v.id) {
                    // Set pointer to previous station on route and set
                    // it as the value of the second stations pi-pointer.
                    it--;
                    v.pi = Stations.find(it->first)->second;

                    // New value for the pi-pointers cost.
                    v.pi->d = it->second->time_at_station;

                    // Station corresponding the pi-pointer is now
                    // on a train with multiple stops.
                    v.pi->is_on_multistop_train = true;

                    // Value searched has been found, break loop.
                    break;
                }
            }
        }
        // Set the used train and pi-pointer.
        else {
            u.used_train = cost_and_train.second;

            v.pi = &u;
        }
    }
}

/**
 * @brief Datastructures::Dijkstra_earliest uses Dijkstra's algorithm to
 * search for the route with earliest arrival time to the destination station.
 * @param station_from the Station where the suitable route is searched from.
 * @param station_to the Station where the the suitable route is searched to.
 * @param time the Time at when one has arrived at the starting <station_from>.
 * @param has_route a boolean to indicate if it's still possible to find a
 * route to the destination station.
 */
void Datastructures::Dijkstra_earliest(Station &station_from, Station &station_to, Time time, bool &has_route)
{
    // Reset the values used in the algorithm for all stations.
    for (auto it = Stations.begin() ; it != Stations.end() ; it++) {
        it->second->colour = WHITE;
        it->second->d = INF;
        it->second->de = INF;
        it->second->prev_waiting_time = 0;
        it->second->used_train = NO_TRAIN;
        it->second->is_on_multistop_train = false;
        it->second->pi = nullptr;
    }

    // Initialize the used priority queue.
    std::priority_queue<std::pair<int, Station*>> Q;

    // Set the colour of the starting station to gray.
    station_from.colour = GRAY;

    // Departing time of the best suitable
    // train from the starting station.
    Time departing_time = NO_TIME;

    // Iterate through every departing train from the starting station.
    for (auto it = station_from.departures.begin() ; it != station_from.departures.end() ; it++) {

        // Find the corresponding Train.
        auto train = Trains.find(it->second);

        // If the iterated train departs at the same time when arriving
        // at the station and the starting station is not the final
        // station on the trains route.
        if (it->first == time && train->second->stops.back().first != station_from.id) {

            // Best suiting train has been found,
            // set departing time and break loop.
            departing_time = it->first;
            break;
        }
        // If the found train departs later than one has arrived to
        // the station and before the earlier than the train found before.
        else if ((it->first > time) && (it->first < departing_time)) {

            // Set new departing time and continue
            // looking for a better suiting train.
            departing_time = it->first;
        }
    }

    // Set the best suiting departure time.
    station_from.d = departing_time;

    // Push the starting station to the queue with the
    // departure time as a negative priority.
    Q.push(std::make_pair(-station_from.d, &station_from));

    // Iterate until the queue is empty.
    while (!Q.empty()) {

        // Extract the member with the highest priority
        // (closest to zero negative Time estimate).
        auto u = Q.top();
        Q.pop();

        // If the extracted member is the destination station,
        // the end is reached and stop iterating.
        if (u.second->id == station_to.id) {
            return;
        }
        // Handle all the adjacent stations.
        for (auto &next_station : next_stations_from(u.second->id)) {

            // Find corresponding Station.
            auto v = Stations.find(next_station);

            // Save the current Time estimate to a variable.
            int old_duration = v->second->de;

            // Use the relax-function to check if a new cheaper route is found.
            relax_dijkstra_earliest(*u.second, *v->second, -u.first, time, has_route);

            // If the route is not possible to find anymore, stop searching.
            if (!has_route) {
                return;
            }

            // If the adjacent station is a new one, set its colour to gray
            // and insert it to the queue with the negative Time estimate as
            // the priority.
            if (v->second->colour == WHITE) {
                v->second->colour = GRAY;
                Q.push(std::make_pair(-v->second->de, v->second));
            }
            // If the adjacent station isn't a new one, but a cheaper route
            // has been found to it, insert it to the priority queue with
            // the new cheaper Time estimate.
            else if (old_duration > v->second->de) {
                Q.push(std::make_pair(-v->second->de, v->second));
            }
        }
        // The extracted station has been fully
        // handled, change its colour to black.
        u.second->colour = BLACK;
    }
}
