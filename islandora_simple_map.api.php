<?php

/**
 * @file
 * Hook definition.
 */

/**
 * Get alternate coordinate types and return them as decimal coordinates.
 *
 * @param AbstractObject $object
 *   The Islandora object.
 *
 * @return array
 *   Array of coordinates in decimal format.
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
