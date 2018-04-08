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
  $form['google_api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Maps Javascript API'),
    'islandora_simple_map_use_gmaps_api' => array(
      '#type' => 'checkbox',
      '#title' => t('Use Google Maps Javascript API'),
      '#description' => t('Use the Google Maps Javascript API, this requires registering for an API key.'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_map_use_gmaps_api', FALSE),
    ),
    'islandora_simple_map_google_maps_api_key' => array(
      '#type' => 'textfield',
      '#title' => t('Google Maps API key'),
      '#description' => format_string('A Google Maps API key, register for one <a href="@url">here</a>',
        array('@url' => 'https://developers.google.com/maps/documentation/javascript/get-api-key')),
      '#length' => 30,
      '#maxlength' => 255,
      '#default_value' => variable_get('islandora_simple_map_google_maps_api_key', ''),
      '#states' => array(
        'required' => array(
          ':input[name="islandora_simple_map_use_gmaps_api"]' => array('checked' => TRUE),
        ),
        'visible' => array(
          ':input[name="islandora_simple_map_use_gmaps_api"]' => array('checked' => TRUE),
        ),
      ),
    ),
    'islandora_simple_map_disable_scroll' => array(
      '#type' => 'checkbox',
      '#title' => t('Disable mouse wheel scroll?'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_map_disable_scroll', FALSE),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_simple_map_use_gmaps_api"]' => array('checked' => TRUE),
        ),
      ),
    ),
    'islandora_simple_map_disable_page_display' => array(
      '#type' => 'checkbox',
      '#title' => t('Disable collapsible div map.'),
      '#description' => t('Using the Google Map API you can display the map in a block. This turns off the collapsible div map embed in the page.'),
      '#return_value' => TRUE,
      '#default_value' => variable_get('islandora_simple_map_disable_page_display', FALSE),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_simple_map_use_gmaps_api"]' => array('checked' => TRUE),
        ),
      ),
    ),
    'islandora_simple_map_collection_maps' => array(
      '#type' => 'checkbox',
      '#title' => t('Enable collection level maps?'),
      '#description' => t('Aggregate object markers inside a collection into a single map for the collection.'),
      '#default_value' => variable_get('islandora_simple_map_collection_maps', FALSE),
      '#return_value' => TRUE,
    ),
  );
  $form['islandora_simple_map_solr_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Solr settings'),
    'islandora_simple_map_use_solr' => array(
      '#type' => 'checkbox',
      '#title' => t('Use Solr for object map queries.'),
      '#description' => t('Use Solr to query the coordinates of an object and/or collection.'),
      '#default_value' => variable_get('islandora_simple_map_use_solr', FALSE),
    ),
    'islandora_simple_map_solr_wrapper' => array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_simple_map_use_solr"]' => array('checked' => TRUE),
        ),
      ),
      'islandora_simple_map_coordinate_solr_field' => array(
        '#type' => 'textfield',
        '#title' => t('Coordinates Solr field.'),
        '#description' => t('Solr field with the coordinates from a record, can be multivalued.'),
        '#default_value' => variable_get('islandora_simple_map_coordinate_solr_field', ''),
        '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
      ),
    ),
  );
  $form['islandora_simple_map_coordinate_delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Coordinate delimiter'),
    '#description' => t('Character or string used to separate multiple coordinates inside the MODS element, leave blank to not split.'),
    '#default_value' => variable_get('islandora_simple_map_coordinate_delimiter', ';'),
  );
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
    '#description' => t('The higher the number, the higher the zoom. If you have multiple points on a map, the zoom level may be lowered to fit all points on the map.'),
  );
  $form['islandora_simple_map_attempt_cleanup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attempt to clean up map data.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('islandora_simple_map_attempt_cleanup', TRUE),
    '#description' => t('Check this option if you want to clean up data before passing it off to Google Maps. Please consult the README file for more information'),
  );
  $form['islandora_simple_map_omit_compound_display'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't display maps for compound parents."),
    '#description' => t("Compounds render as themselves and their first child, so you get two maps. This removes the compound's map."),
    '#return_value' => TRUE,
    '#default_value' => variable_get('islandora_simple_map_omit_compound_display', TRUE),
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
  if (empty($form_state['values']['islandora_simple_map_coordinate_solr_field'])
    && $form_state['values']['islandora_simple_map_use_solr'] == TRUE) {
    form_set_error('islandora_simple_map_coordinate_solr_field',
      'You must specify a field for coordinates to use Solr.');
  }
}

/**
 * Implements hook_form().
 *
 * Determines if we are displaying the collection list or a confirmation screen.
 */
function islandora_simple_map_admin_collection_display($form, &$form_state) {
  if (isset($form_state['values']['collection_listing'])) {
    $to_disable = array_filter($form_state['values']['collection_listing']);
    if ($to_disable) {
      return _islandora_simple_map_islandora_collection_delete_confirm($form_state);
    }
  }
  return _islandora_simple_map_admin_collection_display_form($form_state);

}

/**
 * Implements hook_form().
 *
 * Display all the collections that have maps enabled in a single location.
 */
function _islandora_simple_map_admin_collection_display_form(&$form_state) {
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
  );
  $query = db_select('islandora_simple_map_collections_map', 'n');
  $nids = $query
    ->fields('n', array('pid', 'title'))
    ->extend('PagerDefault')->limit(50)
    ->execute();

  $results = $nids->fetchAllAssoc('pid', PDO::FETCH_ASSOC);

  $form = array(
    'islandora_simple_maps_collections' => array(
      '#type' => 'fieldset',
      '#title' => t('Collections with maps.'),
      '#description' => t('List of collections with maps enabled.'),
      'map_warning' => array(
        '#type' => 'markup',
        '#markup' => '<p><b>NOTE:</b> You can disable individual or multiple maps here, but you must add them at the individual collection manage pages.</p>',
      ),
      'collection_listing' => array(
        '#type' => 'tableselect',
        '#title' => t('Collections'),
        '#header' => $header,
        '#options' => $results,
        '#empty' => t('There are no collections with maps enabled'),
      ),
      'disable_map' => array(
        '#type' => 'submit',
        '#value' => t('Disable checked'),
      ),
      'pager' => array('#markup' => theme('pager')),
    ),
  );
  return $form;
}

/**
 * Implements hook_form().
 *
 * Presents a confirmation form with the selected collections.
 */
function _islandora_simple_map_islandora_collection_delete_confirm(&$form_state) {
  $to_disable = array_filter($form_state['values']['collection_listing']);
  if ($to_disable) {
    array_walk($to_disable,
      function (&$item) {
      $pid = $item;
      $object = islandora_object_load($pid);
        if ($object) {
          $item = $object->label;
        }
      }
    );
  }

  // Tell the submit handler to process the form.
  $form['process'] = array('#type' => 'hidden', '#value' => 'true');
  $markup_list = "";
  foreach ($to_disable as $pid => $title) {
    $markup_list .= "<li>" . check_plain($title) . "</li>";
  }
  $markup_list = "<ul>$markup_list</ul>";
  return confirm_form(
    $form,
    t('Are you sure you want to disable maps on the following collections?'),
    'admin/islandora/tools/islandora_simple_map/collections',
    $markup_list,
    t('Disable'));
}

/**
 * Implements hook_form_submit().
 *
 * Handles the choose collection form and confirmation form.
 */
function islandora_simple_map_admin_collection_display_submit($form_id, &$form_state) {

  if (isset($form_state['clicked_button']) &&
    $form_state['clicked_button']['#value'] == 'Disable' &&
    isset($form_state['values']['process'])) {
    $delete_pids = $form_state['storage']['disable_pids'];
    $deleted = db_delete('islandora_simple_map_collections_map')
      ->condition('pid', $delete_pids, 'IN')
      ->execute();
    if ($deleted == count($delete_pids)) {
      drupal_set_message(t(
        "@num collections disabled.",
        array('@num' => $deleted)));
    }
    else {
      drupal_set_message(t(
        "Disabled @d collection maps, expected @e",
        array('@d' => $deleted, '@e' => count($delete_pids))
      ));
    }
  }
  elseif (isset($form_state['values']['collection_listing'])) {
    $to_disable = array_filter($form_state['values']['collection_listing']);
    if ($to_disable) {
      $form_state['storage']['disable_pids'] = $to_disable;
    }
    else {
      drupal_set_message(t("No collections selected."), "warning");
    }
  }
  $form_state['rebuild'] = TRUE;
}
