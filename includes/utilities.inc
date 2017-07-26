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
  $mods_doc->loadXML($mods);
  if ($mods_doc) {
    foreach ($xpaths as $xpath) {
      $xpath = trim($xpath);
      if (strlen($xpath)) {
        $mods_xpath = new DOMXPath($mods_doc);
        $mods_xpath->registerNamespace('mods', "http://www.loc.gov/mods/v3");
        $mods_carto_xpath = $mods_xpath->query($xpath);
        if ($mods_carto_xpath->length) {
          // Take the first element found by the current XPath.
          $mods_carto = $mods_carto_xpath->item(0);
          $node_value = $mods_carto->nodeValue;
          if (variable_get('islandora_simple_map_attempt_cleanup', 1)) {
            $node_value = islandora_simple_map_clean_coordinates($node_value);
          }
          if (strlen($node_value)) {
            $found_coords[] = $node_value;
          }
        }
      }
    }
  }
  return $found_coords;
}
