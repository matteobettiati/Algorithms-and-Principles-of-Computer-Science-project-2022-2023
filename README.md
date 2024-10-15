# Description

Consider an expressway described as a sequence of service stations. Each service station is located at a distance from the beginning of the expressway, expressed in kilometers as a positive or zero integer. There are no two service stations at the same distance; each service station is uniquely identified by its distance from the start of the expressway.

Every service station has a fleet of electric rental vehicles. Each vehicle is characterized by its battery charge's autonomy, expressed in kilometers as a positive integer. The vehicle fleet at a single station comprises at most 512 vehicles. When renting a car from station 's', it is possible to reach all stations whose distance from 's' is less than or equal to the car's autonomy.

A journey is identified by a sequence of service stations where the driver makes stops. It starts at one service station and ends at another, passing through zero or more intermediate stations. Assume that the driver cannot backtrack during the journey and rents a new car every time they stop at a service station. Therefore, given two consecutive stops 's' and 't', 't' must always be farther from the starting point than 's', and 't' must be reachable using one of the available vehicles at 's'.

# Project Objective

The project's objective is to plan the route with the fewest number of stops between a given pair of stations. If multiple routes exist with the same minimum number of stops (tied), the route that prefers stops with shorter distance from the beginning of the expressway should be chosen. In other words, if there is a set of tied routes P = {p1, p2, ..., pn}, where each route is a tuple of 'm' elements pi = ⟨pi,1, pi,2, ..., pi,m⟩ representing the distance from the start of the expressway of each stop in the order of travel, the unique route pi should be chosen such that no other route pj exists with the same last 'k' stops as pi.

# Input and Outputs

## Input File Format

The incoming text file contains a sequence of commands, one per line, with the following format. All positive or zero integer values can be encoded in 32 bits.

* aggiungi-stazione distance car-count car-autonomy-1 ... car-autonomy-n
Adds a station at the given distance, equipped with a car count and their respective autonomies. If a station already exists at the indicated distance, the command does nothing.
Expected Printout as Response: added or not added.

* demolisci-stazione distance
Removes the station at the indicated distance, if it exists.
Expected Printout as Response: demolished or not demolished.

* aggiungi-auto station-distance car-autonomy-to-add
If the station exists, adds a car to it. Multiple cars with the same autonomy can be added.
Expected Printout as Response: added or not added.

* demolisci-auto station-distance car-autonomy-to-scrap
Removes a car from the indicated station, if the station exists and has at least one car with the specified autonomy.
Expected Printout as Response: scrapped or not scrapped.

* pianifica-percorso start-station-distance end-station-distance
Requests planning a route following the above constraints.
Expected Printout as Response: the stages in order of travel, represented by the distances from the highway's start, separated by spaces, and ending with a line break. The starting and ending points must be included; if they coincide, the station is printed only once. If the route doesn't exist, it prints no route. The planning action doesn't alter the stations or their vehicle fleets. The given stations are definitely present.

# Testing
To test it:
- download the zip of the repository
- open the terminal where you saved the main.c file
- run the following commands:
- gcc main.c test
- ./test < "here take one of the file in the test folder (not the output ones)" > "here put a file where you want to store the results"

If you want to check if the result is correct:
- run "diff filename1 filename2", where filename1 contains the output of my program and filename2 you have to chose the output file that correspons to the correct number.
