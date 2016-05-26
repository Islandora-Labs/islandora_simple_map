# Islandora Simple Map

Islandora module that appends a Google map to an object's display if the object's MODS datastream contains geographic coordinate data. You can see it in action [here](http://digital.lib.sfu.ca/pfp-980/buffalo-stanley-park-vancouver-bc).

## Overview

Geographic coordinates must be in "decimal degrees" format with latitude listed first, then longitude. Google Maps is fairly forgiving of the specific formatting of the values. All of these work:

```
+49.05444,-121.985
+49.05444 -121.985
49.05444 N 121.985 W
49.05444N121.985w
```

By default, the MODS element that this module expects the coordintates to be in is `<subject><cartographics><coordinates>`, but that is configurable.

If you configure this module to use MODS elements that do not contain coordinate data, such as `<subject><geographic>`, Google Maps will attempt to generate a map based on the data it has been told to use. However, the results are not always predictable. For example, the following two values for `<subject><geographic>` produce accurate maps, presumably because they are unambiguous:

```
Dublin, Ireland
Dublin, Ohio
```

but a value of just `Dublin` results in a map showing the Irish city. Another example that illustrates Google Maps' behavior when it is given ambiguous data is a <`subject><geographic>` value of `City of Light`, which results in a map showing a church by that name in the US Northwest, not Paris, probably because when I wrote this I was closer to that location than to Paris, France. If Google Maps cannot disambiguate the location data to a single location to put on a map, it produces a map showing most of the world (depending on the default zoom level in effect) with no points on it.

So, for best results, configure this module to use unambiguous cartographic coordinate data.


## Requirements

* [Islandora](https://github.com/Islandora/islandora)

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Admin settings are available at `admin/islandora/tools/islandora_simple_map` for the XPath expression to the MODS element where your cartographic data is stored, and for the map's height, width, and default zoom level. No Google Maps API key is required.

Once you enable the module, any object whose MODS file contains coordinates in the expected element will have a Google map appended to its display. If multiple elements contain coordinates, data from the first element found is used.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## To do

* Add support for non-Google maps.
* Add support for using non-MODS datastreams for cartographic data.
* Add a Drupal permission to "View Islandora Simple Map maps".

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
