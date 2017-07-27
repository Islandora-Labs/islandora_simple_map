<?php

/**
 * @file
 * General use functions.
 */

/**
 * Parse a MODS record looking for coordinates.
 *
 * @param string $mods
 *   The MODS document.
 *
 * @return array
 *   Array of Lat/Lng coords.
 */
function islandora_simple_map_parse_mods($mods) {
  $xpaths = preg_split('/$\R?^/m', trim(variable_get('islandora_simple_map_xpath', ISLANDORA_SIMPLE_MAP_XPATHS)));
  $found_coords = [];

  $mods_doc = new DOMDocument();
  if ($mods_doc->loadXML($mods)) {
    foreach ($xpaths as $xpath) {
      $xpath = trim($xpath);
      if (strlen($xpath)) {
        $mods_xpath = new DOMXPath($mods_doc);
        $mods_xpath->registerNamespace('mods', "http://www.loc.gov/mods/v3");
        $mods_carto_xpath = @$mods_xpath->query($xpath);
        if ($mods_carto_xpath && $mods_carto_xpath->length > 0) {
          // Take the first element found by the current XPath.
          $mods_carto = $mods_carto_xpath->item(0);
          $node_value = $mods_carto->nodeValue;
          if ((bool) variable_get('islandora_simple_map_attempt_cleanup', TRUE)) {
            $node_value = islandora_simple_map_clean_coordinates($node_value);
          }
          if (strlen($node_value)) {
            if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
              if (islandora_simple_map_is_valid_coordinates($node_value)) {
                // Filter non-coordinates if using the Javascript API.
                $found_coords = explode(trim(variable_get('islandora_simple_map_coordinate_delimiter', ';')), $node_value);
              }
            }
            else {
              $found_coords[] = $node_value;
            }
          }
        }
      }
    }
  }
  return $found_coords;
}

/**
 * Applies some cleanup on data to make it more reliable for Google Maps.
 *
 * @param string $data
 *   The coordinate data.
 *
 * @return string
 *   The cleaned up data.
 */
function islandora_simple_map_clean_coordinates($data) {
  $data = preg_replace('/;/', ',', $data);
  return $data;
}

/**
 * Validate a coordinate, so we don't add text locations.
 *
 * @param string $coordinates
 *   Whatever was returned by the configured XPath.
 *
 * @return bool
 *   If it is a valid coordinate.
 */
function islandora_simple_map_is_valid_coordinates($coordinates) {
  $coordinates = trim($coordinates);
  $val = ((bool) preg_match('/^[+\-]?\d+(\.\d+)?\s*,\s*[+\-]?\d+(\.\d+)?$/', $coordinates));
  return $val;
}
