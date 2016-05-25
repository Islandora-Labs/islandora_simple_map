<?php
/**
 * @file
 * Template file for the Islandora Simple Map module.
 *
 * Available variables:
 *   $coords string
 *     urlescaped geocoordinates from the MODS datastream.
 */
?>
<fieldset class="islandora collapsible collapsed">
  <legend><span class="fieldset-legend"><?php print t('Map'); ?></span></legend>
  <div class="fieldset-wrapper">
    <iframe width="600" height="600" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" src="https://maps.google.com/maps?hl=en&amp;q=<?php print $coords; ?>&amp;iwloc=A&amp;z=14&amp;t=m&amp;output=embed"></iframe>
  </div>
</fieldset>
