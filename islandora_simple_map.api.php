<?php

/**
 * @file
 * Hook definition.
 */

/**
 * Get coordinates from places other than MODS and return them.
 *
 * @param AbstractObject $object
 *   The Islandora object.
 *
 * @return array
 *   Array of coordinates.
 */
function hook_islandora_simple_map_get_coordinates(AbstractObject $object) {
  if (isset($object['coordinates'])) {
    $datastream_content = trim($object['coordinates']->content);
    $coordinates = array();
    $temporary_array = explode(';', $datastream_content);
    foreach ($temporary_array as $coord) {
      $coordinates[] = $coord;
    }
    return $coordinates;
  }
}

/**
 * Register a function to parse a type of coordinate.
 *
 * @return array
 *   Associative array with the format below.
 *   array(
 *     'function_name' => 'name of function to call',
 *     'file' => 'path to file to include for accessing above function.',
 *     'weight' => 'positive integer for setting an order',
 *   );
 *
 *   The callable 'function_name' takes an array of coordinates and returns an
 *   associative array where the key is the original coordinate and the value is
 *   the parsed version.
 */
function hook_islandora_simple_map_parse_coordinates_callback() {
  return array(
    'my_module_implementation' => array(
      'function_name' => 'islandora_test_parse_coordinates',
      'file' => drupal_get_path('module', 'islandora_simple_map') .
      'includes/test_functions.inc',
      'weight' => 100,
    ),
  );
}

/**
 * Get GeoJSON Feature objects for rendering on a map for the given object.
 *
 * @param AbstractObject $object
 *   The object for which to gather GeoJSON Features.
 *
 * @return array
 *   An array of GeoJSON Features.
 *
 * @see http://geojson.org/geojson-spec.html#feature-objects
 */
function hook_islandora_simple_map_get_geojson(AbstractObject $object) {
  $geojson = array();

  $geojson[] = array(
    'type' => 'Feature',
    'geometry' => array(
      'type' => 'Point',
      'coordinates' => array(
        -63.1245071,
        46.2350236,
      ),
    ),
  );

  return $geojson;
}

/**
 * Permit altering of GeoJSON hook values.
 *
 * @param array $geojson
 *   A reference to the array of GeoJSON features gathered.
 * @param AbstractObject $object
 *   The object for which GeoJSON is being gathered.
 *
 * @see hook_islandora_gmap_get_geojson()
 */
function hook_islandora_simple_map_get_geojson_alter(array &$geojson, AbstractObject $object) {
}
