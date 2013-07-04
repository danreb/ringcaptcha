<?php
/**
 * @file
 * Drush integration for the ringcaptcha module.
 */

/**
 * Implements hook_drush_command().
 */
function ringcaptcha_drush_command() {
  $items['ringcaptcha-download'] = array(
    'callback' => 'ringcaptcha_drush_download',
    'description' => dt('Downloads the required RingCaptcha Library from bitbucket.'),
    'arguments' => array(
      'path' => dt('Optional. A path to the download folder. If omitted Drush will use the default location (sites/all/libraries/ringcaptcha).'),
    ),
  );
  return $items;
}

/**
 * Downloads
 */
function ringcaptcha_drush_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/ringcaptcha';
  }
  if (drush_shell_exec('git clone https://bitbucket.org/ringcaptcha/ringcaptcha-php.git ' . $path)) {
    @drush_shell_exec('find ' . $path . ' -name ".git" -type d -exec rm -rf {} \;');
    drush_log(dt('RingCaptcha has been downloaded to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the RingCaptcha to @path.', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_ringcaptcha_post_enable() {
  $modules = func_get_args();
  if (in_array('ringcaptcha', $modules)) {
    ringcaptcha_drush_download();
  }
}
