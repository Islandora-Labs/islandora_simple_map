<?php

/**
 * @file
 * General use functions.
 */

/**
 * Calls the hooks to retrieve coordinates and parse them.
 *
 * @param \AbstractObject $object
 *   The object being viewed.
 *
 * @return array
 *   An array of unique coordinates.
 */
function islandora_simple_map_process_coordinates(AbstractObject $object) {
  $coordinates = islandora_simple_map_get_coordinates($object);
  if (count($coordinates) > 0) {
    $coordinates = islandora_simple_map_parse_coordinates($coordinates);
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
  return array('pid' => $object->id, 'coordinates' => $coordinates);
}

/**
 * Calls the hook_islandora_simple_map_get_coordinates() impls.
 *
 * @param AbstractObject $object
 *   The object being viewed.
 *
 * @return array
 *   Array of coordinates.
 */
function islandora_simple_map_get_coordinates(AbstractObject $object) {
  $coordinates = (array) module_invoke_all(ISLANDORA_SIMPLE_MAP_COORDINATE_HOOK, $object);
  if (count($coordinates) > 0) {
    // Trim any whitespace.
    array_walk($coordinates, function (&$o) {
      $o = trim($o);
    });
  }
  return $coordinates;
}

/**
 * Calls hook_islandora_simple_map_parse_coordinates() impl with coordinates.
 *
 * @param array $coordinates
 *   Coordinates to be parsed to a usable format.
 *
 * @return array
 *   The array of altered coordinates.
 */
function islandora_simple_map_parse_coordinates(array $coordinates) {
  $hooks = (array) module_invoke_all(ISLANDORA_SIMPLE_MAP_PARSE_HOOK);
  uasort($hooks, function ($a, $b) {
                   if (!isset($a['weight'])) {
                     $a['weight'] = 100;
                   }
                   if (!isset($b['weight'])) {
                     $b['weight'] = 100;
                   }
    return (abs($a['weight']) <= abs($b['weight']) ? -1 : 1);
  });

  // Don't need to parse coordinates already in decimal notation.
  $processed = array_filter($coordinates, 'islandora_simple_map_is_valid_coordinates');
  $coordinates = array_diff($coordinates, $processed);
  $to_process = array_combine($coordinates, $coordinates);
  foreach ($hooks as $hook) {
    if (isset($hook['file'])) {
      require_once $hook['file'];
    }
    if (function_exists($hook['function_name'])) {
      $return_coords = call_user_func($hook['function_name'], array_values($to_process));
      $to_process = array_diff_key($to_process, $return_coords);
      $processed = array_merge($processed, array_values($return_coords));
    }
    if (count($to_process) == 0) {
      // No more to process so stop hitting hooks.
      break;
    }
  }
  return $processed;
}

/**
 * Implements hook_islandora_simple_map_get_coordinates().
 */
function islandora_simple_map_islandora_simple_map_get_coordinates(AbstractObject $object) {
  $found_coords = array();
  if (module_exists("islandora_solr") && variable_get('islandora_simple_map_use_solr', FALSE)) {
    $found_coords = _islandora_simple_map_get_coordinates_solr($object);
  }
  else {
    $found_coords = _islandora_simple_map_get_coordinates_parse_mods($object);
  }
  return $found_coords;
}

/**
 * Parse a MODS record looking for coordinates.
 *
 * @param AbstractObject $object
 *   Object to find coordinates for.
 *
 * @return array
 *   Array of raw coordinates.
 */
function _islandora_simple_map_get_coordinates_parse_mods(AbstractObject $object) {
  $mods_results = islandora_simple_map_get_mods_results($object);
  $found_coords = array();
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
 * Search Solr for coordinates.
 *
 * @param AbstractObject $object
 *   Object to find coordinates for.
 *
 * @return array
 *   Array of raw coordinates.
 */
function _islandora_simple_map_get_coordinates_solr(AbstractObject $object) {
  $coord_field = variable_get('islandora_simple_map_coordinate_solr_field', '');
  $found_coords = array();
  $solr_build = new IslandoraSolrQueryProcessor();
  $params = array(
    'fl' => "PID,{$coord_field}",
  );
  $solr_query = format_string("PID:\"!pid\" OR PID:\"info:fedora/!pid\"",
    array(
      '!pid' => $object->id,
    )
  );
  try {
    $solr_build->buildQuery($solr_query, $params);
    _islandora_simple_map_remove_search_restrictions($solr_build->solrParams);
    $solr_build->executeQuery(FALSE);
    $results = $solr_build->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $member = reset($results);
  if (isset($member['solr_doc'][$coord_field])) {
    if (is_array($member['solr_doc'][$coord_field])) {
      $found_coords = $member['solr_doc'][$coord_field];
    }
    else {
      $found_coords = array($member['solr_doc'][$coord_field]);
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
  $results = array();
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
 * Standardize coordinates to account for whitespace differences.
 *
 * @param string $coordinates
 *   A decimal coordinate.
 */
function islandora_simple_map_standardize_format(&$coordinates) {
  if (preg_match('/^\s*([+\-]?\d+(\.\d+)?)\s*,\s*([+\-]?\d+(\.\d+))?\s*$/', $coordinates, $matches)) {
    $coordinates = format_string("!lat,!lng", array('!lat' => $matches[1], '!lng' => $matches[3]));
  }
}

/**
 * Utility to check if we display the map for this collection.
 *
 * @param string $pid
 *   The object pid.
 *
 * @return bool
 *   Whether the collection is set to display a map.
 */
function _islandora_simple_map_display_collection_map($pid) {
  $results = db_query('SELECT pid from {islandora_simple_map_collections_map} WHERE pid = :pid',
    array(':pid' => $pid));
  foreach ($results as $r) {
    if ($r->pid == $pid) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Utility function check if we can/should display collection maps.
 *
 * @return bool
 *   If collection maps can operate.
 */
function _islandora_simple_map_collection_map_enabled() {
  return (variable_get('islandora_simple_map_use_gmaps_api', FALSE)
    && variable_get('islandora_simple_map_collection_maps', FALSE)
    && module_exists('islandora_basic_collection'));
}

/**
 * Get all the collection points from a collection.
 *
 * @param \AbstractObject $object
 *   The collection object.
 *
 * @return array
 *   The collection points (un-parsed).
 */
function _islandora_simple_map_get_collection_points(AbstractObject $object) {
  $points = array();
  if (module_exists('islandora_solr') &&
    variable_get('islandora_simple_map_use_solr', FALSE)) {
    $points = _islandora_simple_map_get_collection_points_solr($object);
  }
  elseif (module_exists('islandora_basic_collection')) {
    $points = _islandora_simple_map_get_collection_points_load($object);
  }
  return $points;
}

/**
 * Get all the collection points from a collection by parsing MODS records.
 *
 * @param \AbstractObject $object
 *   The collection object.
 *
 * @return array
 *   The collection points (un-parsed).
 */
function _islandora_simple_map_get_collection_points_load(AbstractObject $object) {
  $points = array();
  $page_num = 0;
  $page_size = 20;
  $count = NULL;
  $members = array();
  while (is_null($count) || ($count > ($page_num * $page_size) + $page_size)) {
    list($count, $new_members) = islandora_basic_collection_get_member_objects($object, $page_num, $page_size);
    $members = array_merge($members, $new_members);
    $page_num += 1;
  }
  if (count($members) > 0) {
    foreach ($members as $member) {
      $pid = $member['object']['value'];
      $object = islandora_object_load($pid);
      if ($object) {
        $coords = islandora_simple_map_process_coordinates($object);
        if (isset($coords['coordinates']) && count($coords['coordinates']) > 0) {
          $points[] = $coords;
        }
      }
    }
  }
  return $points;
}

/**
 * Get all the collection points from a collection by querying Solr.
 *
 * @param \AbstractObject $object
 *   The collection object.
 *
 * @return array
 *   The collection points (un-parsed).
 */
function _islandora_simple_map_get_collection_points_solr(AbstractObject $object) {
  $points = array();
  $member_field = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
  $coord_field = variable_get('islandora_simple_map_coordinate_solr_field', '');
  $page_num = 0;
  $page_size = 20;
  $count = NULL;
  $members = array();
  $solr_build = new IslandoraSolrQueryProcessor();
  $solr_query = format_string("!member:\"!pid\" OR !member:\"info:fedora/!pid\"",
    array(
      '!member' => $member_field,
      '!pid' => $object->id,
    )
  );
  while (is_null($count) || ($count > ($page_num * $page_size) + $page_size)) {
    $params = array(
      'rows' => $page_size,
      'start' => $page_num,
      'fl' => "PID,{$coord_field}",
    );
    try {
      $solr_build->buildQuery($solr_query, $params);
      _islandora_simple_map_remove_search_restrictions($solr_build->solrParams);
      $solr_build->executeQuery(FALSE);
      $results = $solr_build->islandoraSolrResult;
      $count = $results['response']['numFound'];
      $members = array_merge($members, $results['response']['objects']);
      $page_num += 1;
    }
    catch (Exception $e) {
      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
      break;
    }
  }
  foreach ($members as $member) {
    if (isset($member['solr_doc'][$coord_field])) {
      if (is_array($member['solr_doc'][$coord_field])) {
        $coordinates = $member['solr_doc'][$coord_field];
      }
      else {
        $coordinates = array($member['solr_doc'][$coord_field]);
      }
      $coordinates = islandora_simple_map_parse_coordinates($coordinates);
      if (variable_get('islandora_simple_map_attempt_cleanup', TRUE)) {
        array_walk($coordinates, 'islandora_simple_map_clean_coordinates');
      }
      if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
        // Only filter/standardize coordinates if we are using Javascript API.
        $coordinates = array_filter($coordinates, 'islandora_simple_map_is_valid_coordinates');
        array_walk($coordinates, 'islandora_simple_map_standardize_format');
      }

      $points[] = array(
        'pid' => $member['PID'],
        'coordinates' => $coordinates,
      );
    }
  }
  $points = array_filter($points, function($o) {
    return count($o['coordinates'] > 0);
  });
  return $points;
}

/**
 * Utility function to centralize Google Maps javascript code array.
 *
 * @param array $settings
 *   The array of Google Maps settings.
 *
 * @return array
 *   The #attached array.
 */
function _islandora_simple_map_generate_js_code(array $settings) {
  $module_path = drupal_get_path('module', 'islandora_simple_map');
  return array(
    'js' => array(
      'misc/collapse.js',
      'misc/form.js',
      array(
        'type' => 'setting',
        'data' => $settings,
      ),
      array(
        'type' => 'file',
        'data' => "{$module_path}/js/object_map_markers.js",
      ),
      array(
        'type' => 'external',
        'data' => url("https://maps.googleapis.com/maps/api/js", array(
          'query' => array(
            'key' => variable_get('islandora_simple_map_google_maps_api_key', ''),
            'callback' => 'Drupal.islandora_simple_map.initialize',
          ),
        )),
        'defer' => TRUE,
      ),
    ),
    'css' => array(
      array(
        'type' => 'file',
        'data' => "{$module_path}/css/islandora_simple_map.css",
      ),
    ),
  );
}

/**
 * Remove search alters that stop us from getting any data from Solr.
 *
 * @param array $params
 *   The islandoraSolrQueryProcessor params
 */
function _islandora_simple_map_remove_search_restrictions(array &$params) {
  if (module_exists('islandora_compound_object') &&
    variable_get('islandora_compound_object_hide_child_objects_solr', TRUE)) {
    $fq = variable_get('islandora_compound_object_solr_fq', '-RELS_EXT_isConstituentOf_uri_mt:[* TO *]');
    $params['fq'] = array_diff($params['fq'], array($fq));
  }
}
