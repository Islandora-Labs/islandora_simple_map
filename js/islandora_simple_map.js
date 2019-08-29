

window.IslandoraSimpleMap = (function (jQuery) {

  /**
   *
   * @param mapId
   * @param settings
   * @returns {IslandoraSimpleMap}
   * @constructor
   */
  function IslandoraSimpleMap(mapId, settings) {
    this.loaded = false;
    if (typeof (settings) != 'undefined') {
      if (typeof (settings[mapId]) != 'undefined') {
        this.mapId = mapId;
        this.config = settings[mapId];
        this.map_div = jQuery('#' + mapId).get(0);
        this.multiple_objects = (Object.prototype.toString.call(this.config.map_markers) === '[object Array]' ||
        (this.config.map_geojson && this.config.map_geojson.features.length > 1) ? true : false);
        this.coords = [];
        this.bounds = {};
        this.map = this.initMap();
        this.map.defaultZoom = this.map.getZoom();
        this.map.initialZoom = true;
        this.markers = [];
        this.loaded = true;
      }
    }
  }

  /**
   * Static utility function
   */
  IslandoraSimpleMap.util = {
    isArray: function(object) {
      return (Object.prototype.toString.call(object) === '[object Array]');
    }
  };

  /**
   * Abstract function : Create/return a map object
   * @returns map object
   *
   * IslandoraSimpleMap.prototype.initMap = function() {};
   */

  /**
   * Abstract function : process an array of KML layers.
   * @param kml_layers
   *   Array of KML layers.
   */
  IslandoraSimpleMap.prototype.doKmlLayers = function(kml_layers) {
    // This is a placeholder, and is overridden in Google maps, but
    // thus skipped in OSM maps.
  };


  /**
   * Abstract function : add an event listener for click on marker. Calls
   * the markerClick(marker) function.
   * @param markers
   *   An array of marker objects.
   *
   * IslandoraSimpleMap.prototype.markerEventListener = function(markers) {};
   */

  /**
   * Abstract function : add an event listener to map to close info windows.
   * @param map
   *   The map object.
   *
   * IslandoraSimpleMap.prototype.mapEventListener = function(map) {};
   */


  /**
   * Abstract function : add an event listener on zoom change to handle minimum zoom.
   * @param map
   *   The map object.
   *
   * IslandoraSimpleMap.prototype.zoomChangedEventListener = function(map) {};
   */

  /**
   * Abstract function : open an infowindow on map for marker with objectData content.
   * @param map
   *   The map object.
   * @param marker
   *   The marker object.
   * @param objectData
   *   The data object with keys:
   *   - pid : the pid.
   *   - label : the object label.
   *   - uri : the object's URI.
   *   - thumbnail_uri : The path to the TN dsid (if applicable).
   *
   * IslandoraSimpleMap.prototype.openInfoWindow = function(map, marker, objectData) {};
   */

  /**
   * Abstract function : Handle an array of geoJSON features add to map as markers.
   *
   * @param geojson
   *   Array of geoJSON features.
   *
   * IslandoraSimpleMap.prototype.doGeoJson = function(geojson) {};
   */

  /**
   * Abstract function : Create a map marker and add it to the map.
   *
   * @param bounds
   *   The bounds object.
   * @param pid
   *   The object PID.
   * @param coordinate
   *   The coordinate object.
   *
   * IslandoraSimpleMap.prototype.makePoint = function(bounds, pid, coordinate) {};
   */

  /**
   * Abstract function : refresh and redraw the map.
   *
   * IslandoraSimpleMap.prototype.redraw = function() {};
   */

  /**
   * Initialize an new map.
   */
  IslandoraSimpleMap.prototype.initialize = function() {
    if (typeof(this.config) != 'undefined') {
      var outerObj = this;
      // Handle GeoJSON data.
      if (this.config.map_geojson && this.config.map_geojson.features.length) {
        this.doGeoJson(this.config.map_geojson);
      }
      // Handle KML data.
      if (this.config.map_kml.length) {
        this.doKmlLayers();
      }
      // Handle basic coordinate data.
      if (this.config.map_markers &&
          (IslandoraSimpleMap.util.isArray(this.config.map_markers) ||
              typeof(this.config.map_markers.pid) !== 'undefined')) {
        this.doMapMarkers(this.config.map_markers);
        if (this.multiple_objects) {
          // Only do infoWindows when there are multiple objects.
          this.markers.forEach(function (marker) {
            outerObj.markerEventListener(marker);
          });
        }
      }

      // Multiple sources (ie. collection map)
      if (this.multiple_objects) {
        // Define map click
        this.mapEventListener(this.map)
      }

      // Initialize the fieldset map
      var fieldsets = jQuery(this.map_div).closest('.collapsible.collapsed')
      if (fieldsets.length > 0) {
        for (var f = 0; f < fieldsets.length; f += 1) {
          jQuery(fieldsets[f]).one('collapsed', function() {
            window.setTimeout(function () {
              outerObj.redraw(outerObj.map, outerObj.bounds);
            }, 200);
          });
        }
      }
      else {
        this.redraw(this.map, this.bounds);
      }

      // Initial zoom.
      // On first zoom called from map.fitBounds ensure we don't zoom closer.
      this.zoomChangedEventListener(this.map);
    }
  };

  /**
   * Process a single or array of map markers.
   *
   * @param markers
   *   Array of markers.
   */
  IslandoraSimpleMap.prototype.doMapMarkers = function(markers) {
    var outerObj = this;
    if (typeof(markers.pid) != 'undefined') {
      // Only one coordinate
      var pid = markers.pid;
      markers.coordinates.forEach(function(coordVal) {
        outerObj.makePoint(outerObj.bounds, pid, coordVal);
      });
    } else if (IslandoraSimpleMap.util.isArray(markers)) {
      // multiple coordinates
      markers.forEach(function (objVal) {
        var pid = objVal.pid;
        objVal.coordinates.forEach(function (coordVal) {
          outerObj.makePoint(outerObj.bounds, pid, coordVal);
        });
      });
    }
  };


  /**
   * Utility function that is called when a marker is clicked.
   *
   * @param marker
   *   Marker
   */
  IslandoraSimpleMap.prototype.markerClick = function(marker) {
    var pid = marker.pid;
    var outerObj = this;
    jQuery.ajax('/islandora_simple_map/marker/' + encodeURI(pid),{
      "dataType" : "json",
      "success" : function(data) {
        outerObj.openInfoWindow(outerObj.map, marker, data);
      }
    });
  };

  /**
   * Utility function this is called when a feature is clicked.
   *
   * @param feature
   *   The feature.
   */
  IslandoraSimpleMap.prototype.featureClick = function(feature) {
    var pid = feature.getProperty('pid');
    var outerObj = this;
    jQuery.ajax('/islandora_simple_map/marker/' + encodeURI(pid),{
      "dataType" : "json",
      "success" : function(data) {
        outerObj.openInfoWindow(outerObj.map, feature, data);
      }
    });
  }

  return IslandoraSimpleMap;

})(jQuery);