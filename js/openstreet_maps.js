(function (jQuery) {
  Drupal.behaviors.islandora_simple_map = {
    attach: function(context, settings) {
      jQuery('.islandora-simple-map-holder').once('islandora-simple-map', function(){
        Drupal.islandora_simple_map.initMap(this.id);
      });
    },
  }
  Drupal.islandora_simple_map = {
    maps: {},
    markers: [],
    initMap: function(mapId) {
      if (typeof(Drupal.settings.islandora_simple_map) != 'undefined') {
        if (typeof(Drupal.settings.islandora_simple_map[mapId]) != 'undefined') {
          var config = Drupal.settings.islandora_simple_map[mapId];
          var map_div = jQuery('#' + mapId).get(0);
          var multiple_objects = (Object.prototype.toString.call(config.map_markers) === '[object Array]' ? true : false);
          var bounds = new L.LatLngBounds();
          var coords = [];
          if (multiple_objects) {
            var firstCenter = Drupal.islandora_simple_map.coordToPoint(config.map_markers[0].coordinates[0]);
          } else {
            var firstCenter = Drupal.islandora_simple_map.coordToPoint(config.map_markers.coordinates[0]);
          }
          var defaultZoom = parseInt(config.map_zoom_level);
          var map = L.map(mapId).setView(firstCenter, defaultZoom);
          L.tileLayer(config.provider_settings.template, {
            'attribution' : config.provider_settings.attribution,
            'maxZoom' : 18,
            'id' : 'example'
          }).addTo(map);
          map.defaultZoom = defaultZoom;
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
              marker.on('click', function() {
                Drupal.islandora_simple_map.markerClick(map, this);
              });
            });
            map.on('click', function() {
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
          map.on('zoomstart', function() {
            if (map.initialZoom) {
              map.initialZoom = false;
              if (map.getZoom() > map.defaultZoom) {
                map.setZoom(map.defaultZoom);
              }
            }
          });
          Drupal.islandora_simple_map.maps[mapId] = map;
        }
      }
    },
    coordToPoint: function (coordinate) {
      var c = coordinate.split(',');
      return new L.LatLng(parseFloat(c[0].trim()), parseFloat(c[1].trim()));
    },
    makePoint: function(map, bounds, pid, coordinate) {
      lPoint = Drupal.islandora_simple_map.coordToPoint(coordinate);
      bounds.extend(lPoint);
      var center = bounds.getCenter();
      if (bounds.isValid()) {
        map.panTo(center);
      }
      var marker = L.marker(lPoint).addTo(map);
      marker.pid = pid;
      Drupal.islandora_simple_map.markers.push(marker);
    },
    redraw: function(map, bounds) {
      map.fitBounds(bounds);
      map.invalidateSize();
      map.panTo(bounds.getCenter());
      map.flyToBounds(bounds);
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
      var popup = L.popup()
          .setLatLng(marker.getLatLng())
          .setContent(content);
      map.openPopup(popup);
    }
  }
})(jQuery);
