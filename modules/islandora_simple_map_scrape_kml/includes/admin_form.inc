<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Admin settings form builder.
 */
function islandora_simple_map_scrape_kml_admin_settings_form($form, &$form_state) {
  $form['islandora_simple_map_scrape_kml_mimetypes'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable KML Datastream MIME-types'),
    '#description' => t('MIME-types, one per line.'),
    '#default_value' => variable_get('islandora_simple_map_scrape_kml_mimetypes', 'application/vnd.google-earth.kml+xml'),
    '#required' => TRUE,
  );
  $form['islandora_simple_map_scrape_kml_dsids'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable KML Datastream IDs'),
    '#description' => t('The IDs to which to limit the MIME-type match, one per line. If left blank, matches will be attempted on <em>all</em> datastreams.'),
    '#default_value' => variable_get('islandora_simple_map_scrape_kml_dsids', ''),
  );
  $form['islandora_simple_map_scrape_kml_models'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowable Object models for displaying KML'),
    '#description' => t('Only objects with the given models (one per line) will be matched. Should be the bare name of model PIDs, for example: "islandora:collectionCModel". If left blank, matches will be attempted on <em>all</em> datastreams.'),
    '#default_value' => variable_get('islandora_simple_map_scrape_kml_models', ''),
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
function islandora_simple_map_admin_settings_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];
  variable_set('islandora_simple_map_scrape_kml_mimetypes', $values['islandora_simple_map_scrape_kml_mimetypes']);
  variable_set('islandora_simple_map_scrape_kml_dsids', $values['islandora_simple_map_scrape_kml_dsids']);
  variable_set('islandora_simple_map_scrape_kml_dsids', $values['islandora_simple_map_scrape_kml_models']);
  drupal_set_message(t("Configuration settings saved"));
}