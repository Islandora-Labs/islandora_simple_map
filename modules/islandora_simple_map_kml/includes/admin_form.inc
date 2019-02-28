<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Admin settings form builder.
 */
function islandora_simple_map_kml_admin_settings_form($form, &$form_state) {
  $form['islandora_simple_map_kml_mimetypes'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable KML Datastream MIME-types'),
    '#description' => t('MIME-types, one per line.'),
    '#default_value' => variable_get('islandora_simple_map_kml_mimetypes', 'application/vnd.google-earth.kml+xml'),
    '#required' => TRUE,
  );
  $form['islandora_simple_map_kml_dsids'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable KML Datastream IDs'),
    '#description' => t('The IDs to which to limit the MIME-type match, one per line. If left blank, matches will be attempted on <em>all</em> datastreams.'),
    '#default_value' => variable_get('islandora_simple_map_kml_dsids', ''),
  );
  $form['islandora_simple_map_kml_models'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable Object models for displaying KML'),
    '#description' => t('Only objects with the given models (one per line) will be matched. Should be the bare name of model PIDs, for example: "islandora:collectionCModel". If left blank, matches will be attempted on <em>all</em> datastreams.'),
    '#default_value' => variable_get('islandora_simple_map_kml_models', ''),
  );
  $form['islandora_simple_map_kml_mods_xpaths'] = array(
    '#title' => t('XPath expressions to MODS elements containing KML'),
    '#type' => 'textarea',
    '#default_value' => variable_get('islandora_simple_map_kml_mods_xpaths', ISLANDORA_SIMPLE_MAP_KML_MODS_XPATHS),
    '#description' => t("Enter one XPath expression per line, in preferred order with the most preferred first. The first data found in the object's MODS datastream by this list of XPath expressions will be used to populate the map."),
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
function islandora_simple_map_kml_admin_settings_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];
  $variables = array(
    'islandora_simple_map_kml_mimetypes',
    'islandora_simple_map_kml_dsids',
    'islandora_simple_map_kml_models',
  );
  foreach ($variables as $variable) {
    variable_set($variable, $values[$variable]);
  }
  drupal_set_message(t("Configuration settings saved"));
}