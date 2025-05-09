# Dynamic-Flight-Route-Optimizer
An interactive C++/JavaScript Qt based program that uses real-time weather to calculate optimal flight route geodesics between two cities, navigating around turbulent weather using A* pathfinding algorithm. 

*****NOTE  THIS APPLICATION IS CURRENTLY VERY EARLY IN THE DEVELOPMENT PROCESS, AND HAS LIMITED FUNCTIONALITY*****

### What this program is ###
### --------------------- ###
This is a C++ written application that draws the optimal geodesic on a 2D and 3D map, and reroutes based on real-time weather data and turbulence obtained from OpenWeatherApi, and further displayed on OpenStretMap through a JavaScript, Leaflet.js, implementation. In addition to the flight path calculation, it takes into consideration aircraft type, payload, and determines time to destination, optimal cruising altitude/speed, and fuel required based on aircraft fuel consumption. The program has over 200 major cities which are compatible through their respective latitude/longitude coordinates, in which they're stored in a JSON file read through the JsonCpp library.

As of the most recent update, this application is under very early development, and is not able to do everything descibed in the paragraph above; however, that is the ultimate functionality of this program. Currently, it has a fully developed UI built the Qt-WebEngine library in C++, successfully has access to the 200 city data in a JSON file, and accurately draws a geodesic between any two cities on the built. It has weather overlay from OpenWeatherApi, but the A* based navigation has not been developed yet and will be introduced in further commits. There is a very small selection of aircraft at the moment, but no fuel, time, altitude, or speed calculations are currently done and only filler text is displayed as of now. I have started this project over Spring Break, and currently have limited time to make consistent updates, so it could possibly take a few weeks/months before new functionality is released.

### How to Use/Run ###
### --------------------- ###
Download this application through Github, or by running the following command in git bash:

``` git clone https://github.com/Void-Overflow/Dynamic-Flight-Route-Optimizer.git ```

You must substitute an API key for OpenWeatherApi in order to achieve weather overlay. The API key must be substituted into the map.html file in /x64/Release folder in the section that has API_KEY substituted; however, the geodesic and map features do still work regardless. If desired, the project can be rebuilt in Release mode using Visual Studio through the Dynamic Flight Route Optimizer.sln file. The project can then be run through Visual Studio or through the Dynamic Flight Route Optimizer.exe file under /x64/Release/, in which a window with the application will pop up. As a note, our city JSON data, leaflet and JsonCpp libraries are both found under the /x64/Release/ directory along with numerous Qt libraries to allow the executable to run by itself.

*****///Comyar Dehlavi 2025///*****
