<?php

/**
 * @file
 * Block related functions.
 */

/**
 * Implements hook_block_info().
 */
function islandora_simple_map_block_info() {
  $blocks = array();
  if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
    $blocks['islandora_simple_map_object'] = array(
      'info' => t('Islandora Simple Map Object Block'),
      'cache' => DRUPAL_CACHE_PER_USER,
    );
  }
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function islandora_simple_map_block_view($delta = '') {
  if ($delta == 'islandora_simple_map_object' || $delta == 'islandora_simple_map_collection') {
    if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
      $object = menu_get_object('islandora_object', 2);
      if ($object) {
        $found_coords = islandora_simple_map_process_coordinates($object);
        $block = array();
        if (count($found_coords['coordinates']) > 0) {
          $map_div_id = drupal_html_id('islandora_simple_map_block');
          $settings = array();
          $settings['islandora_simple_map'][$map_div_id] = array(
            'map_markers' => $found_coords,
            'map_div_id' => $map_div_id,
            'map_zoom_level' => (int) variable_get('islandora_simple_map_zoom', '10'),
            'disable_scroll_zoom' => (bool) variable_get('islandora_simple_map_disable_scroll', FALSE),
          );
          $block = array(
            'content' => array(
              'islandora_simple_map_div' => array(
                '#type' => 'container',
                '#attributes' => array(
                  'id' => $map_div_id,
                  'class' => array('islandora-simple-map-holder', 'islandora-simple-map-block'),
                ),
              ),
              '#attached' => _islandora_simple_map_generate_js_code($settings),
            ),
          );
        }
        return $block;
      }
    }
  }
}
