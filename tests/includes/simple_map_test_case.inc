<?php

/**
 * @file
 * Simple Map Test Base Class.
 */

/**
 * Class IslandoraSimpleMapTestCase.
 */
class IslandoraSimpleMapTestCase extends IslandoraWebTestCase {
  /**
   * Location of test resources.
   *
   * @var string
   */
  protected $resourcePath;

  /**
   * Existing Use Google Maps Javascript API setting.
   *
   * @var bool
   */
  protected $jsApiState;

  /**
   * Array of objects created in the test, that should be removed.
   *
   * @var array
   */
  protected $testObjects = array();

  /**
   * {@inheritdoc}
   *
   * @see \IslandoraWebTestCase::setUp()
   */
  public function setUp() {
    $args = func_get_args();
    $args = (isset($args[0]) && is_array($args[0])) ? $args[0] : $args;
    $args[] = 'islandora_simple_map';
    parent::setUp($args);
    $this->jsApiState = variable_get('islandora_simple_map_use_gmaps_api', FALSE);
    $this->resourcePath = realpath(dirname(__FILE__) . "/../resources");
    $this->purgeTestObjects();
  }

  /**
   * Free any objects/resources created for this test.
   *
   * @see IslandoraUnitTestCase::tearDown()
   */
  public function tearDown() {
    $this->purgeTestObjects();
    variable_set('islandora_simple_map_use_gmaps_api', $this->jsApiState);
    parent::tearDown();
  }

  /**
   * Purge any objects created by the test's in this class.
   */
  protected function purgeTestObjects() {
    foreach ($this->testObjects as $object) {
      try {
        $object = $this->admin->repository->getObject($object);
        $object->label = "Don't Block";
        $this->admin->repository->purgeObject($object->id);
      }
      catch (Exception $e) {
        // Meh... Either it didn't exist or the purge failed.
      }
    }
  }

  /**
   * Enable/disable the javascript api setting for some tests.
   *
   * @param bool $val
   *   Value for the use javascript api variable.
   */
  protected function setJavascriptApi($val = FALSE) {
    variable_set('islandora_simple_map_use_gmaps_api', $val);
  }

  /**
   * Compare arrays regardless of order.
   *
   * @param array $expected
   *   The expected array.
   * @param array $actual
   *   The actual result array.
   * @param string $message
   *   The test message.
   *
   * @return bool
   *   The test result.
   */
  protected function assertArrayEquals(array $expected, array $actual, $message) {
    if (count($expected) !== count($actual)) {
      return $this->fail($message);
    }
    $expect_assoc = (count(array_filter(array_keys($expected), 'is_string')) > 0);
    if ($expect_assoc) {
      if (count(array_filter(array_keys($actual), 'is_string')) > 0) {
        return $this->fail($message);
      }
      else {
        // Associative arrays.
        foreach ($expected as $k => $v) {
          if (!array_key_exists($k, $actual) || $v !== $actual[$k]) {
            // Key or value does not match.
            return $this->fail($message);
          }
          else {
            // Remove matched entry from $actual.
            unset($actual[$k]);
          }
        }
      }
    }
    else {
      foreach ($expected as $v) {
        if (!in_array($v, $actual)) {
          // Value not in $actual.
          return $this->fail($message);
        }
        else {
          $temp_key = $index = array_search($v, $actual);
          if ($temp_key !== FALSE) {
            unset($actual[$temp_key]);
          }
          else {
            return $this->fail($message . " -- Error removing key value");
          }
        }
      }
    }
    if (count($actual) == 0) {
      // No excess values left.
      return $this->pass($message);
    }
    else {
      // Still items in $actual.
      return $this->fail($message);
    }
  }

}
