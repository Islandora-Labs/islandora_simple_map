<?php
/**
 * @file
 * Template file for the Islandora Simple Map module.
 *
 * Available variables:
 *   $coords string
 *     urlescaped geocoordinates from the MODS datastream.
 *   $iframe_width string
 *     Width of the map iframe.
 *   $iframe_height string
 *     Width of the map iframe.
 *   $zoom string
 *     The default zoom level.
 *   $collapsed string
 *     The string 'collapsed' or ''.
 */
?>
<fieldset class="islandora collapsible <?php print $collapsed; ?>">
  <legend><span class="fieldset-legend"><?php print t('Map'); ?></span></legend>
  <div class="fieldset-wrapper">
    <iframe width="<?php print $iframe_width; ?>" height="<?php print $iframe_height; ?>" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" src="https://maps.google.com/maps?hl=en&amp;q=<?php print $coords; ?>&amp;iwloc=A&amp;z=<?php print $zoom; ?>&amp;t=m&amp;output=embed"></iframe>
  </div>
</fieldset>
