<?php

/**
 * @file
 * Forms displayed on the object's Manage -> Collection tab.
 */

/**
 * Implements hook_form().
 *
 * Form displayed on Collection objects Manage -> Collection page.
 */
function islandora_simple_map_collection_manage_form($form, &$form_state, $object) {
  $has_collection_map = _islandora_simple_map_display_collection_map($object->id);
  $form = [
    '#action' => request_uri() . '#manage-islandora-simple-maps',
    'islandora_simple_map_display' => [
      '#type' => 'checkbox',
      '#title' => t('Display map of resources in this collection.'),
      '#description' => t('Display a map of the resources in this collection, using the Solr field configured in the Admin -> Islandora -> Islandora Utility -> Islandora Simple Maps'),
      '#return_value' => TRUE,
      '#default_value' => $has_collection_map,
    ],
    'islandora_simple_map_collection_pid' => [
      '#type' => 'hidden',
      '#value' => check_plain($object->id),
    ],
    'islandora_simple_map_collection_submit' => [
      '#type' => 'submit',
      '#value' => t('Save'),
    ],
  ];
  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * Submit handler for above form.
 */
function islandora_simple_map_collection_manage_form_submit($form, &$form_state) {
  if (isset($form_state['values']['islandora_simple_map_collection_pid']) &&
    !empty($form_state['values']['islandora_simple_map_collection_pid'])) {
    $pid = $form_state['values']['islandora_simple_map_collection_pid'];
    $has_collection_map = _islandora_simple_map_display_collection_map($pid);
    if ((bool) $form_state['values']['islandora_simple_map_display'] == FALSE &&
      $has_collection_map) {
      $num_deleted = db_delete('islandora_simple_map_collections_map')->condition('pid', $pid)->execute();
      if ($num_deleted !== 1) {
        form_set_error('islandora_simple_map_display', 'Problem unsetting collection map.');
      }
      else {
        drupal_set_message(t("Collection map display disabled."));
      }
    }
    elseif ((bool) $form_state['values']['islandora_simple_map_display'] == TRUE &&
      !$has_collection_map) {
      $object = islandora_object_load($pid);
      if ($object) {
        db_insert('islandora_simple_map_collections_map')->fields([
          'pid' => $pid,
          'title' => check_plain($object->label),
        ])->execute();
        drupal_set_message(t("Collection map display enabled."));
      }
    }
  }
}
