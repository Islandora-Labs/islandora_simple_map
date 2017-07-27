<?php

/**
 * @file
 * Block related functions.
 */

/**
 * Implements hook_block_info().
 */
function islandora_simple_map_block_info() {
  $blocks = [];
  if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
    $blocks['islandora_simple_map_object'] = [
      'info' => t('Islandora Simple Map Object Block'),
      'cache' => DRUPAL_NO_CACHE,
    ];
  }
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function islandora_simple_map_block_view($delta = '') {
  if ($delta == 'islandora_simple_map_object') {
    if (variable_get('islandora_simple_map_use_gmaps_api', FALSE)) {
      $object = menu_get_object('islandora_object', 2);
      if ($object) {
        if (isset($object['MODS'])) {
          $module_path = drupal_get_path('module', 'islandora_simple_map');
          $block = [];
          module_load_include('inc', 'islandora_simple_map', 'includes/utilities');
          $mods_string = $object['MODS']->content;
          $found_coords = islandora_simple_map_parse_mods($mods_string);
          if (count($found_coords) > 0) {
            $map_div_id = drupal_html_id('islandora_simple_map_block');
            $settings = [];
            $settings['islandora_simple_map'][$map_div_id] = [
              'map_markers' => $found_coords,
              'map_div_id' => $map_div_id,
              'map_zoom_level' => (int) variable_get('islandora_simple_map_zoom', '10'),
              'disable_scroll_zoom' => (bool) variable_get('islandora_simple_maps_disable_scroll', FALSE),
            ];
            $block = [
              'content' => [
                'islandora_simple_map_div' => [
                  '#type' => 'container',
                  '#attributes' => [
                    'id' => $map_div_id,
                    'class' => ['islandora-simple-map-holder', 'islandora-simple-map-block'],
                  ],
                ],
                '#attached' => [
                  'js' => [
                    [
                      'type' => 'setting',
                      'data' => $settings,
                    ],
                    [
                      'type' => 'file',
                      'data' => "{$module_path}/js/object_map_markers.js",
                    ],
                    [
                      'type' => 'external',
                      'data' => url("https://maps.googleapis.com/maps/api/js", [
                        'query' => [
                          'key' => variable_get('islandora_simple_map_google_maps_api_key', ''),
                          'callback' => 'Drupal.islandora_simple_map.initialize',
                        ],
                      ]),
                      'defer' => TRUE,
                    ],
                  ],
                  'css' => [
                    [
                      'type' => 'file',
                      'data' => "{$module_path}/css/islandora_simple_map.css",
                    ],
                  ],
                ],
              ],
            ];
          }
          return $block;
        }
      }
    }
  }
}
