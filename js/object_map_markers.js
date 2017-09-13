(function (jQuery) {
  Drupal.islandora_simple_map = {
    maps: {},
    markers: [],
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
          var multiple_objects = (Object.prototype.toString.call(config.map_markers) === '[object Array]' ? true : false);
          var bounds = new google.maps.LatLngBounds();
          var coords = [];
          var map = new google.maps.Map(map_div, {
            zoom: parseInt(config.map_zoom_level),
            center: coords[0],
            scrollwheel: (!config.disable_scroll_zoom)
          });
          map.defaultZoom = map.getZoom();
          map.initialZoom = true;
          if (multiple_objects) {
            config.map_markers.forEach(function (objVal) {
              var pid = objVal.pid;
              objVal.coordinates.forEach(function (coordVal) {
                Drupal.islandora_simple_map.makePoint(map, bounds, pid, coordVal);
              });
            });
            // Only do infoWindows when there are multiple objects.
            Drupal.islandora_simple_map.markers.forEach(function(marker) {
              google.maps.event.addListener(marker, 'click', function() {
                Drupal.islandora_simple_map.markerClick(map, this);
              });
            });
            google.maps.event.addListener(map, 'click', function() {
              if (typeof(Drupal.islandora_simple_map.mapInfoWindow) != 'undefined'){
                Drupal.islandora_simple_map.mapInfoWindow.close();
              }
            });
          } else {
            var pid = config.map_markers.pid;
            config.map_markers.coordinates.forEach(function(coordVal) {
              Drupal.islandora_simple_map.makePoint(map, bounds, pid, coordVal);
            });
          }
          fieldsets = jQuery(map_div).closest('.collapsible.collapsed')
          if (fieldsets.length > 0) {
            for (var f = 0; f < fieldsets.length; f += 1) {
              jQuery(fieldsets[f]).one('collapsed', function() {
                window.setTimeout(function () {
                  Drupal.islandora_simple_map.redraw(map, bounds);
                }, 200);
              });
            }
          }
          else {
            Drupal.islandora_simple_map.redraw(map, bounds);
          }
          // On first zoom called from map.fitBounds ensure we don't zoom closer.
          google.maps.event.addListener(map, 'zoom_changed', function() {
            if (map.initialZoom) {
              if (map.getZoom() > map.defaultZoom) {
                map.setZoom(map.defaultZoom);
              }
              map.initialZoom = false;
            }
          });
          Drupal.islandora_simple_map.maps[mapId] = map;
        }
      }
    },
    makePoint: function(map, bounds, pid, coordinate) {
      var c = coordinate.split(',');
      var lPoint = new google.maps.LatLng(parseFloat(c[0].trim()), parseFloat(c[1].trim()));
      bounds.extend(lPoint);
      var marker = new google.maps.Marker({
        position: lPoint,
        map: map
      });
      marker.pid = pid;
      Drupal.islandora_simple_map.markers.push(marker);
    },
    redraw: function(map, bounds) {
      google.maps.event.trigger(map, 'resize');
      map.fitBounds(bounds);
      map.panTo(bounds.getCenter());
    },
    markerClick: function(map, marker) {
      var pid = marker.pid;
      jQuery.ajax('/islandora_simple_map/marker/' + encodeURI(pid),{
        "dataType" : "json",
        "success" : function(data) {
          Drupal.islandora_simple_map.openInfoWindow(map, marker, data);
        }
      })
    },
    openInfoWindow: function(map, marker, objectData) {
      var content = '<div id="islandora-simple-map-infowindow" class="islandora-simple-map-infowindow-content">' +
          '<div class="islandora-simple-map-infowindow-title">' +
          '<a href="' + objectData.uri +'">' +
          objectData.label +
          '</a>' +
          '</div>';
      if (typeof(objectData.thumbnail_uri) != 'none') {
        content += '<div class="islandora-simple-map-infowindow-thumbnail">' +
            '<a href="' + objectData.uri +'">' +
            '<img src="' + objectData.thumbnail_uri + '"' +
            'class="islandora-simple-map-infowindow-thumbnail-img"/>' +
            '</a>' +
            '</div>';
      }
      content += '</div>';
      if (typeof(Drupal.islandora_simple_map.mapInfoWindow) == 'undefined') {
        Drupal.islandora_simple_map.mapInfoWindow = new google.maps.InfoWindow();
      }
      Drupal.islandora_simple_map.mapInfoWindow.close();
      Drupal.islandora_simple_map.mapInfoWindow.setContent(content);
      Drupal.islandora_simple_map.mapInfoWindow.open(map, marker);
    }
  }
})(jQuery);
