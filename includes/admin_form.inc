<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Admin settings form builder.
 */
function islandora_simple_map_admin_settings() {
  $form = array();
  $form['google_api'] = [
    '#type' => 'fieldset',
    '#title' => t('Google Maps API'),
    'islandora_simple_map_use_gmaps_api' => [
      '#type' => 'checkbox',
      '#title' => t('Use Google Maps API'),
      '#description' => t('Use the Google Maps API, this requires registering for an API key.'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_map_use_gmaps_api', FALSE),
    ],
    'islandora_simple_map_google_maps_api_key' => [
      '#type' => 'textfield',
      '#title' => t('Google Maps API key'),
      '#description' => format_string('A Google Maps API key, register for one <a href="@url">here</a>',
        ['@url' => 'https://developers.google.com/maps/documentation/javascript/get-api-key']),
      '#length' => 30,
      '#maxlength' => 255,
      '#default_value' => variable_get('islandora_simple_map_google_maps_api_key', ''),
      '#states' => [
        'required' => [
          ':input[name="islandora_simple_map_use_gmaps_api"]' => ['checked' => TRUE],
        ],
        'visible' => [
          ':input[name="islandora_simple_map_use_gmaps_api"]' => ['checked' => TRUE],
        ],
      ],
    ],
    'islandora_simple_maps_disable_scroll' => [
      '#type' => 'checkbox',
      '#title' => t('Disable mouse wheel scroll?'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_maps_disable_scroll', FALSE),
      '#states' => [
        'visible' => [
          ':input[name="islandora_simple_map_use_gmaps_api"]' => ['checked' => TRUE],
        ],
      ],
    ],
    'islandora_simple_maps_disable_page_display' => [
      '#type' => 'checkbox',
      '#title' => t('Disable collapsible div map.'),
      '#description' => t('Using the Google Map API you can display the map in a block. This turns off the collapsible div map embed in the page.'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_maps_disable_page_display', FALSE),
      '#states' => [
        'visible' => [
          ':input[name="islandora_simple_map_use_gmaps_api"]' => ['checked' => TRUE],
        ],
      ],
    ],
  ];
  $form['islandora_simple_map_xpath'] = array(
    '#title' => t('XPath expressions to MODS elements containing map data'),
    '#type' => 'textarea',
    '#default_value' => variable_get('islandora_simple_map_xpath', ISLANDORA_SIMPLE_MAP_XPATHS),
    '#description' => t("Enter one XPath expression per line, in preferred order with the most preferred first. The first data found in the object's MODS datastream by this list of XPath expressions will be used to populate the map."),
  );
  $form['islandora_simple_map_collapsed'] = array(
    '#type' => 'select',
    '#title' => t('Collapse map by default?'),
    '#default_value' => variable_get('islandora_simple_map_collapsed', 'collapsed'),
    '#options' => array(
      'collapsed' => t('Collapsed'),
      '' => t('Expanded'),
    ),
    '#description' => t('Whether or not the fieldset containing the map should be collapsed by default. The user can toggle the collapse.'),
  );
  $form['islandora_simple_map_iframe_width'] = array(
    '#title' => t('Map iframe width'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_simple_map_iframe_width', '600'),
    '#description' => t('The width, in pixels, of the iframe the map will appear in.'),
  );
  $form['islandora_simple_map_iframe_height'] = array(
    '#title' => t('Map iframe height'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_simple_map_iframe_height', '600'),
    '#description' => t('The height, in pixels, of the iframe the map will appear in.'),
  );
  $form['islandora_simple_map_zoom'] = array(
    '#title' => t('Default zoom level'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_simple_map_zoom', '10'),
    '#description' => t('The higher the number, the higher the zoom.'),
  );
  $form['islandora_simple_map_attempt_cleanup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attempt to clean up map data.'),
    '#default_value' => variable_get('islandora_simple_map_attempt_cleanup', 1),
    '#description' => t('Check this option if you want to clean up data before passing it off to Google Maps. Please consult the README file for more information'),
  );
  $form['#validate'][] = 'islandora_simple_map_admin_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Custom conditional required validation.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_simple_map_admin_settings_form_validate(array $form, array &$form_state) {
  if ($form_state['values']['islandora_simple_map_use_gmaps_api'] == TRUE &&
    empty($form_state['values']['islandora_simple_map_google_maps_api_key'])) {
    form_set_error('islandora_simple_map_google_maps_api_key',
      'You must include a Google Maps API key or not use the Google Maps API.');
  }
}
