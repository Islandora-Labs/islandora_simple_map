<?php

/**
 * @file
 * General use functions.
 */

/**
 * Menu callback to deliver KML from MODS data.
 */
function _islandora_simple_map_kml_mods(AbstractDatastream $datastream, $position) {
  $xpaths = preg_split('/$\R?^/m', trim(variable_get('islandora_simple_map_kml_mods_xpaths', ISLANDORA_SIMPLE_MAP_KML_MODS_XPATHS)));
  $elements = islandora_simple_map_get_mods_results($datastream->parent, $xpaths);

  if (!isset($elements[$position])) {
    drupal_not_found();
    return;
  }

  $element = $elements[$position];
  drupal_add_http_header('Content-Type', 'application/vnd.google-earth.kml+xml');
  $doc = new DOMDocument("1.0", "UTF-8");
  $doc->preserveWhitespace = FALSE;
  $doc->formatOutput = TRUE;
  $node = $doc->importNode($element, TRUE);
  $doc->appendChild($node);
  print $doc->saveXML();
  drupal_exit();
}

