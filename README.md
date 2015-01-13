reshp
=====
Command-line tool for manipulating ESRI Shapefiles.

This project is currently a **work-in-progress** which lacks useful functionality.

## Filetypes
Currently the following filetypes are supported for input/output:
* **ESRI Shapefile** - reading and writing *(.shp)*
* **ESRI Indexfile** - writing *(.shx)*

## Shapes
Currently the following Shapefile shape-types are supported for input/output:
* **Null** - empty shapes without any properties
* **Point** - 2D points
* **Polyline** - 2D line segment loops
* **Polygon** - 2D polygons consisting of points and rings
* **Multipoint** - Collection of 2D points

## Features
Features currently implemented for manipulating supported filetypes:
* **grade** - grade the quality of a Shapefile
* **list** - listing the main properties of a Shapefile
* **subtract** - clip away polygons from one Shapefile based on the polygons of another Shapefile
