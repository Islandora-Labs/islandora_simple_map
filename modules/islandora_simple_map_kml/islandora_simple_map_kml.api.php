<?php

/**
 * @file
 * Hook definition.
 */

/**
 * Get KML documents to render on a map.
 *
 * @param AbstractObject $object
 *   The object for which the KML is to be gathered.
 *
 * @return array
 *   An array of URLs pointing at publically-accessible KML documents.
 */
function hook_islandora_simple_map_get_kml(AbstractObject $object) {
  return array(
    'http://googlemaps.github.io/kml-samples/kml/Placemark/placemark.kml',
  );
}

/**
 * Permit altering of KML hook values.
 *
 * @param array $kml
 *   A reference to the array of KML documents gathered.
 * @param AbstractObject $object
 *   The object for which KML is being gathered.
 *
 * @see hook_islandora_gmap_get_kml()
 */
function hook_islandora_simple_map_get_kml_alter(array &$kml, AbstractObject $object) {
}
