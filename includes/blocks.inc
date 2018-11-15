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
  if (_islandora_simple_map_multiple_coord_support()) {
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
    if (_islandora_simple_map_multiple_coord_support()) {
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
              '#attached' => _islandora_simple_map_generate_js_code($settings, $map_div_id),
            ),
          );
        }
        return $block;
      }
    }
  }
}
