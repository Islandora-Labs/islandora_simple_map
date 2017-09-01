<?php

/**
 * @file
 * General use functions.
 */

/**
 * Calls the hook_islandora_simple_map_get_coordinates() impls.
 *
 * @param AbstractObject $object
 *   The object being viewed.
 *
 * @return array
 *   Array of unique decimal lat/lng points.
 */
function islandora_simple_map_get_coordinates(AbstractObject $object) {
  $coordinates = (array) module_invoke_all(ISLANDORA_SIMPLE_MAP_COORDINATE_HOOK, $object);
  if (count($coordinates) > 0) {
    // Trim any whitespace.
    array_walk($coordinates, function (&$o) {
      $o = trim($o);
    });
    if (variable_get('islandora_simple_map_attempt_cleanup', TRUE)) {
      array_walk($coordinates, 'islandora_simple_map_clean_coordinates');
    }
    if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
      // Only filter and standardize coordinates if we are using Javascript API.
      $coordinates = array_filter($coordinates, 'islandora_simple_map_is_valid_coordinates');
      array_walk($coordinates, 'islandora_simple_map_standardize_format');
    }
    $coordinates = array_unique($coordinates);
  }
  return $coordinates;
}

/**
 * Implements hook_islandora_simple_map_get_coordinates().
 *
 * Parse a MODS record looking for coordinates.
 */
function islandora_simple_map_islandora_simple_map_get_coordinates(AbstractObject $object) {
  $mods_results = islandora_simple_map_get_mods_results($object);
  $found_coords = [];
  foreach ($mods_results as $node_value) {
    if (strlen(trim(variable_get('islandora_simple_map_coordinate_delimiter', ';'))) > 0) {
      $temp_array = explode(trim(variable_get('islandora_simple_map_coordinate_delimiter', ';')), $node_value);
      foreach ($temp_array as $item) {
        $found_coords[] = $item;
      }
    }
    else {
      $found_coords[] = $node_value;
    }
  }
  return $found_coords;
}

/**
 * Utility function to return the node values of the provided MODS.
 *
 * @param \AbstractObject $object
 *   The Islandora object.
 *
 * @return array
 *   The unaltered node values.
 */
function islandora_simple_map_get_mods_results(AbstractObject $object) {
  $results = [];
  if (isset($object['MODS'])) {
    $mods = $object['MODS']->content;
    $xpaths = preg_split('/$\R?^/m', trim(variable_get('islandora_simple_map_xpath', ISLANDORA_SIMPLE_MAP_XPATHS)));
    $mods_doc = new DOMDocument();
    if ($mods_doc->loadXML($mods)) {
      $mods_xpath = new DOMXPath($mods_doc);
      $mods_xpath->registerNamespace('mods', "http://www.loc.gov/mods/v3");
      foreach ($xpaths as $xpath) {
        $xpath = trim($xpath);
        if (strlen($xpath)) {
          $mods_carto_xpath = @$mods_xpath->query($xpath);
          if ($mods_carto_xpath && $mods_carto_xpath->length > 0) {
            foreach ($mods_carto_xpath as $mods_carto) {
              $node_value = $mods_carto->nodeValue;
              if (strlen($node_value)) {
                $results[] = $node_value;
              }
            }
          }
        }
      }
    }
  }
  return $results;
}

/**
 * Applies some cleanup on data to make it more reliable for Google Maps.
 *
 * @param string $data
 *   The coordinate data.
 */
function islandora_simple_map_clean_coordinates(&$data) {
  $data = preg_replace('/;/', ',', $data);
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
  return ((bool) preg_match('/^[+\-]?\d+(\.\d+)?\s*,\s*[+\-]?\d+(\.\d+)?$/', $coordinates));
}

/**
 * Standardize coordinates to account for whitespace differences,.
 *
 * @param string $coordinates
 *   A decimal coordinate.
 */
function islandora_simple_map_standardize_format(&$coordinates) {
  if (preg_match('/^\s*([+\-]?\d+(\.\d+)?)\s*,\s*([+\-]?\d+(\.\d+))?\s*$/', $coordinates, $matches)) {
    $coordinates = format_string("!lat,!lng", ['!lat' => $matches[1], '!lng' => $matches[3]]);
  }
}
