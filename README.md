# Islandora Simple Map

Islandora module that appends a Google map to an object's display if its MODS datastream contains the required data. You can see it in action [here](http://digital.lib.sfu.ca/pfp-980/buffalo-stanley-park-vancouver-bc).

## Overview

This module can use geographic coordinates and place names in MODS elements to populate a Google map that is then appended to the object's display. Site admins ccan configure multiple MODS elements in a preferred order by entering XPath expressions in the admin setting's "XPath expressions to MODS elements containing map data" field. Data from the first element to match one of the configured XPath expressions is used to render the map. The module provide sensible default values that prefer `<subject><cartographics><coordinates>` over `<subject><geographic>`.

### Using geographic coordinates to create maps

By default, the MODS element this module expects geographic coordintates to be in is `<subject><cartographics><coordinates>`. Geographic coordinates must be in "decimal degrees" format with latitude listed first, then longitude. Google Maps is fairly forgiving of the specific formatting of the values. All of these work:

```
+49.05444,-121.985
+49.05444 -121.985
49.05444 N 121.985 W
49.05444N121.985w
```

Semicolons separating the latitude and longitude are not allowed, resulting in a map with no points on it:

```
+49.05444;-121.985
```

There is an admin option to "Attempt to clean up map data". If this is enabled (which it is by default), a semicolon in the data will be replaced with a comma before it is passed to Google Maps. Normally this option should be enabled, but if it interferes with your data in unexpected ways, it can be turned off.

### Using place names and other non-coordinate data to create maps

If you configure this module to use MODS elements that do not contain coordinate data, such as `<subject><geographic>`, Google Maps will attempt to generate a map based on whatever data it finds in the configured element. However, the results of using non-cartographic coordinates are not always predictable. For example, the following two values for `<subject><geographic>` produce accurate maps, presumably because they are unambiguous:

```
Dublin, Ireland
Dublin, Ohio
```

but a value of just `Dublin` results in a map showing the Irish city. Another example that illustrates Google Maps' behavior when it is given ambiguous data is a `<subject><geographic>` value of `City of Light`, which results in a map showing a church by that name in the US Northwest, not Paris, France, probably because when I wrote this I was closer to that location than to Paris. From Europe, for example, you get a completely different location for a `<subject><geographic>` value of `City of Light`. Also, if Google Maps cannot associate the data with a geographic location (predictable from a user's perspective or not), it produces a map showing a large portion of the world (depending on the default zoom level in effect) with no points on it.

The XPath expressions used to retrieve map data are executed in the order they are listed in the admin settings. So, for best results, listing the expressions in decreasing likelihood they will contain reliable and unambiguous data is the best strategy. The defaults values do this.


## Requirements

* [Islandora](https://github.com/Islandora/islandora)

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Even though this module uses the Google Maps Embed API, no API key is required.

Admin settings are available at `admin/islandora/tools/islandora_simple_map` for:

* the XPath expressions to the MODS elements where your map data is stored
* the map's height, width, default zoom level, and whether or not the map is collapsed or expanded by default, and
* option to clean up the data before it is passed to Google Maps.

Once you enable the module, any object whose MODS file contains coordinates in the expected element will have a Google map appended to its display.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## To do

* Add support for non-Google maps.
* Add support for using datastreams other than MODS for map data.
* Add a Drupal permission to "View Islandora Simple Map maps".

## Development and feedback

Pull requests are welcome, as are use cases and suggestions. For example, if your coordinate data results in maps with no points on them, please suggest some ways that the data could be normalized (and don't forget to include some sample data).

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
