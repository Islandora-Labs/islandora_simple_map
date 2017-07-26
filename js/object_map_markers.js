(function (jQuery) {
  Drupal.islandora_simple_map = {
    maps: {},
    initMap: function () {
      if (typeof(Drupal.settings.islandora_simple_map) != 'undefined') {
        var map_div = jQuery('#' + Drupal.settings.islandora_simple_map.map_div_id).get(0);
        var coords = [];
        for (var f = 0; f < Drupal.settings.islandora_simple_map.map_markers.length; f += 1) {
          var item = Drupal.settings.islandora_simple_map.map_markers[f];
          var c = item.split(',');
          coords.push(new google.maps.LatLng(parseFloat(c[0].trim()),parseFloat(c[1].trim())));
        }
        var bounds = new google.maps.LatLngBounds();
        var map = new google.maps.Map(map_div, {
          zoom: parseInt(Drupal.settings.islandora_simple_map.map_zoom_level),
          center: coords[0],
          scrollwheel: (!Drupal.settings.islandora_simple_map.disable_scroll_zoom)
        });
        for (var f = 0; f < coords.length; f += 1){
          bounds.extend(coords[f]);
          new google.maps.Marker({
            position: coords[f],
            map: map
          });
        }
        fieldsets = jQuery(map_div).closest('.collapsible.collapsed')
        if (fieldsets.length > 0) {
          for (var f = 0; f < fieldsets.length; f += 1) {
            jQuery(fieldsets[f]).one('collapsed', function() {
              window.setTimeout(function() {
                Drupal.islandora_simple_map.redraw(map, bounds);
              }, 200);
            });
          }
        }
        Drupal.islandora_simple_map.maps[Drupal.settings.islandora_simple_map.map_div_id] = map;
      }
    },
    redraw: function(map, bounds) {
      google.maps.event.trigger(map, 'resize');
      map.panTo(bounds.getCenter());
    }
  }
})(jQuery);
