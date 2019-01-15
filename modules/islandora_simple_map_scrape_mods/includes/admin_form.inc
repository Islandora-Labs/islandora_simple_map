<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Admin settings form builder.
 */
function islandora_simple_map_scrape_mods_admin_settings_form($form, &$form_state) {
  $form['islandora_simple_map_scrape_mods_xpaths'] = array(
    '#type' => 'textarea',
    '#title' => t('Coordinate XPaths'),
    '#default_value' => variable_get('islandora_simple_map_scrape_mods_xpaths', SCRAPE_MODS_XPATHS),
  );
  $form['islandora_simple_map_scrape_mods_kml_xpaths'] = array(
    '#type' => 'textarea',
    '#title' => t('KML XPaths'),
    '#default_value' => variable_get('islandora_simple_map_scrape_mods_kml_xpaths', SCRAPE_MODS_KML_XPATHS),
    '#description' => t('XPaths matching the root &lt;kml/&gt; element of embedded KML documents.'),
  );
  $form['islandora_simple_map_scrape_mods_attempt_cleanup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attempt cleanup?'),
    '#default_value' => variable_get('islandora_simple_map_scrape_mods_attempt_cleanup', FALSE),
    '#description' => t('Ideally, values should be comma-separated, signed floating-point numbers indicating latitude and longitude respectively... Attempt some parsing to match expectations.'),
  );
  $form['islandora_simple_map_scrape_mods_fallback_to_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Search fallback?'),
    '#default_value' => variable_get('islandora_simple_map_scrape_mods_fallback_to_search', FALSE),
    '#description' => t('To support more human-readable values, such as "Charlottetown, PE, Canada", fallback to passing non-coordinate data as a text search to <a href="@api_url">Google Places API</a>. Notice the reference in the API documentation regarding higher (10-times) API quota usage.', array(
      '@api_url' => 'https://developers.google.com/places/web-service/search#TextSearchRequests',
    )),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Implements hook_form_submit().
 */
function islandora_simple_map_scrape_mods_admin_settings_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];
  $variables = array(
    'islandora_simple_map_scrape_mods_xpaths',
    'islandora_simple_map_scrape_mods_kml_xpaths',
    'islandora_simple_map_scrape_mods_attempt_cleanup',
    'islandora_simple_map_scrape_mods_fallback_to_search',
  );
  foreach ($variables as $variable) {
    variable_set($variable, $values[$variable]);
  }
  drupal_set_message(t("Configuration settings saved"));
}