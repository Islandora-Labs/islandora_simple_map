(function (jQuery) {
  Drupal.islandora_simple_map = {
    maps: {},
    initialize: function() {
      jQuery('.islandora-simple-map-holder').once('islandora-simple-map', function(){
        Drupal.islandora_simple_map.initMap(this.id);
      });
    },
    initMap: function(mapId) {
      if (typeof(Drupal.settings.islandora_simple_map) != 'undefined') {
        if (typeof(Drupal.settings.islandora_simple_map[mapId]) != 'undefined') {
          var config = Drupal.settings.islandora_simple_map[mapId];
          var map_div = jQuery('#' + mapId).get(0);
          var coords = [];
          for (var f = 0; f < config.map_markers.length; f += 1) {
            var item = config.map_markers[f];
            var c = item.split(',');
            coords.push(new google.maps.LatLng(parseFloat(c[0].trim()), parseFloat(c[1].trim())));
          }
          var bounds = new google.maps.LatLngBounds();
          var map = new google.maps.Map(map_div, {
            zoom: parseInt(config.map_zoom_level),
            center: coords[0],
            scrollwheel: (!config.disable_scroll_zoom)
          });
          for (var f = 0; f < coords.length; f += 1) {
            bounds.extend(coords[f]);
            new google.maps.Marker({
              position: coords[f],
              map: map
            });
          }
          fieldsets = jQuery(map_div).closest('.collapsible.collapsed')
          if (fieldsets.length > 0) {
            for (var f = 0; f < fieldsets.length; f += 1) {
              jQuery(fieldsets[f]).one('collapsed', function () {
                window.setTimeout(function () {
                  Drupal.islandora_simple_map.redraw(map, bounds);
                }, 200);
              });
            }
          }
          Drupal.islandora_simple_map.maps[mapId] = map;
        }
      }
    },
    redraw: function(map, bounds) {
      google.maps.event.trigger(map, 'resize');
      map.panTo(bounds.getCenter());
    }
  }
})(jQuery);
