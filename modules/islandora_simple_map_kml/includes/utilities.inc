<?php

/**
 * @file
 * General use functions.
 */

/**
 * Menu callback to deliver KML from MODS data.
 */
function _islandora_simple_map_kml_mods(AbstractDatastream $datastream, $position) {
  $xpaths = variable_get('islandora_simple_map_kml_mods_xpaths', ISLANDORA_SIMPLE_MAP_KML_MODS_XPATHS);
  $elements = islandora_simple_map_get_mods_results($datastream->parent, $xpaths);
  _islandora_simple_map_deliver_kml($elements, $position);
}

/**
 * Helper to deliver KML data from elements.
 */
function _islandora_simple_map_deliver_kml($elements, $position) {
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

/**
 * Helper to get KML from MODS data.
 */
function _islandora_simple_map_kml_mods_get_kml(AbstractObject $object) {
  $xpaths = variable_get('islandora_simple_map_kml_mods_xpaths', ISLANDORA_SIMPLE_MAP_KML_MODS_XPATHS);

  $get_value = function (DOMNode $node, $pos) use ($object) {
    return url("islandora/object/{$object->id}/datastream/MODS/islandora_simple_map_kml_mods/$pos/loc.kml", array(
      'absolute' => TRUE,
    ));
  };
  $kml = array();
  foreach (islandora_simple_map_get_mods_results($object, $xpaths) as $pos => $node) {
    $kml[] = $get_value($node, $pos);
  }
  return $kml;
}
