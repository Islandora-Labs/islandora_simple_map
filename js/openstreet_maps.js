(function (jQuery) {

  IslandoraOSMSimpleMap = function(mapId, settings) {
    IslandoraSimpleMap.call(this, mapId, settings);
    this.bounds = new L.LatLngBounds();
  };

  jQuery.extend(IslandoraOSMSimpleMap.prototype, IslandoraSimpleMap.prototype);


  IslandoraOSMSimpleMap.util = {
    coordToPoint: function(coordinate) {
      if (coordinate) {
        var c = coordinate.split(',');
        return new L.LatLng(parseFloat(c[0].trim()), parseFloat(c[1].trim()));
      }
    }
  };

  IslandoraOSMSimpleMap.prototype.initMap = function() {
    var firstCenter = new L.LatLng(0, 0);
    if (this.multiple_objects && this.config.map_markers.length && this.config.map_markers[0].coordinates.length) {
      firstCenter = IslandoraOSMSimpleMap.util.coordToPoint(this.config.map_markers[0].coordinates[0]);
    } else if (!this.multiple_objects && this.config.map_markers.coordinates.length) {
      firstCenter = IslandoraOSMSimpleMap.util.coordToPoint(this.config.map_markers.coordinates[0]);
    }
    else if (this.config.map_geojson && this.config.map_geojson.features.length) {
      var coordArray = this.config.map_geojson.features[0].geometry.coordinates;
      // NB GeoJSON coordinates are [ lng, lat ]. Array.reverse() alters the object so don't use it yet.
      firstCenter = IslandoraOSMSimpleMap.util.coordToPoint(coordArray[1] + "," + coordArray[0]);
    }

    var defaultZoom = parseInt(this.config.map_zoom_level);
    var map = L.map(this.mapId).setView(firstCenter, defaultZoom);
    var default_config = this.config.provider_settings.tile_config;
    L.tileLayer(this.config.provider_settings.template, default_config).addTo(map);
    map.closePopupOnClick = true;
    return map;
  };


  IslandoraOSMSimpleMap.prototype.markerEventListener = function(marker) {
    var outerObj = this;
    marker.on('click', function () {
      outerObj.markerClick(this);
    });
  };

  IslandoraOSMSimpleMap.prototype.mapEventListener = function(map) {
    var outerObj = this;
    map.on('click', function() {
      if (typeof(outerObj.mapInfoWindow) != 'undefined'){
        outerObj.mapInfoWindow.close();
      }
    });
  };

  IslandoraOSMSimpleMap.prototype.zoomChangedEventListener = function(map) {
    // On first zoom called from map.fitBounds ensure we don't zoom closer.
    map.on('zoomstart', function() {
      if (map.initialZoom) {
        if (map.getZoom() > map.defaultZoom) {
          map.setZoom(map.defaultZoom);
        }
        map.initialZoom = false;
      }
    });
  };

  IslandoraOSMSimpleMap.prototype.openInfoWindow = function(map, marker, objectData) {
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

    // Leaflet has an auto-close option when a new popup is opened, or when a
    // user clicks on the map. So we don't need to re-use the popup.
    marker.bindPopup(content).openPopup();

  };

  IslandoraOSMSimpleMap.prototype.doGeoJson = function(geojsons) {
    var outerObj = this;
    // Add a listener to extend bounds whenever a geojson feature is added.
    L.geoJSON(geojsons, {
      pointToLayer: function(geoJsonPoint, LatLng) {
        outerObj.makeMarker(LatLng, geoJsonPoint.properties.pid);
      }
    }).addTo(outerObj.map);

    if (this.multiple_objects) {
      this.markers.forEach(function (marker) {
        outerObj.markerEventListener(marker);
      });
    }
  };

  IslandoraOSMSimpleMap.prototype.makePoint = function(bounds, pid, coordinate) {
    var lPoint = IslandoraOSMSimpleMap.util.coordToPoint(coordinate);
    this.makeMarker(lPoint, pid);
  };

  IslandoraOSMSimpleMap.prototype.makeMarker = function(lPoint, pid) {
    this.bounds.extend(lPoint);
    var center = this.bounds.getCenter();
    if (this.bounds.isValid()) {
      this.map.panTo(center);
    }
    var marker = L.marker(lPoint).addTo(this.map);
    marker.pid = pid;
    this.markers.push(marker);
  };

  IslandoraOSMSimpleMap.prototype.redraw = function() {
    this.map.fitBounds(this.bounds);
    this.map.invalidateSize();
    this.map.panTo(this.bounds.getCenter());
    this.map.flyToBounds(this.bounds);
  };

  Drupal.behaviors.islandora_simple_map = {
    attach: function(context, settings) {
      jQuery('.islandora-simple-map-holder').once('islandora-simple-map', function(){
        var theMap = new IslandoraOSMSimpleMap(this.id, Drupal.settings.islandora_simple_map);
        if (theMap.loaded) {
          theMap.initialize();
          Drupal.islandora_simple_map.maps[this.id] = theMap;
        }
      });
    }
  };

  Drupal.islandora_simple_map = {
    maps: {}
  }
})(jQuery);
