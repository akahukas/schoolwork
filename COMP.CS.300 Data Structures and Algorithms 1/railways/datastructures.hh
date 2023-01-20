// Datastructures.hh
//
// Student name: Saku Hakamäki
// Student email: *********************
// Student number: *******

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <iterator>
#include <cmath>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Constant integer value for infinity.
const int INF = std::numeric_limits<int>::max();

// Color constants used in Dijkstra's or A* -algorithm.
const std::string WHITE = "white";
const std::string GRAY = "gray";
const std::string BLACK = "black";


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    float dist1 = sqrt(pow(c1.x, 2) + pow(c1.y, 2));
    float dist2 = sqrt(pow(c2.x, 2) + pow(c2.y, 2));

    if (dist1 < dist2) { return true; }
    else if (dist2 < dist1) { return false; }
    else { return c1.y < c2.y; }
}


// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::size is constant.
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::clear and std::map::clear
    // is linear in the size of the container.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::vector::push_back is amortized constant
    // and it is executed at most n times.
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is linear at worst.
    // Operation std::unordered_map::insert is
    // logarithmic and std::map::insert is constant.
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is linear at worst.
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is linear at worst.
    Coord get_station_coordinates(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::vector::push_back is amortized constant
    // and it is executed at most n times.
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::vector::push_back is amortized constant
    // and it is executed at most n times.
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Complexity of
    // std::map::find is logarithmic.
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is constant. Operation
    // std::map::erase is amortized constant and,
    // std::map::insert is logarithmic.
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is constant, std::find_if is
    // logarithmic and std::vector::push_back is constant.
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is constant, std::copy_if is
    // logarithmic and std::vector::size is constant.
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is constant, std::sort is
    // linearithmic and std::copy_if is linear.
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is constant and
    // std::unordered_map::insert is linear.
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::vector::push_back is constant and is
    // executed at most n times.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is linear at worst.
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unordered_map::find is linear at worst.
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is constant and
    // std::vector::push_back is constant.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is constant and
    // std::find_if is linear.
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is linear and
    // recursive_station_in_regions is logarithmic.
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is linear,
    // std::unoredered_map::empty is constant and
    // std::vector::pop_back is constant.
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is constant and
    // std::sort is linearithmic.
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is linear,
    // std::unoredered_map::erase is constant and
    // std::map::erase is constant.
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Complexity of
    // std::unoredered_map::find is constant,
    // search_common_parent is linear.
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: A train with n stops is added
    // to the <Trains> -data structure. Also all the n stops are added
    // to the information of their corresponding station.
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(n*m)
    // Short rationale for estimate: All of the n number of
    // trains m number of stops are iterated through at worst.
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Operation
    // std::find_if is linear.
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(m*n^2)
    // Short rationale for estimate: All the memory used by
    // the m <Stop>-structs of all the n <Train>-structs are
    // deallocated, after which the linear
    // std::unordered_map::clear is called.
    void clear_trains();

    // Estimate of performance: O((n^2+k)*log(n))
    // Short rationale for estimate: A* -algorithm is
    // O((n+k)*log(n)), after which the route is travelled
    // between n stations.
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance: O(n^2+k)
    // Short rationale for estimate: BFS-algorithm is
    // O(n+k) (n stations and k edges), after which the
    // route is travelled between n stations.
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance: O(n^2+k)
    // Short rationale for estimate: DFS-algorithm is
    // O(n+k) (n stations and k edges), after which the
    // route is travelled between n stations.
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance: O((n^2+k)*log(n))
    // Short rationale for estimate: A* -algorithm is
    // O((n+k)*log(n)), after which the route is travelled
    // between n stations.
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance: O((n^2+k)*log(n))
    // Short rationale for estimate: Dijkstra's algorithm is
    // O((n+k)*log(n)), after which the route is travelled
    // between n stations.
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:

    // Struct for data needed by a region.
    struct Region {
        Name name = NO_NAME;
        RegionID id;
        std::vector<Coord> shape = {NO_COORD};
        Region* parent_region = nullptr;
        std::vector<Region*> subregions = {};
    };

    // Data structure where information about different
    // regions is stored by using RegionID as key and
    // pointer to Region-struct as value.
    std::unordered_map<RegionID, Region*> Regions;

    // Struct for data needed by a train station.
    struct Station {
        Name name = NO_NAME;
        StationID id;
        Coord location = NO_COORD;
        Region* parent = nullptr;
        std::vector<std::pair<Time, TrainID>> departures = {};

        // Added in prg2.
        std::string colour = WHITE;
        int d = INF;
        int de = INF;
        int prev_waiting_time = NO_VALUE;
        TrainID used_train = NO_TRAIN;
        bool is_on_multistop_train = false;
        Station* pi = nullptr;
        Station* pi2 = nullptr;
        Station* next = nullptr;
    };

    // Data structure where information about different
    // train stations is stored by using StationID as key
    // and pointer to Station-struct as value.
    std::unordered_map<StationID, Station*> Stations;

    // Data structure where StationIDs are
    // stored in ascending order by distance from
    // their location to the origin (x=0, y=0).
    std::map<Coord, StationID> Stations_by_Coord;

    // Data structure where StationIDs
    // are stored in alphabetical order.
    std::map<Name, StationID> Stations_Alphabetized;

    // Searches recursively for all of the
    // parent regions of the RegionID given as parameter.
    void recursive_station_in_regions(std::vector<RegionID> &recursive_regions, Region* region);

    // Struct for data needed by train station distance calculation.
    struct StationDistance {
        StationID id;
        Coord location;
        float distance;
    };

    // Calculates distance between station and coordinate
    // given as parameter and saves the result to the vector parameter.
    void calculate_distance(std::vector<StationDistance> &id_and_distance, StationID id, Coord xy);

    // Finds recursively all the subregions of
    // the station corresponding the given <id> -parameter.
    void recursive_subregions(std::vector<RegionID> &subregions, Region* region);

    // Searches for closest common parent region of the regions given as parameter.
    RegionID search_common_parent(Region* region1, Region* region2);

    // Finds recursively all the parent regions
    // of the station corresponding the given <id> -parameter.
    void recursive_parentregions(std::vector<RegionID> &parentregions, Region* region);

    // Information needed by a train stop.
    struct Stop;

    struct Stop {
        TrainID trainid;
        Station* from = nullptr;
        Station* to = nullptr;
        Stop* prev_departure = nullptr;
        Stop* next_departure = nullptr;
        Time time_at_station;
    };


    // Struct for data needed by a train.
    struct Train {
        TrainID id;
        std::vector<std::pair<StationID, Stop*>> stops;
    };

    // Data structure where information about different
    // trains is stored by using TrainID as key and
    // pointer to Train-struct as value.
    std::unordered_map<TrainID, Train*> Trains;

    // Breadth first search.
    void BFS_station(Station &station_from);

    // Depth first search.
    void DFS_station(Station &station_from);

    // Function calculating cost in Distance between two stations.
    Distance cost_shortest(Station &u, Station &v);

    // Relaxation function used in A* -algorithm.
    void relax_astar_shortest(Station &u, Station &v, Station &g);

    // A* -algorithm searching for the shortest
    // route Distance between two stations.
    void Astar_shortest(Station &station_from, Station &station_to);

    // Function calculating cost in travel time between two stations.
    std::pair<std::pair<Time, Time>, TrainID> cost_earliest(Station &u, Station &v, Time time);

    // Relaxation function used in Dijkstra's algorithm.
    void relax_dijkstra_earliest(Station &u, Station &v, Time time, Time starttime, bool &has_route);

    // Dijkstra's algorithm searching for the route that has
    // the earliest possible arrival time to the destination station.
    void Dijkstra_earliest(Station &station_from, Station &station_to, Time time, bool &has_route);
};

#endif // DATASTRUCTURES_HH
