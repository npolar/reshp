reshp
=====
Command-line tool for manipulating ESRI Shapefiles.

This project is currently a **work-in-progress** which lacks useful functionality.

## Filetypes
Currently the following filetypes are supported for input/output:
* **ESRI Shapefile** - reading and writing *(.shp)*

## Shapes
Currently the following Shapefile shape-types are supported for input/output:
* **Null** - empty shapes without any properties
* **Point** - 2D points
* **Polyline** - 2D line segment loops
* **Polygon** - 2D polygons consisting of points and rings
* **Multipoint** - Collection of 2D points

## Features
Features currently implemented for manipulating supported filetypes:
* **list** - listing the main properties of a Shapefile

## TODO
Following are incomplete lists of features to be implemented:

#### Filetypes
* **GeoJSON** - reading and writing *(.geojson)*
* **TopoJSON** - reading and writing *(.topojson)*

#### Shapes
* **PointZ** - 4D points
* **PolylineZ** - 4D line segment loops
* **PolygonZ** - 4D polygons consisting of points and rings
* **MultipointZ** - Collection of 4D points
* **PointM** - 3D points
* **PolylineM** - 3D line segment loops
* **PolygonM** - 3D polygons consisting of points and rings
* **MultipointM** - Collection of 3D points

#### Features
* **subtract** - subtract shapes from input file
* **convert** - convert input file to a specified format
