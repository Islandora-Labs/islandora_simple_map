<?php
/**
 * @file
 * Template file for the Islandora Simple Map module.
 *
 * Available variables:
 *   $collapsed string
 *     The string 'collapsed' or ''.
 */
?>
<fieldset class="islandora collapsible <?php print $collapsed; ?>">
  <legend><span class="fieldset-legend"><?php print t('Map'); ?></span></legend>
  <!-- Looks like all of the map's ancestors, not just its immediate one, need to
       have an explicit CSS height property of 100%. -->
  <div class="fieldset-wrapper">
    <div id="map-canvas-wrapper">
    <div id="map-canvas"></div>
    </div>
  </div>
</fieldset>
