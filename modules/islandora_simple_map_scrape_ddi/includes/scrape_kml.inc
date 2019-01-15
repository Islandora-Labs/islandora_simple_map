<?php
/**
 * @file
 * Menu callback.
 */

/**
 * Menu callback; grab the given item.
 */
function _islandora_simple_map_scrape_ddi_get_kml(AbstractDatastream $datastream, $position) {
  module_load_include('inc', 'scrape_ddi', 'includes/utilities');

  $elements = _islandora_simple_map_scrape_ddi_get_kml_elements($datastream);

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
