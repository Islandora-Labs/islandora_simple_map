(function (jQuery) {

  IslandoraGoogleSimpleMap = function(mapId, settings) {
    IslandoraSimpleMap.call(this, mapId, settings);
    this.bounds = new google.maps.LatLngBounds();
  };

  jQuery.extend(IslandoraGoogleSimpleMap.prototype, IslandoraSimpleMap.prototype);

  IslandoraGoogleSimpleMap.prototype.initMap = function() {
    return new google.maps.Map(this.map_div, {
      zoom: parseInt(this.config.map_zoom_level),
      center: this.coords[0],
      scrollwheel: (!this.config.disable_scroll_zoom)
    });
  };

  IslandoraGoogleSimpleMap.prototype.doKmlLayers = function(kml_layers) {
    this.config.map_kml.forEach(function (url) {
      var layer = new google.maps.KmlLayer(url, {
        preserveViewport: true,
        map: this.map
      });
    });
  };

  IslandoraGoogleSimpleMap.prototype.markerEventListener = function(marker) {
    var outerObj = this;
    marker.addListener('click', function () {
      outerObj.markerClick(this);
    });
  };

  IslandoraGoogleSimpleMap.prototype.mapEventListener = function(map) {
    var outerObj = this;
    google.maps.event.addDomListener(map, 'click', function() {
      if (typeof(outerObj.mapInfoWindow) != 'undefined'){
        outerObj.mapInfoWindow.close();
      }
    });
  };

  IslandoraGoogleSimpleMap.prototype.zoomChangedEventListener = function(map) {
    // On first zoom called from map.fitBounds ensure we don't zoom closer.
    google.maps.event.addDomListener(map, 'zoom_changed', function() {
      if (map.initialZoom) {
        if (map.getZoom() > map.defaultZoom) {
          map.setZoom(map.defaultZoom);
        }
        map.initialZoom = false;
      }
    });
  };

  IslandoraGoogleSimpleMap.prototype.openInfoWindow = function(map, marker, objectData) {
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
    if (typeof(this.mapInfoWindow) == 'undefined') {
      this.mapInfoWindow = new google.maps.InfoWindow();
    }
    this.mapInfoWindow.close();
    this.mapInfoWindow.setContent(content);
    if (marker instanceof google.maps.Data.Feature) {
      // anchor the infowindow on the marker
      this.mapInfoWindow.setPosition(marker.getGeometry().get());
      this.mapInfoWindow.open(map);
    } else {
      this.mapInfoWindow.open(map, marker);
    }
  };

  IslandoraGoogleSimpleMap.prototype.doGeoJson = function(geojsons) {
    var outerObj = this;
    // Add a listener to extend bounds whenever a geojson feature is added.
    this.map.data.addListener('addfeature', function(f) {
      outerObj.processPoint(f.feature);
    });
    if (this.multiple_objects) {
      this.map.data.addListener('click', function(event) {
        outerObj.featureClick(event.feature);
      });
    }

    // Add the geojson.
    this.map.data.addGeoJson(this.config.map_geojson);
  }

  IslandoraGoogleSimpleMap.prototype.processPoint = function(feature) {
    var outerObj = this;
    var geometry = feature.getGeometry();
    var pid = feature.getProperty('pid');

    if (geometry instanceof google.maps.LatLng) {
      this.makeMarker(geometry, pid);
    } else if (geometry instanceof google.maps.Data.Point) {
      this.makeMarker(geometry.get(), pid);
    } else {
      geometry.getArray().forEach(function(g) {
        outerObj.processPoint(g);
      });
    }
  };

  IslandoraGoogleSimpleMap.prototype.makePoint = function(bounds, pid, coordinate) {
    var c = coordinate.split(',');
    var lPoint = new google.maps.LatLng(parseFloat(c[0].trim()), parseFloat(c[1].trim()));
    this.makeMarker(lPoint, pid);
  };

  IslandoraGoogleSimpleMap.prototype.makeMarker = function(lPoint, pid) {
    this.bounds.extend(lPoint);
    var marker = new google.maps.Marker({
      position: lPoint,
      map: this.map
    });
    marker.pid = pid;
    this.markers.push(marker);
  };

  IslandoraGoogleSimpleMap.prototype.redraw = function() {
    google.maps.event.trigger(this.map, 'resize');
    this.map.fitBounds(this.bounds);
    this.map.panTo(this.bounds.getCenter());
  };

  Drupal.islandora_simple_map = {
    maps: {},
    initialize: function() {
      jQuery('.islandora-simple-map-holder').once('islandora-simple-map', function(){
        var theMap = new IslandoraGoogleSimpleMap(this.id, Drupal.settings.islandora_simple_map);
        if (theMap.loaded) {
          theMap.initialize();
          Drupal.islandora_simple_map.maps[this.id] = theMap;
        }
      });
    }
  }
})(jQuery);
