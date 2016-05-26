# Islandora Simple Map

## Introduction

Islandora module that adds a Google map to an object's display if the object's MODS datastream contains cartographic coordinates. Coordinates must be in "decimal degrees" format with latitude listed first, then longitude. Google Maps is remarkably forgiving of the specific formatting of the values, however. All of these work:

```
+49.05444,-121.985
+49.05444 -121.985
49.05444 N 121.985 W
49.05444N121.985w
```

By default, the element that this module expects the coordintates to be in is `<subject><cartographics><coordinates>`, but that is configurable.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Admin settings are available at `admin/islandora/tools/islandora_simple_map` for the XPath expression to the MODS element where your cartographic data is stored, and for the map's height, width, and default zoom level. No Google Maps API key is required.

Once you enable the module, any object whose MODS file contains coordinates in the expected element will have a Google map appended to its display. If multiple elements contain coordinates, the first one is used.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## To do

* Add support for non-Google maps.
* Add support for using alternative (non-MODS) datastreams for cartographic data.

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
