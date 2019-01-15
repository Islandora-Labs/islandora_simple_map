<?php
/**
 * @file
 * Helper functions.
 */

/**
 * Helper to parse textarea variable values to arrays.
 *
 * @param string $paths
 *   A string of XPaths separated with Windows line endings, as provided by
 *   textareas.
 *
 * @return string[]
 *   An array of XPaths, as strings.
 */
function _islandora_simple_map_scrape_mods_parse_paths($paths) {
  return array_filter(array_map('trim', explode("\r\n", $paths)));
}

/**
 * Helper to perform the XPath queries.
 *
 * @param AbstractObject $object
 *   The object from which to scrape the MODS.
 * @param array $xpaths
 *   The array of XPaths to run, as strings.
 *
 * @return DOMNode[]
 *   An array of DOMNode instances resulting from the XPath queries.
 */
function _islandora_simple_map_scrape_mods_scrape(AbstractObject $object, array $xpaths) {
  if (!isset($object['MODS'])) {
    return array();
  }

  $doc = new DOMDocument();
  $doc->loadXML($object['MODS']->content);

  $xpath = new DOMXPath($doc);
  // XXX: May need to expose the ability to register namespaces at some point?
  $xpath->registerNamespace('mods', 'http://www.loc.gov/mods/v3');

  return call_user_func_array('array_merge', array_map('iterator_to_array', array_map(array($xpath, 'query'), $xpaths)));
}

/**
 * Callback helper to actually parse the configured XPaths.
 *
 * @param AbstractDatastream $datastream
 *   The datastream from which to scrape elements.
 *
 * @return DOMNode[]
 *   An array of DOMNode instances, scraped from the datastream.
 */
function _islandora_simple_map_scrape_mods_get_kml_elements(AbstractDatastream $datastream) {
  $xpaths = _islandora_simple_map_scrape_mods_parse_paths(variable_get('islandora_simple_map_scrape_mods_kml_xpaths', SCRAPE_MODS_KML_XPATHS));
  return _islandora_simple_map_scrape_mods_scrape($datastream->parent, $xpaths);
}
